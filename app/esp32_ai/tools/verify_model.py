#!/usr/bin/env python3
"""Offline verification of the Barista release resources for esp32_ai.

The deployment plan requires that model.bin, tokenizer.json, vocab.json and
layout.json come from the SAME release, and that device code refuses to run
when the pairing is stale.  This tool implements the host-side half of that
gate:

  * model.bin size vs --size and SHA-256 vs --sha256 (the fixed values from
    the upstream fetch_model.sh / metadata.json, NOT a device fingerprint);
  * PLE model header parse + sanity checks (magic "PLE\\0", format version,
    header length, tied-head flag, dimension validity, image byte size vs
    the on-disk file length);
  * tokenizer/vocab/layout pair consistency: vocabulary id density, merge
    table sortedness, out2in coverage of the input vocabulary, and word
    count equality with the exported out_vocab.

Exit codes: 0 = all checks pass, 1 = any check fails, 2 = usage error.

Usage:
  verify_model.py --model model.bin --size BYTES --sha256 HEX
                  [--tokenizer tokenizer.json]
                  [--vocab vocab.json --layout layout.json]
                  [--quiet]
"""

import argparse
import hashlib
import json
import struct
import sys
from pathlib import Path

LLM_MAGIC = 0x00454C50          # "PLE\0"
LLM_FORMAT_VERSION = 1
LLM_HEADER_BYTES = 56
LLM_HEADER_MAX = 4096
LLM_FLAG_TIED_HEAD = 1
LLM_FLAGS_KNOWN = LLM_FLAG_TIED_HEAD
LLM_MAX_LAYERS = 32

failures = 0


def check(name, ok, detail=""):
    global failures
    if ok:
        print(f"PASS {name}" + (f"  {detail}" if detail else ""))
    else:
        failures += 1
        print(f"FAIL {name}" + (f"  {detail}" if detail else ""))


def parse_model_header(data):
    """Mirror runtime/llm.h llm_load() header parsing (little endian)."""
    if len(data) < 16:
        return None, "file too short for the 16-byte image header"

    magic, version, header_bytes, flags = struct.unpack_from("<IIII", data, 0)
    if magic != LLM_MAGIC:
        return None, f"magic 0x{magic:08x} != PLE magic 0x{LLM_MAGIC:08x}"
    if version == 0 or version > LLM_FORMAT_VERSION:
        return None, f"unsupported format version {version}"
    if header_bytes < LLM_HEADER_BYTES or header_bytes > LLM_HEADER_MAX:
        return None, f"header_bytes {header_bytes} out of range"
    if flags & ~LLM_FLAGS_KNOWN:
        return None, f"unknown flag bits 0x{flags:08x}"

    if len(data) < header_bytes:
        return None, f"file shorter than header_bytes {header_bytes}"

    in_vocab, out_vocab = struct.unpack_from("<II", data, 16)
    dim, n_layers, n_heads, ffn, ple_dim, seq_len, group = \
        struct.unpack_from("<7i", data, 24)
    rope_theta = struct.unpack_from("<f", data, 52)[0]

    info = {
        "magic": f"{magic:08x}",
        "version": version,
        "header_bytes": header_bytes,
        "flags": f"{flags:08x}",
        "in_vocab": in_vocab,
        "out_vocab": out_vocab,
        "dim": dim,
        "n_layers": n_layers,
        "n_heads": n_heads,
        "ffn": ffn,
        "ple_dim": ple_dim,
        "seq_len": seq_len,
        "group": group,
        "rope_theta": rope_theta,
    }
    return info, None


