# 阶段 A 实施记录：ESP32-P4X-Function-EV-Board → OpenVela/NuttX

本文件是 [`openvela-阶段A-最小可启动配置实施步骤.md`](./openvela-阶段A-最小可启动配置实施步骤.md)
§0 放行表与 §5/§6 交付项的**实机记录**，用于记录阶段 A 的放行条件、执行命令、实机结果、
失败日志与未支持功能。板级形态目录：`contest2026_378_zhijudui/board/contest_board/`
（经 manifest `<linkfile>` 映射到工作树 `vendor/openvela/boards/contest2026_378_board/`）。

## 元信息

| 项 | 值 |
| --- | --- |
| 日期 | 2026-10-16 |
| 工作树根 | `ESP32-P4/`（openvela 工作树） |
| 内核仓库 | `nuttx/`，分支 `openvela/dev-ai-contest-2026`，base commit `76354c63785` |
| 板级形态 | `contest2026_378_board`（out-of-tree 自定义板卡） |
| 目标配置 | `configs/nsh`（最小可启动，进入可交互 NSH） |
| 实测内核版本串 | `NuttX 0.0.0 76354c63785-dirty ... risc-v contest2026_378_board` |

> 本记录中的“已实测”均指**真实硬件**（`/dev/ttyACM0`）上的观测；未实测项显式标注为“未验证”。
> 涉及公共仓库（`nuttx/`）的上游改动**不**进入队伍仓，见第 0 节与
> `/ESP32-P4/_p4_upstream_patches/esp32p4-support/`。

## 0. 边界与放行条件（已填写）

| 放行项 | 核实方式 | 记录内容 |
| --- | --- | --- |
| 硬件 | 对照实物丝印、板卡资料和原理图 | 板卡 **ESP32-P4X-Function-EV-Board**；主控 **ESP32-P4**，板卡资料称 P4X 为 **v3.1 及之后**；实机 USB-Serial/JTAG 串号 `E8:F6:0A:E3:A9:3A`。构建按 **rev3**（未设置 `CONFIG_ESP32P4_SELECTS_REV_LESS_V3`，即针对 v3.0 及以上）。PCB 版本以实物丝印为准（本记录未逐版记录） |
| 芯片支持 | 检查实际集成的架构目录、Kconfig、构建文件与启动链 | 来源：**`apache/nuttx` master** 的 ESP32-P4 移植；已整理为独立上游补丁序列并应用到本 fork 的 `nuttx/`（见 §0.1）。已集成目录：`arch/risc-v/src/esp32p4/`、`arch/risc-v/include/esp32p4/`、`arch/risc-v/src/common/espressif/`、`boards/risc-v/esp32p4/`；`arch/risc-v/Kconfig` 有 `ARCH_CHIP_ESP32P4`。工具链：`prebuilts/gcc/linux-x86_64/riscv-none-elf`（**gcc 13.4.0**）。支持的构建方式：**Make** 路径（CMake 路径未验证，见 §4） |
| 启动依赖 | 检查复位入口、内存/链接、时钟、供电、串口及 Flash 代码 | 复位入口：`arch/risc-v/src/common/espressif/esp_start.c` 的 `esp_initialize()` → `esp_board_initialize()`（板级空实现，`esp_start.c:682`）→ `nx_start()`。时钟/内存/链接/Flash/串口由芯片启动代码与 ROM 负责；**板级未重复配置**。仍缺的驱动/HAL：无（最小启动所需均可用） |
| 烧录与控制台 | 对照板卡资料和工具说明 | 映像：**simple-boot 单平面镜像** `nuttx/nuttx.bin`；烧录偏移 **0x2000**（P4 ROM 从 0x2000 读启动镜像）；工具 **esptool 4.12.0**（`-c esp32p4`）；物理端口：**USB-Serial/JTAG**（主机侧 `/dev/ttyACM0`）。**未启用安全下载模式**（勘误表 ROM-770）。控制台即该 USB-Serial/JTAG，见 §3 |
| 电源域 | 对照参考设计和芯片启动代码 | 最小启动**不依赖**板级启用 `LDO_VO3/LDO_VO4`：板级/defconfig 未配置这两路 LDO，电压与使能由 ROM/芯片启动代码负责；未确认处**不猜值**。后续启用依赖特定 VDD 域的外设前，需按原理图重新核对（见 §7） |

