# generated/ — Barista 设备头（由主机工具生成，不入仓）

本目录存放从**同一套已校验发布资源**生成的设备头。构建/运行缺文件时会**明确失败**，
而不是输出看似正常的错误答案。

| 文件 | 生成来源 | 上游工具 |
| --- | --- | --- |
| `tokenizer_encoder.h` | `tokenizer.json` | `tools/generate_tokenizer_header.py` |
| `barista_words.h` | `vocab.json` / `layout.json` 的输出词表 | `tools/generate_vocab_headers.py` |
| `barista_out2in.h` | `layout.json` 的 out2in 映射 | `tools/generate_vocab_headers.py` |

要求：

- 生成时绑定资源版本/哈希；`tokenizer_encoder.h` 内嵌的 `TOKENIZER_ENCODER_ASSET`
  与 `model.bin` 的 vocab 必须同版（`esp32_ai chat` 启动时逐项校验，不匹配即拒绝）。
- `CONFIG_ESP32_AI_BARISTA_RESOURCES=y` 时，若三份头任一缺失，Make 路径在**构建期**
  报错。

操作步骤见上级 `README.md` 的「资源准备」。
