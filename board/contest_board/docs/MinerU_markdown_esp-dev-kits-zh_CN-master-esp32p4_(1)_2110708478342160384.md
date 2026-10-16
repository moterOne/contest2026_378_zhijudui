ESP32-P4 

esp-dev-kits 文档

## Table of contents

Table of contents
1 ESP32-P4X-Function-EV-Board 3
1.1 ESP32-P4X-Function-EV-Board . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 3
1.1.1 入门指南 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 3
1.1.2 硬件参考 . . . . . . . . . . . . . . . . . . . . . . . . . . . . 12
1.1.3 硬件版本 . . . . . . . . . . . . . . . . . 14
1.1.4 相关文档 . . . . . 14
1.1.5 免责声明和版权公告 15
2 ESP32-P4X-C5-Function-EV-Board 17
2.1 ESP32-P4X-C5-Function-EV-Board 17
2.1.1 开发板概述 17
2.1.2 开始开发应用 23
2.1.3 硬件参考 25
2.1.4 硬件版本 26
2.1.5 相关文档 26
2.1.6 免责声明和版权公告 26
3 ESP32-P4X-EYE 27
3.1 ESP32-P4X-EYE 27
3.1.1 入门指南 27
3.1.2 硬件参考 34
3.1.3 硬件版本 36
3.1.4 相关文档 36
3.1.5 免责声明和版权公告 40
4 寿命终止开发板 41
4.1 ESP32-P4-Function-EV-Board 41
4.1.1 ESP32-P4-Function-EV-Board v1.5.2 41
4.2 ESP32-P4-EYE 57
4.2.1 ESP32-P4-EYE 57
5 相关文档和资源 71
5.1 开发者社区 71
5.2 产品 71
5.3 联系我们 71
6 免责声明和版权公告 73
6.1 开发板使用说明 73

该文档详细介绍了 ESP32-P4系列开发板的用户指南和示例。

备注: 如需获取乐鑫全部系列开发板的有关信息，请访问 乐鑫开发板概览。

# ESP32-P4X-Function-EV-Board

ESP32-P4X-Function-EV-Board 是一款基于 ESP32-P4 芯片的多媒体开发板。ESP32-P4 搭载双核 RISC-V处理器，支持最大 32 MB PSRAM。此外，ESP32-P4 支持 USB 2.0 标准，MIPI-CSI/DSI，H264 Encoder 等多种外设，可满足客户对低成本、高性能、低功耗的多媒体产品的开发需求。

## 1.1 ESP32-P4X-Function-EV-Board

备注: 搭载 ESP32-P4 芯片版本 v3.1 的 ESP32-P4X-Function-EV-Board 不支持安全下载功能，请不要启用安全下载模式。详情请见 ESP32-P4系列芯片勘误表 > ROM-770。

要识别所使用的芯片版本，请查看 ESP32-P4 系列芯片勘误表 > 芯片版本标识。

本指南将帮助您快速上手 ESP32-P4X-Function-EV-Board，并提供该款开发板的详细信息。

ESP32-P4X-Function-EV-Board 是一款基于 ESP32-P4 芯片的多媒体开发板。ESP32-P4 搭载双核 RISC-V处理器，支持最大 32 MB PSRAM。此外，ESP32-P4 支持 USB 2.0 标准，MIPI-CSI/DSI，H264 Encoder 等多种外设，可满足客户对低成本、高性能、低功耗的多媒体产品的开发需求。

此开发板搭载了 ESP32-C6-MINI-1 2.4 GHz Wi-Fi 6 & Bluetooth 5 (LE) 模组，用于该开发板的 Wi-Fi 和蓝牙通信；它还配置了一块7英寸电容式触摸屏，分辨率为1024x600，并配有一颗200万像素的MIPICSI接口摄像头，丰富了用户的交互体验。该开发板适用于可视门铃、网络摄像头、智能家居中控屏、LCD电子价签、两轮车仪表盘等产品的原型构建。

板上模组大部分管脚均已引出至排针，开发人员可根据实际需求，轻松通过跳线连接多种外围设备。

本指南包括如下内容：

• 入门指南：简要介绍了 ESP32-P4X-Function-EV-Board 和硬件、软件设置指南。

• 硬件参考：详细介绍了 ESP32-P4X-Function-EV-Board 的硬件。

• 硬件版本：介绍硬件历史版本和已知问题，并提供链接至历史版本开发板的入门指南（如有）。

• 相关文档：列出了相关文档的链接。

• 免责声明和版权公告: 链接到免责声明和版权公告。

## 1.1.1 入门指南

本小节将简要介绍 ESP32-P4X-Function-EV-Board，说明如何在 ESP32-P4X-Function-EV-Board 上烧录固件及相关准备工作。

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/bea978efcb34cee368350d243613414edb4e4228de2238de1dbd89959dcaa2ad.jpg)



图 1: ESP32-P4X-Function-EV-Board


## 组件介绍

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/095350c9e019b1b047c7cbc8d81b75b74815e34d6275408b3422c3e30a945149.jpg)



图 2: ESP32-P4X-Function-EV-Board - 正面（点击放大）



下文从 J1开始按顺时针方向依次介绍开发板正面和背面的主要组件。


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/aac2ad656ac6183ded27ce4cf865afbd92ed3b157dde56bbee7a24c772391a3f.jpg)



图 3: ESP32-P4X-Function-EV-Board - 背面（点击放大）


<table><tr><td>组件编号</td><td>主要组件</td><td>介绍</td></tr><tr><td>1</td><td>J1</td><td>所有可用 GPIO 管脚均已引出至排针 J1,详细信息请见排针。</td></tr><tr><td>2</td><td>ESP32-C6 Module Programming Connector (ESP32-C6模组固件烧录接口)</td><td>该接口可配合使用 ESP-Prog 或其他 UART 工具为 ESP32-C6 模组烧录固件。</td></tr><tr><td>3</td><td>ESP32-C6-MINI-1 Module (ESP32-C6-MINI-1 模组)</td><td>ESP32-C6-MINI-1 模组用于该开发板的 Wi-Fi 和蓝牙通信。</td></tr><tr><td>4</td><td>Microphone(麦克风)</td><td>板载麦克风,连接至 Audio Codec Chip 接口。</td></tr><tr><td>5</td><td>Reset Button (Reset键)</td><td>复位按键。</td></tr><tr><td>6</td><td>Audio Codec Chip(音频编解码芯片)</td><td>音频编解码器芯片 ES8311 是一种低功耗单声道音频编解码器,包含单通道 ADC、单通道 DAC、低噪声前置放大器、耳机驱动器、数字音效、模拟混音和增益功能。它通过 I2S 和 I2C 总线与 ESP32-P4 芯片连接,以提供独立于音频应用程序的硬件音频处理。</td></tr><tr><td>7</td><td>Speaker Output Port(扬声器输出端口)</td><td>该输出端口用于连接扬声器。最高输出功率可以驱动 4 Ω 3 W 扬声器,引脚间距为 2.00 mm/0.08”。</td></tr><tr><td>8</td><td>Audio PA Chip(音频功率放大器)</td><td>NS4150 是一款低 EMI、3 W 单声道 D 类音频功率放大器,用于放大来自音频编解码芯片的音频信号,以驱动扬声器。</td></tr><tr><td>9</td><td>5 V to 3.3 V LDO(5 V转 3.3 V LDO)</td><td>电源转换器,输入 5 V,输出 3.3 V。</td></tr><tr><td>10</td><td>BOOT Button (BOOT 键)</td><td>启动模式控制按键,保持按住 BOOT 键的同时按一下 Reset 键,ESP-P4 将重新启动并进入“固件下载”模式,这时可以通过 USB 串口/JTAG 接口下载固件到板载 SPI flash 中。</td></tr><tr><td>11</td><td>Ethernet PHY IC</td><td>以太网 PHY 芯片 IP101GR,与 ESP32-P4 EMAC RMII 接口和 RJ45 以太网模块端口连接。</td></tr><tr><td colspan="2">Expressif System Block Converter(降压转换器)</td><td>用于 3.3 V 电源的降压型 DC-DC 转换器。 Release master Submit Document Feedback</td></tr><tr><td>13</td><td>5 V Power-on LED(5 V 电源指示灯)</td><td>开发板通过任一 USB Type-C 接口连接电源后,该指示灯亮起。</td></tr><tr><td>19</td><td>Power Switch(电源开关)</td><td>电源开关。拨向ON一侧,开发板连接5V电源上电;拨离ON一侧,开发板断开5V电源掉电。</td></tr><tr><td>20</td><td>Switch(开关)</td><td>TPS2051C是一款USB电源开关。在开发板上,USB 2.0 Type-A接口用于ESP32-P4作为USB Host连接外部USB设备时,需要向外部设备提供电源。TPS2051C将对外供电电流限制在500mA以内,符合USB规范,防止外部设备过度消耗电流;同时可避免因外部设备短路或过载导致开发板损坏。</td></tr><tr><td>21</td><td>MIPI CSI Connector(MIPI CSI连接器)</td><td>FPC连接器1.0K-GT-15PB,用以连接外接摄像头模组,实现图像传输,详情请见相关文档中的1.0K-GT-15PB规格书。适配的FPC规格:间距1.0mm,管脚宽度0.7mm,厚度0.3mm,管脚数量15。</td></tr><tr><td>22</td><td>Buck Converter(降压转换器)</td><td>用于VDD_HP电源的降压型DC-DC转换器。</td></tr><tr><td>23</td><td>ESP32-P4</td><td>ESP32-P4是一款高性能MCU,支持大容量片上内存,具有强大的图像和语音处理能力。</td></tr><tr><td>24</td><td>40 MHz XTAL</td><td>40 MHz晶振,用于提供系统时钟。</td></tr><tr><td>25</td><td>32.768 kHz XTAL</td><td>32.768 kHz晶振,可提供Deep-sleep下使用的低功耗时钟。</td></tr><tr><td>26</td><td>MIPI DSI Connector(MIPI DSI连接器)</td><td>FPC连接器1.0K-GT-15PB,用以连接LCD扩展板,详情请见相关文档中的1.0K-GT-15PB规格书。适配的FPC规格:间距1.0mm,管脚宽度0.7mm,厚度0.3mm,管脚数量15。</td></tr><tr><td>27</td><td>SPI flash<eq>^{1}</eq></td><td>通过SPI接口与ESP32-P4芯片连接,大小为16MB。</td></tr><tr><td>28</td><td>MicroSD Card Slot(MicroSD卡槽)</td><td>本开发板支持4-bit模式的MicroSD卡,可以存储或播放MicroSD卡中的音频文件。</td></tr></table>

备注: 关于 LDO_VO3 / LDO_VO4 的使用说明：

在 ESP32-P4X-Function-EV-Board 上，LDO_VO3 与 LDO_VO4 用于为板上部分 VDD 电源域供电。用户需在软件中配置正确的输出电压及使能状态。

在 Light-sleep 或 Deep-sleep 模式下，若 LDO_VO3 / LDO_VO4 保持使能，系统功耗将相对偏高；即使关闭，由于板级供电结构的影响，整体功耗仍可能高于芯片规格书中的典型低功耗指标。

如应用对功耗有严格要求，建议在自定义硬件设计中优化电源架构。

## 开发板配件

ESP32-P4X-Function-EV-Board 的包装盒中包含以下可选配件：

• 显示屏及其附件（可选）

– 7 英寸 MIPI DSI 电容式触摸屏，分辨率为 1024 x 600 像素

– LCD 屏幕适配板

– 附件包，包括杜邦线、屏幕排线、长铜柱（长度为 20 mm）、短铜柱（长度为8 mm）

• 摄像头及其附件（可选）

– 200 万像素 MIPI CSI 接口摄像头

– 摄像头适配板

– 摄像头排线

备注: 同向线序的排线为摄像头排线，其排线两侧的金属触点在同一面上；反向线序的排线为屏幕排线，其排线两侧的金属触点在不同面上。

## Strip


Ribbon Cable in the Forward Direction for Camera


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/2b27355b6b9dba9805c2eedf16af1e89e2fc3da894f364b5845588d69c3c4648.jpg)


## Strip


Ribbon Cable in the Reverse Direction for LCD


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/b9d1ce2866e9653e955aefe83eac05eae7a19f08e4bd0507ebad167075e125cf.jpg)



图4: 同向和反向线序排线


## 应用示例

以下为开发板的应用示例：

• ESP_Brookesia Phone - 展示了一个类似安卓的界面，使用 ESP_Brookesia，利用开发板上的 MIPI-DSI、MIPI-CSI、ESP32-C6、SD卡和音频接口运行多个应用程序，为高效的多媒体应用开发提供了基础。

• LVGL Demo v8 - 演示如何在配备 7 英寸 LCD 屏幕的 ESP32-P4X-Function-EV-Board 上移植 LVGLv8，并使用 LVGL的内置演示进行性能测试，为基于 LVGLv8 开发应用程序提供基础。

• LVGL Demo v9 - 演示如何在 ESP32-P4X-Function-EV-Board 上移植 LVGL v9 并使用 LVGL 内置的演示进行性能测试，作为基于 LVGLv9 开发应用程序的基础。

更多示例及最新更新请参阅 examples 文件夹。

也可以通过ESPLaunchpad直接在浏览器中体验出厂示例及其他预编译示例。ESPLaunchpad提供便捷的固件烧录方式，无需安装 ESP-IDF，也无需自行编译源代码。

如需尝试应用示例或开发自定义应用，请参照开始开发应用小节中的步骤进行操作。

## 开始开发应用

通电前，请确保 ESP32-P4X-Function-EV-Board 完好无损。

## 必备硬件

• ESP32-P4X-Function-EV-Board 

• USB 数据线

• 电脑（Windows、Linux 或 macOS）

## 备注: 请确保使用优质 USB数据线。部分数据线仅可用于充电，无法用于数据传输和编程。

## 可选硬件

• MicroSD 卡

硬件设置 使用 USB 数据线将 ESP32-P4X-Function-EV-Board 连接到电脑，可通过任何一个 USB Type-C端口为开发板供电。建议使用 USB串口/JTAG接口烧录固件和调试。

下图展示了开发板、LCD 屏幕适配板和摄像头组装完成后的整体效果。主要组件说明请参考组装后的ESP32-P4X-Function-EV-Board 组件说明。

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/92c745a84672a18f2f16f31901c317a914739cb80eb00101c4a765b93123ee03.jpg)



图 5: 完整组装后的 ESP32-P4X-Function-EV-Board


如需使用显示屏，请按照以下步骤连接显示屏：

1. 用短铜柱将开发板固定在LCD 屏幕适配板中心的四个螺母柱上。

2. 用屏幕排线（反向线序）连接 LCD 屏幕适配板的 J3 排针和开发板的 MIPI DSI 连接器。LCD 屏幕适配板已与LCD 连接。

3. 用杜邦线将开发板 J1 排针的 GPIO27 引脚连接至 LCD 屏幕适配板 J6 排针的 RST_LCD 引脚。RST_LCD 对应的 GPIO 可由软件设置，默认为 GPIO27。

