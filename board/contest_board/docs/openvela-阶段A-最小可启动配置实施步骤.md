# 阶段 A：ESP32-P4X-Function-EV-Board 最小可启动配置实施步骤

本文是 [`openvela-板级适配实现方案.md`](./openvela-板级适配实现方案.md) 中阶段 A 的操作清单，目标是让 **ESP32-P4X-Function-EV-Board** 在 OpenVela/NuttX 上冷启动和复位后进入可交互 NSH。当前板级文件均为占位示例；以下是待实施步骤，**不是已完成的配置、构建或烧录记录**。

## 0. 边界与放行条件

- 板级文件只在 `contest2026_378_zhijudui/board/contest_board/` 中维护，经现有 manifest `<linkfile>` 映射到 `vendor/openvela/boards/contest2026_378_board/`。不要直接编辑映射目标，也不要在 `nuttx/` 或其他生产仓库打临时补丁后将其冒充为队伍仓成果。
- **先完成主方案阶段 0。** 目前 `nuttx/arch/risc-v/src/` 无 ESP32-P4 芯片实现，`nuttx/arch/risc-v/Kconfig` 无 P4 芯片选择项，`vendor/espressif/` 也没有可复用的 P4 实现。没有可复现集成的芯片级支持时，停止在本节，不填写臆造的 `CONFIG_ESP32P4_*`，也不宣称阶段 A 已通过。
- 芯片级支持如需新增，走相应公共仓库的独立上游 PR；本文只描述依赖可用**之后**的板级实施。板卡集中注册若需要公共仓库变更，也遵循同一边界。

在队伍仓库的板级 README 或实施记录中填写下表，空项即未放行：

| 放行项 | 核实方式 | 记录内容 |
| --- | --- | --- |
| 硬件 | 对照实物丝印、板卡资料和原理图 | 板卡/PCB/芯片版本；确认是 P4X 而非旧版 P4 板 |
| 芯片支持 | 检查实际集成的架构目录、Kconfig、构建文件与启动链 | 代码来源、版本/PR、可用工具链、所支持的构建方式 |
| 启动依赖 | 检查复位入口、内存/链接、时钟、供电、串口及 Flash 代码 | 各初始化责任归属；仍缺的驱动或 HAL |
| 烧录与控制台 | 对照板卡资料和工具说明 | 映像组成与偏移、烧录工具及芯片参数、物理 USB/UART 端口；**不启用安全下载模式** |
| 电源域 | 对照参考设计和芯片启动代码 | 最小启动涉及的 `LDO_VO3/LDO_VO4` 电压、使能是否已有责任方；未确认不得猜值 |

放行判据：上述条目能指向当前工作树中**实际可用**的代码、资料或已集成的上游变更，而不只是另一份 NuttX 的示例。板载 Flash 是 16 MB；PSRAM 实装容量未知，最小启动不应依赖未经确认的 PSRAM 配置。

## 1. 固定板卡发现方式与构建路径

先只选**一种**构建路径验证。建议优先评估 CMake + 自定义板卡路径，因为本队伍目录已有 CMake 骨架，且当前 `nuttx/CMakeLists.txt` 支持通过配置路径定位板目录并用 `CONFIG_ARCH_BOARD_CUSTOM_DIR` 指定板目录。此建议以所取得的 P4 芯片支持兼容该路径为前提；如果仅支持 Make，则改按第 4 节的 Make 分支，不同时声称两条路径已适配。

### 路径 1：自定义板卡（优先核验）

1. 确认 manifest 映射后的目标目录确实存在且指向队伍仓库源目录；`<linkfile>` 只负责映射，**不会自动注册 Kconfig 板卡选择项**。映射未出现时先排查 repo 同步/冲突，不手工复制到 `vendor/`。
2. 核对当前 `nuttx/boards/Kconfig` 的 `ARCH_BOARD_CUSTOM`、`ARCH_BOARD_CUSTOM_DIR`、`ARCH_BOARD_CUSTOM_DIR_RELPATH`、`ARCH_BOARD_CUSTOM_NAME` 的实际语义，并核对所选构建路径是否处理了自定义板卡。自定义路径必须在生成的配置中指向本板真实目录；相对路径的基准按该版本构建代码确认。若使用绝对路径，不要提交与开发者机器绑定的路径。
3. 自定义模式**不要**把未注册的 `CONFIG_ARCH_BOARD_CONTEST2026_378_BOARD=y` 放进 defconfig。确认本目录 `Kconfig` 是否经自定义板卡路径引入；原骨架只在 `if ARCH_BOARD_CONTEST2026_000_BOARD` 下声明标签，在自定义模式下需要按实际引入方式调整或删去无效的守卫。
4. 生成 `.config` 后检查板卡选择、目录路径及 NSH 配置是否保留；若被 Kconfig 消解、目录指向别处或板级 CMake 未进入构建，暂停并回到发现机制排查。