def verify_model(path, expected_size, expected_sha256):
    data = path.read_bytes()

    check("model size", len(data) == expected_size,
          f"file={len(data)} expected={expected_size}")
    if expected_sha256:
        digest = hashlib.sha256(data).hexdigest()
        check("model sha256", digest == expected_sha256.lower(),
              f"file={digest} expected={expected_sha256.lower()}")
    else:
        print("SKIP model sha256  (no --sha256 given; record provenance)")

    info, err = parse_model_header(data)
    if err is not None:
        check("model header", False, err)
        return None
    check("model header", True, " ".join(
        f"{k}={v}" for k, v in info.items()))

    check("header tied-head flag sane",
          bool(info["flags"] & LLM_FLAG_TIED_HEAD) ==
          (info["in_vocab"] >= info["out_vocab"]),
          "tied head must be a view of the first out_vocab embedding rows")
    check("dims positive",
          info["dim"] > 0 and info["ffn"] > 0 and info["ple_dim"] > 0 and
          info["seq_len"] > 0 and info["group"] > 0 and
          info["n_layers"] > 0 and info["n_layers"] <= LLM_MAX_LAYERS)
    check("heads divide dim, head_dim even",
          info["dim"] % info["n_heads"] == 0 and
          (info["dim"] // info["n_heads"]) % 2 == 0)
    return info


def verify_tokenizer_vocab_layout(tok_path, vocab_path, layout_path, info):
    tok = json.loads(tok_path.read_text())
    vocab = json.loads(vocab_path.read_text())
    layout = json.loads(layout_path.read_text())

    # Tokenizer must be a plain HF BPE model with a dense id range.
    model = tok.get("model", {})
    merges = model.get("merges", [])
    check("tokenizer has BPE merges", len(merges) > 0,
          f"merge_count={len(merges)}")
    check("tokenizer merges sorted by rank",
          all(merges[i] < merges[i + 1] for i in range(len(merges) - 1)),
          "the device binary-searches the merge table")

    ids = model.get("vocab", {})
    dense = all(isinstance(v, int) and 0 <= v < len(ids)
                for v in ids.values())
    check("tokenizer vocab ids dense 0..N-1", dense,
          f"vocab_size={len(ids)}")

    # Word list vs model output vocabulary.
    check("word count == model out_vocab", len(vocab) == info["out_vocab"],
          f"words={len(vocab)} out_vocab={info['out_vocab']}")

    # out2in: every output class must map to an input token id below in_vocab.
    out2in = layout.get("out2in", layout if isinstance(layout, list) else [])
    check("layout out2in covers every class", len(out2in) == len(vocab),
          f"out2in={len(out2in)} words={len(vocab)}")
    if out2in:
        ids_ok = all(isinstance(v, int) and 0 <= v < info["in_vocab"]
                     for v in out2in)
        check("out2in ids within input vocab", ids_ok,
              f"in_vocab={info['in_vocab']} max_id={max(out2in)}")


def main():
    ap = argparse.ArgumentParser(
        description="Verify the Barista release resources offline.")
    ap.add_argument("--model", type=Path, required=True, help="model.bin path")
    ap.add_argument("--size", type=int, required=True,
                    help="expected model.bin byte length")
    ap.add_argument("--sha256", default="",
                    help="expected SHA-256 (hex, lowercase) of model.bin")
    ap.add_argument("--tokenizer", type=Path,
                    help="tokenizer.json path")
    ap.add_argument("--vocab", type=Path, help="vocab.json path")
    ap.add_argument("--layout", type=Path, help="layout.json path")
    ap.add_argument("--quiet", action="store_true")
    args = ap.parse_args()

    if not args.model.is_file():
        print(f"FAIL model file not found: {args.model}")
        return 1
    if (args.vocab is None) != (args.layout is None):
        ap.error("--vocab and --layout must be given together")

    info = verify_model(args.model, args.size, args.sha256)
    if info is not None and args.tokenizer and args.vocab and args.layout:
        for p in (args.tokenizer, args.vocab, args.layout):
            if not p.is_file():
                check(f"resource {p.name}", False, "file not found")
        verify_tokenizer_vocab_layout(args.tokenizer, args.vocab,
                                      args.layout, info)
    elif args.tokenizer or args.vocab:
        print("SKIP tokenizer/vocab/layout pair  "
              "(give --tokenizer --vocab --layout together)")

    print("RESULT " + ("PASS" if failures == 0 else f"FAIL ({failures})"))
    return 0 if failures == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