4. 用杜邦线将将开发板 J1 排针的 GPIO26 引脚连接至 LCD 屏幕适配板 J6 排针的 PWM 引脚。PWM对应的GPIO 可由软件设置，默认为 GPIO26。

5. 推荐使用外接 USB 供电连接 LCD 屏幕适配板 J1 排针对屏幕供电。如果条件不允许，也可以在开发板供电充足的情况下，使用杜邦线将开发板 J1 排针的 5V 与 GND 引脚连接至 LCD 屏幕适配板的 5V 与 GND 管脚。

连接关系如下表所示：

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/07483a02c3b0b1fe0bdd934caac58238df7cddf1f6523820334cd0f607ca1e3c.jpg)



图6: LCD 屏幕排线细节


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/648819846f1a80e1866fa226251dbc3bbe62e778d149ac4cf69aba78e4b40055.jpg)



图 7: 杜邦线连接细节



表1: 杜邦线连接表


<table><tr><td>ESP32-P4X-Function-EV</td><td>LCD 屏幕适配板</td></tr><tr><td>MIPI DSI 连接器</td><td>J3 排针</td></tr><tr><td>J1 排针 GPIO27 引脚</td><td>J6 排针 RST_LCD 引脚</td></tr><tr><td>J1 排针 GPIO26 引脚</td><td>J6 排针 PWM 引脚</td></tr><tr><td>J1 排针 5V 引脚</td><td>J6 排针 5V 引脚</td></tr><tr><td>J1 排针 GND 引脚</td><td>J6 排针 GND 引脚</td></tr></table>


6. 将长铜柱固定在 LCD 屏幕适配板四周的四个螺母柱上，可使 LCD 竖直放置。


## 备注:

• 如果使用外接 USB 供电连接 LCD 屏幕适配板，则不需要连接 5V 和 GND 引脚。

• 如需使用摄像头，请将摄像头排线（同向线序）连接至摄像头适配板和开发板的 MIPICSI 连接器。

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/327f2ed29b90f4215a91e4c77c70f6122c420263c451e00c155811acefc7231d.jpg)



图 8: 摄像头



表 2: 组装后的 ESP32-P4X-Function-EV-Board 组件说明


<table><tr><td>组件编号</td><td>主要组件</td></tr><tr><td>1</td><td>Long Brass Standoff(长铜柱)</td></tr><tr><td>2</td><td>Camera Ribbon Cable(摄像头排线)</td></tr><tr><td>3</td><td>Short Brass Standoff(短铜柱)</td></tr><tr><td>4</td><td>USB Cable(USB 数据线)</td></tr><tr><td>5</td><td>LCD Ribbon Cable(LCD 屏幕排线)</td></tr><tr><td>6</td><td>GPIO27 to RST_LCD</td></tr><tr><td>7</td><td>GPIO26 to PWM</td></tr><tr><td>8</td><td>GND to GND</td></tr><tr><td>9</td><td>5V to 5V</td></tr><tr><td>10</td><td>Camera Front(摄像头正面)</td></tr></table>

软件设置 请前往 ESP-IDF 快速入门，查看如何快速设置开发环境，将应用程序烧录至您的开发板。

## 内含组件和包装

订购信息 该开发板的型号信息详见下表。

<table><tr><td>物料编号</td><td>搭载模组</td><td>Flash</td><td>PSRAM</td><td>天线</td></tr><tr><td>ESP32-P4X-Function-EV-Board</td><td>ESP32-C6-MINI-1</td><td>16 MB</td><td>32 MB</td><td>PCB 板载天线</td></tr></table>

如需了解更多可选型号，请访问乐鑫产品选型工具，选择开发板标签页。如果在本表或乐鑫产品选型工具中未找到所需型号，或有特殊定制需求，请联系 乐鑫商务。

零售订单 如购买样品，每个开发板将以防静电袋或零售商选择的其他方式包装。

零售订单请前往 https://www.espressif.com/zh-hans/company/contact/buy-a-sample。

批量订单 如批量购买，开发板将以大纸板箱包装。

批量订单请前往 https://www.espressif.com/zh-hans/contact-us/sales-questions。

## 1.1.2 硬件参考

## 功能框图

ESP32-P4X-Function-EV-Board 的主要组件和连接方式如下图所示。

电源选项

可通过以下接口为开发板供电：

• USB 2.0 Type-C 接口

• USB 全速接口

• USB 串口/JTAG 接口

如果用于调试的 USB数据线无法提供足够的电流，可通过空闲的 USBType-C 接口连接电源适配器。

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/380b7ef1aca05b1e1047bd4b8a60254f67acee3234266912c4286df980066471.jpg)



图 9: ESP32-P4X-Function-EV-Board 功能框图（点击放大）


## 排针

下表列出了开发板排针 J1 的 名称和 功能，排针的名称如图ESP32-P4X-Function-EV-Board- 正面（点击放大） 所示，排针的序号与 ESP32-P4X-Function-EV-Board 参考设计 里的原理图一致。

<table><tr><td>序号</td><td>名称</td><td>类型<eq>^{Page 14,2}</eq></td><td>功能</td></tr><tr><td>1</td><td>3V3</td><td>P</td><td>3.3 V电源</td></tr><tr><td>2</td><td>5V</td><td>P</td><td>5 V电源</td></tr><tr><td>3</td><td>7</td><td>I/O/T</td><td>GPIO7</td></tr><tr><td>4</td><td>5V</td><td>P</td><td>5 V电源</td></tr><tr><td>5</td><td>8</td><td>I/O/T</td><td>GPIO8</td></tr><tr><td>6</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>7</td><td>23</td><td>I/O/T</td><td>GPIO23</td></tr><tr><td>8</td><td>37</td><td>I/O/T</td><td>U0TXD, GPIO37</td></tr><tr><td>9</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>10</td><td>38</td><td>I/O/T</td><td>U0RXD, GPIO38</td></tr><tr><td>11</td><td>21</td><td>I/O/T</td><td>GPIO21</td></tr><tr><td>12</td><td>22</td><td>I/O/T</td><td>GPIO22</td></tr><tr><td>13</td><td>20</td><td>I/O/T</td><td>GPIO20</td></tr><tr><td>14</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>15</td><td>6</td><td>I/O/T</td><td>GPIO6</td></tr><tr><td>16</td><td>5</td><td>I/O/T</td><td>GPIO5</td></tr><tr><td>17</td><td>3V3</td><td>P</td><td>3.3 V电源</td></tr><tr><td>18</td><td>4</td><td>I/O/T</td><td>GPIO4</td></tr><tr><td>19</td><td>3</td><td>I/O/T</td><td>GPIO3</td></tr></table>


表 3 – 接上页


<table><tr><td>序号</td><td>名称</td><td>类型<eq>^{Page\ 14,2}</eq></td><td>功能</td></tr><tr><td>20</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>21</td><td>2</td><td>I/O/T</td><td>GPIO2</td></tr><tr><td>22</td><td>NC(1)</td><td>I/O/T</td><td><eq>GPIO1^3</eq></td></tr><tr><td>23</td><td>NC(0)</td><td>I/O/T</td><td><eq>GPIO0^3</eq></td></tr><tr><td>24</td><td>36</td><td>I/O/T</td><td>GPIO36</td></tr><tr><td>25</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>26</td><td>32</td><td>I/O/T</td><td>GPIO32</td></tr><tr><td>27</td><td>NC</td><td>-</td><td>空管脚</td></tr><tr><td>28</td><td>NC</td><td>-</td><td>空管脚</td></tr><tr><td>29</td><td>33</td><td>I/O/T</td><td>GPIO33</td></tr><tr><td>30</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>31</td><td>26</td><td>I/O/T</td><td>GPIO26</td></tr><tr><td>32</td><td>54</td><td>I/O/T</td><td>GPIO54</td></tr><tr><td>33</td><td>48</td><td>I/O/T</td><td>GPIO48</td></tr><tr><td>34</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>35</td><td>53</td><td>I/O/T</td><td>GPIO53</td></tr><tr><td>36</td><td>46</td><td>I/O/T</td><td>GPIO46</td></tr><tr><td>37</td><td>47</td><td>I/O/T</td><td>GPIO47</td></tr><tr><td>38</td><td>27</td><td>I/O/T</td><td>GPIO27</td></tr><tr><td>39</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>40</td><td>NC(45)</td><td>I/O/T</td><td><eq>GPIO45^4</eq></td></tr></table>

## J1

## 1.1.3 硬件版本

开发板 ESP32-P4X-Function-EV-Board 相较于ESP32-P4-Function-EV-Board 的区别是主芯片更新为 ESP32-P4芯片版本 v3.1及之后版本。

## 1.1.4 相关文档

请前往 esp-dev-kits 文档 HTML 网页版本 下载以下文档。

• ESP32-P4X-Function-EV-Board 参考设计 (ZIP)

• ESP32-P4 系列芯片勘误表

• 1.0K-GT-15PB 规格书 (PDF)

• 摄像头规格书 (PDF)

• 显示屏规格书 (PDF)

• 显示屏驱动芯片 EK73217BCGA 规格书 (PDF)

• 显示屏驱动芯片 EK79007AD 规格书 (PDF)

• 显示屏适配板原理图(PDF)

• 显示屏适配板 PCB 布局图 (PDF)

• 摄像头适配板原理图(PDF)

• 摄像头适配板 PCB 布局图 (PDF)

• 显示屏适配板参考设计 (ZIP)

• 摄像头适配板参考设计 (ZIP)

有关本开发板的更多设计文档，请联系我们的商务部门 sales@espressif.com。

## 1.1.5 免责声明和版权公告

请参阅免责声明和版权公告。

# ESP32-P4X-C5-Function-EV-Board

ESP32-P4X-C5-Function-EV-Board 是一款基于 ESP32-P4 芯片的多媒体开发板。ESP32-P4 搭载双核 RISCV 处理器，支持最大 32 MB PSRAM。此外，ESP32-P4 支持 USB 2.0 标准，MIPI-CSI/DSI，H264 Encoder等多种外设。此开发板搭载 ESP32-C5-MINI-1 模组，支持 2.4 GHz & 5 GHz 双频 Wi-Fi 6 及 Bluetooth 5(LE)，用于 Wi-Fi 和蓝牙通信。凭借上述特性，该开发板是开发低成本、高性能、低功耗联网音视频产品的理想选择。

## 2.1 ESP32-P4X-C5-Function-EV-Board

本指南将帮助您快速上手 ESP32-P4X-C5-Function-EV-Board，并提供该款开发板的详细信息。

• 开发板概述：简要介绍了开发板的软件和硬件

• 开始开发应用：介绍了应用程序开发过程中的软硬件设置

• 硬件参考：详细介绍了开发板的硬件

• 硬件版本：列出了硬件历史版本和已知问题，并链接至历史版本开发板的入门指南（如有）

• 相关文档：列出了相关文档的链接

• 免责声明和版权公告: 链接到免责声明和版权公告。

## 2.1.1 开发板概述

ESP32-P4X-C5-Function-EV-Board 是一款基于 ESP32-P4 芯片的多媒体开发板。ESP32-P4 搭载双核 RISC-V 处理器，支持最大 32 MB PSRAM。此外，ESP32-P4 支持 USB 2.0 标准，MIPI-CSI/DSI，H264 Encoder等多种外设，可满足客户对低成本、高性能、低功耗的多媒体产品的开发需求。

此开发板搭载 ESP32-C5-MINI-1 模组，支持 2.4 GHz & 5 GHz 双频 Wi-Fi 6 及 Bluetooth 5 (LE)，用于 Wi-Fi和蓝牙通信；它还配置了一块 7 英寸电容式触摸屏，分辨率为 1024 x 600，并配有一颗 200 万像素的MIPI CSI 接口摄像头，丰富了用户的交互体验。该开发板适用于可视门铃、网络摄像头、智能家居中控屏、LCD 电子价签、两轮车仪表盘等产品的原型构建。

特性列表

该开发板具有以下特性：

• 主控芯片：ESP32-P4，双核 RISC-V 处理器，板载 16 MB SPI flash、32 MB PSRAM

• 内置模组：ESP32-C5-MINI-1，支持 2.4 GHz & 5 GHz 双频 Wi-Fi 6、Bluetooth 5 (LE) 及 IEEE 802.15.4

• 显示：MIPI DSI 接口，可连接 7 英寸电容式触摸屏 (1024 × 600)

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/19a99f7f97f5c28bdcebb28cb69d5cad6d03830cfd39f8d2627a20ce5016be63.jpg)



图 1: ESP32-P4X-C5-Function-EV-Board（板载 ESP32-C5-MINI-1 模组）


• 摄像头：MIPI CSI 接口，可连接 2MP 摄像头

• 音频：ES8311音频编解码芯片、NS4150B 音频功放、板载麦克风、扬声器输出接口

• 存储：MicroSD 卡槽（4-bit 模式）

• USB：USB 2.0 OTG Type-C 接口、USB 2.0 Type-A 接口、USB Full-Speed 接口、USB Serial/JTAG 接口

• 网络：RJ45 以太网接口（10/100 Mbps 自适应）

• 扩展：J1 排针引出可用 GPIO 引脚

• 按键：BOOT 键和 Reset 键

## 功能框图

ESP32-P4X-C5-Function-EV-Board 的主要组件和连接方式如下图所示。

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/16681d704cf01232772f3b79e49f5b7b287e6b1e1539fb083b6a62b533649c36.jpg)



图 2: ESP32-P4X-C5-Function-EV-Board 功能框图


## 组件介绍

下文从 J1开始按顺时针方向依次介绍开发板正面和背面的主要组件。

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/efd4d71298ff0cb5788caa261fa97b0ffbf2e37e3543269965286bb25f26e62a.jpg)



(16) USB Serial/JTAG Port



图 3: ESP32-P4X-C5-Function-EV-Board - 正面


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/897d6128b4fa0ee207fdd113bdf5e57a805f48ad4af04e9cae8c81cea42a78d7.jpg)



图 4: ESP32-P4X-C5-Function-EV-Board - 背面


