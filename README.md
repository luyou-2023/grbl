![GitHub Logo](https://github.com/gnea/gnea-Media/blob/master/Grbl%20Logo/Grbl%20Logo%20250px.png?raw=true)

***
_点击 `Release` 页签下载编译好的 `.hex` 文件 或 [点击这里](https://github.com/gnea/grbl/releases)_
***
Grbl是性能高，成本低，基于并口运动控制，用于CNC雕刻。这个Grbl版本运行在携带了328p处理其的Arduino上（例如Uno,Duimilanove,Nano,Micro等）。控制器由C编写并优化，利用了AVR 芯片的每一个灵巧特性来实现精确时序和异步控制。它可以保持超过30kHz的稳定、无偏差的控制脉冲。它接受标准的G代码而且通过了数个CAM工具的输出测试。弧形、圆形和螺旋的运动都可以像其他一些基本G代码命令一样完美支持。函数和变量并不支持，但是我们认为GUI可以更好的完成工作。 Grbl 包含完整的前瞻性加速度控制。它意味着控制器将提前16到20个运动来规划运行速度，以实现平稳的加速和无冲击的转弯。

* [许可证](https://github.com/gnea/grbl/wiki/Licensing): Grbl是自由软件, 在 GPLv3 许可证下发布。

* 更多信息或帮助, 查看我们的WiKi **[WiKi页面!](https://github.com/MillerRen/grbl/wiki)** 如果上面的信息过时了, 请编辑它保持最新并和我们沟通! 谢谢!

* 核心开发者: Sungeun "Sonny" Jeon, Ph.D. (USA) aka @chamnit

* Simen Svale Skogsrud (挪威)编写并创建了神奇的 Grbl v0.6 (2011) 固件。

***

### Grbl CNC 项目的官方支持者
![官方支持者们](https://github.com/gnea/gnea-Media/blob/master/Contributors.png?raw=true)


***

## v1.1版本更新概要
- **重要:** 你的EEPROM将会被擦除并恢复成新设置。 这是由于增加了两个新的主轴转速设置'$'。

- **Real-time Overrides** : Alters the machine running state immediately with feed, rapid, spindle speed, spindle stop, and coolant toggle controls. This awesome new feature is common only on industrial machines, often used to optimize speeds and feeds while a job is running. Most hobby CNC's try to mimic this behavior, but usually have large amounts of lag. Grbl executes overrides in realtime and within tens of milliseconds.

- **Jogging Mode** : The new jogging commands are independent of the g-code parser, so that the parser state doesn't get altered and cause a potential crash if not restored properly. Documentation is included on how this works and how it can be used to control your machine via a joystick or rotary dial with a low-latency, satisfying response.

- **Laser Mode** : The new "laser" mode will cause Grbl to move continuously through consecutive G1, G2, and G3 commands with spindle speed changes. When "laser" mode is disabled, Grbl will instead come to a stop to ensure a spindle comes up to speed properly. Spindle speed overrides also work with laser mode so you can tweak the laser power, if you need to during the job. Switch between "laser" mode and "normal" mode via a `$` setting.

	- **Dynamic Laser Power Scaling with Speed** : If your machine has low accelerations, Grbl will automagically scale the laser power based on how fast Grbl is traveling, so you won't have burnt corners when your CNC has to make a turn! Enabled by the `M4` spindle CCW command when laser mode is enabled!

- **Sleep Mode** : Grbl may now be put to "sleep" via a `$SLP` command. This will disable everything, including the stepper drivers. Nice to have when you are leaving your machine unattended and want to power down everything automatically. Only a reset exits the sleep state.

- **Significant Interface Improvements**: Tweaked to increase overall performance, include lots more real-time data, and to simplify maintaining and writing GUIs. Based on direct feedback from multiple GUI developers and bench performance testing. _NOTE: GUIs need to specifically update their code to be compatible with v1.1 and later._

	- **New Status Reports**: To account for the additional override data, status reports have been tweaked to cram more data into it, while still being smaller than before. Documentation is included, outlining how it has been changed. 
	- **Improved Error/Alarm Feedback** : All Grbl error and alarm messages have been changed to providing a code. Each code is associated with a specific problem, so users will know exactly what is wrong without having to guess. Documentation and an easy to parse CSV is included in the repo.
	- **Extended-ASCII realtime commands** : All overrides and future real-time commands are defined in the extended-ASCII character space. Unfortunately not easily type-able on a keyboard, but helps prevent accidental commands from a g-code file having these characters and gives lots of space for future expansion.
	- **Message Prefixes** : Every message type from Grbl has a unique prefix to help GUIs immediately determine what the message is and parse it accordingly without having to know context. The prior interface had several instances of GUIs having to figure out the meaning of a message, which made everything more complicated than it needed to be.

- New OEM specific features, such as safety door parking, single configuration file build option, EEPROM restrictions and restoring controls, and storing product data information.
 
- New safety door parking motion as a compile-option. Grbl will retract, disable the spindle/coolant, and park near Z max. When resumed, it will perform these task in reverse order and continue the program. Highly configurable, even to add more than one parking motion. See config.h for details.

- New '$' Grbl settings for max and min spindle rpm. Allows for tweaking the PWM output to more closely match true spindle rpm. When max rpm is set to zero or less than min rpm, the PWM pin D11 will act like a simple enable on/off output.

- Updated G28 and G30 behavior from NIST to LinuxCNC g-code description. In short, if a intermediate motion is specified, only the axes specified will move to the stored coordinates, not all axes as before.

- Lots of minor bug fixes and refactoring to make the code more efficient and flexible.

- **NOTE:** Arduino Mega2560 support has been moved to an active, official Grbl-Mega [project](http://www.github.com/gnea/grbl-Mega/). All new developments here and there will be synced when it makes sense to.


```
List of Supported G-Codes in Grbl v1.1:
  - Non-Modal Commands: G4, G10L2, G10L20, G28, G30, G28.1, G30.1, G53, G92, G92.1
  - Motion Modes: G0, G1, G2, G3, G38.2, G38.3, G38.4, G38.5, G80
  - Feed Rate Modes: G93, G94
  - Unit Modes: G20, G21
  - Distance Modes: G90, G91
  - Arc IJK Distance Modes: G91.1
  - Plane Select Modes: G17, G18, G19
  - Tool Length Offset Modes: G43.1, G49
  - Cutter Compensation Modes: G40
  - Coordinate System Modes: G54, G55, G56, G57, G58, G59
  - Control Modes: G61
  - Program Flow: M0, M1, M2, M30*
  - Coolant Control: M7*, M8, M9
  - Spindle Control: M3, M4, M5
  - Valid Non-Command Words: F, I, J, K, L, N, P, R, S, T, X, Y, Z
```
