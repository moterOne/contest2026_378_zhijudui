# `esp32-ai-main.zip` 移植至 ESP32-P4X-Function-EV-Board / OpenVela 方案

> 状态：**实施方案，尚未移植或烧录本项目**。依据工作树根目录的 `esp32-ai-main.zip` 内文件、[`阶段 A 实施记录`](./openvela-阶段A-实施记录.md)和[`阶段 B 实施记录`](./openvela-阶段B-实施记录.md)。软件/硬件状态以实施时重新核查的工作树和实机为准。

## 1. 目标、路线与现状

目标是在 **ESP32-P4X-Function-EV-Board 上运行 OpenVela/NuttX 原生应用**，先完成 Barista 的**串口问答**，随后按资源和驱动就绪程度决定是否移植 TinyStories 和 Fly；OLED/MIPI 显示不作为首版条件。不把在 ESP32-S3 上运行 Arduino 原版视为在 P4X/OpenVela 上部署成功。

当前工作树**已有进展**，不是“完全没有 P4 支持”：`nuttx/arch/risc-v/src/esp32p4/` 已集成 P4 代码，阶段 A 记录在 P4X v3.2 实机上通过 **Make + simple-boot** 构建、`nuttx/nuttx.bin` 于 **0x2000** 烧录并进入可交互 NSH；阶段 B 记录 GPIO `/dev/gpio0..2` 的实机回读。但 P4 公共支持来自**待提交上游的补丁序列**，可复现集成/上游落地需持续核查。**PSRAM 未启用，MIPI/音频/存储/以太网未形成已验证的运行基线；CMake 路径未验证。** 现有 NSH 验证不等于具备 AI 推理条件。

### ZIP 内实际包含的三个目标

| 演示 | ZIP 内入口与设备资源 | 原 ESP32-S3 假设 | P4X/OpenVela 首要障碍 |
| --- | --- | --- | --- |
| **Barista（首选 MVP）** | `firmware/esp32_barista/esp32_barista.ino`、`runtime/llm.h`，`model.bin` **4,600,186 B**，`tokenizer.json`、`vocab.json`、`layout.json` | Arduino/ESP-IDF 分区 API、`heap_caps_*`、FreeRTOS 双核任务、USB 串口；可选 I2C SH1106/SSD1306 OLED | PSRAM + Flash 数据读取 + 模型/表匹配 + NuttX 应用/串口适配；先禁用显示、先单核 |
| TinyStories | `firmware/esp32_tinystories/esp32_tinystories.ino`、`runtime/llm.h`；`model.bin` **14,912,348 B**、`tokenizer.json` | S3 16 MB Flash 的 `model` 数据分区、PSRAM、可选 OLED | 模型几乎占满 16 MB Flash；须重算 OpenVela 镜像与模型是否能共存，不能假定原布局可用 |
| Fly | `firmware/esp32_fly/esp32_fly.ino`、`runtime/connectome_*.h`、`runtime/lz4_block*.h`，`connectome.fcl` **14,174,411 B**；电路、金值及清单另有文件 | S3 双核、PSRAM 缓存、Flash 映射、**Xtensa/S3 专用** `connectome_simd.S` | 资源余量、图格式验证、S3 汇编无法在 P4 RISC-V 上直接编译；先标量基线，再单独优化 |

ZIP 内约 106 个文件，**不含** `artifacts/` 下的模型/图与 `generated/` 设备头文件。仓库主 README 的“28.9M 参数、9.88 tok/s”是 **S3 + TinyStories** 的原项目测量，不能用于 Barista、Fly 或 P4X 的验收/性能承诺；Barista 模型用于咖啡问题，不是通用聊天机器人。三套固件不是一个可直接一键部署的 OpenVela 应用。

## 2. 为什么不能直接运行原来的 `deploy.sh`