放行判据：以上条目均指向当前工作树中**实际可用**的代码（`nuttx/` 已集成 P4 支持）或已集成变更，
而非另一份 NuttX 的示例。板载 Flash 为 16 MB；本阶段**未启用 PSRAM**（实装容量未确认）。

### 0.1 公共仓库（上游）变更的位置与出口

P4 芯片/板级支持属于 **`nuttx/` 公共仓库**的上游工作，不属于队伍板级提交。已导出为规范补丁序列：

```text
/ESP32-P4/_p4_upstream_patches/esp32p4-support/
├── 0001-nuttx-debug-header.patch
├── 0002-espressif-platform-sync.patch
├── 0003-esp32p4-chip-support.patch
├── 0004-riscv-kconfig-esp32p4.patch
├── 0005-esp32p4-boards.patch
├── 0006-boards-kconfig-esp32p4.patch
├── 0007-tools-espressif-32m-flash.patch
└── README.md
```

- 出口：走 `nuttx` 公共仓库的**独立上游流程**（PR/分支），**不并入**队伍仓 `contest2026_378_zhijudui/`。
- 该序列对 3 个上游 P4 参考板（`esp32p4-function-ev-board`、`esp32p4-pico-wifi-wareshare`、`esp32p4-tab5`）
  做了本 fork 兼容适配，使其在正常运行下与上游的差异最小、且均可构建：
  每个板相对上游 `apache/nuttx` 仅改 **2 个文件**（`configs/nsh/defconfig` 与 `src/esp32p4_boot.c`）。
- 本 fork 兼容适配要点（与本队伍板同源，详见该目录 `README.md`）：
  1. `include/nuttx/debug.h` 补齐（上游迁移路径）；
  2. `common/espressif` 换用上游新一代树（含 `platform_include`），补 `esp_atomic64.c`（rv32 无 libatomic）；
  3. P4 芯片 Kconfig `select ONESHOT_COUNT`；
  4. 板级需提供 `board_app_initialize()`（本 fork 经 `boardctl` 调用）；
  5. NSH 入口 `init_main` → `nsh_main`（本 fork apps 无 `nxinit`）；
  6. `tools/espressif/Config.mk` 补 `CONFIG_ESPRESSIF_FLASH_32M → 32MB` 映射。

## 1. 固定板卡发现方式与构建路径

采用**路径 1：自定义板卡（out-of-tree）**，只用 **Make** 构建路径。

1. manifest 映射已确认存在，且目标指向队伍仓源目录（软链接）：

   ```text
   vendor/openvela/boards/contest2026_378_board -> ../../../contest2026_378_zhijudui/board/contest_board
   ```

   对应 `contest2026_378_zhijudui.xml`：`<linkfile src="board/contest_board" dest="vendor/openvela/boards/contest2026_378_board"/>`。

2. 生成的 `.config` 中自定义板卡项实际生效（来自 `nuttx/.config`，非仅 defconfig 文本）：

   ```text
   CONFIG_ARCH_BOARD_CUSTOM=y
   CONFIG_ARCH_BOARD_CUSTOM_DIR="../vendor/openvela/boards/contest2026_378_board"
   CONFIG_ARCH_BOARD_CUSTOM_DIR_RELPATH=y
   CONFIG_ARCH_BOARD_CUSTOM_NAME="contest2026_378_board"
   ```

3. 自定义模式下 defconfig **不含**未注册符号 `CONFIG_ARCH_BOARD_CONTEST2026_378_BOARD`（正确）。
   板级 `Kconfig` 无 `if ARCH_BOARD_CONTEST2026_000_BOARD` 守卫（自定义模式无该符号），只作说明注释。
4. 生成的 `.config` 保留了板卡选择、目录与 NSH 配置（见 §2）。

未采用路径 2（正式注册板卡）：本 fork 未注册 `ARCH_BOARD_CONTEST2026_378_BOARD`，注册需改公共配置树（`boards/Kconfig`），按边界应走独立上游流程，不在本阶段内。

## 2. 占位替换与最小 NSH defconfig

- 队伍板级文件已清理 `CONTEST2026_000_BOARD` / `contest2026_000_board` 占位（`grep` 无非文档命中）。
- 统一标识 `contest2026_378_board`；文件头注释同步更新。
- `configs/nsh/defconfig` 以上游 `esp32p4-function-ev-board` 的 NSH 配置为基线，
  **仅**把“正式注册板卡”三行替换为“自定义板卡”四行（`ARCH_BOARD_CUSTOM*`）。
