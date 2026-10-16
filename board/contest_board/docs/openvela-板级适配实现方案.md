# ESP32-P4X-Function-EV-Board 的 OpenVela 板级适配实现方案

## 1. 目标与范围

本文以本目录中的 `MinerU_markdown_esp-dev-kits-zh_CN-master-esp32p4_(1)_2110708478342160384.md` 为硬件依据，说明如何将 **ESP32-P4X-Function-EV-Board** 适配到 OpenVela/NuttX。

板级适配代码的唯一源目录为：

```text
contest2026_378_zhijudui/board/contest_board/
```

该目录通过 manifest 映射到工作树中的：

```text
vendor/openvela/boards/contest2026_378_board/
```

本队伍仓库中的板级适配内容限制在板级目录及必要的 manifest 映射中。`packages/nuttx/`、`vendor/`、`nuttx/` 等生产仓库内容不得作为队伍仓库的板级改动直接修改或提交；工作树里映射出来的目标路径只用于构建时集成，修改应回到队伍仓库的源目录。若缺少芯片级支持或板卡注册机制，则必须通过相应公共仓库的独立上游流程解决，不能声称仅凭本目录即可完成适配。

本文是实施方案，不代表驱动已经实现或板子已经通过启动验证。当前 `contest_board` 中的 Kconfig、defconfig 和启动代码仍为占位示例，需按下文逐步替换。

## 2. 硬件依据与已知约束

文档描述的目标硬件为 **ESP32-P4X-Function-EV-Board**，不是旧版 ESP32-P4-Function-EV-Board。资料说明该板搭载 ESP32-P4 芯片 v3.1 及之后版本；下载/安全策略必须遵守芯片勘误表及板卡说明，**不要启用安全下载模式**。

| 项目 | 文档中可确认的信息 | 对适配的影响 |
| --- | --- | --- |
| 主控 | ESP32-P4，双核 RISC-V；板卡资料称 P4X 为 v3.1 及之后版本 | 先取得并验证与当前 OpenVela 工作树兼容的 ESP32-P4 架构/芯片支持；板级代码只描述本板配置，不在本目录复制芯片级实现 |
| 时钟 | 40 MHz 主晶振、32.768 kHz 晶振 | 时钟树、低功耗时钟配置以芯片驱动和板卡原理图为准，不仅凭晶振频率猜测初始化值 |
| SPI Flash | 板载 16 MB SPI Flash | 核实启动链、分区/映像布局和驱动配置一致；不要仅因芯片可支持某容量就假定板上存储容量 |
| PSRAM | 芯片资料说明 ESP32-P4 支持最大 32 MB PSRAM | 目标板实际容量及初始化参数需查板卡 BOM/原理图或启动信息后确认，不可直接按最大值配置 |
| 电源域 | 板卡资料注明 LDO_VO3/LDO_VO4 为部分 VDD 电源域供电，需配置正确电压和使能状态 | 查明最小启动所需电源域是否已由 ROM/芯片启动代码配置；启用相关外设前依据原理图核对电压、使能和上电顺序，不凭经验填值 |
| 下载和调试 | 有 USB Serial/JTAG 接口；BOOT 与 Reset 组合可进入固件下载模式 | 首先使用已验证的下载方式完成最小映像烧录和日志观测；不启用安全下载模式 |
| UART 引脚 | J1 表列 GPIO37 为 U0TXD、GPIO38 为 U0RXD | 可作为 UART0 控制台候选引脚；最终控制台路由仍需结合芯片默认路由、板级配置和实机验证 |
| 网络 | 板载 ESP32-C6-MINI-1 用于 Wi-Fi/蓝牙；板载以太网 PHY 为 IP101GR，连接 P4 EMAC RMII | Wi-Fi 需另外确认 P4 与 C6 的通信链路及 OpenVela 驱动；以太网需确认 MAC、RMII 时钟/引脚和 PHY 地址等原理图信息后再启用 |
| 音频 | ES8311 编解码器经 I2C/I2S 连接，NS4150 功放驱动扬声器，板载麦克风接入音频路径 | 需要 codec、I2C/I2S、时钟及功放控制支持，列为后续可选功能，不纳入最小启动条件 |
| 显示与摄像头 | 可选 7 英寸 1024×600 MIPI-DSI 电容触摸屏和 2 MP MIPI-CSI 摄像头 | 屏幕/触摸/摄像头均为可选配件；接口存在不等同于驱动已支持，首版不依赖这些设备启动 |
| 其他接口 | USB 2.0、USB Host、MicroSD（4-bit） | 按驱动可用性分阶段启用；需从原理图确认引脚、供电和卡检测等板级细节 |
| GPIO | J1 引出多路 GPIO，具体针脚见硬件资料 1.1.2「排针」 | 初期只用于可控的外接测试负载；资料中的 5 V 电源指示灯不是可控 GPIO LED，不应当作板载软件 LED 使用 |