- `scripts/deploy.sh` 固定使用 `esp32:esp32:esp32s3` 的 Arduino FQBN、`--chip esp32s3`、S3 的 Flash/PSRAM 参数与 `arduino-cli upload`；这与当前 P4X 上的 NuttX **simple-boot `0x2000`** 不兼容。**不要对这块板执行原脚本中的刷写步骤。**
- 原 `partitions.csv` 是 S3 的应用 + `model`/`flymodel` 分区表：Barista/TinyStories 的数据偏移 `0x110000`，Fly 为 `0x210000`；当前 P4X 的 NuttX simple-boot **没有对应分区表**。这些位置不能不经空间规划就直接写入，否则可能覆盖 NuttX 映像或其他数据。
- 代码依赖 `esp_partition_find_first`/`esp_partition_mmap`、`heap_caps_malloc`、`xTaskCreatePinnedToCore`/通知、Arduino `Serial`/`Wire`/Adafruit 显示库与 ESP 定时 API；不能把 `.ino` 加入 NuttX 构建就期望编译运行。Fly 的 `.S` 使用 Xtensa 指令，在 RISC-V 上必须禁用/替换。
- ZIP 模型资源需**另行取得**；`scripts/fetch_model.sh <barista|tinystories|fly>` 是主机侧下载、大小和 SHA-256 校验流程，**不是**设备固件。首次执行 `uv` 或获取模型/Arduino 工具时也可能访问网络；审核来源、许可、大小和网络/隐私要求后再下载，不在设备上放下载凭据。

若只追求“尽快在板上看到原界面”，可另开 **Arduino-ESP32 对 P4X 的可行性试验**，但必须确认该板的 Arduino P4 支持、PSRAM、引脚、分区和镜像格式并重新移植；该路线是**另一套固件**，会替换 NuttX，不满足本文件的 OpenVela 部署目标，绝不可复用 S3 的 FQBN/偏移。

## 3. 开工前的五道门槛（任一不满足则停在评估）

1. **可复现基线**：按阶段 A 的版本和上游补丁，在目标 P4X 上完成 Make 构建、烧录、冷启动/复位后交互 NSH；记录补丁来源及 PR 状态。阶段 B 的 GPIO 通过不代表其他外设已可用。
2. **确认并启用 PSRAM**：依据实物/BOM/原理图及启动自检确定实装容量、总线/速率/ECC/供电与稳定性；审阅当前 `nuttx/arch/risc-v/src/common/espressif/Kconfig` 的 `ESPRESSIF_SPIRAM*` 选项，在**单独的实验配置**上先启用启动检测/内存测试，记录可用容量、最大连续块与持续读写结果，再运行模型。原版 Barista 在 S3 的 8 MB PSRAM 上分配 int8 核心、输出头及 KV；P4 可用容量和实际峰值需**实测**，不能由“芯片最大支持 32 MB”推定。保留内部 RAM 给内核/任务栈；PSRAM 未稳定时停止推理移植。
3. **Flash 访问能力与空间规划**：核对实板 16 MB Flash、当前生成镜像头的容量字段（阶段 A 记录为 4 MB）、P4 simple-boot 映像实际占用/链接布局，以及 NuttX 可用的原始 Flash/MTD/映射接口。先以跨越 4 MB 的**可擦测试区**做受控只读、写入、重启验证；容量字段/驱动不正确时先解决。不得把原 S3 分区 CSV 直接用于 P4 烧录。建立不与 `0x2000` 镜像、启动数据或保留区冲突的模型存储方案；若无可用的随机读或映射能力，评估 SD 卡等**已完成实测**的外部介质及性能，不能假设 mmap 已存在。
4. **内存与文件契约**：Barista 需要权重原始二进制、tokenizer、vocab、layout 同版；生成的编码器/输出表头要与 `model.bin` 的 vocab 和映射一致。先用模型大小、内核镜像实测大小、对齐/擦除粒度、元数据和回滚余量做容量表，留足 PSRAM/SRAM 运行时及任务栈空间；不要凭“Flash 16 MB”就同时承诺 TinyStories/Fly。
5. **控制台与集成路径**：阶段 A 实测的 USB-Serial/JTAG 控制台（主机 `/dev/ttyACM0`）可交互；确认串口设备在应用中如何打开、如何避免与 NSH 抢读同一输入。优先以**NSH 启动的交互命令**验证，命令运行时独占输入或另设专用端口，并可退出返回 NSH；不要依赖原 Arduino USB CDC `Serial` 的行为。