- 关键选项生效核查（保存于 `nuttx/.config`，逐项确认而非仅看 defconfig）：

  | 项目 | 生效值 | 来源/说明 |
  | --- | --- | --- |
  | 架构 | `CONFIG_ARCH="risc-v"`、`CONFIG_ARCH_RISCV=y` | 芯片实现 |
  | 芯片 | `CONFIG_ARCH_CHIP="esp32p4"`、`CONFIG_ARCH_CHIP_ESP32P4=y`、`CONFIG_ESPRESSIF_CHIP_SERIES="esp32p4"` | chip 目录 |
  | 板卡 | `CONFIG_ARCH_BOARD_CUSTOM=y`、`..._NAME="contest2026_378_board"` | §1 |
  | 控制台 | `CONFIG_ESPRESSIF_USBSERIAL=y`（`CONFIG_ESPRESSIF_UART0` 未设置） | USB-Serial/JTAG，见 §3 |
  | NSH 入口 | `CONFIG_INIT_ENTRYPOINT="nsh_main"`、`CONFIG_SYSTEM_NSH=y`、`CONFIG_NSH_ARCHINIT=y` | 本 fork apps |
  | 启动模式 | `CONFIG_ESPRESSIF_SIMPLE_BOOT=y`（非 MCUBOOT） | 单平面镜像，见 §5 |
  | 定时器 | `CONFIG_ONESHOT=y` | 本 fork `riscv_mtimer.c` 需要 |
  | 内存 | `CONFIG_MM_KERNEL_HEAP=y`、`CONFIG_ARCH_STACKDUMP=y` | 最小 NSH |

- **未**启用（保持最小）：PSRAM、以太网（EMAC）、C6 无线、音频、SD、USB Host、显示、摄像头、
  GPIO/按键（`CONFIG_DEV_GPIO`/`CONFIG_ARCH_BUTTONS` 未开，相关板级源文件因此未编译）。
- **本阶段未预填** P4 专属的 RAM 地址、波特率、Flash 偏移或 PSRAM 容量；取值来自芯片实现与参考配置。

## 3. 控制台与板级启动入口

1. 复位→板级调用链（函数名与调用点均已核对）：

   | 入口 | 位置 | 调用条件/顺序 |
   | --- | --- | --- |
   | 复位入口 | `common/espressif/esp_start.c` `esp_initialize()` | 芯片复位后 |
   | 板级早期入口 | `esp_board_initialize()`（本板 `src/contest2026_378_boot.c`） | 由芯片在 `esp_start.c:682` 调用（`nx_start()` 之前）；本板为空实现 |
   | 板级晚期入口 | `board_late_initialize()` → `esp_bringup()` | **仅当** `CONFIG_BOARD_LATE_INITIALIZE=y`；本最小配置**未开启**，故 `esp_bringup()` 不执行 |
   | 应用初始化 | `board_app_initialize()` | 经 `boardctl(BOARDIOC_INIT)`（NSH `NSH_ARCHINIT`）；本板返回 0 |
   | 复位接口 | `board_reset()` → `up_shutdown_handler()`/`up_systemreset()` → `esp_restart_noos()` | `CONFIG_BOARDCTL_RESET=y` |

   > 说明：原骨架的占位 `openvela_board_initialize()` 已删除；本板提供的是芯片启动流程**实际调用**的
   > `esp_board_initialize()`，满足“函数被编译≠被调用”的核查要求。

2. 控制台：本阶段选定并验证 **USB-Serial/JTAG** 一条通路。
   - 芯片驱动：`common/espressif/esp_usbserial.c`，`g_uart_usbserial.isconsole = true`。
   - 主机侧设备：`/dev/ttyACM0`（内核 `cdc_acm`），实测可收发。
   - `CONFIG_ESPRESSIF_UART0` **未设置**，故 J1 的 GPIO37/U0TXD、GPIO38/U0RXD（资料值）**未**作为 NSH 控制台。
   - 下载口与控制台口均为同一 USB-Serial/JTAG 接口；已区分“只看到 ROM 日志≠NSH 已启动”。

3. 时钟/串口/存储/供电最小启动所需部分由 ROM/芯片代码负责；板级未重复初始化。

## 4. 接通所选构建系统（Make）