<table><tr><td>组件编号</td><td>主要组件</td><td>介绍</td></tr><tr><td>1</td><td>J1</td><td>所有可用 GPIO 管脚均已引出至排针 J1,详细信息请见排针。</td></tr><tr><td>2</td><td>ESP32-C5-MINI-1 Module (ESP32-C5-MINI-1 模组)</td><td>ESP32-C5-MINI-1 模组用于该开发板的 Wi-Fi 和蓝牙通信。</td></tr><tr><td>3</td><td>ESP32-C5 Module Programming Connector (ESP32-C5模组固件烧录接口)</td><td>该接口可配合使用 ESP-Prog 或其他 UART 工具为 ESP32-C5 模组烧录固件。</td></tr><tr><td>4</td><td>Microphone (麦克风)</td><td>板载麦克风,连接至 Audio Codec Chip 接口。</td></tr><tr><td>5</td><td>Reset Button (Reset键)</td><td>复位按键。</td></tr><tr><td>6</td><td>Audio Codec Chip(音频编解码芯片)</td><td>音频编解码器芯片 ES8311 是一种低功耗单声道音频编解码器,包含单通道 ADC、单通道 DAC、低噪声前置放大器、耳机驱动器、数字音效、模拟混音和增益功能。它通过 I2S 和 I2C 总线与 ESP32-P4 芯片连接,以提供独立于音频应用程序的硬件音频处理。</td></tr><tr><td>7</td><td>Speaker Output Port (扬声器输出接口)</td><td>该输出接口用于连接扬声器。最高输出功率可以驱动 4 Ω 3 W 扬声器,引脚间距为 2.00 mm/0.08”。</td></tr><tr><td>8</td><td>Audio PA Chip (音频功率放大器)</td><td>NS4150B 是一款低 EMI、3 W 单声道 D 类音频功率放大器,用于放大来自音频编解码芯片的音频信号,以驱动扬声器。</td></tr><tr><td>9</td><td>5 V to 3.3 V LDO (5 V 转 3.3 V LDO)</td><td>电源转换器,输入 5 V,输出 3.3 V。</td></tr><tr><td>10</td><td>BOOT Button (BOOT 键)</td><td>启动模式控制按键,保持按住 BOOT 键的同时按一下 Reset 键,ESP32-P4 将重新启动并进入“固件下载”模式,这时可以通过 USB 串口/JTAG 接口下载固件到板载 SPI flash 中。</td></tr><tr><td>11</td><td>Ethernet PHY IC</td><td>以太网 PHY 芯片 IP101GR,与 ESP32-P4 EMAC RMII 接口和 RJ45 以太网接口连接。</td></tr><tr><td>12</td><td>Buck Converter (降压转换器)</td><td>用于 3.3 V 电源的降压型 DC-DC 转换器。</td></tr><tr><td>13</td><td>5 V Power-on LED (5 V 电源指示灯)</td><td>开发板通过任一 USB Type-C 接口连接电源后,该指示灯亮起。</td></tr><tr><td>14</td><td>RJ45 Ethernet Port (RJ45 以太网接口)</td><td>以太网接口,支持 10/100 Mbps 自适应。</td></tr><tr><td>15</td><td>USB Full-speed Port (USB 全速接口)</td><td>USB Type-C 接口,支持 USB 2.0 Full-speed 速率,可用作开发板的供电接口和通信接口。</td></tr><tr><td>16</td><td>USB Serial/JTAG Port (USB 串口/JTAG 接口)</td><td>USB Type-C 接口,支持 USB 2.0 Full-speed 速率,可烧录固件至 ESP32-P4 芯片,可通过 USB 协议与芯片通信,也可用于 JTAG 调试。</td></tr><tr><td>17</td><td>USB 2.0 Type-C Port (USB 2.0 Type-C 接口)</td><td>USB 2.0 Type-C 接口与 ESP32-P4 芯片的 USB 2.0 OTG High-Speed 接口连接,支持 USB 2.0 标准。通过该接口进行 USB 通讯时,ESP32-P4 作为 USB 设备连接至其他 USB 主机设备。USB 2.0 Type-C 接口和 USB 2.0 Type-A 接口需要二选一使用,不能同时使用。该接口也可用作开发板的供电接口。</td></tr><tr><td>18</td><td>USB 2.0 Type-A Port (USB 2.0 Type-A 接口)</td><td>USB 2.0 Type-A 接口与 ESP32-P4 芯片的 USB 2.0 OTG High-Speed 接口连接,支持 USB 2.0 标准。通过该接口进行 USB 通讯时,ESP32-P4 作为 USB 主机与其它 USB 设备连接,对外提供最高 500 mA 电流。USB 2.0 Type-C 接口和 USB 2.0 Type-A 接口需要二选一使用,不能同时使用。</td></tr><tr><td>19</td><td>Power Switch(电源开关)</td><td>电源开关。拨向ON一侧,开发板连接5V电源上电;拨离ON一侧,开发板断开5V电源掉电。</td></tr><tr><td>20</td><td>Switch(开关)</td><td>TPS2051C是一款USB电源开关。在开发板上,USB 2.0 Type-A接口用于ESP32-P4作为USB Host连接外部USB设备时,需要向外部设备提供电源。TPS2051C将对外供电电流限制在500mA以内,符合USB规范,防止外部设备过度消耗电流;同时可避免因外部设备短路或过载导致开发板损坏。</td></tr><tr><td>21</td><td>MIPI CSI Connector (MIPI CSI连接器)</td><td>FPC连接器1.0K-GT-15PB,用以连接外接摄像头模组,实现图像传输,详情请见相关文档中的1.0K-GT-15PB规格书。适配的FPC规格:间距1.0mm,管脚宽度0.7mm,厚度0.3mm,管脚数量15。</td></tr><tr><td>22</td><td>Buck Converter(降压转换器)</td><td>用于VDD_HP电源的降压型DC-DC转换器。</td></tr><tr><td>23</td><td>ESP32-P4</td><td>ESP32-P4是一款高性能MCU,支持大容量片上内存,具有强大的图像和语音处理能力。</td></tr><tr><td>24</td><td>40 MHz XTAL</td><td>40 MHz晶振,用于提供系统时钟。</td></tr><tr><td>25</td><td>32.768 kHz XTAL</td><td>32.768 kHz晶振,可提供Deep-sleep下使用的低功耗时钟。</td></tr><tr><td>26</td><td>MIPI DSI Connector (MIPI DSI连接器)</td><td>FPC连接器1.0K-GT-15PB,用以连接LCD扩展板,详情请见相关文档中的1.0K-GT-15PB规格书。适配的FPC规格:间距1.0mm,管脚宽度0.7mm,厚度0.3mm,管脚数量15。</td></tr><tr><td>27</td><td>SPI flash<eq>^{1}</eq></td><td>通过SPI接口与ESP32-P4芯片连接,大小为16MB。</td></tr><tr><td>28</td><td>ESP32-P4 Programming Connector (ESP32-P4芯片烧录接口)</td><td>该接口可配合使用ESP-Prog或其他UART工具为ESP32-P4(芯片版本v3.x及以上)烧录固件。</td></tr><tr><td>29</td><td>MicroSD Card Slot(MicroSD卡槽)</td><td>本开发板支持4-bit模式的MicroSD卡,可以存储或播放MicroSD卡中的音频文件。</td></tr></table>


备注: 关于 LDO_VO3 / LDO_VO4 的使用说明：


在 ESP32-P4X-C5-Function-EV-Board 上，LDO_VO3 与 LDO_VO4 用于为板上部分 VDD 电源域供电。用户需在软件中配置正确的输出电压及使能状态。

在 Light-sleep 或 Deep-sleep 模式下，若 LDO_VO3 / LDO_VO4 保持使能，系统功耗将相对偏高；即使关闭，由于板级供电结构的影响，整体功耗仍可能高于芯片规格书中的典型低功耗指标。

如应用对功耗有严格要求，建议在自定义硬件设计中优化电源架构。

## 开发板配件

ESP32-P4X-C5-Function-EV-Board 的包装盒中包含以下可选配件：

• 显示屏及其附件（可选）

– 7 英寸 MIPI DSI 电容式触摸屏，分辨率为 1024 x 600 像素

– LCD 屏幕适配板

– 附件包，包括杜邦线、屏幕排线、长铜柱（长度为 20 mm）、短铜柱（长度为8 mm）

• 摄像头及其附件（可选）

– 200 万像素 MIPI CSI 接口摄像头

– 摄像头适配板

– 摄像头排线

<sup>1</sup> 默认情况下，与芯片连接的板载SPIflash支持的最大时钟频率为80MHz，且不支持自动暂停功能。如需使用120MHz的flash时钟频率或需要flash自动暂停功能，请 联系我们。


Ribbon Cable in the Forward Direction for Camera


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/4b5392ee2b5bda3a7b2015e4a51d9fb01ce9c1a82402451a16990d6467703ef2.jpg)



Ribbon Cable in the Reverse Direction for LCD


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/c5cf6e0999dc6cba7d06ecce34ef70eeb60cb6ae50d1438cf8c5eed773f88a01.jpg)



图5: 同向和反向线序排线


备注: 同向线序的排线为摄像头排线，其排线两侧的金属触点在同一面上；反向线序的排线为屏幕排线，其排线两侧的金属触点在不同面上。

## 应用示例

以下为开发板的应用示例：

• ESP_Brookesia Phone - 展示了一个类似安卓的界面，使用 ESP_Brookesia，利用开发板上的 MIPI-DSI、MIPI-CSI、ESP32-C5、SD卡和音频接口运行多个应用程序，为高效的多媒体应用开发提供了基础。可以通过ESPLaunchpad直接在浏览器中体验出厂示例及其他预编译示例。ESPLaunchpad提供便捷的固件烧录方式，无需安装 ESP-IDF，也无需自行编译源代码。

如需尝试应用示例或开发自定义应用，请参照开始开发应用小节中的步骤进行操作。

## 2.1.2 开始开发应用

本节介绍硬件和软件的设置方法，以及烧录固件至开发板以开发应用程序的说明。

必备硬件

• ESP32-P4X-C5-Function-EV-Board 

• USB 数据线

• 电脑（Windows、Linux 或 macOS）

备注: 请确保使用支持数据传输的 USB数据线。部分数据线仅可用于充电，无法用于数据传输和编程。

## 可选硬件

• MicroSD 卡

## 电源选项

开发板有以下 3种供电方式，您可以任选其一。

• 通过 USB 2.0 Type-C 接口供电

• 通过 USB 全速接口供电

• 通过 USB 串口/JTAG 接口供电

如果用于调试的 USB 数据线无法提供足够电流，可通过任一 USB Type-C 接口将开发板连接至电源适配器供电。

## 硬件设置

使用 USB 数据线将 ESP32-P4X-C5-Function-EV-Board 连接到电脑，可通过任何一个 USB Type-C 接口为开发板供电。建议使用 USB串口/JTAG接口烧录固件和调试。

如需使用显示屏，请按照以下步骤连接显示屏：

1. 用短铜柱将开发板固定在LCD 屏幕适配板中心的四个螺母柱上。

2. 用屏幕排线（反向线序）连接 LCD 屏幕适配板的 J3 排针和开发板的 MIPI DSI 连接器。LCD 屏幕适配板已与LCD 连接。

3. 用杜邦线将开发板 J1 排针的 GPIO27 引脚连接至 LCD 屏幕适配板 J6 排针的 RST_LCD 引脚。RST_LCD 对应的 GPIO 可由软件设置，默认为 GPIO27。

4. 用杜邦线将开发板 J1 排针的 GPIO26 引脚连接至 LCD 屏幕适配板 J6 排针的 PWM 引脚。PWM 对应的 GPIO可由软件设置，默认为 GPIO26。

5. 推荐使用外接 USB 供电连接 LCD 屏幕适配板 J1 排针对屏幕供电。如果条件不允许，也可以在开发板供电充足的情况下，使用杜邦线将开发板 J1 排针的 5V 与 GND 引脚连接至 LCD 屏幕适配板的 5V 与 GND 管脚。

连接关系如下表所示：


表1: 杜邦线连接表


<table><tr><td>ESP32-P4X-C5-Function-EV-Board</td><td>LCD 屏幕适配板</td></tr><tr><td>MIPI DSI 连接器</td><td>J3 排针</td></tr><tr><td>J1 排针 GPIO27 引脚</td><td>J6 排针 RST_LCD 引脚</td></tr><tr><td>J1 排针 GPIO26 引脚</td><td>J6 排针 PWM 引脚</td></tr><tr><td>J1 排针 5V 引脚</td><td>J6 排针 5V 引脚</td></tr><tr><td>J1 排针 GND 引脚</td><td>J6 排针 GND 引脚</td></tr></table>

6. 将长铜柱固定在 LCD 屏幕适配板四周的四个螺母柱上，可使 LCD 竖直放置。

## 备注:

• 如果使用外接USB供电连接LCD 屏幕适配板，则不需要连接 5V 和GND 引脚。

• 如需使用摄像头，请将摄像头排线（同向线序）连接至摄像头适配板和开发板的 MIPICSI 连接器。

## 软件设置

硬件设置完成后，请前往 ESP-IDF 快速入门，查看如何快速设置开发环境，将应用程序烧录至您的开发板。

## 内含组件和包装

订购信息 该开发板的型号信息详见下表。

<table><tr><td>物料编号</td><td>搭载模组</td><td>Flash</td><td>PSRAM</td><td>天线</td></tr><tr><td>ESP32-P4X-C5-Function-EV-Board</td><td>ESP32-C5-MINI-1</td><td>16 MB</td><td>32 MB</td><td>PCB 板载天线</td></tr></table>

如需了解更多可选型号，请访问乐鑫产品选型工具，选择开发板标签页。如果在本表或乐鑫产品选型工具中未找到所需型号，或有特殊定制需求，请联系 乐鑫商务。

零售订单 如购买样品，每个开发板将以防静电袋或零售商选择的其他方式包装。

零售订单请前往 https://www.espressif.com/zh-hans/company/contact/buy-a-sample。

批量订单 如批量购买，开发板将以大纸板箱包装。

批量订单请前往 https://www.espressif.com/zh-hans/contact-us/sales-questions。

## 2.1.3 硬件参考

## 排针

下表列出了开发板排针 J1 的 名称和 功能，排针的名称如图ESP32-P4X-C5-Function-EV-Board - 正面 所示，排针的序号与 ESP32-P4X-C5-Function-EV-Board 原理图 一致。

<table><tr><td>序号</td><td>名称</td><td>类型Page 26, 2</td><td>功能</td></tr><tr><td>1</td><td>3V3</td><td>P</td><td>3.3 V电源</td></tr><tr><td>2</td><td>5V</td><td>P</td><td>5 V电源</td></tr><tr><td>3</td><td>7</td><td>I/O/T</td><td>GPIO7</td></tr><tr><td>4</td><td>5V</td><td>P</td><td>5 V电源</td></tr><tr><td>5</td><td>8</td><td>I/O/T</td><td>GPIO8</td></tr><tr><td>6</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>7</td><td>23</td><td>I/O/T</td><td>GPIO23</td></tr><tr><td>8</td><td>37</td><td>I/O/T</td><td>U0TXD, GPIO37</td></tr><tr><td>9</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>10</td><td>38</td><td>I/O/T</td><td>U0RXD, GPIO38</td></tr><tr><td>11</td><td>21</td><td>I/O/T</td><td>GPIO21</td></tr><tr><td>12</td><td>22</td><td>I/O/T</td><td>GPIO22</td></tr><tr><td>13</td><td>20</td><td>I/O/T</td><td>GPIO20</td></tr><tr><td>14</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>15</td><td>NC(6)</td><td>I/O/T</td><td><eq>\text{GPIO6}^3</eq></td></tr><tr><td>16</td><td>5</td><td>I/O/T</td><td>GPIO5</td></tr><tr><td>17</td><td>3V3</td><td>P</td><td>3.3 V电源</td></tr><tr><td>18</td><td>4</td><td>I/O/T</td><td>GPIO4</td></tr><tr><td>19</td><td>3</td><td>I/O/T</td><td>GPIO3</td></tr><tr><td>20</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>21</td><td>2</td><td>I/O/T</td><td>GPIO2</td></tr><tr><td>22</td><td>NC(1)</td><td>I/O/T</td><td><eq>\text{GPIO1}^4</eq></td></tr><tr><td>23</td><td>NC(0)</td><td>I/O/T</td><td><eq>\text{GPIO0}^4</eq></td></tr><tr><td>24</td><td>36</td><td>I/O/T</td><td>GPIO36</td></tr><tr><td>25</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>26</td><td>32</td><td>I/O/T</td><td>GPIO32</td></tr><tr><td>27</td><td>NC</td><td>-</td><td>空管脚</td></tr><tr><td>28</td><td>NC</td><td>-</td><td>空管脚</td></tr><tr><td>29</td><td>33</td><td>I/O/T</td><td>GPIO33</td></tr><tr><td>30</td><td>GND</td><td>GND</td><td>接地</td></tr></table>


