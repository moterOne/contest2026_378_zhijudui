# esp32_ai — OpenVela/NuttX 上的 Barista 移植（ESP32-P4X）

本应用把上游 [`esp32-ai-main.zip`](../../esp32-ai-main.zip) 中的 **Barista**（单输出类
咖啡问答 LLM）按 [`board/contest_board/docs/esp32-ai-移植至ESP32-P4X-OpenVela部署方案.md`](../../board/contest_board/docs/esp32-ai-移植至ESP32-P4X-OpenVela部署方案.md)
移植为 OpenVela/NuttX 原生命令。TinyStories / Fly 不在本应用首版范围（见部署方案
§5 P3）。

## 目录结构

```text
app/esp32_ai/
├── Kconfig                 # 应用开关 + 存储/内存/资源选项（含 Flash 容量参数）
├── Make.defs               # Make 路径接入 packages/demos（自动发现）
├── Makefile                # Make 路径构建；生成头缺失时构建期报错
├── CMakeLists.txt          # 仅占位；P4 CMake 路径未验证，不宣称支持
├── LICENSE                 # 上游 MIT 许可（esp32-ai-main）
├── README.md               # 本文件
├── PORTING-NOTES.md        # 与上游的差异、许可与标注
├── runtime/
│   ├── llm.h               # 上游原样复制（MIT）
│   └── bpe_tokenizer.h     # 上游原样复制（MIT）
├── platform/
│   ├── esp32_ai_memory.[ch]    # SRAM/PSRAM 定向分配接口
│   ├── esp32_ai_storage.[ch]   # 只读随机访问存储视图（越界检查）
│   ├── esp32_ai_time.h         # 单调时钟（微秒）
│   └── esp32_ai_hash.h         # FNV-1a（与 SHA-256 区分）
├── generated/              # 由 tools 生成，缺文件时构建/运行明确失败
│   ├── tokenizer_encoder.h
│   ├── barista_words.h
│   └── barista_out2in.h
├── tools/
│   ├── generate_tokenizer_header.py  # 上游原样复制
│   ├── generate_vocab_headers.py     # 上游原样复制
│   ├── export.py                     # 上游模型导出工具（参考）
│   └── verify_model.py               # 离线资源校验（新增）
└── esp32_ai_main.c         # NSH 命令入口：info / selftest / chat
```

板级配合（`board/contest_board/`）：

```text
board/contest_board/src/
├── contest2026_378_model.[ch]   # 注册 /dev/model（模型区 MTD，只读）
└── contest2026_378_bringup.c    # bring-up 时调用 contest2026_378_model_init()
```

## 命令

| 命令 | 作用 | 对应阶段 |
| --- | --- | --- |
| `esp32_ai` | 打印模型/资源/内存摘要 | — |
| `esp32_ai info` | 同上（显式子命令） | — |
| `esp32_ai selftest` | P1 自检：存储视图、模型头、限量随机读、越界拒绝、分配/释放、整镜像读取与 FNV-1a、内存层级报告 | P1 |
| `esp32_ai chat` | P2 串口问答 MVP（单核、无显示）；EOF 或输入 `quit` 退回 NSH | P2 |

`chat` 仅在 `CONFIG_ESP32_AI_BARISTA_RESOURCES=y` 且 `generated/` 三份头齐全时可用；
否则明确拒绝，绝不输出看似正常的错误答案。

## 资源准备（P0，主机侧）

1. 按部署方案 P0 从经授权来源获取 Barista 资源（`model.bin`、`tokenizer.json`、
   `vocab.json`、`layout.json`），用上游 `scripts/fetch_model.sh barista` 的固定
   大小/SHA-256 校验。
2. 生成设备头：

   ```sh
   python3 tools/generate_tokenizer_header.py <tokenizer.json> \
       > generated/tokenizer_encoder.h
   python3 tools/generate_vocab_headers.py <vocab.json> <layout.json> \
       > generated/barista_words.h   # 依上游工具实际参数
   python3 tools/generate_vocab_headers.py ... > generated/barista_out2in.h
   ```

   各工具参数以 `tools/*.py` 顶部 docstring / `--help` 为准。
3. 离线校验（新增工具）：

   ```sh
   python3 tools/verify_model.py --model model.bin \
       --size 4600186 --sha256 <上游校验值> \
       --tokenizer tokenizer.json --vocab vocab.json --layout layout.json
   ```

4. 把模型烧录到经 P1 确认的 Flash 区域（`CONFIG_ESP32_AI_MODEL_FLASH_OFFSET`），
   写入后读回做摘要校验。模型资源体积大，默认不入仓；记录下载来源/版本/尺寸/摘要。

## 构建与烧录（P1/P2，Make 路径）

```sh
# 工作树根目录（本仓的上一级）
./build.sh vendor/openvela/boards/contest2026_378_board/configs/esp32_ai -j8
./.espvenv/bin/esptool.py -c esp32p4 -p /dev/ttyACM0 -b 460800 \
    write_flash 0x2000 nuttx/nuttx.bin
```

配置要点（见 `board/contest_board/configs/esp32_ai/defconfig`）：

- `CONFIG_ESPRESSIF_SPIRAM=y` + `CONFIG_ESPRESSIF_SPIRAM_USER_HEAP=y`：用户堆 = PSRAM
  （模型镜像与 int8 staging 落 PSRAM；内部 SRAM 留给内核/任务栈）。
- `CONFIG_ESPRESSIF_SPIFLASH=y`：启用 SPI Flash MTD 能力。
- `CONFIG_ESP32_AI_MODEL_STORAGE=y`：板级注册 `/dev/model`。
- `CONFIG_LVX_USE_DEMO_CONTEST2026_378_ESP32_AI=y`：使能本应用。

**CMake 路径未验证**；不要因 `CMakeLists.txt` 存在就宣称 P4 CMake 可用。

## 当前状态

按部署方案的分阶段口径，本应用代码已就绪，但**尚未在实机完成 P1/P2 验收**：

- [ ] P1：PSRAM 实测容量/稳定性；Flash 高位读写；`/dev/model` 自检通过（`esp32_ai selftest`）
- [ ] P2：模型写入后 `esp32_ai chat` 串口问答、耗时、重启回归
- [ ] P3：单核基线后的多核/显示/其他模型均未承诺

> 模型写入是破坏性操作：写入前保存阶段 A 可恢复映像；先构建新镜像并完成主机验证，
> 再写模型到已证明安全的地址；写入后先读回做摘要校验，失败不得启动推理。