- 支持路径：**Make**。板级接入文件：`scripts/Make.defs`（pin `BOARD_DIR`、`BOARD_COMMON_DIR=.../esp32p4/common`、链接脚本）与
  `src/Make.defs`（`CSRCS = contest2026_378_boot.c contest2026_378_bringup.c`，按配置条件追加 reset/gpio/buttons/ethernet）。
- 构建命令（工作树根目录）与结果：

  ```sh
  ./build.sh vendor/openvela/boards/contest2026_378_board/configs/nsh -j8
  # 退出码 0；build.sh 展开为 configure.sh -e + make + make savedefconfig
  # 结尾：Minimal configuration saved to 'defconfig.tmp'，且 "No configuration change."（defconfig 未被改写）
  ```

  产物：`nuttx/nuttx`（ELF）、`nuttx/nuttx.bin`（ESP32-P4 镜像）。
- **CMake 路径未验证**：本板 `CMakeLists.txt` 调用 `add_subdirectory(src)`，但 `src/` 下**没有**
  `CMakeLists.txt`，故 `./build.sh ... --cmake` 无法完成板级接入。本阶段不宣称 CMake 可用。

## 5. 映像、烧录与实机验证

1. 映像与布局（依当前 P4 芯片实现与构建产物）：
   - 启动模式 `CONFIG_ESPRESSIF_SIMPLE_BOOT=y` → **单个平面镜像**，无独立 bootloader/分区表。
   - 构建时由 `esptool elf2image --ram-only-header ... -o nuttx.bin` 生成；ROM 从 flash **偏移 0x2000** 读取。
   - 链接脚本（板级 `scripts/`）：`esp32p4_aliases.ld`、`esp32p4_flat_memory.ld`、`esp32p4_sections.rev3.ld`。
2. 烧录（工具与参数实测）：

   ```sh
   ./.espvenv/bin/esptool.py -c esp32p4 -p /dev/ttyACM0 -b 460800 write_flash 0x2000 nuttx/nuttx.bin
   ```

   | 项 | 值 |
   | --- | --- |
   | 工具 | esptool **4.12.0**（Python 3.14.4；`.espvenv`） |
   | 芯片参数 | `-c esp32p4` |
   | 串口 | `/dev/ttyACM0`（USB-Serial/JTAG） |
   | 输入映像 / 偏移 | `nuttx/nuttx.bin` @ **0x2000** |
   | 下载模式 | BOOT+Reset 进入；**未启用安全下载模式** |
3. 实机验证（见 §5.1 原始日志）：
   - **进入可交互 NSH**：实机已捕获 `uname -a`（返回 `... risc-v contest2026_378_board`）与 `help`
     （键盘输入→输出）。
   - **冷启动（断电重上电）**：已实测。实机旁断电重上电后，设备重新枚举并运行 NuttX/NSH，
     `uname -a` 可交互。
   - **复位**：已实测。烧录结束时 esptool 硬复位（`Hard resetting via RTS pin...`）后设备重新枚举、
     NuttX 重新启动并进入 NSH；另一次软复位（NSH `reboot`）后 USB‑Serial/JTAG 亦成功重新枚举
     （内核日志 14:39:33）。
   - **ROM/启动器日志**：USB‑Serial/JTAG 在复位时重新枚举，ROM 早期横幅在主机重新接入前已输出，
     **难以在主机侧捕获**；ROM 级信息改由 esptool 连接输出提供（`ESP32-P4 (revision v3.2)`、
     `Crystal 40MHz`、`USB mode: USB-Serial/JTAG`、`MAC e8:f6:0a:e3:a9:3a`）。
4. 失败日志与解决依据：见 §5.2。

### 5.1 实机日志（原始）

烧录时 esptool 报告的 ROM 级信息（`write_flash 0x2000`）：

```text
esptool.py v4.12.0
Serial port /dev/ttyACM0
Connecting...
Chip is ESP32-P4 (revision v3.2)
Features: High-Performance MCU
Crystal is 40MHz
USB mode: USB-Serial/JTAG
MAC: e8:f6:0a:e3:a9:3a
Flash will be erased from 0x00002000 to 0x00045fff...
Wrote 278068 bytes (134653 compressed) at 0x00002000 in 2.0 seconds...
Hash of data verified.
Hard resetting via RTS pin...
```

NSH 交互（USB-Serial/JTAG，`/dev/ttyACM0`）：

