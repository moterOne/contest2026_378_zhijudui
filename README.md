# contest2026_378_zhijudui（智局队）— OpenVela 移植 ESP32-P4X + Barista AI 问答

## 一、作品简介

把 **OpenVela/NuttX 移植到 ESP32-P4X-Function-EV-Board**（新硬件适配），并在这块新板上
运行 OpenVela/NuttX 原生应用：将上游 [`esp32-ai`](esp32-ai-main.zip) 的 **Barista**（咖啡
问答 LLM）移植为可在真实板卡上通过串口提问、获得回答的 NSH 原生命令（`esp32_ai chat`）。

- **板级适配（已实机验证）**：阶段 A 完成最小可启动——Make 路径构建、simple-boot 单平面
  镜像在 `0x2000` 烧录，冷启动/复位后进入可交互 NSH；阶段 B 完成 GPIO `/dev/gpio0..2`
  实机回读。全程只改队伍仓，公共仓（`nuttx/`）改动整理为上游补丁序列单独走 PR 流程。
- **AI 应用（代码就绪，硬件验收进行中）**：严格遵守部署方案的阶段门槛——先 P1 资源自检
  （`esp32_ai selftest`），再 P2 串口问答 MVP（`esp32_ai chat`）；无模型/词表错配时**明确拒绝
  推理**，绝不输出看似正常的错误答案；不混用 S3 的固件格式、分区偏移或性能数据。
- 亮点：单命令可退出回 NSH；存储层用“有界只读随机访问”替代不存在的 mmap；SRAM/PSRAM
  分层分配有显式回退计数；主机侧 `tools/verify_model.py` 做离线资源校验，与设备端 FNV-1a
  指纹严格区分。

## 二、选题方向

**新硬件适配**（为主）+ **AI 硬件产品创新**（应用验证）。

理由：ESP32-P4X 尚无可用 OpenVela 平台基线，必须先完成板级移植（这也是本作品的主体与
最大工作量）；Barista LLM 应用则用来证明新板具备运行 AI 推理的完整链路（Flash 数据读取、
PSRAM 内存分层、模型/词表同版校验、串口交互）。显示、双核加速、TinyStories/Fly 等按
实测状态另行标记，不提前承诺。

## 三、目录结构

```text
contest2026_378_zhijudui/
├── contest2026_378_zhijudui.xml   # 本队 manifest（linkfile 映射到 openvela 编译树）
├── openvela.xml                   # openvela 基础工程清单（官方）
├── app/
│   ├── esp32_ai/                  # 本作品应用：Barista 的 OpenVela 移植（NSH 命令）
│   │   ├── runtime/               # 上游 llm.h / bpe_tokenizer.h（MIT，原样保留）
│   │   ├── platform/              # 存储视图、SRAM/PSRAM 分层分配、时钟、FNV-1a
│   │   ├── tools/                 # 资源生成/离线校验工具（含新增 verify_model.py）
│   │   ├── generated/             # 生成的设备头（缺文件时构建/运行明确失败）
│   │   ├── esp32_ai_main.c        # 命令入口：info / selftest / chat
│   │   ├── Kconfig / Make.defs / Makefile / CMakeLists.txt
│   │   └── README.md / PORTING-NOTES.md / LICENSE
│   └── hello_app/                 # 组委会占位样例（保留作对照）
├── board/contest_board/           # 自定义板卡（out-of-tree，MANIFEST 映射）
│   ├── configs/nsh|psram|esp32_ai # 三份 defconfig：最小 NSH / PSRAM 实验 / AI 应用
│   ├── docs/                      # 实施方案与阶段 A/B 实机记录、部署方案
│   ├── include/ scripts/ src/     # 板级头文件、链接脚本、板级启动/外设/模型存储
│   └── README.md
├── quickapp/hello_quickapp/       # 组委会占位样例（未使用可删）
├── logs/                          # AI Coding 日志（按组委会工具导出后提交）
└── README.md                      # 本文件
```

## 四、运行方式

### 1. 拉取完整工程

```bash
repo init -u https://github.com/open-vela/contest2026_378_zhijudui \
  -b dev-ai-contest-2026 -m contest2026_378_zhijudui.xml
repo sync -c -j8
```

