# 阶段 B 实施记录：基础板级外设（ESP32-P4X-Function-EV-Board）

本文件按 [`openvela-阶段B-基础板级外设实施步骤.md`](./openvela-阶段B-基础板级外设实施步骤.md)
的“每项外设一张卡”循环，逐项记录基础外设的接入与验证。阶段 A 的基线与放行记录见
[`openvela-阶段A-实施记录.md`](./openvela-阶段A-实施记录.md)。

**范围**：GPIO、按键、MicroSD、USB、以太网。**不含** ESP32-C6 Wi‑Fi/蓝牙、ES8311/NS4150 音频、
MIPI DSI/CSI、触摸、H.264。

**边界**：板级改动只在 `contest2026_378_zhijudui/board/contest_board/`。芯片/公共驱动缺失时按公共
仓库独立上游流程处理，不复制其他 ESP32 芯片驱动。

## 状态总览

| 功能 | 状态 | 说明 |
| --- | --- | --- |
| GPIO | **通过**（实机） | §1；`/dev/gpio0..2` 读写回读一致，NSH/控制台无退化 |
| 以太网 | **阻塞**（已尝试，未通过） | §2；驱动存在、可构建，但启用 `CONFIG_NET` 后固件**不进入 NSH（启动静默）**，属 fork 级问题，待深查 |
| MicroSD | **阻塞** | §3；当前树中**没有 ESP32-P4 的 SD/MMC 控制器驱动**（`CONFIG_ESPRESSIF_SD*` 不存在） |
| 按键 | **不支持板载用户键** | §4；仅有 Reset（复位）与 BOOT（下载模式）两键，按步骤 §3 不注册为普通用户键 |
| USB | **阻塞** | §5；需参考设计先区分 USB Serial/JTAG、USB FS、USB 2.0 OTG/Host 与 VBUS 开关 |

阶段 A 基线（用于回归对照）：`nuttx` base `76354c63785`；控制台 USB‑Serial/JTAG `/dev/ttyACM0`；
烧录偏移 `0x2000`；Make 构建路径；镜像 `nuttx/nuttx.bin`。

> 结论：阶段 B 中只有 **GPIO** 完成完整闭环并实机通过；其余各项按真实状态标为阻塞/不支持，
> 未以其他板的代码示例代替实机验证。失败的以太网尝试已按步骤 §1 要求**回退**（未纳入交付）。

---

## 1. GPIO（首项，已通过）

| 字段 | 内容 |
| --- | --- |
| 硬件依据 | 板卡 ESP32‑P4X‑Function‑EV‑Board；J1 排针表（本目录 MinerU 文档「排针」节）。选定 J1 引出且**不落在 ESP32‑P4 默认外设引脚映射**中的引脚：**GPIO2（J1 pin 21）**、**GPIO3（J1 pin 19）** 作输出；**GPIO23（J1 pin 7）** 作中断输入。J1 类型 I/O/T，3.3 V I/O（不使用 5 V）。未涉及 VDD 电源域/LDO_VO3/VO4 |
| 软件依据 | P4 GPIO 控制器：`arch/risc-v/src/esp32p4/` + `common/espressif/esp_gpio.c`；NuttX GPIO 驱动 `CONFIG_DEV_GPIO`（节点 `/dev/gpioN`）；板级 `src/contest2026_378_gpio.c`；中断 `CONFIG_ESPRESSIF_GPIO_IRQ`；NSH 测试工具 `CONFIG_EXAMPLES_GPIO`（`apps/examples/gpio`） |
| 资源冲突 | 未占用控制台/启动管脚/已用外设引脚。排除：GPIO37/38（UART0）、GPIO0/1/45（J1 标 NC）、GPIO36（UART0 CTS）；以太网 RMII 默认 28–31/34/35/49–52、SPI2 28–31、SPI3 20/21/22/27、LPSPI0 4/6/7/8、LPI2C0 4/5、MCPWM 10/11、LPUART0 14/15。与 BOOT/Restore 键无关 |
| 接入点 | Make：`src/Make.defs` 在 `CONFIG_DEV_GPIO` 时编译 `contest2026_378_gpio.c`；调用：`board_late_initialize()` → `esp_bringup()` → `esp_gpio_init()`（新增 `CONFIG_BOARD_LATE_INITIALIZE=y`） |
| 测试 | `/dev/gpio0`(GPIO2,out)、`/dev/gpio1`(GPIO3,out)、`/dev/gpio2`(GPIO23,int in) |
| 回归 | 冷启动/复位、NSH 输入输出、控制台与烧录路径保持（见下） |