### 路径 2：正式注册板卡

仅当当前分支已有本板正式注册，或对应公共仓库变更已按上游流程集成时使用。核对板卡选择项、`ARCH_BOARD` 的取值、板级 `Kconfig` 的 `source` 和配置路径映射均已接入；此时才在 defconfig 使用实际已注册的 `CONFIG_ARCH_BOARD_CONTEST2026_378_BOARD=y`。**只改本目录 `Kconfig` 或只添加 manifest 映射，不满足正式注册条件。**

## 2. 替换占位配置并建立最小 NSH defconfig

按以下顺序编辑队伍仓库中的文件，不直接修改映射目标：

1. 在 `Kconfig`、`CMakeLists.txt`、`src/CMakeLists.txt`、`configs/nsh/defconfig`、`README.md` 和启动源文件中清理 `CONTEST2026_000_BOARD`、`contest2026_000_board` 等占位内容。标识 `contest2026_378_board` 只是命名建议；实际配置符号取决于第 1 节选择的发现方式。
2. 以**已集成**的 ESP32-P4 芯片实现和其可用示例配置为依据，逐项选择架构、芯片型号、工具链、RAM/链接、时钟、Flash 映像和串口相关选项；用 `nuttx/boards/risc-v/esp32c6/esp32c6-devkitc/configs/nsh/defconfig` 只参考通用 NSH 配置形式，**不得把 C6 专属选项套用到 P4**。
3. 使配置只启用最小系统：启动入口、NSH、可输入输出的控制台及芯片启动所需能力。先关闭尚未验证的 PSRAM、以太网、C6 无线模块、音频、SD 卡、USB Host、显示和摄像头功能。不要将 5 V 电源指示灯当软件 LED 使用。
4. 逐项检查 Kconfig 的实际生效结果：`CONFIG_ARCH`、芯片选择与芯片目录、板卡选择与目录、控制台实例、NSH 启动入口、所选构建方式需要的工具链和映像配置。仅有 defconfig 文本不算通过；保存生成配置/构建日志中这些项目的核查结果。

**本阶段不预填 P4 专属 Kconfig 名称、RAM 地址、波特率、Flash 偏移或 PSRAM 容量。** 获取实际芯片支持和参考设计后，记录每个最终取值的来源；未知项是阻塞项，不用其他型号的默认值顶替。

## 3. 确认控制台与板级启动入口

1. 在实际 P4 启动代码中沿复位入口追踪到 UART 早期输出、NuttX 控制台注册、板级早期入口和可选的晚期入口；列出函数名、调用条件和初始化顺序。原 `src/board_boot.c` 中的 `openvela_board_initialize()` 只是占位函数，**函数被编译不等于被调用**。按所用芯片实现中的真实入口命名和声明，先做到最小空实现；需要的板级动作再增量加入。
2. 按板卡资料，J1 的 **8 号针脚为 GPIO37/U0TXD，10 号针脚为 GPIO38/U0RXD**。它们只是 UART0 的候选接线；确认芯片驱动选择了相同 UART、管脚复用和电平，才将其作为 NSH 控制台。外接 USB-UART 时按实物连 TX→RX、RX→TX 和共地，并使用 3.3 V 电平；不要把排针 5 V 接入 GPIO。
3. 板上的 USB Serial/JTAG 接口可作为下载/调试口，但不自动等于 NuttX 的 NSH 控制台。先选定且验证**一条** NSH 输入/输出通路，分别记录下载口和控制台口，避免只看到 ROM 日志便误判 NSH 已启动。
4. 若芯片/ROM 已完成时钟、Flash、UART 或最小启动所需的电源初始化，板级不得重复配置。若某电源域必须在板级启用，应先从原理图及 P4 芯片资料取得电压/顺序依据，明确与现有启动代码的责任边界后实施。

阶段性检查：板级入口在启动链上有明确调用点；控制台的 TX 输出和 RX 输入均有可测试路径；无未验证的外设初始化。

## 4. 接通所选构建系统

### CMake 分支

- `nuttx/boards/CMakeLists.txt` 已调用 `nuttx_add_kernel_library(board ...)`；原 `src/CMakeLists.txt` 的 `nuttx_add_library(board)` 会重复创建同名目标。确认当前构建链确实进入板级目录后，改为按该版本约定向已有 `board` 内核目标添加 `board_boot.c`，例如 `target_sources(board PRIVATE board_boot.c)`；不要在本板目录再建同名库。
- 检查顶层板级 `CMakeLists.txt` 是否实际进入 `src/`，并确认源文件编译命令、最终链接结果和对应入口符号。若 CMake 报板目录不存在、Kconfig 未选中或芯片目录不存在，先修复各自的发现/依赖问题，不能靠修改板级名称掩盖。
- 完成第 0～3 节后，在 **openvela 工作树根目录**使用已确认存在的映射路径配置和构建：

  ```sh
  ./build.sh vendor/openvela/boards/contest2026_378_board/configs/nsh --cmake -j8
  ```

  该命令是待执行步骤，不表示当前工作树可以成功构建。执行前核对 `build.sh` 对该配置路径的处理、所需 CMake/Kconfig 工具与已集成的 P4 工具链；记录完整命令、退出码及产物位置。

