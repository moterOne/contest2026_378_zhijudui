# contest_board（ESP32-P4X-Function-EV-Board 板级适配）

本目录是本次比赛 378 队的板级适配形态，映射到 openvela 工作树的
`vendor/openvela/boards/contest2026_378_board`（见 `contest2026_378_zhijudui.xml`
的 `<linkfile>`）。

- 板卡：ESP32-P4X-Function-EV-Board（ESP32-P4，芯片版本 v3.1 及之后；按 rev3 构建）
- 形态名：`contest2026_378_board`
- 目标配置：`configs/nsh`（最小可启动、进入可交互 NSH）
- 构建方式：out-of-tree 自定义板卡（`CONFIG_ARCH_BOARD_CUSTOM`），仅 **Make** 路径
- 控制台：USB-Serial/JTAG（主机侧 `/dev/ttyACM0`），`CONFIG_ESPRESSIF_USBSERIAL=y`

> 详细的放行条件、构建/烧录命令、实机日志、失败记录与验收清单见
> [`docs/openvela-阶段A-实施记录.md`](docs/openvela-阶段A-实施记录.md)；
> 基础外设逐项记录见 [`docs/openvela-阶段B-实施记录.md`](docs/openvela-阶段B-实施记录.md)。

## 构建

```sh
# 在工作树根目录
./build.sh vendor/openvela/boards/contest2026_378_board/configs/nsh -j8
```

- 工具链：`prebuilts/gcc/linux-x86_64/riscv-none-elf`（gcc 13.4.0）
- 产物：`nuttx/nuttx`（ELF）、`nuttx/nuttx.bin`（ESP32-P4 镜像）
- 构建结束时 `make savedefconfig` 应报告 `No configuration change.`（defconfig 未被改写）

## 烧录

ESP32-P4 使用 simple-boot：单个平面镜像烧到 flash 偏移 **0x2000**
（ESP32-P4 ROM 从 0x2000 读取启动镜像），**不启用安全下载模式**（勘误表 ROM-770）。

```sh
# 工具：esptool 4.12.0；串口：USB-Serial/JTAG
esptool.py -c esp32p4 -p /dev/ttyACM0 -b 460800 write_flash 0x2000 nuttx/nuttx.bin
```

BOOT + Reset 进入下载模式。下载口与控制台口是同一个 USB-Serial/JTAG 接口。

## 控制台

- 选定通路：芯片驱动 `arch/risc-v/src/common/espressif/esp_usbserial.c`
  （`g_uart_usbserial.isconsole = true`），主机侧 `/dev/ttyACM0`。
- 未启用 `CONFIG_ESPRESSIF_UART0`；J1 的 GPIO37/U0TXD、GPIO38/U0RXD（资料值）未作为 NSH 控制台。

## 验证结果（实机）

- 烧录/复位后进入**可交互 NSH**；`uname -a` → `... risc-v contest2026_378_board`，`help` 可交互。
- **冷启动（断电重上电）与复位**后均能进入可交互 NSH（已实测）。
- ROM 级信息（esptool）：`ESP32-P4 (revision v3.2)`、`Crystal 40MHz`、`USB mode: USB-Serial/JTAG`、
  `MAC e8:f6:0a:e3:a9:3a`。（USB-Serial/JTAG 复位即重枚举，ROM 早期横幅难以在主机侧捕获。）

## 基础外设（阶段 B）

首项接入 **GPIO**（实机已通过）：

- 输出：GPIO2（J1 pin 21）、GPIO3（J1 pin 19）；中断输入：GPIO23（J1 pin 7）。
- 生成配置新增：`CONFIG_DEV_GPIO=y`、`CONFIG_ESPRESSIF_GPIO_IRQ=y`、`CONFIG_EXAMPLES_GPIO=y`、
  `CONFIG_BOARD_LATE_INITIALIZE=y`（使 `esp_bringup()` → `esp_gpio_init()` 在 late-init 执行）。
- 节点：`/dev/gpio0`（GPIO2）、`/dev/gpio1`（GPIO3）、`/dev/gpio2`（GPIO23）；NSH `gpio` 命令读写。
- 实机：`/dev/gpio0..2` 均出现；`gpio -o 1 /dev/gpio0` 回读 `Verify: Value=1`，写/读一致；
  控制台与 NSH 无退化。
- 以太网：驱动存在、可构建，但启用 `CONFIG_NET` 后固件**启动静默、不进入 NSH**（fork 级问题），
  已按步骤回退，未纳入交付。
- MicroSD：**缺 ESP32-P4 SD/MMC 控制器驱动**（公共驱动缺失），阻塞。
- 按键：仅有 Reset/BOOT 两键，按步骤**不支持板载用户键**。
- USB：需参考设计先区分接口与供电，阻塞。
- 详见 [`docs/openvela-阶段B-实施记录.md`](docs/openvela-阶段B-实施记录.md)。

## 依赖说明

ESP32-P4 的芯片级支持（`arch/risc-v/src/esp32p4`、`arch/risc-v/src/common/espressif`、
`arch/risc-v/include/esp32p4`、`boards/risc-v/esp32p4` 等）属于公共仓库（`nuttx/`）的上游变更，
**不在本队伍板级目录内维护**；已整理为独立补丁序列
`/ESP32-P4/_p4_upstream_patches/esp32p4-support/`，走 `nuttx` 公共仓库的独立上游流程（PR/分支）。

## 已知限制与未支持功能

- 已适配：最小 NSH 启动与命令交互、**GPIO（阶段 B）**。未支持：按键/存储/以太网/音频/显示/摄像头等。
  （外设项仅在满足各自验收后才标记“已适配”。）
- **PSRAM 未启用**（实装容量未知，最小启动不依赖）。
- **CMake 构建路径未验证**（`src/CMakeLists.txt` 缺失）。
- 镜像头 Flash 大小字段当前为 4 MB（默认），板载为 16 MB；simple-boot 单平面镜像不走分区表，未影响启动。
- `CONFIG_BOARD_LATE_INITIALIZE` 已在阶段 B（GPIO）开启，`esp_bringup()` 会在 late-init 运行（挂载 procfs、注册 GPIO 等）。
- `LDO_VO3/LDO_VO4` 未由板级启用；启用依赖这些电源域的外设前需按原理图核对。

失败日志与解决依据（基线 HAL 不可构建、HAL 平台层 API 适配、rv32 无 libatomic、0x2000 偏移、
`build.sh -e` 的 distclean 问题、软复位后主机 xHCI 掉线等）见
[`docs/openvela-阶段A-实施记录.md`](docs/openvela-阶段A-实施记录.md) §5.2。