**改动**（仅在队伍板级目录）：
- `src/contest2026_378_gpio.c`：输出脚 `GPIO2`/`GPIO3`，中断 `GPIO23`；RTC‑GPIO 路径关闭（骨架默认脚 GPIO0 未确认，不盲驱）。
- `configs/nsh/defconfig`：新增 `CONFIG_BOARD_LATE_INITIALIZE=y`、`CONFIG_DEV_GPIO=y`、
  `CONFIG_ESPRESSIF_GPIO_IRQ=y`、`CONFIG_EXAMPLES_GPIO=y`。

**实机结果（2026‑10‑16，通过）**——烧录 `nuttx/nuttx.bin` @ 0x2000 后，NSH：

```text
nsh> uname -a
NuttX 0.0.0 76354c63785-dirty ... risc-v contest2026_378_board
nsh> ls /dev
/dev:
 console  gpio0  gpio1  gpio2  null  oneshot  random  ttyACM0  zero
nsh> gpio /dev/gpio0
  Output pin:    Value=0
nsh> gpio -o 1 /dev/gpio0
  Writing:       Value=1
  Verify:        Value=1
nsh> gpio -o 0 /dev/gpio0
  Verify:        Value=0
nsh> gpio -o 1 /dev/gpio1
  Verify:        Value=1
nsh> gpio /dev/gpio2
  Interrupt pin: Value=1
```

- 三节点均出现 → late‑init 的 `esp_gpio_init()` 确实运行；输出写/读回一致；中断输入读到电平；NSH 无退化。
- 补充（需实机旁操作，未做）：万用表测 J1 pin21/pin19 电平；用受控 3.3 V 输入/信号验证 `/dev/gpio2` 的读取与中断触发沿。

**已知限制**：引脚依据为“J1 引出 + 不在 P4 默认外设映射”，尚缺本板原理图页号复核；未启用 RTC‑GPIO 输出路径。

---

## 2. 以太网（IP101GR + P4 EMAC RMII）——已尝试，未通过，已回退

按步骤 §6 以“先无网线启动仍进入 NSH”为第①层测试。**结果：未通过**，故按步骤 §1 回退，不纳入交付。

**硬件依据**：本板以太网为 IP101GR + P4 EMAC RMII（MinerU 文档「组件介绍」）；PHY 地址绑带、
RMII 参考时钟方向/频率、复位/中断未在本目录资料中给出，缺原理图页号依据。

**软件依据（存在）**：`CONFIG_ESPRESSIF_EMAC`（依赖 `ARCH_CHIP_ESP32P4`，select `NET`/`ARCH_PHY_INTERRUPT`/
`ESPRESSIF_HR_TIMER`/`SCHED_LPWORK`）；驱动 `common/espressif/esp_emac.c`（netdev‑lowerhalf + 通用 PHY
`esp_eth_phy_new_generic`）；芯片 Kconfig 已给出 P4 的 RMII/MDC/MDIO/PHY_RST 默认引脚与 PHY 地址默认 1。
板级 `src/contest2026_378_ethernet.c` 的 `board_emac_init()` 由 `esp_bringup()` 调用。

**做了什么**：新增第二个配置 `configs/net`（= nsh 基线 + `ESPRESSIF_EMAC` + net 栈 + DHCPC/netinit），
构建通过、烧录成功；为定位问题还临时加了 `board_emac_init` 的打印与 `SYSLOG_CONSOLE`。

**现象（关键失败记录）**：烧录后固件**完全没有串口输出**，`/dev/ttyACM0` 稳定枚举但读不到任何字节
（多次尝试、20s 抓取窗口均为 0 字节）；NSH 提示符不出现。逐步隔离发现：

1. 完全版 net 配置（EMAC + DHCPC + netinit + LATEINIT）→ 静默。
2. 去掉 DHCPC/netinit（保留 EMAC + LATEINIT + net 栈）→ 静默。
3. **去掉 EMAC**（仅 `CONFIG_NET` + `NETDEV_LATEINIT`）→ **仍静默**。

即**启用 `CONFIG_NET` 本身**就使本 fork 的固件在启动阶段不再进入 NSH。这是 fork 级问题（与本板引脚无关），
需要按步骤 §1「先恢复基线，再调查」继续定位（建议：JTAG/GDB 或临时在 `net_initialize`/`up_initialize`
加 `ARCH_LOWPUTC` 级早期打印）。