### Flash 容量表（必须按实测重新填写）

| 项 | 已知/待实测 | 验证要求 |
| --- | --- | --- |
| 物理 Flash | 板卡资料：16 MB | 实机型号/容量及高地址读写；核对镜像头容量字段 |
| NuttX 占用 | simple-boot 基线 `0x2000`；**镜像末端未知** | 以最终应用构建产物和启动格式计算实际占用，含升级余量 |
| Barista 权重 | 4,600,186 B（约 4.39 MiB） | 存储区域大小 ≥ 文件 + 对齐/元数据；重启后完整校验 |
| TinyStories 权重 | 14,912,348 B（约 14.22 MiB） | 除模型仍需 OS 镜像、引导及元数据，初步列为高风险，只有空间计算通过才推进 |
| Fly 图 | 14,174,411 B（约 13.52 MiB） | 同上；还需 PSRAM 缓存及多核/运算适配 |

**模型写入是破坏性操作**：实施前保存阶段 A 可恢复映像和烧录方法；先构建新镜像并完成主机验证，再写模型到已证明安全的地址/设备。模型大小、偏移、镜像边界、擦除范围均需交叉检查；写入后先读回做摘要校验，失败不得启动推理。没有可靠的双份/回滚空间时，明确记录断电更新可能使应用与模型不匹配的风险，不声称具备 OTA 原子更新。

## 4. 应用代码组织与移植接口

**拟新增、尚不存在**：队伍仓 `contest2026_378_zhijudui/app/esp32_ai/`，仅放移植所需应用代码、构建文件、版权说明及离线资源校验/打包工具；新增 manifest `<linkfile src="app/esp32_ai" dest="packages/demos/contest2026_378_esp32_ai"/>` 前先核对目录和 dest 不冲突。`app/hello_app/` 是占位样例，不直接把整个 ZIP 解压覆盖进去。板级公共引脚/电源仍放 `board/contest_board/`；公共芯片/MTD/PSRAM 能力缺失按独立上游流程处理，不能把芯片驱动塞进应用目录。

| 原 ZIP 的模块 | 预期处理 | 放行标准 |
| --- | --- | --- |
| `runtime/llm.h`、`runtime/bpe_tokenizer.h` | 尽量保留算法及模型格式；建立最小 NuttX C/C++ 包装，在主机与设备使用同版运行时 | 主机测试通过；P4 上模型加载、词表边界、单步推理可核对 |
| `esp32_barista.ino` | 拆分 CLI/串口输入、模型加载、内存分配、推理与 UI；移植 `setup()/loop()` 到可退出的 NuttX 应用入口 | 串口单核无显示能运行且退回 NSH，不死循环占满控制台 |
| `esp_partition_*` / `heap_caps_*` | 实现**明确的**存储视图接口（支持只读随机访问或安全映射、长度和越界检查）及 SRAM/PSRAM 定向分配接口；资源释放对称 | 无无效映射/跨区读取；推理热数据放对内存层级；失败有诊断 |
| FreeRTOS 双核任务/通知 | MVP 暂用单核计算；性能基线通过后再评估 NuttX SMP 任务、同步与栈大小 | 单核与多核结果一致，无竞争/死锁；不能直接使用 FreeRTOS API |
| Arduino `Serial`、`Wire`、OLED | 串口替换为经验证的 NuttX 设备/API；首版 `USE_DISPLAY=0` 等效无显示路径 | NSH、问答读写与恢复正常；S3 的 GPIO18/46 不能照搬 |
| 模型导出与 `generated/*.h` | 主机侧复用生成工具，但生成到新应用构建目录/可复现路径；部署前绑定权重与词表的版本/哈希 | 缺文件或不同版本时构建/启动明确失败，不能输出看似正常的错误答案 |

