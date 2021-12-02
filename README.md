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

- **实时覆盖** : 通过进给、快速、主轴转速、主轴停止和冷却液切换控制，立即改变机器运行状态。这个令人激动的新功能通常只会出现在工业机器上，用来优化运行状态下优化速度和给料。大多数爱好CNC试图模仿这种行为，但通常有大量的滞后。 Grbl能在几十毫秒内执行实时覆盖。

- **手动模式** : 新的手动命令独立于G代码解析器，因此如果没有正确恢复解析器状态，解析器状态不会发生改变，也不会导致潜在的崩溃。  文档包含了这是怎么工作的和它怎么能用控制杆和手轮控制你的机器 文档包含了这是如何工作的，它可以用来控制您的机器通过一个操纵杆或旋转拨号与低延迟，达到令人满意的响应。

- **激光模式** : 新的“激光”模式将使Grbl连续移动通过连续的G1、G2和G3命令和主轴转速变化。当“激光”模式禁用时，Grbl将转而停止，以确保主轴达到适当的速度。主轴速度overrides也与激光模式工作，所以你可以在工作期间调整激光功率。“激光”和“普通”模式可以通过`$`设置切换。

	- **按速度比例调整激光功率** : 如果你的机器有比较小的加速度，Grbl将自动根据行进多快自动调整激光功率，因此当你的CNC转弯时不会烧焦拐角!当激光模式开启时启用`M4`主轴逆时针转动命令开启这个特性！

- **睡眠模式** : Grbl现在可以通过`$SLP`命令进入睡眠模式。这回禁用所有功能包括电机驱动器。当你离开你的机器但是又不想关闭电源时就特别有用。只有重启才会退出睡眠状态。

- **重要接口改进**: 优化提高整体性能，包括更多的实时数据，并简化GUI的维护和编写。 基于来自多个GUI开发人员和平台性能测试的直接反馈。 _注意: GUIs 需要专门更新它们的代码以兼容v1.1和以后的版本。._

	- **新的状态报告**: 为了考虑到额外的覆盖数据，状态报告被调整为填充更多的数据，但仍然比以前更小。 文档包含了它大概是怎么改变的。
	- **改进了错误/警报反馈** :所有的Grbl错误和警告信息已经被改成了错误代码形式,每个代码都与特定的问题相关联，因此用户无需猜测就能确切地知道哪里出了问题。在这个仓库的文档里有一个方便解析的CSV。
	- **扩展的-ASCII 实时命令** : 所有Override和未来的实时命令都在扩展ascii字符空间中定义。 不幸的是，不容易在键盘上键入，但有助于防止意外命令从g-code文件具有这些字符，并为未来的扩展提供了大量的空间。
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