```text
nsh> uname -a
NuttX 0.0.0 76354c63785-dirty Oct 16 2026 14:29:40 risc-v contest2026_378_board
nsh> help
help usage:  help [-v] [<cmd>]

    .           cp          exit        ls          reboot      truncate
    [           cmp         expr        mkdir       rm          uname
    ?           dirname     false       mkrd        rmdir       umount
    alias       dd          fdinfo      mount       set         unset
    unalias     df          free        mv          sleep       uptime
    basename    dmesg       help        pidof       source      usleep
    break       echo        hexdump     printf      test        watch
    cat         env         kill        ps          time        xd
    cd          exec        pkill       pwd         true        wait

Builtin Apps:
    dumpstack    getprime     nsh          ostest       sh
nsh>
```

构建结尾（链接与镜像，`./build.sh ... -j8`，退出码 0）：

```text
LD: nuttx
Memory region         Used Size  Region Size  %age Used
   tcm_idram_seg:          68 B         8 KB      0.83%
        irom_seg:      147026 B        64 MB      0.22%
        sram_seg:       31288 B     978880 B      3.20%
        drom_seg:      243132 B        64 MB      0.36%
      lp_ram_seg:         104 B      32744 B      0.32%
  extern_ram_seg:           0 B        64 MB      0.00%
CP: nuttx.hex
MKIMAGE: NuttX binary
esptool.py -c esp32p4 elf2image --ram-only-header -fs 4MB -fm dio -ff 80m -o nuttx.bin nuttx
esptool.py v4.12.0
Successfully created esp32p4 image.
Generated: nuttx.bin
...
Minimal configuration saved to 'defconfig.tmp'
```

主机内核 USB 日志（复位→重新枚举）：

```text
10月 16 14:29:54 ... cdc_acm 1-2.4:1.0: ttyACM0: USB ACM device
10月 16 14:39:33 ... usb 1-2.4: USB disconnect, device number 15
10月 16 14:39:33 ... usb 1-2.4: New USB device found, idVendor=303a, idProduct=1001, bcdDevice= 1.02
10月 16 14:39:33 ... usb 1-2.4: Product: USB JTAG/serial debug unit
10月 16 14:39:33 ... usb 1-2.4: Manufacturer: Espressif
10月 16 14:39:33 ... usb 1-2.4: SerialNumber: E8:F6:0A:E3:A9:3A
10月 16 14:39:33 ... cdc_acm 1-2.4:1.0: ttyACM0: USB ACM device
```

### 5.2 失败日志与解决依据（供复现）

1. **基线不可构建**：固定的 HAL `esp-hal-3rdparty` v5.1.c 使用 `spinlock_t x = 0`，而本 fork 的
   `spinlock_t` 恒为结构体 → 编译失败。解决：换用 HAL `release/master.c`（`cf95a27f`，含 P4 SOC + 平台层），
   并按上游方式打 `esp-hal-patches/` 适配（见补丁序列 0002）。
2. **HAL 平台层与本 fork 内核 API 不匹配**：`platform/os.c` 的 `nxtask_init` 参数不同、缺 `<fcntl.h>/<spawn.h>`。
   解决：`esp-hal-patches/0001-nuttx-platform-os-api-adapt.patch` 在构建时打补丁
   （`nxtask_init(tcb, name, entry, NULL, &attr, argv, NULL)`，用 `posix_spawnattr_t` 传 priority/stacksize）。
3. **rv32 无 libatomic**：缺 `__atomic_*_8`。解决：`esp_atomic64.c` 用临界区实现。
4. **P4 镜像偏移**：openvela 的 `tools/espressif/Config.mk` 对 P4 使用 `APP_OFFSET := 0x0000` 不适用。
   解决：按 P4 ROM 从 **0x2000** 读取，改烧到 0x2000。
5. **`build.sh` 的 `-e` 强制 `distclean`** 在 `.config` 不完整时失败（`chip/hal_.mk` 的 `CHIP_SERIES` 为空）。
   解决：清理 `nuttx/.config`、`nuttx/defconfig` 与 `arch/risc-v/src/{chip,board}` 软链接后重跑。
6. **误用其他板固件**：把 pico-wifi-wareshare 固件烧到 EV 板会崩溃（`Guru Meditation … Store/AMO access fault`）——
   属板卡不匹配（32 MB flash/引脚/控制台不同），**不是有效测试**，不予采用。