表 2 – 接上页


<table><tr><td>序号</td><td>名称</td><td>类型Page 26,2</td><td>功能</td></tr><tr><td>31</td><td>26</td><td>I/O/T</td><td>GPIO26</td></tr><tr><td>32</td><td>NC(54)</td><td>I/O/T</td><td><eq>\text{GPIO54}^5</eq></td></tr><tr><td>33</td><td>48</td><td>I/O/T</td><td>GPIO48</td></tr><tr><td>34</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>35</td><td>NC(53)</td><td>I/O/T</td><td><eq>\text{GPIO53}^6</eq></td></tr><tr><td>36</td><td>46</td><td>I/O/T</td><td>GPIO46</td></tr><tr><td>37</td><td>47</td><td>I/O/T</td><td>GPIO47</td></tr><tr><td>38</td><td>27</td><td>I/O/T</td><td>GPIO27</td></tr><tr><td>39</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>40</td><td>NC(45)</td><td>I/O/T</td><td><eq>\text{GPIO45}^7</eq></td></tr></table>

## J1

## 2.1.4 硬件版本

无历史版本。

## 2.1.5 相关文档

请前往 esp-dev-kits 文档 HTML 网页版本 下载以下文档。

• ESP32-P4X-C5-Function-EV-Board 原理图 (PDF)

• ESP32-P4X-C5-Function-EV-Board PCB 布局图 (PDF)

• ESP32-P4X-C5-Function-EV-Board 尺寸图 (PDF)

• ESP32-P4X-C5-Function-EV-Board 尺寸图源文件 (DXF) - 可使用 Autodesk Viewer 查看

• ESP32-P4 系列芯片勘误表

• 1.0K-GT-15PB 规格书 (PDF)

• 摄像头规格书 (PDF)

• 显示屏规格书 (PDF)

• 显示屏驱动芯片 EK73217BCGA 规格书 (PDF)

• 显示屏驱动芯片 EK79007AD 规格书 (PDF)

• 显示屏适配板原理图(PDF)

• 显示屏适配板 PCB 布局图 (PDF)

• 摄像头适配板原理图(PDF)

• 摄像头适配板 PCB 布局图 (PDF)

• 显示屏适配板参考设计 (ZIP)

• 摄像头适配板参考设计 (ZIP)

有关本开发板的更多设计文档，请联系我们的商务部门 sales@espressif.com。

## 2.1.6 免责声明和版权公告

请参阅免责声明和版权公告。

# ESP32-P4X-EYE

ESP32-P4X-EYE 是一款基于 ESP32-P4芯片的视觉开发板，主要面向摄像头应用。开发板的应用示例存放在 Examples 文件夹中。

## 3.1 ESP32-P4X-EYE

本指南将帮助您快速上手 ESP32-P4X-EYE，并提供该款开发板的详细信息。

ESP32-P4X-EYE 是一款基于 ESP32-P4 芯片的视觉开发板，主要面向摄像头应用。ESP32-P4 搭载双核RISC-V 处理器，支持最大 32 MB PSRAM。此外，ESP32-P4 支持 USB 2.0 标准, MIPI-CSI/DSI, H264 Encoder等多种外设，可满足客户对低成本、高性能、低功耗的多媒体产品的开发需求。

此开发板搭载了 ESP32-C6-MINI-1U 模组，用于该开发板的 Wi-Fi 和蓝牙通信；支持 MIPI-CSI 摄像头接口和USB2.0High-Speed从机模式。板载功能丰富，包括摄像头、显示屏、麦克风和MicroSD卡扩展，使得设备能够实时监测周围环境并采集图像、音频数据。适用于智能安防摄像头、视觉模型检测、物联网边缘计算等需要实时图像处理与无线通信的应用场景。

板上芯片大部分管脚均已引出至排母，开发人员可根据实际需求，轻松通过跳线连接多种外围设备。

• 入门指南：简要介绍了 ESP32-P4X-EYE和硬件、软件设置指南。

• 硬件参考：详细介绍了 ESP32-P4X-EYE 的硬件。

• 硬件版本：介绍硬件历史版本和已知问题，并提供链接至历史版本开发板的入门指南（如有）。

• 相关文档：列出了相关文档的链接。

• 免责声明和版权公告: 链接到免责声明和版权公告。

## 3.1.1 入门指南

本节介绍如何开始使用 ESP32-P4X-EYE。首先，介绍一些关于 ESP32-P4X-EYE 的基本信息，然后在应用程序开发 章节介绍如何开始使用该开发板进行开发。

## 组件介绍

以下按照顺时针的顺序依次介绍正面 PCB 上的主要组件。为了方便用户使用，我们同时在 ESP32-P4XEYE 的外壳上标注出了这些组件或者接口。

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/4b42c579bdcf7f3577870d84975f08c5683e95b4e0acc0ef3012b5ed308edca3.jpg)



图 1: ESP32-P4X-EYE 正面图（点击放大）


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/6ff781764652026e591ce3d3d66403f4f3021e66d0464b512de7f69481c956c3.jpg)



图 2: ESP32-P4X-EYE 背面图（点击放大）


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/e020f4eb42cb406111c13bd0c3261950c0a69343e73fda0edd56ff78030c0a16.jpg)



图 3: ESP32-P4X-EYE PCB 正面图（点击放大）


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/63bc13778177a4e13d416be05c897783199f94ee5f4eadb4e6be0db2e1a1937b.jpg)



图 4: ESP32-P4X-EYE 正面图（点击放大）


<table><tr><td>主要组件</td><td>描述</td></tr><tr><td>MicroSD Card Slot (MicroSD卡槽)</td><td>支持通过4线SD总线连接MicroSD卡,兼容SDIO协议与SPI协议两种通信模式。</td></tr><tr><td>Test Points(测试点)</td><td>用于对ESP32-C6-MINI-1U进行烧录测试的测试点,可焊接杜邦线。</td></tr><tr><td>USB 2.0 Device Port (USB 2.0Device接口)</td><td>USB 2.0 Device接口与ESP32-P4芯片的USB 2.0 OTG High-Speed接口连接,支持USB 2.0标准。通过该接口进行USB通讯时,ESP32-P4作为USB Device设备与其他USB Host设备连接。该接口也可用作开发板的供电接口,外壳上以USB 2.0符号标识。</td></tr><tr><td>USB Debug Port (USB Debug接口)</td><td>该接口可用作开发板的供电接口,可烧录固件至芯片,也可作为通信接口,与ESP32-P4芯片上的USB Serial JTAG接口连接。外壳上以Debug符号标识。</td></tr><tr><td>Power Switch(电源开关)</td><td>电源开关。拨向I一侧,开发板连接5V电源上电;拨向○一侧,开发板断开5V电源掉电。</td></tr><tr><td>User-defined Buttons(用户自定义按键)</td><td>用户可根据应用自定义相关功能。</td></tr><tr><td>LCD FPC Connector(屏幕连接器)</td><td>用于连接1.54英寸LCD屏幕。</td></tr><tr><td>LCD</td><td>1.54英寸LCD,SPI接口,分辨率为240x240,支持Camera图像实时显示。详细参数请参考显示屏规格书。</td></tr><tr><td>Charging Indicator(充电指示灯)</td><td>为电池充电时,充电指示灯为红色;充电完成后,指示灯变为绿色。</td></tr></table>

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/10dd236133581ee1a1b1d5d0d49665d5aa5f2a96f3d1dc4e8f600f050134e0a4.jpg)



图 5: ESP32-P4X-EYE 开发板背面图（点击放大）


以下按照顺时针的顺序依次介绍背面 PCB 上的主要组件。为了方便用户使用，我们同时在 ESP32-P4XEYE 的外壳上标注出了这些组件或者接口。

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/abe4e72bed1bb7ef091bfc7bfb0aa763cb8652f72a1d7f4335ccbb2173c54675.jpg)



图 6: ESP32-P4X-EYE 背面图（点击放大）


<table><tr><td>主要组件</td><td>描述</td></tr><tr><td>Rotary Encoder(旋转编码器)</td><td>用户可根据应用自定义相关功能,比如可用于拨动控制LCD界面和摄像头像素缩放。</td></tr><tr><td>ESP32-C6-MINI-1U</td><td>ESP32-C6-MINI-1U模组用于该开发板的Wi-Fi和蓝牙通信。</td></tr><tr><td>Fill Light(补光灯)</td><td>用于拍照录像时补光。</td></tr><tr><td>Digital Microphone(数字麦克风)</td><td>用于语音识别或者录制视频时收音。</td></tr><tr><td>MIPI CSI Connector(MIPI CSI连接器)</td><td>用于连接摄像头模组。</td></tr><tr><td>Female Header(排母)</td><td>2 x 10P排母,用户可根据应用进行自定义。</td></tr><tr><td>SPI flash</td><td>通过SPI接口与ESP32-P4芯片连接,大小为16 MB。</td></tr><tr><td>ESP32-P4</td><td>ESP32-P4是一款高性能MCU,支持超大片上内存,具有强大的图像和语音处理能力。</td></tr><tr><td>Battery Connector(电池接口)</td><td>用于连接锂电池。</td></tr><tr><td>Reset Button (Reset 键)</td><td>复位按键,外壳上以符号标识。</td></tr><tr><td>Boot Button (Boot 键)</td><td>启动模式控制按键,外壳上以符号标识。保持按住Boot键的同时按一下Reset键,ESP-P4将重新启动并进入“固件下载”模式,这时可以通过USB Debug接口下载固件到板载SPI flash中。正常使用中可以作为确认按钮。</td></tr><tr><td>Camera</td><td>分辨率为200万像素,可旋转摄像头进行手动聚焦。详细参数请参考摄像头规格书。</td></tr></table>

## 应用示例

以下为开发板的应用示例：

• Factory Demo - 展示了一个基于 ESP32-P4X-EYE 的全功能迷你相机方案。它不仅实现了拍照、定时拍照、录像、相册预览、USBSD卡挂载和图像参数设置等基础相机功能，还集成了先进的AI视觉能力，包括面部检测、行人检测和基于 YOLOv11nano 模型的实时物体检测。

更多示例及最新更新请参阅 examples 文件夹。

如需尝试应用示例或开发自定义应用，请参照应用程序开发小节中的步骤进行操作。

## 应用程序开发

通电前，请确保 ESP32-P4X-EYE 完好无损。

## 必备硬件

• ESP32-P4X-EYE 

• USB 数据线

• 电脑（Windows、Linux 或 macOS）

## 备注: 请确保使用优质 USB数据线。部分数据线仅可用于充电，无法用于数据传输和编程。

## 可选硬件

• MicroSD 卡

• 锂电池

硬件设置 使用 USB 数据线将 ESP32-P4X-EYE 连接到电脑，可通过 USB 2.0 Device 接口或 USBDebug 接口为开发板供电。建议使用USB Debug 接口烧录固件和调试。

软件设置 请前往 快速入门中详细安装步骤 一节查看如何快速设置开发环境。

## 内含组件和包装

订购信息 该开发板的型号信息详见下表。

<table><tr><td>物料编号</td><td>搭载模组</td><td>Flash</td><td>PSRAM</td><td>天线</td></tr><tr><td>ESP32-P4X-EYE</td><td>ESP32-C6-MINI-1U</td><td>16 MB</td><td>32 MB</td><td>外部天线连接器</td></tr></table>

如需了解更多可选型号，请访问乐鑫产品选型工具，选择开发板标签页。如果在本表或乐鑫产品选型工具中未找到所需型号，或有特殊定制需求，请联系 乐鑫商务。

零售订单 如购买样品，每个开发板将以防静电袋或零售商选择的其他方式包装。

零售订单请前往 https://www.espressif.com/zh-hans/company/contact/buy-a-sample。

批量订单 如批量购买，开发板将以大纸板箱包装。

批量订单请前往 https://www.espressif.com/zh-hans/contact-us/sales-questions。

## 3.1.2 硬件参考

## 功能框图

ESP32-P4X-EYE 的主要组件和连接方式如下图所示。

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/b0c4fbfe9a357cb725af356c02ed5f571b8f73089daaa4ff6fc1f097e454a9bf.jpg)



图 7: ESP32-P4X-EYE 功能框图（点击放大）


## 电源选项

可通过以下方法为开发板供电：

1. 通过 USB 2.0 Device 接口或 USB Debug 接口供电

使用该方法供电时，应使用 USB Type-C 数据线将开发板通过上述任一接口连接至供电设备。如果已安装锂电池，会同时对锂电池进行充电。

## 2. 通过 电池接口使用外部锂电池供电

使用该方法供电时，应将外壳拆开，然后将电池接入电池接口。锂电池体积不超过4mmx25mmx 45mm，使用1.25 mm 间距连接器，正负极需与开发板上标识保持一致。

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/aa61dd0668635051dc9ef2abd084250cdb5d0a77aa10b0aecd913dca5087b87f.jpg)



图 8: 电池连接图（点击放大）



排母


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/07c9afa82dc8c6413ec6fd84584b32d72078d42bbce2e0ab7ec90d3a7bd2f58a.jpg)



图 9: 排母实物图（点击放大）


USB 2.0 Device 接口


USB HighSpeed & Power in:


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/283eed87dff49488bf6ca5a920fb9989953de7b42bfaa230911aae69e81c7002.jpg)



图 10: USB 2.0 Device 接口电路图（点击放大）


## LCD 接口

请注意，该接口支持连接SPI接口屏幕，该开发板使用的屏幕型号为ST7789，LCD_BL(GPIO20)可用于控制屏幕背光。

## SD 卡接口

请注意，SD 卡接口支持：

• 1-线和 4-线的 SD 总线配置

• 可通过SDIO 协议或 SPI协议通信

充电电路

麦克风接口

摄像头接口

滚动编码器接口

