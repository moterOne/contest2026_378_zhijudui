# tools/ — 主机侧资源校验与生成

| 文件 | 来源 | 用途 |
| --- | --- | --- |
| `generate_tokenizer_header.py` | 上游原样复制（MIT） | `tokenizer.json` → `generated/tokenizer_encoder.h` |
| `generate_vocab_headers.py` | 上游原样复制（MIT） | `vocab.json`/`layout.json` → `generated/barista_words.h`、`barista_out2in.h` |
| `export.py` | 上游原样复制（MIT） | 模型导出参考（与 `src/model.py` 配套） |
| `verify_model.py` | 新增（Apache-2.0） | 离线校验 `model.bin` 大小/摘要与 tokenizer/vocab/layout 一致性 |

> 上游 `scripts/fetch_model.sh` 是主机侧**下载+校验**流程，不是设备固件；下载来源、
> 许可、大小与网络/隐私要求先审核后再执行。本目录不存放任何下载凭据。
