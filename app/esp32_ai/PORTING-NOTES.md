# PORTING-NOTES — 与上游 esp32-ai 的差异、许可与标注

## 许可

- 上游项目（`esp32-ai-main.zip`）为 **MIT**（见本目录 `LICENSE`）。
- `runtime/llm.h`、`runtime/bpe_tokenizer.h` 与 `tools/generate_tokenizer_header.py`、
  `tools/generate_vocab_headers.py`、`tools/export.py` 为**上游原样复制**，保留其
  版权与许可要求，未作修改。
- 新增代码（`platform/`、`esp32_ai_main.c`、`tools/verify_model.py`、Kconfig/
  Makefile/文档）按 Apache-2.0（与 openvela 仓一致）。
- 模型/图资源与数据集可能另有独立许可与署名条件（Fly 文档提到 MaleCNS CC BY 4.0），
  单独核对后再发布；模型资源默认不提交到队伍仓。

## 从 .ino 移植的差异（esp32_barista.ino → esp32_ai_main.c）

| 上游（Arduino/ESP-IDF） | 本移植（NuttX） | 说明 |
| --- | --- | --- |
| `setup()`/`loop()` 常驻 | NSH 命令 `esp32_ai info/selftest/chat`，可退出 | chat 以 EOF/`quit` 结束，不占满控制台 |
| `esp_partition_find_first` + `esp_partition_mmap` | `esp32_ai_storage`（`/dev/model`，`pread`，越界检查） | 不假设 mmap 存在；整镜像读入 PSRAM 后由 `llm_load` 绑定 |
| `heap_caps_malloc(MALLOC_CAP_SPIRAM)` | `esp32_ai_mem_alloc(PSRAM)` → 用户堆（`CONFIG_ESPRESSIF_SPIRAM_USER_HEAP` 下 = PSRAM） | 见 `platform/esp32_ai_memory.h` 的层级映射与回退计数 |
| `heap_caps_malloc(MALLOC_CAP_INTERNAL)`（热工作集） | `esp32_ai_mem_alloc(SRAM)` → 内核堆 `kmm_*`（flat build 可直接调用） | 回退到用户堆时计数 `sram_fallbacks`，标签绝不静默失真 |
| `xTaskCreatePinnedToCore` 双核 matvec | 单核 `MATVEC`（`layer_matvec`/`head_matvec` 保持 NULL） | 多核属 P3，不在此版本承诺 |
| `Serial`/USB CDC | `stdio`（NSH 控制台 stdin/stdout） | 不经 Arduino `Serial` 抽象 |
| I2C OLED（`USE_DISPLAY=1`） | 无显示（等价 `USE_DISPLAY=0`） | 显示属 P3，先核对 P4X 接线/驱动 |
| `esp_timer_get_time()` | `esp32_ai_time_us()`（`CLOCK_MONOTONIC`） | — |
| `esp32_barista.ino` 的 `setup()` 校验（词表/out2in/分配失败即停） | `model_tables_consistent()` + 构建期缺失资源报错 | 保留“失败即停”，不输出看似正常的错误答案 |
| FNV-1a `fp`（设备端） | 保留；与主机侧 SHA-256（`CONFIG_ESP32_AI_MODEL_SHA256`）**严格区分** | 不混用两类指纹 |

## 未改动的上游行为

- `llm.h` 的模型格式、张量布局、`llm_load` 校验、matvec/RMSNorm/RoPE/PLE 数值路径。
- `bpe_tokenizer.h` 的 BTK1 布局与编码路径。
- 贪心解码、`BARISTA_MAX_PIECES`/`BARISTA_ANSWER_ROOM` 语义。
- int8 staging（`llm_stage_core_int8_alloc`）逐张量分配、失败保留 int4 路径的语义。

## 已知限制（如实记录，不当作已完成）

- 未在实机验证：PSRAM 容量/稳定读写、高位 Flash 读、模型烧录与读回、推理耗时。
- 未实现：显示、双核/SMP、TinyStories、Fly。
- 未验证：CMake 构建路径。
- 存储视图当前为“整镜像读入 RAM”后端；若后续出现可映射 Flash 的能力，可增加 map
  后端而不改调用方。