ESP32-C6-MINI-1U 模组接口

## 3.1.3 硬件版本

开发板 ESP32-P4X-EYE 相较于ESP32-P4-EYE 的区别是主芯片更新为 ESP32-P4 芯片版本 v3.1 及之后版本。

## 3.1.4 相关文档

请前往 esp-dev-kits 文档 HTML 网页版本 下载以下文档。


MicroSD Card:


## 1.54 INCH LCD:

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/0b7a9feba07f6b118bcdea0911b23001269e1790c03c0fce1c3df4ba928b3758.jpg)



图11: LCD 接口电路图（点击放大）


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/96da8d49e0e433212bd54c13786edb2885922c17f3cf23f16f4d2da03110ee7e.jpg)



图 12: SD 卡接口电路图（点击放大）


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/6742c264a19434e71bd63296b91b537689446d3f0542f7bf278169090d44185f.jpg)


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/4f12ca15295cdb0f4e4d9c48ee080df9ab28e1418ab95138b40616504969e1ed.jpg)



图13: 充电电路图（点击放大）


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/44a88ad7d4975f3fdb989a3eaf50db92286b7b71f2bb673a364f5d4b1ec35ff9.jpg)



图14: 麦克风接口电路图（点击放大）



I2C LevelShift:


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/8970e5971e878dc95a9675bc497b4f00eda095915f05875de3329ce85d49be4b.jpg)



图15: 摄像头接口电路图（点击放大）


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/33484c9a6cd2e9b4be364a2fa2a0519938837bb3190db03d5b592401ee672f0f.jpg)



图16: 滚动编码器接口电路图（点击放大）



WiFi & BT Module:


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/87e373bc8383f01679c9d47e05d649300f6f3dba4e700466b72e6873f9d6389d.jpg)



图 17: ESP32-C6-MINI-1U 模组接口电路图（点击放大）


• ESP32-P4X-EYE 参考设计 (ZIP)

• 摄像头规格书 (PDF)

• 显示屏规格书 (PDF)

• ST7789VW 规格书 (PDF)

• OV2710 概述 (PDF)

## 3.1.5 免责声明和版权公告

请参阅免责声明和版权公告。

# Chapter 4

# 寿命终止开发板

本节包含 ESP32-P4 寿命终止开发板的用户指南，内容仅供参考。这些开发板虽然可能仍在市场上销售或在遗留系统中使用，但已停止接收更新、错误修复及官方支持。建议切换至新款开发板，以享受更优性能和更丰富的功能。

## 4.1 ESP32-P4-Function-EV-Board

ESP32-P4-Function-EV-Board 是一款基于 ESP32-P4 芯片的多媒体开发板。ESP32-P4 搭载双核 RISC-V 处理器，支持最大 32 MB PSRAM。此外，ESP32-P4 支持 USB 2.0 标准, MIPI-CSI/DSI, H264 Encoder 等多种外设，可满足客户对低成本、高性能、低功耗的多媒体产品的开发需求。

## 4.1.1 ESP32-P4-Function-EV-Board v1.5.2

更早版本：ESP32-P4-Function-EV-Board v1.4

备注: 若您使用的是搭载芯片版本 v3.x 的 ESP32-P4X-Function-EV-Board，请参阅此用户指南。

版本号v1.5.2指开发板的硬件版本。要识别所使用的芯片版本，请查看ESP32-P4系列芯片勘误表>芯片版本标识。

本指南将帮助您快速上手 ESP32-P4-Function-EV-Board，并提供该款开发板的详细信息。

ESP32-P4-Function-EV-Board 是一款基于 ESP32-P4 芯片的多媒体开发板。ESP32-P4 搭载双核 RISC-V 处理器，支持最大 32 MB PSRAM。此外，ESP32-P4 支持 USB 2.0 标准, MIPI-CSI/DSI, H264 Encoder 等多种外设，可满足客户对低成本、高性能、低功耗的多媒体产品的开发需求。

此开发板搭载了 ESP32-C6-MINI-1 2.4 GHz Wi-Fi 6 & Bluetooth 5 (LE) 模组，用于该开发板的 Wi-Fi 和蓝牙通信；它还配置了一块7英寸电容式触摸屏，分辨率为1024x600，并配有一颗200万像素的MIPICSI接口摄像头，丰富了用户的交互体验。该开发板适用于可视门铃、网络摄像头、智能家居中控屏、LCD电子价签、两轮车仪表盘等产品的原型构建。

板上模组大部分管脚均已引出至排针，开发人员可根据实际需求，轻松通过跳线连接多种外围设备。

本指南包括如下内容：

• 入门指南：简要介绍了 ESP32-P4-Function-EV-Board 和硬件、软件设置指南。

• 硬件参考：详细介绍了 ESP32-P4-Function-EV-Board 的硬件。

• 硬件版本：介绍硬件历史版本和已知问题，并提供链接至历史版本开发板的入门指南（如有）。

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/8be710a7af4320d978388e93dcdd07f8c9e010e0032812de0ddc35e61a6cd88a.jpg)



图 1: ESP32-P4-Function-EV-Board


• 相关文档：列出了相关文档的链接。

• 免责 $\lneqq$ 明和版权公告: 链接到免责声明和版权公告。

## 入门指南

本小节将简要介绍 ESP32-P4-Function-EV-Board，说明如何在 ESP32-P4-Function-EV-Board 上烧录固件及相关准备工作。

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/4edb2451b816e1e4f87023896855daa0dba79f244569b398450d9391d26d7144.jpg)



(16) USB Serial/JTAG Port



图 2: ESP32-P4-Function-EV-Board - 正面（点击放大）



组件介绍 下面从 J1开始按照顺时针的顺序依次介绍开发板正面和背面的主要组件。


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/0def13ff1f60f5b3166a1640f099ec6ea23d7db0f4031fcfb8c549b672f9b0ae.jpg)



图 3: ESP32-P4-Function-EV-Board - 背面（点击放大）


<table><tr><td>组件编号</td><td>主要组件</td><td>介绍</td></tr><tr><td>1</td><td>J1</td><td>所有可用 GPIO 管脚均已引出至排针 J1,详细信息请见排针。</td></tr><tr><td>2</td><td>ESP32-C6 Module Programming Connector (ESP32-C6模组固件烧录接口)</td><td>该接口可配合使用 ESP-Prog 或其他 UART 工具为 ESP32-C6 模组烧录固件。</td></tr><tr><td>3</td><td>ESP32-C6-MINI-1 Module (ESP32-C6-MINI-1 模组)</td><td>ESP32-C6-MINI-1 模组用于该开发板的 Wi-Fi 和蓝牙通信。</td></tr><tr><td>4</td><td>Microphone (麦克风)</td><td>板载麦克风,连接至 Audio Codec Chip 接口。</td></tr><tr><td>5</td><td>Reset Button (Reset键)</td><td>复位按键。</td></tr><tr><td>6</td><td>Audio Codec Chip(音频编解码芯片)</td><td>音频编解码器芯片 ES8311 是一种低功耗单声道音频编解码器,包含单通道 ADC、单通道 DAC、低噪声前置放大器、耳机驱动器、数字音效、模拟混音和增益功能。它通过 I2S 和 I2C 总线与 ESP32-P4 芯片连接,以提供独立于音频应用程序的硬件音频处理。</td></tr><tr><td>7</td><td>Speaker Output Port (扬声器输出端口)</td><td>该输出端口用于连接扬声器。最高输出功率可以驱动 4 Ω 3 W 扬声器,引脚间距为 2.00 mm/0.08”。</td></tr><tr><td>8</td><td>Audio PA Chip (音频功率放大器)</td><td>NS4150 是一款低 EMI、3 W 单声道 D 类音频功率放大器,用于放大来自音频编解码芯片的音频信号,以驱动扬声器。</td></tr><tr><td>9</td><td>5 V to 3.3 V LDO (5 V转 3.3 V LDO)</td><td>电源转换器,输入 5 V,输出 3.3 V。</td></tr><tr><td>10</td><td>BOOT Button (BOOT 键)</td><td>启动模式控制按键,保持按住 BOOT 键的同时按一下 Reset 键,ESP-P4 将重新启动并进入“固件下载”模式,这时可以通过 USB 串口/JTAG 接口下载固件到板载 SPI flash 中。</td></tr><tr><td>11</td><td>Ethernet PHY IC</td><td>以太网 PHY 芯片,与 ESP32-P4 EMAC RMII 接口和 RJ45 以太网模块端口连接。</td></tr><tr><td colspan="2">Expressif System Block Converter (降压转换器)</td><td>用于 3.3 V 电源的降压型 DC-DC 转换器。 Release master Submit Document Feedback</td></tr><tr><td>13</td><td>5 V Power-on LED (5 V 电源指示灯)</td><td>开发板通过任一 USB Type-C 接口连接电源后,该指示灯亮起。</td></tr><tr><td>19</td><td>Power Switch(电源开关)</td><td>电源开关。拨向ON一侧,开发板连接5V电源上电;拨离ON一侧,开发板断开5V电源掉电。</td></tr><tr><td>20</td><td>Switch(开关)</td><td>TPS2051C是一款USB电源开关,提供500mA输出电流限制。</td></tr><tr><td>21</td><td>MIPI CSI Connector (MIPI CSI连接器)</td><td>FPC连接器1.0K-GT-15PB,用以连接外接摄像头模组,实现图像传输,详情请见相关文档中的1.0K-GT-15PB规格书。适配的FPC规格:间距1.0mm,管脚宽度0.7mm,厚度0.3mm,管脚数量15。</td></tr><tr><td>22</td><td>Buck Converter(降压转换器)</td><td>用于VDD_HP电源的降压型DC-DC转换器。</td></tr><tr><td>23</td><td>ESP32-P4</td><td>ESP32-P4是一款高性能MCU,支持超大片上内存,具有强大的图像和语音处理能力。</td></tr><tr><td>24</td><td>40 MHz XTAL</td><td>40 MHz晶振,用于提供系统时钟。</td></tr><tr><td>25</td><td>32.768 kHz XTAL</td><td>32.768 kHz晶振,可提供Deep-sleep下使用的低功耗时钟。</td></tr><tr><td>26</td><td>MIPI DSI Connector (MIPI DSI连接器)</td><td>FPC连接器1.0K-GT-15PB,用以连接LCD扩展板,详情请见相关文档中的1.0K-GT-15PB规格书。适配的FPC规格:间距1.0mm,管脚宽度0.7mm,厚度0.3mm,管脚数量15。</td></tr><tr><td>27</td><td>SPI flash</td><td>通过SPI接口与ESP32-P4芯片连接,大小为16MB。</td></tr><tr><td>28</td><td>MicroSD Card Slot(MicroSD卡槽)</td><td>本开发板支持4-bit模式的MicroSD卡,可以存储或播放MicroSD卡中的音频文件。</td></tr></table>

开发板配件 ESP32-P4-Function-EV-Board 的包装盒中包含以下可选配件：

• 显示屏及其附件（可选）

– 7 英寸 MIPI DSI 电容式触摸屏，分辨率为 1024 x 600 像素

– LCD 屏幕适配板

– 附件包，包括杜邦线、屏幕排线、长铜柱（长度为 20 mm）、短铜柱（长度为8 mm）

• 摄像头及其附件（可选）

– 200 万像素 MIPI CSI 接口摄像头

– 摄像头适配板

– 摄像头排线


Ribbon Cable in the Forward Direction for Camera


<table><tr><td colspan="6">AWM 20624 80C 60V VW-1</td><td colspan="5">AWM 20624 80C 60V VW-1</td></tr><tr><td>AWM</td><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td></tr><tr><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V VW-1</td></tr><tr><td>VW-1</td><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V VW-1</td><td></td><td></td><td>AWM</td><td>20624</td><td>80C 60V</td></tr><tr><td>60V VW-1</td><td></td><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V VW-1</td><td></td><td></td><td>AWM</td><td>20624 80C</td></tr></table>

## Strip


Ribbon Cable in the Reverse Direction for LCD


<table><tr><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td><td></td><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td><td></td><td></td><td>AWM</td></tr><tr><td>AWM</td><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td><td></td><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td><td></td><td></td></tr><tr><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td><td></td><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td><td></td></tr><tr><td></td><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td><td></td><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td></tr><tr><td>VW-1</td><td></td><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td><td></td><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V</td></tr></table>


图4: 正向和反向线序排线


备注: 同向线序的排线为摄像头排线，其排线两侧的金属触点在同一面上；反向线序的排线为屏幕排线，

其排线两侧的金属触点在不同面上。

应用示例 以下为开发板的应用示例：

• ESP_Brookesia Phone - 展示了一个类似安卓的界面，使用 ESP_Brookesia，利用开发板上的 MIPI-DSI、MIPI-CSI、ESP32-C6、SD卡和音频接口运行多个应用程序，为高效的多媒体应用开发提供了基础。

• LVGL Demo v8 - 演示如何在配备 7 英寸 LCD 屏幕的 ESP32-P4-Function-EV-Board 上移植 LVGL v8并使用LVGL的内置演示进行性能测试，为基于 LVGLv8 开发应用程序提供基础。

• LVGL Demo v9 - 演示如何在 ESP32-P4-Function-EV-Board 上移植 LVGL v9 并使用 LVGL 内置的演示进行性能测试，作为基于 LVGLv9 开发应用程序的基础。

更多示例及最新更新请参阅 examples 文件夹。

如需尝试应用示例或开发自定义应用，请参照开始开发应用小节中的步骤进行操作。

开始开发应用 通电前，请确保 ESP32-P4-Function-EV-Board 完好无损。

## 必备硬件

• ESP32-P4-Function-EV-Board 

• USB 数据线

• 电脑（Windows、Linux 或 macOS）

备注: 请确保使用优质 USB 数据线。部分数据线仅可用于充电，无法用于数据传输和编程。

## 可选硬件

• MicroSD 卡

硬件设置 使用 USB 数据线将 ESP32-P4-Function-EV 连接到电脑，可通过任何一个 USB Type-C 端口为开发板供电。建议使用 USB 串口/JTAG 接口烧录固件和调试。

如需使用显示屏，请按照以下步骤连接显示屏：

1. 用短铜柱将开发板固定在LCD 屏幕适配板中心的四个螺母柱上。

2. 用屏幕排线（反向线序）连接 LCD 屏幕适配板的 J3 排针和开发板的 MIPI DSI 连接器。LCD 屏幕适配板已与LCD 连接。

3. 用杜邦线将LCD屏幕适配板J6排针的RST_LCD引脚连接到开发板J1排针的GPIO27引脚。使用时RST_LCD 由ESP32-P4 芯片控制，具体引脚可由软件设置，默认为 GPIO27。

4. 用杜邦线将 LCD 屏幕适配板 J6 排针的 PWM 引脚连接到开发板 J1 排针的 GPIO26 引脚。使用时PWM由 ESP32-P4芯片控制，具体引脚可由软件设置，默认为 GPIO26。