7. **软复位后主机 xHCI 掉线（环境问题，非固件问题）**：连续快速两次 `reboot` 后，主机侧 USB 控制器
   `xhci_hcd` 报 `device descriptor read/64, error -110`、`device not accepting address`、
   `invalid context state for evaluate context command`，最终 `unable to enumerate USB device`。
   首次复位时设备已正常重新枚举（见 §5.1），判定为**主机侧枚举时序**问题；恢复需**物理插拔 USB**。

## 6. 交付与验收（对照 §6 清单）

- [x] 阶段 0 的芯片支持来源（`apache/nuttx` master，补丁序列 0001–0007）、板卡版本、烧录布局
      （simple-boot/0x2000）和电源责任有可复现记录；依赖的公共仓库变更未混入队伍板级提交（§0.1）。
- [x] manifest 映射存在且目标不冲突；所有板级改动回到 `contest_board/`；无 `000` 占位符。
- [x] 生成的配置确实选中 P4 芯片、正确的板目录（自定义板卡）、控制台及 NSH；未知芯片选项未猜值。
- [x] 所选 **Make** 路径能完成配置与链接，板级启动入口（`esp_board_initialize()`）已确认由芯片启动流程调用；
      **CMake 未测试，不宣称支持**。
- [x] 依据真实产物和参考设计烧录，未启用安全下载模式；**冷启动（断电重上电）与复位**后均能进入可交互 NSH。
- [x] 控制台 TX/RX、接口、电平、波特率与日志有实机记录；Flash/最小启动电源配置有来源（PSRAM 未启用）。
- [x] `README.md` 记录构建命令、工具版本、映像/烧录步骤、控制台接法、验证结果和未支持功能；
      失败日志与解决依据见 §5.2。

> 未通过第 0 节放行或没有实机日志时，阶段 A 为“阻塞/未验证”。本记录中各放行与验收项均已满足
> 并有实机依据；ROM 早期横幅因 USB‑Serial/JTAG 复位重枚举而无法在主机侧捕获（ROM 级信息以
> esptool 输出为据），见 §5.3。

## 7. 已知限制 / 未支持功能

- 仅验证**最小 NSH 启动与命令交互**；GPIO/按键/存储/以太网/音频/显示/摄像头等外设未逐项启用与验证。
- **PSRAM 未启用**：实装容量未知，最小启动不依赖未经确认的 PSRAM 配置。
- **CMake 构建路径未验证**（`src/CMakeLists.txt` 缺失）。
- 镜像头 Flash 大小字段当前为 **4 MB**（defconfig/上游默认），而板载为 **16 MB**；simple-boot 单平面镜像
  不走分区表，故未影响启动。后续若要严格对齐硬件，可在 defconfig 选 16 MB（需重新验证）。
- `CONFIG_BOARD_LATE_INITIALIZE` 未开启，故 `esp_bringup()`（procfs/tmpfs 挂载及各外设 bring-up）在本最小配置
  中**不执行**；这是刻意的“最小”取舍，NSH 不依赖它。
- `LDO_VO3/LDO_VO4` 未由板级启用；启用依赖这些电源域的外设（如以太网/音频/显示）前，需按原理图核对
  电压、使能与上电顺序。

## 8. 待办（补测项）

- [ ] 将 `_p4_upstream_patches/esp32p4-support/` 通过 `nuttx` 公共仓库的独立上游流程提交（PR/分支）。
- [x] 冷启动（断电重上电）与复位后进入 NSH 已实测（见 §5.3）。
- [ ] 若需保留 ROM/启动器原始横幅：改用接 J1 的 **UART0（GPIO37/38）** 或外接 USB‑UART 作为日志口
      另做一次冷启动捕获（当前 USB‑Serial/JTAG 复位即重枚举，主机侧难以捕获其早期输出）。

## 参考

- [`openvela-阶段A-最小可启动配置实施步骤.md`](./openvela-阶段A-最小可启动配置实施步骤.md)：§0 放行表、§5/§6 交付清单。
- [`openvela-板级适配实现方案.md`](./openvela-板级适配实现方案.md)：总方案与仓库边界。
- [`MinerU_markdown_esp-dev-kits-zh_CN-master-esp32p4_(1)_2110708478342160384.md`](./MinerU_markdown_esp-dev-kits-zh_CN-master-esp32p4_(1)_2110708478342160384.md)：硬件依据。
- `/ESP32-P4/_p4_upstream_patches/esp32p4-support/`：P4 上游变更补丁序列（公共仓库工作，非队伍提交）。