如实物型号、PCB 版本或芯片版本与上述资料不一致，应先更新硬件依据，再调整配置。尤其不能把“芯片最大支持能力”误当成“本板已装配能力”。

### 2.1 当前工作树的准入门槛

当前工作树的 `nuttx/arch/risc-v/src/` 中没有 ESP32-P4 芯片目录，`nuttx/arch/risc-v/Kconfig` 中没有 P4 选择项，`vendor/espressif/` 也未提供可复用的 P4 实现。因此“当前仓库已有 ESP32-P4 芯片支持”**不是已满足的前提**。即使其他版本的 NuttX 支持 P4，也必须核实与本次使用的 OpenVela 分支、构建系统及工具链兼容。

进入最小 NSH 实现前，先确认芯片级启动与异常处理、时钟、电源、内存/链接脚本、Flash 映像与烧录、UART 控制台、所需 HAL 和工具链均有可用实现，并记录其来源与版本。若缺失，先通过对应公共仓库的独立 PR/正式上游流程引入并验证；在支持合入或以可复现方式提供之前，不进入“仅改板级目录即可构建”的步骤。

## 3. 目录和配置设计

保留 `board/contest_board/` 作为本作品的单一板级形态。现有 `configs/nsh/`、`src/`、`Kconfig`、`CMakeLists.txt` 是**尚未接入构建**的占位骨架；不需要为了未计划支持的外设新建空目录，也不要删除所选构建方式所需的 `configs/nsh`、`src` 等结构。

现有目录仅为起点，不是保证 Make/CMake 两种构建方式均可运行的“完成清单”：

```text
board/contest_board/
├── CMakeLists.txt
├── Kconfig
├── README.md
├── configs/
│   └── nsh/
│       └── defconfig
└── src/
    ├── CMakeLists.txt
    └── board_boot.c
```

选择 Make 路径时，先对照 `nuttx/tools/configure.sh` 确认本板配置能找到 `Make.defs`，并按芯片/板级构建约定补齐实际需要的 `scripts/Make.defs`、`src/Make.defs`、Makefile、链接相关文件等；不能以现有 CMake 文件代替 Make 接入。选择 CMake 路径时，先确认配置目录的发现方式及芯片支持，再对照 `nuttx/boards/CMakeLists.txt`：它已创建内核目标 `board`，板级 `src/CMakeLists.txt` 应按该版本规则向已有目标加入源文件，**不得再次调用 `nuttx_add_library(board)`**。以一次实际配置和构建分别验证所选路径；未验证的另一种构建方式不列为支持。

### 3.1 统一板级标识

当前占位文件使用 `CONTEST2026_000_BOARD` / `contest2026_000_board`。实现时应统一替换为本队伍和板卡标识，例如：

- 板卡正式注册后的 Kconfig 符号（仅当该符号确实已接入配置树时）：`ARCH_BOARD_CONTEST2026_378_BOARD`
- 对应 defconfig 选择项（同上）：`CONFIG_ARCH_BOARD_CONTEST2026_378_BOARD=y`
- 板级标签：`contest2026_378_board`
- 工作树目标目录：`vendor/openvela/boards/contest2026_378_board`

**映射目录不等于注册板卡。** 当前 `nuttx/boards/Kconfig` 以集中式 `choice` 定义板卡选择项、设置 `ARCH_BOARD` 字符串并显式引入板级 Kconfig；单改本目录的 `Kconfig` 不会让上述新符号自动可选。先明确采用正式注册（如需变更公共配置树，走独立上游流程）还是经验证可用的自定义板卡机制（如 `ARCH_BOARD_CUSTOM` 及其目录配置）；后一种方式不要再把未注册的 `CONFIG_ARCH_BOARD_CONTEST2026_378_BOARD=y` 写进 defconfig。无论采用哪种机制，都应验证生成的 `.config` 中架构、芯片、板级路径及标识实际生效，提交前搜索确认无遗留 `000` 占位符。

### 3.2 配置职责