5. 推荐使用外接 USB 供电连接 LCD 屏幕适配板 J1 排针对屏幕供电。如果条件不允许，也可以在开发板供电充足的情况下，使用杜邦线将 LCD 屏幕适配板的 5V 与 GND 管脚连接到开发板的 J1 排针的 5V与 GND引脚。

6. 将长铜柱固定在 LCD 屏幕适配板四周的四个螺母柱上，可以将 LCD 立放。

连接关系如下表所示：

<table><tr><td>LCD屏幕适配板</td><td>ESP32-P4-Function-EV</td></tr><tr><td>J3排针</td><td>MIPI DSI连接器</td></tr><tr><td>J6排针RST_LCD引脚</td><td>J1排针GPIO27引脚</td></tr><tr><td>J6排针PWM引脚</td><td>J1排针GPIO26引脚</td></tr><tr><td>J6排针5V引脚</td><td>J1排针5V引脚</td></tr><tr><td>J6排针GND引脚</td><td>J1排针GND引脚</td></tr></table>

备注:

• 如果使用外接USB供电连接LCD 屏幕适配板，则不需要连接 5V 和GND 引脚。

• 如需使用摄像头，请将摄像头排线（同向线序）连接至摄像头适配板和开发板的 MIPICSI 连接器。

软件设置 请前往 ESP-IDF 快速入门，查看如何快速设置开发环境，将应用程序烧录至您的开发板。

硬件参考

功能框图 ESP32-P4-Function-EV-Board 的主要组件和连接方式如下图所示。

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/124b2da6fb803b3424c9e7cec1452c20a82a0a8cb55605759e20c67f0656ba7c.jpg)



图 5: ESP32-P4-Function-EV-Board 功能框图（点击放大）


电源选项 可通过以下接口为开发板供电：

• USB 2.0 Type-C 接口

• USB 全速接口

• USB 串口/JTAG 接口

如果用于调试的 USB数据线无法提供足够的电流，可通过空闲的 USBType-C 接口连接电源适配器。

排针 下表列出了开发板排针J1的名称和功能，排针的名称如图ESP32-P4-Function-EV-Board-正面（点击放大） 所示，排针的序号与 ESP32-P4-Function-EV-Board 原理图 一致。

<table><tr><td>序号</td><td>名称</td><td>类型<eq>^{Page\ 48,1}</eq></td><td>功能</td></tr><tr><td>1</td><td>3V3</td><td>P</td><td>3.3 V电源</td></tr><tr><td>2</td><td>5V</td><td>P</td><td>5 V电源</td></tr><tr><td>3</td><td>7</td><td>I/O/T</td><td>GPIO7</td></tr><tr><td>4</td><td>5V</td><td>P</td><td>5 V电源</td></tr><tr><td>5</td><td>8</td><td>I/O/T</td><td>GPIO8</td></tr><tr><td>6</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>7</td><td>23</td><td>I/O/T</td><td>GPIO23</td></tr><tr><td>8</td><td>37</td><td>I/O/T</td><td>U0TXD, GPIO37</td></tr><tr><td>9</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>10</td><td>38</td><td>I/O/T</td><td>U0RXD, GPIO38</td></tr><tr><td>11</td><td>21</td><td>I/O/T</td><td>GPIO21</td></tr><tr><td>12</td><td>22</td><td>I/O/T</td><td>GPIO22</td></tr><tr><td>13</td><td>20</td><td>I/O/T</td><td>GPIO20</td></tr><tr><td>14</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>15</td><td>6</td><td>I/O/T</td><td>GPIO6</td></tr><tr><td>16</td><td>5</td><td>I/O/T</td><td>GPIO5</td></tr><tr><td>17</td><td>3V3</td><td>P</td><td>3.3 V电源</td></tr><tr><td>18</td><td>4</td><td>I/O/T</td><td>GPIO4</td></tr><tr><td>19</td><td>3</td><td>I/O/T</td><td>GPIO3</td></tr><tr><td>20</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>21</td><td>2</td><td>I/O/T</td><td>GPIO2</td></tr><tr><td>22</td><td>NC(1)</td><td>I/O/T</td><td><eq>GPIO1^2</eq></td></tr><tr><td>23</td><td>NC(0)</td><td>I/O/T</td><td><eq>GPIO0^2</eq></td></tr><tr><td>24</td><td>36</td><td>I/O/T</td><td>GPIO36</td></tr><tr><td>25</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>26</td><td>32</td><td>I/O/T</td><td>GPIO32</td></tr><tr><td>27</td><td>NC</td><td>-</td><td>空管脚</td></tr><tr><td>28</td><td>NC</td><td>-</td><td>空管脚</td></tr><tr><td>29</td><td>33</td><td>I/O/T</td><td>GPIO33</td></tr><tr><td>30</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>31</td><td>26</td><td>I/O/T</td><td>GPIO26</td></tr><tr><td>32</td><td>54</td><td>I/O/T</td><td>GPIO54</td></tr><tr><td>33</td><td>48</td><td>I/O/T</td><td>GPIO48</td></tr><tr><td>34</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>35</td><td>53</td><td>I/O/T</td><td>GPIO53</td></tr><tr><td>36</td><td>46</td><td>I/O/T</td><td>GPIO46</td></tr><tr><td>37</td><td>47</td><td>I/O/T</td><td>GPIO47</td></tr><tr><td>38</td><td>27</td><td>I/O/T</td><td>GPIO27</td></tr><tr><td>39</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>40</td><td>NC(45)</td><td>I/O/T</td><td><eq>GPIO45^3</eq></td></tr></table>

## J1

硬件版本

首次发布

备注: 开发板 v1.52 版本相较于 v1.4 版本的主要改进包括：将调试用的 USB-to-UART Type-C 接口替换为 ESP32-P4 芯片内置的 USB 串口/JTAG 接口，并新增了全速 USB OTG 至 Type-C 的接口引出。IO24、IO25 管脚不再引出到排针。

## 相关文档

请前往 esp-dev-kits 文档 HTML 网页版本 下载以下文档。

• ESP32-P4-Function-EV-Board 原理图 (PDF)

• ESP32-P4-Function-EV-Board PCB 布局图 (PDF)

• ESP32-P4-Function-EV-Board 元件位号图 (PDF)

• ESP32-P4-Function-EV-Board 尺寸图 (PDF)

• ESP32-P4-Function-EV-Board 尺寸图源文件 (DXF) - 可使用 Autodesk Viewer 查看

• 1.0K-GT-15PB 规格书 (PDF)

• 摄像头规格书 (PDF)

• 显示屏规格书 (PDF)

• 显示屏驱动芯片 EK73217BCGA 规格书 (PDF)

• 显示屏驱动芯片 EK79007AD 规格书 (PDF)

• 显示屏适配板原理图(PDF)

• 显示屏适配板 PCB 布局图 (PDF)

• 摄像头适配板原理图(PDF)

• 摄像头适配板 PCB 布局图 (PDF)

有关本开发板的更多设计文档，请联系我们的商务部门 sales@espressif.com。

## ESP32-P4-Function-EV-Board v1.4

最新版本：ESP32-P4-Function-EV-Board v1.5.2

备注: 若您使用的是搭载芯片版本 v3.x 的 ESP32-P4X-Function-EV-Board，请参阅此用户指南。

版本号 v1.4 指开发板的硬件版本。要识别所使用的芯片版本，请查看 ESP32-P4 系列芯片勘误表 > 芯片版本标识。

本指南将帮助您快速上手 ESP32-P4-Function-EV-Board，并提供该款开发板的详细信息。

ESP32-P4-Function-EV-Board 是一款基于 ESP32-P4 芯片的多媒体开发板。ESP32-P4 搭载双核 RISC-V 处理器，支持最大 32 MB PSRAM。此外，ESP32-P4 支持 USB 2.0 标准, MIPI-CSI/DSI, H264 Encoder 等多种外设，可满足客户对低成本、高性能、低功耗的多媒体产品的开发需求。

此开发板搭载了 ESP32-C6-MINI-1 2.4 GHz Wi-Fi 6 & Bluetooth 5 (LE) 模组，用于该开发板的 Wi-Fi 和蓝牙通信；它还配置了一块7英寸电容式触摸屏，分辨率为1024x600，并配有一颗200万像素的MIPICS接口摄像头，丰富了用户的交互体验。该开发板适用于可视门铃、网络摄像头、智能家居中控屏、LCD电子价签、两轮车仪表盘等产品的原型构建。

板上模组大部分管脚均已引出至排针，开发人员可根据实际需求，轻松通过跳线连接多种外围设备。

本指南包括如下内容：

• 入门指南：简要介绍了 ESP32-P4-Function-EV-Board 和硬件、软件设置指南。

• 硬件参考：详细介绍了 ESP32-P4-Function-EV-Board 的硬件。

• 硬件版本：介绍硬件历史版本和已知问题，并提供链接至历史版本开发板的入门指南（如有）。

• 相关文档：列出了相关文档的链接。

• 免责声明和版权公告: 链接到免责声明和版权公告。

入门指南 本小节将简要介绍 ESP32-P4-Function-EV-Board，说明如何在 ESP32-P4-Function-EV-Board 上烧录固件及相关准备工作。

组件介绍 以下按照顺时针的顺序依次介绍开发板上的主要组件。

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/3a6b15835437aed16f8cd4550210290db234224c25e124ea9f971bafde091275.jpg)



图 6: ESP32-P4-Function-EV-Board


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/5ef7f0997f24bc5645c8727fe9a77e27cac08e23ddfa3bd994efc4282107e0b9.jpg)



图 7: ESP32-P4-Function-EV-Board - 正面（点击放大）


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/610eff4a0a2306f849199380d6760950f93148c6548406d5fde24770cfa2bb32.jpg)



图 8: ESP32-P4-Function-EV-Board - 背面（点击放大）


<table><tr><td>主要组件</td><td>介绍</td></tr><tr><td>J1</td><td>所有可用 GPIO 管脚均已引出至排针 J1,详细信息请见排针。</td></tr><tr><td>ESP32-C6 Module Programming Connector (ESP32-C6 模组固件烧录接口)</td><td>该接口可配合使用 ESP-Prog 或其他 UART 工具为 ESP32-C6 模组烧录固件。</td></tr><tr><td>ESP32-C6-MINI-1 Module (ESP32-C6-MINI-1 模组)</td><td>ESP32-C6-MINI-1 模组用于该开发板的 Wi-Fi 和蓝牙通信。</td></tr><tr><td>Microphone (麦克风)</td><td>板载麦克风,连接至 Audio Codec Chip 接口。</td></tr><tr><td>Reset Button (Reset 键)</td><td>复位按键。</td></tr><tr><td>Audio Codec Chip (音频编解码芯片)</td><td>音频编解码器芯片 ES8311 是一种低功耗单声道音频编解码器,包含单通道 ADC、单通道 DAC、低噪声前置放大器、耳机驱动器、数字音效、模拟混音和增益功能。它通过 I2S 和 I2C 总线与 ESP32-P4 芯片连接,以提供独立于音频应用程序的硬件音频处理。</td></tr><tr><td>Speaker Output Port (扬声器输出端口)</td><td>该输出端口用于连接扬声器。最高输出功率可以驱动 4 Ω 3 W 扬声器,引脚间距为 2.00 mm/0.08”。</td></tr><tr><td>Audio PA Chip (音频功率放大器)</td><td>NS4150 是一款低 EMI、3 W 单声道 D 类音频功率放大器,用于放大来自音频编解码芯片的音频信号,以驱动扬声器。</td></tr><tr><td>5 V to 3.3 V LDO (5 V 转 3.3 V LDO)</td><td>电源转换器,输入 5 V,输出 3.3 V。</td></tr><tr><td>BOOT Button (BOOT 键)</td><td>启动模式控制按键,保持按住 BOOT 键的同时按一下 Reset 键,ESP-P4 将重新启动并进入“固件下载”模式,这时可以通过 USB 转 UART 接口下载固件到板载 SPI flash 中。</td></tr><tr><td>Ethernet PHY IC</td><td>以太网 PHY 芯片,与 ESP32-P4 EMAC RMII 接口和 RJ45 以太网模块端口连接。</td></tr><tr><td>Buck Converter (降压转换器)</td><td>用于 3.3 V 电源的降压型 DC-DC 转换器。</td></tr><tr><td>USB-to-UART Bridge Chip (USB 转 UART 桥接器)</td><td>单芯片 USB 转 UART 桥接器芯片 CP2102N,与 ESP32-P4 UART0 接口、CHIP_PU 及 GPIO35 管脚 (strapping pin) 连接,可提供高达 3 Mbps 的传输速率,用于固件下载和调试 log 打印,支持自动下载功能。</td></tr><tr><td>5 V Power-on LED (5 V 电源指示灯)</td><td>开发板通过任一 USB Type-C 接口连接电源后,该指示灯亮起。</td></tr><tr><td>RJ45 Ethernet Port (RJ45 以太网接口)</td><td>以太网接口,支持 10/100 Mbps 自适应。</td></tr><tr><td>USB-to-UART Port (USB 转 UART 接口)</td><td>USB Type-C 接口,可用作开发板的供电接口,可烧录固件至芯片,也可作为通信接口,通过板载 USB 转 UART 桥接器与 ESP32-P4 芯片通信。</td></tr><tr><td>USB Power-in Port (USB 电源输入接口)</td><td>开发板的 USB Type-C 供电接口。</td></tr><tr><td>USB 2.0 Type-C Port (USB 2.0 Type-C 接口)</td><td>USB 2.0 Type-C 接口与 ESP32-P4 芯片的 USB 2.0 OTG High-Speed 接口连接,支持 USB 2.0 标准。通过该接口进行 USB 通讯时,ESP32-P4 作为 USB Device 设备与其他 USB Host 设备连接。USB 2.0 Type-C 接口和 USB 2.0 Type-A 接口需要二选一使用,不能同时使用。该接口也可用作开发板的供电接口。</td></tr><tr><td>USB 2.0 Type-A Port (USB 2.0 Type-A 接口)</td><td>USB 2.0 Type-A 接口与 ESP32-P4 芯片的 USB 2.0 OTG High-Speed 接口连接,支持 USB 2.0 标准。通过该接口进行 USB 通讯时,ESP32-P4 作为 USB Host 与其它 USB device 设备连接,对外提供最高 500 mA 电流。USB 2.0 Type-C 接口和 USB 2.0 Type-A 接口需要二选一使用,不能同时使用。</td></tr><tr><td>Power Switch (电源开关)</td><td>电源开关。拨向 ON 一侧,开发板连接 5 V 电源上电;拨离 ON 一侧,开发板断开 5 V 电源掉电。</td></tr><tr><td>Switch(开关)</td><td>TPS2051C是一款USB电源开关,提供500mA输出电流限制。</td></tr><tr><td>MIPI CSI Connector(MIPI CSI连接器)</td><td>FPC连接器1.0K-GT-15PB,用以连接外接摄像头模组,实现图像传输,详情请见相关文档中的1.0K-GT-15PB规格书。适配的FPC规格:间距1.0mm,管脚宽度0.7mm,厚度0.3mm,管脚数量15。</td></tr><tr><td>Buck Converter(降压转换器)</td><td>用于VDD_HP电源的降压型DC-DC转换器。</td></tr><tr><td>ESP32-P4</td><td>ESP32-P4是一款高性能MCU,支持超大片上内存,具有强大的图像和语音处理能力。</td></tr><tr><td>40 MHz XTAL</td><td>40 MHz晶振,用于提供系统时钟。</td></tr><tr><td>32.768 kHz XTAL</td><td>32.768 kHz晶振,可提供Deep-sleep下使用的低功耗时钟。</td></tr><tr><td>MIPI DSI Connector(MIPI DSI连接器)</td><td>FPC连接器1.0K-GT-15PB,用以连接LCD扩展板,详情请见相关文档中的1.0K-GT-15PB规格书。适配的FPC规格:间距1.0mm,管脚宽度0.7mm,厚度0.3mm,管脚数量15。</td></tr><tr><td>SPI flash</td><td>通过SPI接口与ESP32-P4芯片连接,大小为16MB。</td></tr><tr><td>MicroSD Card Slot(MicroSD卡槽)</td><td>本开发板支持4-bit模式的MicroSD卡,可以存储或播放MicroSD卡中的音频文件。</td></tr></table>