**附带发现并修复的 fork 兼容缺陷（属公共仓库变更）**：ESP‑HAL 平台层
`esp-hal-3rdparty/nuttx/include/platform/os.h` 的 `esp_os_task_delay_adapter()` 调用 `nxsched_usleep()`，
而本 fork 无该符号（上游 NuttX 有）→ 一旦链路进入 eFuse（`esp_read_mac(ESP_MAC_ETH)`）等路径即
`undefined reference to 'nxsched_usleep'`。已新增 HAL 补丁
`arch/risc-v/src/common/espressif/esp-hal-patches/0002-nuttx-platform-os-header-delay.patch`
（`nxsched_usleep` → `usleep`，并补 `<unistd.h>`）。该补丁属**公共仓库**改动，不进入队伍板级目录。

**回退**：删除实验配置 `configs/net`；`src/contest2026_378_ethernet.c` 的临时调试打印已还原。
基线 `configs/nsh` 未受影响，重新烧录后 NSH 正常（17:48 版）。

**结论**：以太网**阻塞**——驱动存在且可构建，但本 fork 启用 `CONFIG_NET` 后启动不进入 NSH，需先解决
该 fork 级问题方可继续（PHY 地址/时钟等仍须原理图确认）。

---

## 3. MicroSD——阻塞（缺芯片级驱动）

资料说明本板有 4‑bit MicroSD 卡槽（MinerU 文档「组件介绍」#28），但**未给出**本 PCB 的 CMD/CLK/D0–D3/
卡检测/上拉/供电/复位时序。

更关键的是**软件依据缺失**：检查当前树，`arch/risc-v/src/esp32p4/` 与 `common/espressif/` 中**没有
ESP32-P4 的 SD/MMC（SDHCI/SDMMC）控制器驱动**，也没有 `CONFIG_ESPRESSIF_SD*` 选项
（`nuttx/drivers/mmcsd/` 只有通用 SDIO 层，需芯片低层驱动配合）。按步骤 §4.2 与 §0 边界，
芯片级驱动缺失时**停在核查阶段**，不在板级目录伪造寄存器驱动。

**待办**：通过 `nuttx` 公共仓库的独立上游流程引入 P4 SD/MMC 驱动；再依原理图确认引脚/供电/卡检测后接入。

---

## 4. 按键——不支持板载用户键

按步骤 §3：Reset 键用于复位、BOOT 键控制下载模式（MinerU 文档「组件介绍」#5/#10）。资料未显示
本板有独立的板载用户键；在核对原理图连接/上拉/启动采样前，**不将 Reset/BOOT 注册为普通用户键**，
也不在启动阶段改变其电平。故本项按步骤 §3.3 标注为“**不支持板载用户按键**”。

如需用户键，可用符合 3.3 V 的 J1 外接按键（记录引脚、极性、上下拉、消抖/中断）后再接入。

---

## 5. USB——阻塞（需先分清接口）

本板有 USB Serial/JTAG（下载/调试，已作为控制台）、USB 全速接口、USB 2.0 OTG（Type‑C Device 或
Type‑A Host 二选一，Host 侧经 TPS2051C 限流约 500 mA）（MinerU 文档「组件介绍」#16/#17/#18/#20）。
按步骤 §5，需先依据**本板参考设计**确认各接口的控制器、连接器与供电，再选 Device 或 Host 一侧接入；
Type‑C 与 Type‑A 共享 OTG 资源，禁止未验证同时启用。当前缺参考设计接线依据，停在核查阶段。

---

## 6. 阶段 B 验收清单进展

- [x] 阶段 A 基线通过并留有冷启动/复位与 NSH 日志（见阶段 A 记录）。
- [x] GPIO 完成“硬件依据 → 驱动依据 → 板级接入 → 实测 → 回归”闭环（本文件 §1）。
- [x] GPIO 未占用串口/启动管脚，未把 5 V LED 当软件 LED；Reset/BOOT 未被误认作普通用户键。
- [x] 以太网测试失败后已回退板级变更并证明基线仍可用（本文件 §2）。
- [x] 未把公共芯片/驱动改动混入队伍板级目录；缺驱动项（MicroSD）如实标为阻塞。
- [ ] 其余外设（MicroSD/USB/以太网）——**阻塞**，待补公共驱动 / 原理图依据 / fork 修复。

## 参考

- [`openvela-阶段B-基础板级外设实施步骤.md`](./openvela-阶段B-基础板级外设实施步骤.md)
- [`openvela-阶段A-实施记录.md`](./openvela-阶段A-实施记录.md)
- [`MinerU_markdown_esp-dev-kits-zh_CN-master-esp32p4_(1)_2110708478342160384.md`](./MinerU_markdown_esp-dev-kits-zh_CN-master-esp32p4_(1)_2110708478342160384.md)：J1 排针表、组件说明
- `_p4_upstream_patches/esp32p4-support/`：P4 公共变更补丁序列（含 HAL 平台层补丁）
