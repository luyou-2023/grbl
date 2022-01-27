# Grbl中文注解版，代码行级注释。
Grbl是一个非常优秀的CNC固件，在业余CNC爱好者中被广泛使用，但是对Grbl源码了解的并不多，它的代码和注释是用英文编写的，其中涉及到的知识点非常广，并且专业度比较高，网上教程也比较少，因此我把Grbl的注释做了汉化，并对某些部分添加了自己的注解，希望能对想了解Grbl或对Grbl进行移植的小伙伴们提供一些帮助。本固件注解使用最新版Grbl，比起拿老版本和机翻骗钱的，自然是要花费更大精力，如果您觉得有帮助，不妨在下面的二维码打赏。

***
_点击 `Release` 页签下载编译好的 `.hex` 文件 或 [点击这里](https://github.com/MillerRen/grbl/releases)_
***
Grbl是性能高，成本低，基于并口运动控制，用于CNC雕刻。这个Grbl版本运行在携带了328p处理其的Arduino上（例如Uno,Duimilanove,Nano,Micro等）。控制器由C编写并优化，利用了AVR 芯片的每一个灵巧特性来实现精确时序和异步控制。它可以保持超过30kHz的稳定、无偏差的控制脉冲。它接受标准的G代码而且通过了数个CAM工具的输出测试。弧形、圆形和螺旋的运动都可以像其他一些基本G代码命令一样完美支持。函数和变量并不支持，但是我们认为GUI可以更好的完成工作。 Grbl 包含完整的前瞻性加速度控制。它意味着控制器将提前16到20个运动来规划运行速度，以实现平稳的加速和无冲击的转弯。

* [许可证](https://github.com/gnea/grbl/wiki/Licensing): Grbl是自由软件, 在 GPLv3 许可证下发布。

* 更多信息或帮助, 查看我们的WiKi **[WiKi页面!](https://github.com/MillerRen/grbl/wiki)** 如果上面的信息过时了, 请编辑它保持最新并和我们沟通! 谢谢!

## 支持及赞助
<img src="./images/donate.png">   

## CNC技术讨论
<img src="./images/cncnc.png">