## 开发板配件 ESP32-P4-Function-EV-Board 的包装盒中包含以下可选配件：

• 显示屏及其附件（可选）

– 7 英寸 MIPI DSI 电容式触摸屏，分辨率为 1024 x 600 像素

– LCD 屏幕适配板

– 附件包，包括杜邦线、屏幕排线、长铜柱（长度为 20 mm）、短铜柱（长度为8 mm）

• 摄像头及其附件（可选）

– 200 万像素 MIPI CSI 接口摄像头

– 摄像头适配板

– 摄像头排线


Ribbon Cable in the Forward Direction for Camera


<table><tr><td colspan="6">AWM 20024 80C 60V VW-1</td><td colspan="6">AWM 20024 80C 60V VW-1</td></tr><tr><td rowspan="2">1</td><td colspan="5">AWM 20624 80C 60V VW-1</td><td colspan="6">AWM 20624 80C 60V VW-1</td></tr><tr><td colspan="5">AWM 20624 80C 60V VW-1</td><td colspan="6">AWM 20624 80C 60V VW-1</td></tr><tr><td colspan="2">VW-1</td><td colspan="5">AWM 20624 80C 60V VW-1</td><td colspan="5">AWM 20624 80C 60V VW-1</td></tr><tr><td colspan="2">60V VW-1</td><td colspan="5">AWM 20624 80C 60V VW-1</td><td colspan="5">AWM 20624 80C 60V VW-1</td></tr></table>


Strip



Ribbon Cable in the Reverse Direction for LCD


<table><tr><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td><td></td><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td><td></td><td></td><td>AWM</td></tr><tr><td>AWM</td><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td><td></td><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td><td></td><td></td></tr><tr><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td><td></td><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td><td></td></tr><tr><td></td><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td><td></td><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td></tr><tr><td>VW-1</td><td></td><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V</td><td>VW-1</td><td></td><td></td><td>AWM</td><td>20624</td><td>80C</td><td>60V</td></tr></table>


图9: 正向和反向线序排线


备注: 同向线序的排线为摄像头排线，其排线两侧的金属触点在同一面上；反向线序的排线为屏幕排线，其排线两侧的金属触点在不同面上。

应用示例 以下为开发板的应用示例：

• ESP_Brookesia Phone - 展示了一个类似安卓的界面，使用 ESP_Brookesia，利用开发板上的 MIPI-DSI、MIPI-CSI、ESP32-C6、SD卡和音频接口运行多个应用程序，为高效的多媒体应用开发提供了基础。

• LVGL Demo v8 - 演示如何在配备 7 英寸 LCD 屏幕的 ESP32-P4-Function-EV-Board 上移植 LVGL v8，并使用LVGL的内置演示进行性能测试，为基于 LVGLv8 开发应用程序提供基础。

• LVGL Demo v9 - 演示如何在 ESP32-P4-Function-EV-Board 上移植 LVGL v9 并使用 LVGL 内置的演示进行性能测试，作为基于 LVGL v9 开发应用程序的基础。

更多示例及最新更新请参阅 examples 文件夹。

如需尝试应用示例或开发自定义应用，请参照开始开发应用小节中的步骤进行操作。

开始开发应用 通电前，请确保 ESP32-P4-Function-EV-Board 完好无损。

## 必备硬件

• ESP32-P4-Function-EV-Board 

• USB 数据线

• 电脑（Windows、Linux 或 macOS）

备注: 请确保使用优质 USB数据线。部分数据线仅可用于充电，无法用于数据传输和编程。

## 可选硬件

• MicroSD 卡

硬件设置 使用 USB 数据线将 ESP32-P4-Function-EV 连接到电脑，可通过任何一个 USB Type-C 端口为开发板供电。建议使用 USB转UART接口烧录固件和调试。

如需使用显示屏，请按照以下步骤连接显示屏：

1. 用短铜柱将开发板固定在LCD 屏幕适配板中心的四个螺母柱上。

2. 用屏幕排线（反向线序）连接 LCD 屏幕适配板的 J3 排针和开发板的 MIPI DSI 连接器。LCD 屏幕适配板已与LCD 连接。

3. 用杜邦线将LCD屏幕适配板J6排针的RST_LCD引脚连接到开发板J1排针的GPIO27引脚。使用时RST_LCD 由ESP32-P4 芯片控制，具体引脚可由软件设置，默认为 GPIO27。

4. 用杜邦线将 LCD 屏幕适配板 J6 排针的 PWM 引脚连接到开发板 J1 排针的 GPIO26 引脚。使用时PWM由 ESP32-P4芯片控制，具体引脚可由软件设置，默认为 GPIO26。

5. 推荐使用外接 USB 供电连接 LCD 屏幕适配板 J1 排针对屏幕供电。如果条件不允许，也可以在开发板供电充足的情况下，使用杜邦线将 LCD 屏幕适配板的 5V 与 GND 管脚连接到开发板的 J1 排针的 5V与 GND引脚。

6. 将长铜柱固定在 LCD 屏幕适配板四周的四个螺母柱上，可以将 LCD 立放。

连接关系如下表所示：

<table><tr><td>LCD 屏幕适配板</td><td>ESP32-P4-Function-EV</td></tr><tr><td>J3 排针</td><td>MIPI DSI 连接器</td></tr><tr><td>J6 排针 RST_LCD 引脚</td><td>J1 排针 GPIO27 引脚</td></tr><tr><td>J6 排针 PWM 引脚</td><td>J1 排针 GPIO26 引脚</td></tr><tr><td>J6 排针 5V 引脚</td><td>J1 排针 5V 引脚</td></tr><tr><td>J6 排针 GND 引脚</td><td>J1 排针 GND 引脚</td></tr></table>

## 备注:

• 如果使用外接USB供电连接LCD 屏幕适配板，则不需要连接 5V 和GND 引脚。

续下页

• 如需使用摄像头，请将摄像头排线（同向线序）连接至摄像头适配板和开发板的 MIPICSI 连接器。

软件设置 请前往 ESP-IDF 快速入门，查看如何快速设置开发环境，将应用程序烧录至您的开发板。

硬件参考

功能框图 ESP32-P4-Function-EV-Board 的主要组件和连接方式如下图所示。

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/dcdf2e985edc1fba5fda9461af8f36bb1f6809fa57c54211c15b4fc8d6094499.jpg)



图 10: ESP32-P4-Function-EV-Board v1.4（点击放大）



电源选项 可通过以下接口为开发板供电：



• USB 2.0 Type-C 接口



• USB 电源输入接口



• USB 转 UART 接口



如果用于调试的 USB数据线无法提供足够的电流，可通过空闲的 USBType-C 接口连接电源适配器。


排针 下表列出了开发板排针J1的名称和功能，排针的名称如图ESP32-P4-Function-EV-Board-正面（点击放大） 所示，排针的序号与 ESP32-P4-Function-EV-Board v1.4 原理图 一致。

<table><tr><td>序号</td><td>名称</td><td>类型Page 56,1</td><td>功能</td></tr><tr><td>1</td><td>3V3</td><td>P</td><td>3.3 V电源</td></tr><tr><td>2</td><td>5V</td><td>P</td><td>5 V电源</td></tr><tr><td>3</td><td>7</td><td>I/O/T</td><td>GPIO7</td></tr><tr><td>4</td><td>5V</td><td>P</td><td>5 V电源</td></tr><tr><td>5</td><td>8</td><td>I/O/T</td><td>GPIO8</td></tr><tr><td>6</td><td>GND</td><td>GND</td><td>接地</td></tr></table>


表 2 – 接上页


<table><tr><td>序号</td><td>名称</td><td>类型<eq>^{Page 56,1}</eq></td><td>功能</td></tr><tr><td>7</td><td>23</td><td>I/O/T</td><td>GPIO23</td></tr><tr><td>8</td><td>37</td><td>I/O/T</td><td>U0TXD, GPIO37</td></tr><tr><td>9</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>10</td><td>38</td><td>I/O/T</td><td>U0RXD, GPIO38</td></tr><tr><td>11</td><td>21</td><td>I/O/T</td><td>GPIO21</td></tr><tr><td>12</td><td>22</td><td>I/O/T</td><td>GPIO22</td></tr><tr><td>13</td><td>20</td><td>I/O/T</td><td>GPIO20</td></tr><tr><td>14</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>15</td><td>6</td><td>I/O/T</td><td>GPIO6</td></tr><tr><td>16</td><td>5</td><td>I/O/T</td><td>GPIO5</td></tr><tr><td>17</td><td>3V3</td><td>P</td><td>3.3 V电源</td></tr><tr><td>18</td><td>4</td><td>I/O/T</td><td>GPIO4</td></tr><tr><td>19</td><td>3</td><td>I/O/T</td><td>GPIO3</td></tr><tr><td>20</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>21</td><td>2</td><td>I/O/T</td><td>GPIO2</td></tr><tr><td>22</td><td>NC(1)</td><td>I/O/T</td><td><eq>GPIO1^2</eq></td></tr><tr><td>23</td><td>NC(0)</td><td>I/O/T</td><td><eq>GPIO0^2</eq></td></tr><tr><td>24</td><td>36</td><td>I/O/T</td><td>GPIO36</td></tr><tr><td>25</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>26</td><td>32</td><td>I/O/T</td><td>GPIO32</td></tr><tr><td>27</td><td>24</td><td>I/O/T</td><td>GPIO24</td></tr><tr><td>28</td><td>25</td><td>I/O/T</td><td>GPIO25</td></tr><tr><td>29</td><td>33</td><td>I/O/T</td><td>GPIO33</td></tr><tr><td>30</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>31</td><td>26</td><td>I/O/T</td><td>GPIO26</td></tr><tr><td>32</td><td>54</td><td>I/O/T</td><td>GPIO54</td></tr><tr><td>33</td><td>48</td><td>I/O/T</td><td>GPIO48</td></tr><tr><td>34</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>35</td><td>53</td><td>I/O/T</td><td>GPIO53</td></tr><tr><td>36</td><td>46</td><td>I/O/T</td><td>GPIO46</td></tr><tr><td>37</td><td>47</td><td>I/O/T</td><td>GPIO47</td></tr><tr><td>38</td><td>27</td><td>I/O/T</td><td>GPIO27</td></tr><tr><td>39</td><td>GND</td><td>GND</td><td>接地</td></tr><tr><td>40</td><td>NC(45)</td><td>I/O/T</td><td><eq>GPIO45^3</eq></td></tr></table>

## J1

硬件版本 该开发板为最新硬件，尚未有历史版本。

相关文档 请前往 esp-dev-kits文档 HTML 网页版本下载以下文档。

• ESP32-P4-Function-EV-Board v1.4 原理图 (PDF)

• ESP32-P4-Function-EV-Board v1.4 PCB 布局图 (PDF)

• ESP32-P4-Function-EV-Board v1.4 尺寸图 (PDF)

• ESP32-P4-Function-EV-Board v1.4 尺寸图源文件 (DXF) - 可使用 Autodesk Viewer 查看

• 1.0K-GT-15PB 规格书 (PDF)

• 摄像头规格书 (PDF)

• 显示屏规格书 (PDF)

• 显示屏驱动芯片 EK73217BCGA 规格书 (PDF)

• 显示屏驱动芯片 EK79007AD 规格书 (PDF)

• 显示屏适配板原理图(PDF)

• 显示屏适配板 PCB 布局图 (PDF)

• 摄像头适配板原理图(PDF)

• 摄像头适配板 PCB 布局图 (PDF)

有关本开发板的更多设计文档，请联系我们的商务部门 sales@espressif.com。

免责声明和版权公告 请参阅免责声明和版权公告。

免责声明和版权公告

请参阅免责声明和版权公告。

## 4.2 ESP32-P4-EYE

ESP32-P4-EYE是一款基于 ESP32-P4 芯片的视觉开发板，主要面向摄像头应用。

开发板的应用示例存放在 Examples 文件夹中。

## 4.2.1 ESP32-P4-EYE

备注: 若您使用的是搭载芯片版本 v3.x的ESP32-P4X-EYE，请参阅此用户指南。

要识别所使用的芯片版本，请查看 ESP32-P4 系列芯片勘误表 > 芯片版本标识。

本指南将帮助您快速上手 ESP32-P4-EYE，并提供该款开发板的详细信息。

ESP32-P4-EYE 是一款基于 ESP32-P4 芯片的视觉开发板，主要面向摄像头应用。ESP32-P4 搭载双核 RISC-V 处理器，支持最大 32 MB PSRAM。此外，ESP32-P4 支持 USB 2.0 标准, MIPI-CSI/DSI, H264 Encoder 等多种外设，可满足客户对低成本、高性能、低功耗的多媒体产品的开发需求。

此开发板搭载了 ESP32-C6-MINI-1U 模组，用于该开发板的 Wi-Fi 和蓝牙通信；支持 MIPI-CSI 摄像头接口和USB2.0High-Speed从机模式。板载功能丰富，包括摄像头、显示屏、麦克风和MicroSD卡扩展，使得设备能够实时监测周围环境并采集图像、音频数据。适用于智能安防摄像头、视觉模型检测、物联网边缘计算等需要实时图像处理与无线通信的应用场景。

板上芯片大部分管脚均已引出至排母，开发人员可根据实际需求，轻松通过跳线连接多种外围设备。

本指南包括如下内容：

• 入门指南：简要介绍了 ESP32-P4-EYE和硬件、软件设置指南。

• 硬件参考：详细介绍了 ESP32-P4-EYE 的硬件。

• 硬件版本：介绍硬件历史版本和已知问题，并提供链接至历史版本开发板的入门指南（如有）。

• 相关文档：列出了相关文档的链接。