- `Kconfig`：仅在已明确板级配置树的引入路径后定义本板选项；板级选择项需由配置树正式注册或改用经验证的自定义板卡机制，不能只在本文件中用 `if ARCH_BOARD_...` 包裹标签就认为已注册。不要在这里重定义 ESP32-P4 芯片级配置。
- `configs/nsh/defconfig`：按**实际已注册的板级机制**选择本板，启用最小 NSH 启动所需架构、芯片、串口控制台、时钟和驱动选项。选项名称以当前配置树为准，不复制未经核对的配置项。
- 构建文件：按选定的 Make 或 CMake 约定纳入板级源文件，不重复建 `board` 目标，也不重复引入芯片级库或生产仓库源代码。
- `src/board_boot.c`：先从所用 P4 芯片启动代码追踪早期板级初始化及可选的晚期初始化入口，再实现**与实际调用点匹配**的函数；占位的 `openvela_board_initialize()` 不能作为已接入的证明。仅执行确有必要且有驱动支持的初始化，避免重复初始化串口、时钟和存储或访问未验证的外设。
- `README.md`：记录板卡版本、配置名称、烧录/控制台方式以及已知限制。

## 4. 分阶段实现

### 阶段 0：芯片支持和构建路径准入

1. 按 2.1 节核查当前分支是否具备可工作的 ESP32-P4 芯片支持；若没有，先通过独立上游流程补齐并验证，停止仅板级目录的构建尝试。
2. 确定板卡注册/自定义板卡方式和 Make/CMake 中首个要支持的构建路径；用当前源码确认 Kconfig 引入、配置目录定位、板级源文件编译及启动入口调用链。
3. 确认板卡版本、烧录接口、16 MB Flash 映像布局，以及最小启动所需供电域的初始化责任。未通过这些门槛时，保留占位骨架，不宣称已达到最小启动条件。

### 阶段 A：最小可启动配置

详细实施与验收步骤见 [`openvela-阶段A-最小可启动配置实施步骤.md`](./openvela-阶段A-最小可启动配置实施步骤.md)；必须先通过阶段 0 准入检查。

1. 将占位符板名更新为 `contest2026_378_board`，按选定的板卡机制统一 Kconfig、defconfig、构建条件和目录/映射命名；确保生成配置中的选择项确实生效。
2. 以 `configs/nsh` 作为首个可验证配置，根据已验证的 ESP32-P4 芯片支持选择架构、启动方式、串口和时钟配置，补齐所选构建路径需要的文件。
3. 确认控制台输出使用的 UART 实例、GPIO 复用和波特率。GPIO37/38 是资料给出的 U0TXD/U0RXD 引脚信息，仍需验证是否符合当前芯片驱动的控制台接法。
4. 从芯片启动流程确认真实板级入口与调用时机，只添加最少且实际会执行的板级初始化。若串口、时钟、存储及最小启动所需的电源域已由 ROM/芯片代码负责，板级代码不得重复配置；否则依据芯片资料和原理图明确责任后实现。
5. 用选定的 OpenVela 构建路径生成映像，确认板级代码被编译和调用，再按已验证的烧录方式烧录；通过串口或 USB Serial/JTAG 获取冷启动、复位和 NSH 启动日志。

### 阶段 B：基础板级外设

逐项实施与验收步骤见 [`openvela-阶段B-基础板级外设实施步骤.md`](./openvela-阶段B-基础板级外设实施步骤.md)；必须先完成阶段 A 的可交互 NSH 基线。

最小系统启动稳定后，按硬件资料和原理图逐个添加并测试 GPIO、按键、存储卡、USB 或以太网等功能。每个功能需先确认：

- 对应驱动已经存在于当前 OpenVela/NuttX 版本；
- 本板引脚复用、极性、时钟、供电和复位关系有原理图依据；
- 板级配置只在本板 defconfig 中启用；若确需芯片/公共代码支持，作为独立上游变更处理，而非直接修改生产仓库；
- 单独验证不影响冷启动、复位和已有控制台功能。

启用依赖特定 VDD 电源域的外设前，应根据原理图验证 LDO_VO3/LDO_VO4 的电压、使能和上电顺序。以太网应先核对 IP101GR 的 PHY 地址、RMII 时钟来源、复位/中断连接和 EMAC 引脚。Wi-Fi/蓝牙不能仅因板载 ESP32-C6 模组而在 P4 上直接启用；需先明确模组通信接口、固件/协议及对应驱动。音频、MIPI-DSI/CSI、触摸和 H.264 等功能作为独立后续工作项，不作为本板达到最小适配的前置条件。