应用构建先以当前 **Make** 路径接入：参照队伍仓 `app/hello_app/` 的 `Kconfig`、`Make.defs`、`Makefile` 骨架，但用新的唯一标识与实际栈/内存要求，验证应用目录确实进入构建、生成配置实际保留选项、NSH 能找到命令；不要复制样例的 2048 字节栈作为推理任务栈，也不要因为 CMake 文件存在就声称 P4 的 CMake 路径可用。若 `packages/demos/` 的自动发现/映射不生效，先检查本版本 apps 的 Kconfig/Make 入口；公共仓需要改动时通过上游流程处理。

上游 `LICENSE` 为 **MIT**，移植复制源码时保留原版权和许可文本，并标注修改；模型/图资源与数据集可能有**独立许可证和署名条件**（Fly 文档提到 MaleCNS CC BY 4.0），单独核对再发布。模型资源体积大，默认**不提交**到队伍仓；记录下载来源、版本、尺寸与摘要供复现。

## 5. 推荐执行次序与每步验收

### P0：主机资源准备（不触板）

1. 安全查看 ZIP 并提取至隔离工作目录，先检查归档路径与许可证，不执行其脚本；不将整个研究/训练依赖安装到设备。主机运行依赖按该 ZIP 的 `pyproject.toml`/相应工具说明安装，并保留版本记录。
2. 经授权从上游发布源获取 **Barista** 资源；用 ZIP 中 `scripts/fetch_model.sh barista` 的固定 SHA-256/大小及发布 `metadata.json` 校验，确认 `model.bin`、`tokenizer.json`、`vocab.json`、`layout.json` 来自同一套发布。下载过程访问第三方网络，不包含设备密钥；需要时先审查脚本及目标域。ZIP 自身不含这些文件。
3. 运行上游现有主机端 tokenizer、模型格式、staging/数值与显示（可选）测试；`golden.txt` 可能未随发布提供，原脚本会跳过缺失的 golden gate，**不能把跳过说成 golden 通过**。记录 host PASS、SKIP 及原因；先做 `llm.h` 的 NuttX-independent 编译与单位测试。

**P0 验收**：资源可信、许可明确、摘要与模型尺寸匹配、主机验证状态可复现；尚未烧录任何模型。

### P1：板上资源门槛

1. 在阶段 A/B 基线的副本配置中逐项验证 PSRAM 初始化/测试、容量和稳定读写；检查对 NSH/GPIO 的回归。Flash 镜像头容量字段与真实 16 MB 不一致的问题须在模型进入高地址前解决并验证。
2. 仅当完整存储映射与擦除范围通过审阅时，新增 Barista 的只读模型存储接口：先用**小型测试数据**写入/读回/重启校验，再用完整资源。记录实际偏移/设备、镜像上限、模型头版本、长度、SHA-256（主机/刷后读回）和设备端可计算的指纹；区分 SHA-256 与原固件输出的 FNV-1a `fp`，不要混用。
3. 在设备上仅做“读权重头 + 校验大小 + 限量随机读 + SRAM/PSRAM 分配释放”的自检，不启动长时间推理。

**P1 验收**：P4X 实机 PSRAM 与高地址 Flash 可靠、模型不覆盖启动映像，冷启动/Reset 后仍可进入 NSH；门槛不达标就停止。

### P2：Barista 串口 MVP