• 免责声明和版权公告: 链接到免责声明和版权公告。

## 入门指南

本节介绍如何开始使用ESP32-P4-EYE。首先，介绍一些关于ESP32-P4-EYE的基本信息，然后在应用程序开发 章节介绍如何开始使用该开发板进行开发。

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/f9bd2ed1fcc102f7cb699daf5ce60da62287b1a27085e1f8b39d32398916c651.jpg)



图 11: ESP32-P4-EYE 正面图（点击放大）


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/e58f57018b0078a7714fb3f86826c418f784e1499e4af8a6bdb5379201bf570a.jpg)



图 12: ESP32-P4-EYE 背面图（点击放大）


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/7d1291f7411bde5b0a7e7c822071c78ff9fb1623f0f3ec4f26d8affe46711bfb.jpg)



图 13: ESP32-P4-EYE PCB 正面图（点击放大）


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/d9a0af2edcb25590d8b2633863891fd837093a7a9c060daba4263d2c1015c359.jpg)



图 14: ESP32-P4-EYE 正面图（点击放大）


组件介绍 以下按照顺时针的顺序依次介绍正面 PCB 上的主要组件。为了方便用户使用，我们同时在ESP32-P4-EYE的外壳上标注出了这些组件或者接口。

<table><tr><td>主要组件</td><td>描述</td></tr><tr><td>MicroSD Card Slot (MicroSD 卡槽)</td><td>支持通过4线SD总线连接MicroSD卡,兼容SDIO协议与SPI协议两种通信模式。</td></tr><tr><td>Test Points(测试点)</td><td>用于对ESP32-C6-MINI-1U进行烧录测试的测试点,可焊接杜邦线。</td></tr><tr><td>USB 2.0 Device Port (USB 2.0 Device接口)</td><td>USB 2.0 Device接口与ESP32-P4芯片的USB 2.0 OTG High-Speed接口连接,支持USB 2.0标准。通过该接口进行USB通讯时,ESP32-P4作为USB Device设备与其他USB Host设备连接。该接口也可用作开发板的供电接口,外壳上以USB 2.0符号标识。</td></tr><tr><td>USB Debug Port (USB Debug 接口)</td><td>该接口可用作开发板的供电接口,可烧录固件至芯片,也可作为通信接口,与ESP32-P4芯片上的USB Serial JTAG接口连接。外壳上以Debug符号标识。</td></tr><tr><td>Power Switch(电源开关)</td><td>电源开关。拨向I一侧,开发板连接5V电源上电;拨向○一侧,开发板断开5V电源掉电。</td></tr><tr><td>User-defined Buttons(用户自定义按键)</td><td>用户可根据应用自定义相关功能。</td></tr><tr><td>LCD FPC Connector(屏幕连接器)</td><td>用于连接1.54英寸LCD屏幕。</td></tr><tr><td>LCD</td><td>1.54英寸LCD,SPI接口,分辨率为240x240,支持Camera图像实时显示。详细参数请参考显示屏规格书。</td></tr><tr><td>Charging Indicator(充电指示灯)</td><td>为电池充电时,充电指示灯为红色;充电完成后,指示灯变为绿色。</td></tr></table>

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/11cbae3e2c245ecaa1a4bc911423067673a02f76d69d727415a13f9841200c50.jpg)



图 15: ESP32-P4-EYE 开发板背面图（点击放大）


以下按照顺时针的顺序依次介绍背面PCB上的主要组件。为了方便用户使用，我们同时在ESP32-P4-EYE的外壳上标注出了这些组件或者接口。

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/275c52e59b482805363f71b8c48bead72275eb5e39aa5b889699b3a879560e27.jpg)



图 16: ESP32-P4-EYE 背面图（点击放大）


<table><tr><td>主要组件</td><td>描述</td></tr><tr><td>Rotary Encoder(旋转编码器)</td><td>用户可根据应用自定义相关功能,比如可用于拨动控制LCD界面和摄像头像素缩放。</td></tr><tr><td>ESP32-C6-MINI-1U</td><td>ESP32-C6-MINI-1U模组用于该开发板的Wi-Fi和蓝牙通信。</td></tr><tr><td>Fill Light(补光灯)</td><td>用于拍照录像时补光。</td></tr><tr><td>Digital Microphone(数字麦克风)</td><td>用于语音识别或者录制视频时收音。</td></tr><tr><td>MIPI CSI Connector(MIPI CSI连接器)</td><td>用于连接摄像头模组。</td></tr><tr><td>Female Header(排母)</td><td>2 x 10P排母,用户可根据应用进行自定义。</td></tr><tr><td>SPI flash</td><td>通过SPI接口与ESP32-P4芯片连接,大小为16 MB。</td></tr><tr><td>ESP32-P4</td><td>ESP32-P4是一款高性能MCU,支持超大片上内存,具有强大的图像和语音处理能力。</td></tr><tr><td>Battery Connector(电池接口)</td><td>用于连接锂电池。</td></tr><tr><td>Reset Button (Reset 键)</td><td>复位按键,外壳上以符号标识。</td></tr><tr><td>Boot Button (Boot 键)</td><td>启动模式控制按键,外壳上以符号标识。保持按住Boot键的同时按一下Reset键,ESP-P4将重新启动并进入“固件下载”模式,这时可以通过USB Debug接口下载固件到板载SPI flash中。正常使用中可以作为确认按钮。</td></tr><tr><td>Camera</td><td>分辨率为200万像素,可旋转摄像头进行手动聚焦。详细参数请参考摄像头规格书。</td></tr></table>

## 应用示例 以下为开发板的应用示例：

• Factory Demo - 展示了一个基于 ESP32-P4-EYE 的全功能迷你相机方案。它不仅实现了拍照、定时拍照、录像、相册预览、USBSD卡挂载和图像参数设置等基础相机功能，还集成了先进的AI视觉能力，包括面部检测、行人检测和基于 YOLOv11nano模型的实时物体检测。

更多示例及最新更新请参阅 examples 文件夹。

如需尝试应用示例或开发自定义应用，请参照应用程序开发小节中的步骤进行操作。

应用程序开发 通电前，请确保 ESP32-P4-EYE完好无损。

## 必备硬件

• ESP32-P4-EYE 

• USB 数据线

• 电脑（Windows、Linux 或 macOS）

备注: 请确保使用优质 USB数据线。部分数据线仅可用于充电，无法用于数据传输和编程。

## 可选硬件

• MicroSD 卡

• 锂电池

硬件设置 使用 USB 数据线将 ESP32-P4-EYE 连接到电脑，可通过 USB 2.0 Device 接口或 USBDebug 接口为开发板供电。建议使用USB Debug 接口烧录固件和调试。

软件设置 请前往 快速入门中详细安装步骤 一节查看如何快速设置开发环境。

## 硬件参考

功能框图 ESP32-P4-EYE 的主要组件和连接方式如下图所示。

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/186081c1aa4c8f24b1070eb51a85ea7192f192afbd8dce347f6113ce0f29ecf3.jpg)



图 17: ESP32-P4-EYE 功能框图（点击放大）


电源选项 可通过以下方法为开发板供电：

1. 通过 USB 2.0 Device 接口或 USB Debug 接口供电

使用该方法供电时，应使用 USB Type-C 数据线将开发板通过上述任一接口连接至供电设备。如果已安装锂电池，会同时对锂电池进行充电。

2. 通过 电池接口使用外部锂电池供电

使用该方法供电时，应将外壳拆开，然后将电池接入电池接口。锂电池体积不超过4mmx25mmx 45mm，使用1.25 mm 间距连接器，正负极需与开发板上标识保持一致。

排母

USB 2.0 Device 接口

LCD 接口 请注意，该接口支持连接 SPI 接口屏幕，该开发板使用的屏幕型号为 ST7789，LCD_BL(GPIO20) 可用于控制屏幕背光。

SD 卡接口 请注意，SD 卡接口支持：

• 1-线和 4-线的 SD 总线配置

• 可通过SDIO 协议或 SPI协议通信

充电电路

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/64341b4105570f4fd1b8dc00eca33e524f0df18f521a78a35280e44637da3456.jpg)



图18: 电池连接图（点击放大）


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/5f49b34796a1ce23c7482f34c099802395ca4d14e8e180fdf784d4a9b9bbf136.jpg)



图19: 排母实物图（点击放大）



USB HighSpeed & Power in:


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/8abb3c61a61166bf3117c6e54bbf2ed3bd612e70b139c8b24757a441d53b0371.jpg)



图 20: USB 2.0 Device 接口电路图（点击放大）


## 1.54 INCH LCD:

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/bfcca56d5171a04a91163ce97322729936d5865e5311b856026315105a46da55.jpg)



图21: LCD 接口电路图（点击放大）


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/a32c73bca939f5fef94041e29f9b6a9c8f39989bd03660b60add5d2fc29f0cf0.jpg)



图 22: SD 卡接口电路图（点击放大）


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/b31399d02385e3e220f0eac77facb08ac6cceed54ad8be98e7e062a64b0950fb.jpg)



图23: 充电电路图（点击放大）


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/d2c418b597d693723c526f82b8a2c0d47406598c0d138eea0aac74618eb08df4.jpg)



图24: 麦克风接口电路图（点击放大）



麦克风接口


Camera Connector: 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/4b864f68c157951f4333eb5f9ef6095023d0f1a54702cc6dd00fd1a5d03dbddb.jpg)



图25: 摄像头接口电路图（点击放大）


摄像头接口

滚动编码器接口

ESP32-C6-MINI-1U 模组接口

![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/05ec1cc3a383893672564e47331643c06dc649eeb898b1406c34fc77f8c2d428.jpg)



图26: 滚动编码器接口电路图（点击放大）



WiFi & BT Module:


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/dfd1a2372906e9dbd10f837624e0592b721c90df53d0c358de74587de87a930e.jpg)



图 27: ESP32-C6-MINI-1U 模组接口电路图（点击放大）


## 硬件版本

该开发板为最新硬件，尚未有历史版本。

## 相关文档

请前往 esp-dev-kits 文档 HTML 网页版本 下载以下文档。

• ESP32-P4-EYE 原理图 (PDF)

• ESP32-P4-EYE PCB 布局图 (PDF)

• 摄像头规格书 (PDF)

• 显示屏规格书 (PDF)

• ST7789VW 规格书 (PDF)

• OV2710 概述 (PDF)

## 免责声明和版权公告

请参阅免责声明和版权公告。


ESP32-P4 系列开发板


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/eb5336286405f9cfb92fcaab8a316d3536463f28e4815e759dfbda432330d810.jpg)


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/e555d49e6f931213f828745e2d8398793f91cbae9b4d52e397ff214cb7f93a40.jpg)



ESP32-P4X-Function-EV-Board



ESP32-P4X-EYE


![image](https://cdn-mineru.openxlab.org.cn/result/2026-10-15/1dc65fc0-6e68-4ad6-9399-f4335db21a02/def5e14b226663fcd81b5607481870775b898b094facc990269765c6a3be1e0b.jpg)



ESP32-P4X-C5-Function-EV-Board


# Chapter 5

# 相关文档和资源

## 5.1 开发者社区

• ESP32-P4 ESP-IDF 编程指南 –ESP-IDF 开发框架的文档中心。

• ESP-IoT-Solution 编程指南 –ESP-IoT-Solution 开发框架的文档中心。

• ESP-FAQ-由乐鑫官方推出的针对常见问题的总结。

• ESP-IDF 及 GitHub 上的其它开发框架

https://github.com/espressif 

• ESP32 论坛–工程师对工程师 (E2E) 的社区，您可以在这里提出问题、解决问题、分享知识、探索观点。https://esp32.com/

• TheESP Journal –分享乐鑫工程师的最佳实践、技术文章和工作随笔。

https://blog.espressif.com/ 

• SDK和演示、App、工具、AT等下载资源https://espressif.com/zh-hans/support/download/sdks-demos

## 5.2 产品

• ESP32-P4 系列芯片–ESP32-P4 全系列芯片。

https://espressif.com/zh-hans/products/socs?id=ESP32-P4 

• ESP32-P4 系列模组–ESP32-P4 全系列模组。

https://espressif.com/zh-hans/products/modules?id=ESP32-P4 

• ESP32-P4 系列开发板–ESP32-P4 全系列开发板。

https://espressif.com/zh-hans/products/devkits?id=ESP32-P4 

• ESP Product Selector（乐鑫产品选型工具）–通过筛选性能参数、进行产品对比快速定位您所需要的产品。

https://products.espressif.com/#/product-selector 

## 5.3 联系我们

• 商务问题、技术支持、电路原理图&PCB设计审阅、购买样品（线上商店）、成为供应商、意见与建议

https://espressif.com/zh-hans/contact-us/sales-questions 

# 免责声明和版权公告

本文档中的信息，包括供参考的 URL 地址，如有变更，恕不另行通知。

本文档可能引用了第三方的信息，所有引用的信息均为“按现状”提供，乐鑫不对信息的准确性、真实性做任何保证。

乐鑫不对本文档的内容做任何保证，包括内容的适销性、是否适用于特定用途，也不提供任何其他乐鑫提案、规格书或样品在他处提到的任何保证。

乐鑫不对本文档是否侵犯第三方权利做任何保证，也不对使用本文档内信息导致的任何侵犯知识产权的行为负责。本文档在此未以禁止反言或其他方式授予任何知识产权许可，不管是明示许可还是暗示许可。

Wi-Fi 联盟成员标志归 Wi-Fi 联盟所有。蓝牙标志是 BluetoothSIG 的注册商标。

文档中提到的所有商标名称、商标和注册商标均属其各自所有者的财产，特此声明。

## 6.1 开发板使用说明

本开发板定位于研发评估、工程设计验证及前期开发用途，旨在为客户提供便捷的开发和测试平台。开发板并非作为最终量产组件或面向终端消费者的成品，因此其产品管理方式与量产芯片、模组产品存在差异。

开发板未进行特定的国家或行业认证，不适用于 CE、UL、CCC 等相关法规体系，因此可能不满足这些法规要求，用户应自行确保其使用符合所在国家或地区的法规要求。同时，开发板未按照可靠性测试、生产或商业化部署用途的要求进行设计、验证及管理。

用户应对产品的正确和安全使用承担全部责任。对于超出开发板定位用途的应用场景，包括但不限于预量产测试、系统集成、最终产品交付或其他商业用途，所涉及的系统级设计、产品认证、可靠性验证、法规符合性评估及商业化部署要求，用户应负责在量产前结合自身产品要求，对相关技术适配性、可靠性要求、法规合规性及系统集成风险进行充分评估，并完成必要的验证、测试及认证工作。乐鑫不对超出开发板定位用途的应用场景所产生的任何责任承担义务。无论双方之间是否已签署供应商框架协议、质量协议或其他类似协议，开发板均不适用其中有关产品认证、可靠性管理、退料分析(RMA)、产品/工艺变更通知(PCN)、产品停产管理 (EOL)、长期供货承诺及其他基于量产产品管理模式建立的要求和承诺。