同步后本仓位于工作区 `contest2026_378_zhijudui/`，openvela 全量源码在外层
（`nuttx/`、`packages/`、`vendor/` 等）。manifest 通过 `<linkfile>` 把
`app/esp32_ai`、`board/contest_board` 软链进编译树，无需手动复制。

### 2. 构建（Make 路径）

```bash
# 最小 NSH（阶段 A 已验证基线）
./build.sh vendor/openvela/boards/contest2026_378_board/configs/nsh -j8

# AI 应用配置（PSRAM 用户堆 + SPI Flash MTD + /dev/model + esp32_ai 命令）
./build.sh vendor/openvela/boards/contest2026_378_board/configs/esp32_ai -j8
```

> CMake 路径未验证，不宣称支持；产物为 `nuttx/nuttx.bin`（simple-boot 单平面镜像）。

### 3. 烧录与控制台

```bash
./.espvenv/bin/esptool.py -c esp32p4 -p /dev/ttyACM0 -b 460800 \
    write_flash 0x2000 nuttx/nuttx.bin
```

控制台为板载 USB-Serial/JTAG（主机 `/dev/ttyACM0`）。

### 4. 运行 esp32_ai

```bash
nsh> esp32_ai info        # 模型/资源/内存摘要
nsh> esp32_ai selftest    # P1 自检：存储视图、模型头、限量随机读、分配/释放
nsh> esp32_ai chat        # P2 串口问答 MVP（输入问题，回车；quit/EOF 退回 NSH）
```

`chat` 需要提前：① 按部署方案 P0 从经授权来源获取 Barista 资源并校验；② 用
`app/esp32_ai/tools/` 生成 `generated/` 三份头；③ 开启
`CONFIG_ESP32_AI_BARISTA_RESOURCES`；④ 把 `model.bin` 烧到
`CONFIG_ESP32_AI_MODEL_FLASH_OFFSET`（默认 `0x800000`）并读回校验。

### 5. 当前状态

| 项 | 状态 | 依据 |
| --- | --- | --- |
| 最小 NSH 启动 / 冷启动 / 复位 | ✅ 已实机验证 | `docs/openvela-阶段A-实施记录.md` |
| GPIO 回读（`/dev/gpio0..2`） | ✅ 已实机验证 | `docs/openvela-阶段B-实施记录.md` |
| esp32_ai 应用代码与配置 | ✅ 已就绪（未触板） | `app/esp32_ai/`、`configs/esp32_ai/defconfig` |
| PSRAM 实装容量/稳定读写 | ⏳ 待实测 | P1 门槛 |
| 模型烧录与 `esp32_ai chat` | ⏳ 待实测 | P2 MVP |
| 显示 / 双核 / TinyStories / Fly | ⏳ 未承诺 | P3 |

详细方案、放行条件与分阶段验收见 `board/contest_board/docs/`
（`openvela-板级适配实现方案.md`、`openvela-阶段A/B-实施记录.md`、
`esp32-ai-移植至ESP32-P4X-OpenVela部署方案.md`）。

## 五、AI Coding 使用说明

本作品以“先方案、后代码、按门槛放行”的方式与 AI 协作：

- **需求拆解与方案设计**：先用 AI 起草 OpenVela 移植与 esp32-ai 部署方案（阶段 A/B 实施
  步骤、flash/PSRAM 容量表、分阶段验收标准），明确“任一门槛不满足则停在评估”。
- **编码**：按方案第 4 节的应用代码组织落地 `app/esp32_ai/`（存储视图、分层内存、NSH
  命令、主机校验工具），并同步补 `contest2026_378_model.c` 板级模型存储与 `esp32_ai`
  defconfig；AI 负责生成与自检，人工核对 NuttX 真实 API（如 `esp_spiflash_alloc_mtdpart`、
  `kmm_*` 堆接口）并据此修正。
- **调试/文档**：AI 整理失败日志与解决依据（如镜像偏移、HAL 适配点），并持续更新实机
  记录；明确区分“已验证”与“待实测”，不把跳过/推测写成通过。

AI 对效率的帮助主要体现在：跨仓库 API 调研与方案落地并行推进、代码与文档一致性维护、
以及“不混用 S3 数据/不虚报实机状态”这类约束的持续检查。完整对话日志按组委会工具导出
至 `logs/` 目录随作品提交。

> 本文件为作品说明；构建/烧录细节变更时请以 `board/contest_board/README.md` 与
> `docs/` 内实施记录为准。