### Make 分支（仅当芯片实现支持此路径）

- `nuttx/tools/configure.sh` 对自定义配置路径会检查 `Make.defs`；当前板级目录仅有 CMake 骨架，**直接运行 Make 构建会缺文件**。对照该版本相近的、确实使用 Make 的板级实现，补齐配置路径可定位的 `Make.defs`、板级 `src/Make.defs`、必要的 Makefile 和链接支持；检查板级对象被编译并链接，不能仅配置成功。
- 接入完成后使用工作树根目录的配置路径调用 `./build.sh vendor/openvela/boards/contest2026_378_board/configs/nsh -j8`。注意 `build.sh` 会运行 `savedefconfig` 并可能写回队伍仓库中的 `configs/nsh/defconfig`：对照原配置审阅变化，检查关键选项未被丢弃。不要使用不存在的 `board:config` 简写或未验证的输出路径。

对所选分支，配置失败先核对芯片/板卡 Kconfig，CMake 目标冲突先核对重复 `board`，Make 找不到 `Make.defs` 先核对文件位置；**不要**为绕过诊断去复制 C6 芯片文件或直接修改生产仓库。

## 5. 映像、烧录与实机验证

1. 从**当前 P4 芯片实现**的链接脚本和构建产物确认复位入口、Flash/内存段与映像格式；再用板卡参考设计确认 16 MB Flash 的映像布局。启动加载器/分区/应用映像需要几个文件、各自烧录偏移是什么，以实际构建产物和芯片工具说明为准，不能照搬 ESP32-C6 的 `0x0` 等示例。
2. 记录烧录工具及版本、实际芯片参数、串口设备、所需输入映像及各偏移；核对板卡的 BOOT + Reset 进入下载模式的方法，**不要启用安全下载模式**。没有可信烧录布局时停止，不试写未知地址。
3. 先连接选定控制台获取日志，分别进行断电重上电和复位验证；完整记录 ROM/启动器日志、NuttX 日志及 NSH 提示符，并在 NSH 执行一条无副作用的命令（例如 `help`）验证键盘输入与输出。仅串口有输出、仅 USB 枚举或仅看到提示符但无法交互都不算通过。
4. 若启动失败，按“无 ROM 日志 → ROM 有而 NuttX 无 → NuttX 有而 NSH 无 → NSH 无法输入”的阶段保存原始日志，依次核对供电/端口、映像偏移/入口、芯片与板级初始化、控制台 RX 与启动配置。每次只调整有证据指向的配置，再重复冷启动和复位测试。

## 6. 交付与验收

- [ ] 阶段 0 的芯片支持来源、板卡版本、烧录布局和电源责任有可复现记录；依赖的公共仓库变更不混入队伍板级提交。
- [ ] manifest 映射存在且目标不冲突；所有板级改动回到 `contest_board/`；无 `000` 占位符。
- [ ] 生成的配置确实选中了 P4 芯片、正确的板目录、控制台及 NSH；未知芯片选项没有靠猜测填值。
- [ ] 所选 Make 或 CMake 路径能够完成配置与链接，板级启动入口已确认实际被调用；未测试的另一条路径不宣称支持。
- [ ] 依据真实产物和参考设计烧录，未启用安全下载模式；冷启动与复位后均能进入可交互 NSH。
- [ ] 控制台 TX/RX、接口、电平、波特率和日志均有实机记录；Flash/PSRAM 及最小启动电源配置有来源。
- [ ] `README.md` 记录构建命令、工具版本、映像/烧录步骤、控制台接法、验证结果和未支持功能；保留失败日志及解决依据供复现。

未通过第 0 节放行或没有实机日志时，阶段 A 状态为**阻塞/未验证**，不得标记“最小适配完成”。

## 参考路径

- `contest2026_378_zhijudui/board/contest_board/docs/openvela-板级适配实现方案.md`：总方案和仓库边界。
- 本目录的 `MinerU_markdown_esp-dev-kits-zh_CN-master-esp32p4_(1)_2110708478342160384.md`：P4X 板卡、J1 引脚、下载限制及电源注意事项。
- `nuttx/boards/Kconfig`、`nuttx/CMakeLists.txt`、`nuttx/boards/CMakeLists.txt`、`nuttx/tools/configure.sh`、`build.sh`：以实际分支源码确认配置和构建行为；上游升级后重新核对。