## 5. Manifest 映射与作品目录规则

当前 `contest2026_378_zhijudui.xml` 已包含本板所需映射：

```xml
<linkfile src="board/contest_board" dest="vendor/openvela/boards/contest2026_378_board"/>
```

因此，当前作品不需要重复增加第二条相同映射。新增作品/板级形态时，按“一种作品一个队伍仓库子目录、一个明确的工作树目标路径”增加目录，并在同一 `<project>` 下增加对应的 `<linkfile>`，例如：

```xml
<linkfile src="board/<新形态目录>" dest="vendor/openvela/boards/contest2026_378_<新形态名>"/>
```

如果某个形态目录不再使用，可删除该形态的队伍仓库目录并移除它在 XML 中的映射；不要因此删除正在使用的 `contest_board`，也不要删除单个板级形态内部的必需构建目录。确认 `src` 存在、`dest` 不冲突，并遵从 repo manifest 对 `<linkfile>` 的路径约束。

## 6. 仓库边界与禁止事项

允许变更的范围：

- `contest2026_378_zhijudui/board/contest_board/**`
- 只有新增/删除作品形态时，才相应调整 `contest2026_378_zhijudui/contest2026_378_zhijudui.xml` 中的 `<linkfile>`。

禁止直接修改或提交：

- `packages/nuttx/**`、`packages/**` 中的生产包代码；
- `vendor/**`、`nuttx/**` 中的生产仓库/公共实现；
- 通过映射暴露的 `vendor/openvela/boards/contest2026_378_board` 工作树目标副本。

本地工作树已发现 ESP32-P4 公共芯片支持缺失；这是实施门槛而不是构建失败后再处理的可选事项。芯片级实现、板级集中注册等如需改动公共仓库，应通过相应生产仓库的正式上游流程提交和验证；不得为完成本队伍板级适配而在上述生产目录打补丁，也不得把仅在队伍仓库中存在的板级文件误报为可独立构建。

## 7. 验收清单

- [ ] 板卡具体型号、PCB 版本和 ESP32-P4 芯片版本与实施目标相符。
- [ ] 已确认并记录当前分支可用的 ESP32-P4 芯片启动、工具链、内存/Flash 映像与串口支持；缺失部分的上游变更已可复现地集成。
- [ ] 已确定正式注册或自定义板卡机制，生成的 `.config` 中架构、芯片、板级目录及选择项确实生效；板级名称一致且无残留占位标识。
- [ ] 已明确支持 Make 或 CMake 中的哪条构建路径；所需 Make.defs/Makefile 或 CMake 源文件接入已验证，无重复 `board` 目标。
- [ ] OpenVela/NuttX 能选用本板的 NSH 配置并完成构建，板级启动入口确实由芯片启动流程调用。
- [ ] 按板卡手册完成烧录；冷启动和复位后均能稳定进入 NSH。
- [ ] 控制台 UART/USB 路由、引脚和波特率已在实机上验证。
- [ ] 没有启用不受支持的安全下载模式；Flash/PSRAM 配置及 LDO_VO3/LDO_VO4 的电压、使能责任有实际硬件依据。
- [ ] 后续外设逐项验证，并有对应原理图依据和独立测试记录；未支持功能未被误报为已适配。
- [ ] manifest 映射指向队伍仓库源目录；不存在目标路径冲突或重复映射。
- [ ] 变更仅涉及队伍仓库板级目录和必要的 manifest 文件；`packages/nuttx/`、`vendor/`、`nuttx/` 无直接改动。

## 8. 参考资料

- 本目录 `MinerU_markdown_esp-dev-kits-zh_CN-master-esp32p4_(1)_2110708478342160384.md`：尤其是 ESP32-P4X-Function-EV-Board 概述、组件介绍、硬件参考、排针及硬件版本章节。
- `contest2026_378_zhijudui.xml`：当前队伍仓库的板级 `<linkfile>` 映射。
- `nuttx/arch/risc-v/src/`、`nuttx/arch/risc-v/Kconfig`、`nuttx/boards/Kconfig`、`nuttx/boards/CMakeLists.txt` 与 `nuttx/tools/configure.sh`：当前工作树的芯片支持、板卡发现和构建路径核查依据；芯片支持状态可能随上游版本变化，实施前应重新核对。