1. 在新应用中接入 `runtime/llm.h`、 tokenizer 和生成的三份 Barista 头；先关闭显示和双核，保留原模型格式校验（输入/输出词表、encoder 最高 ID 与 `out2in` 上界）以及分配失败即停的行为。串口模式读取一行问题并输出答案与时间；增加超长输入、非 ASCII、无模型、断线/退出的可解释错误。
2. 用当前板的 **Make** 构建一次只含资源自检的应用，再构建单次 `llm_forward`，最后开放完整问答。每一步记录 NuttX 栈余量、内部 RAM/PSRAM 可用量、模型读取耗时、看门狗/调度器行为；不要假设 S3 的 60.2 ms/piece 或 9.88 tok/s 能复现。
3. 保持阶段 A simple-boot `0x2000` 的已验证刷机方式用于**NuttX 镜像**；模型数据另按 P1 确认的设备/地址写入，不调用原 `deploy.sh` 的 `esptool --chip esp32s3`。每次更新确保应用与模型版本成对校验。实机提问并记录输出、首词延迟、总耗时和重启后状态；与同模型主机结果在明确的误差/解码策略下比对。

**MVP 通过**：在 P4X 上 OpenVela 原生命令加载已校验模型，可从实际串口收问题并输出回复；断电重启和 Reset 后 NSH 可用，无模型/模型错配时安全拒绝推理。主机数值测试和板端资源/性能日志可供复现。

### P3：扩展而非 MVP 承诺

- **性能**：单核正确性稳定后再引入 P4 的 NuttX 多核/SMP 加速和缓存优化，分开记录模型读带宽、内存层级和纯推理/串口输出耗时；保持单核回归基线。
- **显示**：原项目的 128×64 SH1106/SSD1306 是**外接 I2C OLED**，P4X 可选 7 英寸 MIPI 屏是另一套接口。先核对该板 I2C 引脚、电源、驱动及 OLED 实物，才可重写显示适配；没有屏时维持串口版正常工作。不能把原 S3 GPIO18/46 当 P4X 接线。
- **TinyStories**：先做镜像 + 14,912,348 B 权重 + 元数据/升级空间的物理容量证明，再验证词表生成、PSRAM 分配和长序列数值；放不下则明确标为**该存储方案不支持**，另议外部存储或新硬件，不覆盖 NuttX。
- **Fly**：先按 `model_bundle.json` 校验完整资源并运行 `firmware/esp32_fly/tools/verify.py host`；移植标量 C/图读取、逃生世界和文本日志，再替换 `connectome_simd.S` 的 Xtensa 实现。P4 上若使用 RISC-V 优化内核，应与标量/金值逐项比对、重测对齐及多核同步；未完成不得声称 Fly 的 SIMD 或 1.7 s 决策速度已移植。

## 6. 总体验收和交付

- [ ] 上游 P4 公共支持的来源、补丁/PR、工具链与 Make 基线可从干净工作树复现；仍在本地未上游的补丁明确列为外部依赖。
- [ ] PSRAM/Flash/映像大小实测满足 Barista，模型存储范围与 NuttX 镜像不相交，擦写/掉电失败有恢复说明。
- [ ] 新增应用只维护于队伍仓新目录及必要 manifest 映射，代码与配置实际被选中；MIT 源码许可及模型/图资源许可记录完整。
- [ ] Barista 模型及词表同版校验、主机测试与板端推理通过；串口输入输出、性能与内存日志以及冷启动/复位回归均留存。
- [ ] 显示、TinyStories、Fly 分别按实测状态标记通过/阻塞/未测；不混用 S3 的固件格式、芯片参数、分区偏移或性能数据。

交付物建议包括：新应用源代码与构建入口、可复现资源获取/校验说明、板级依赖/上游补丁链接、经实测确认的 Flash/PSRAM 容量表、无密钥的部署步骤、失败恢复方法、主机及实机日志。**本方案未触板、未下载模型、未修改 ZIP 内项目，也未承诺目前可以直接烧录运行。**
