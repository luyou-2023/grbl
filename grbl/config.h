/*
  config.h - 编译时配置
  Grbl的一部分

  版权所有 2011-2016 Sungeun K. Jeon for Gnea Research LLC
  版权所有 2009-2011 Simen Svale Skogsrud
  
  Grbl 是自由软件：你可以在自由软件基金会的GNU 普通公共许可(GPL v3+)条款下发行，或修改它。
  Grbl的发布是希望它能有用，但没有任何保证;甚至没有隐含的保证适销性或适合某一特定目的。
  更多详细信息，请参阅GNU通用公共许可证。

  您应该已经收到GNU通用公共许可证的副本和Grbl一起。如果没有，请参阅<http://www.gnu.org/licenses/>。
*/

//此文件包含Grbl内部系统的编译时配置。
//在大多数情况下，用户不需要直接修改它们，但它们是为了满足特定的需要，即性能调整或适应非典型机器。

//重要提示：此处的任何更改都需要重新编译源代码以更新它们。

#ifndef config_h
#define config_h
#include "grbl.h" // 兼容 Arduino IDE .


//定义CPU引脚映射和默认设置。
//注意：OEM可以避免维护/更新defaults.h和cpu_map.h文件，只使用一个配置文件，方法是将其特定的默认值和引脚映射放置在此文件底部。
//如果这样做，只需注释掉这两个定义，并参见下面的说明。
#define DEFAULTS_GENERIC
#define CPU_MAP_ATMEGA328P//阿杜伊诺一个CPU

//串行波特率
// #define BAUD_RATE 230400
#define BAUD_RATE 115200

//定义实时命令特殊字符。这些字符不会直接从grbl数据流中读取到grbl执行。选择流式g代码程序中不存在且不得存在的字符。
//如果每个用户设置都有ASCII控制字符，则可以使用ASCII控制字符。
//此外，扩展ASCII码（>127）永远不会出现在g代码程序中，可以选择用于接口程序。
//注意：如果更改，请手动更新报告中的帮助消息。C

#define CMD_RESET 0x18 // ctrl-x.
#define CMD_STATUS_REPORT '?'
#define CMD_CYCLE_START '~'
#define CMD_FEED_HOLD '!'

// 注意：所有覆盖实时命令必须在扩展ASCII字符集中，从字符值128（0x80）开始，最多255（0xFF）。
//如果将正常的实时命令集（如状态报告、馈电保持、复位和循环启动）移动到扩展设置空间，则需要修改serial.c的RX ISR以适应更改。
// #define CMD_RESET 0x80
// #define CMD_STATUS_REPORT 0x81
// #define CMD_CYCLE_START 0x82
// #define CMD_FEED_HOLD 0x83
#define CMD_SAFETY_DOOR 0x84
#define CMD_JOG_CANCEL  0x85
#define CMD_DEBUG_REPORT 0x86//仅当启用调试时，才会以“{}”大括号发送调试报告。
#define CMD_FEED_OVR_RESET 0x90//将进给覆盖值恢复为100%。
#define CMD_FEED_OVR_COARSE_PLUS 0x91
#define CMD_FEED_OVR_COARSE_MINUS 0x92
#define CMD_FEED_OVR_FINE_PLUS  0x93
#define CMD_FEED_OVR_FINE_MINUS  0x94
#define CMD_RAPID_OVR_RESET 0x95//将快速覆盖值恢复为100%。
#define CMD_RAPID_OVR_MEDIUM 0x96
#define CMD_RAPID_OVR_LOW 0x97
// #define CMD_RAPID_OVR_EXTRA_LOW 0x98 // *NOT SUPPORTED*
#define CMD_SPINDLE_OVR_RESET 0x99//将主轴覆盖值恢复为100%。
#define CMD_SPINDLE_OVR_COARSE_PLUS 0x9A
#define CMD_SPINDLE_OVR_COARSE_MINUS 0x9B
#define CMD_SPINDLE_OVR_FINE_PLUS 0x9C
#define CMD_SPINDLE_OVR_FINE_MINUS 0x9D
#define CMD_SPINDLE_OVR_STOP 0x9E
#define CMD_COOLANT_FLOOD_OVR_TOGGLE 0xA0
#define CMD_COOLANT_MIST_OVR_TOGGLE 0xA1

//如果启用了归零，则归零初始锁定会在通电时将Grbl设置为报警状态。
//这将强制用户在执行任何其他操作之前执行重新定位循环（或覆盖锁）。
//这主要是提醒用户归位的安全功能，因为Grbl不知道位置。
#define HOMING_INIT_LOCK // 注释后禁用

// 使用位掩码定义归位循环模式。 
//归位循环首先执行搜索模式以快速接合限位开关，然后执行较慢的定位模式，最后通过短距离回拉动作来断开限位开关。
//以下归零循环定义以后缀0开始的顺序执行，并仅完成指定轴的归零例程。
//如果定义中省略了轴，则该轴将不在原点，系统也不会更新其位置。
//这意味着，这允许使用非标准笛卡尔机器的用户，例如车床（x然后z，没有y）根据自己的需要配置回零循环行为。
//注意：如果轴不在同一周期内，则回零周期设计为允许共享限位引脚，但这需要在cpu_map.h中更改一些引脚设置。
//例如，默认归零循环可以与X或Y限位引脚共享Z限限位引脚，因为它们处于不同的周期。
//这个引脚为了其他目的腾出了一个宝贵的IO。
//理论上，如果所有轴都以单独的周期进行定位，则所有轴限位销可减少为一个引脚，
//反之亦然，所有三个轴均位于单独的引脚上，但在一个周期内进行定位。 
//此外，还应注意，硬限位的功能不会受到管脚共享的影响。
// 注：默认值是为传统的三轴数控机床设置的。先清除Z轴，然后清除X和Y轴。
#define HOMING_CYCLE_0 (1<<Z_AXIS)//必需：首先移动Z以清除工作区。
#define HOMING_CYCLE_1 ((1<<X_AXIS)|(1<<Y_AXIS))  // 可选：然后同时移动X、Y。
// #define HOMING_CYCLE_2                         // OPTIONAL: Uncomment and add axes mask to enable

// 注：以下是为2轴机器设置回零的两个示例。
// #define HOMING_CYCLE_0 ((1<<X_AXIS)|(1<<Y_AXIS))  // NOT COMPATIBLE WITH COREXY: Homes both X-Y in one cycle. 

// #define HOMING_CYCLE_0 (1<<X_AXIS)  // COREXY COMPATIBLE: First home X
// #define HOMING_CYCLE_1 (1<<Y_AXIS)  // COREXY COMPATIBLE: Then home Y

//机器初始点动至限位开关后执行的回位循环次数。
// 这有助于防止过冲，并应提高重复性。此值应为1或1更大。
#define N_HOMING_LOCATE_CYCLE 1 // 整数 (1-128)

//启用单轴原点命令$用于X、Y和Z轴原点的HX、$HY和$HZ。$H命令仍会调用整个归位循环。这在默认情况下是禁用的。
//这里只针对需要在两轴和三轴机器之间切换的用户。这实际上是非常罕见的。
//如果你有一个双轴机器，不要使用这个。相反，只需改变两个轴的归零周期即可。

// #define HOMING_SINGLE_AXIS_COMMANDS // Default disabled. Uncomment to enable.

//归零后，Grbl将默认将整个机器空间设置为负空间，这是专业CNC机器的典型情况，无论限位开关位于何处。
//取消对该定义的注释，以强制Grbl始终将机器原点设置在原点位置，而不管开关方向如何。

// #define HOMING_FORCE_SET_ORIGIN // Uncomment to enable.

//启动时执行的Grbl块数。这些块存储在EEPROM中，其大小和地址在settings.h中定义
//使用当前设置，最多可存储2个启动块，并按顺序执行。这些启动块通常用于根据用户偏好设置g代码解析器状态。
#define N_STARTUP_LINE 2 // Integer (1-2)

//Grbl为某些值类型打印的浮点小数点的数目。这些设置由数控机床中的实际值和常见观察值确定。
//例如，位置值不能小于0.001mm或0.0001in，因为机器在物理上不能比这更精确。
//所以，可能没有必要更改这些，但是如果需要，可以在这里更改。
//注意：必须是0到~4之间的整数值。超过4个可能会出现舍入误差。
#define N_DECIMAL_COORDVALUE_INCH 4//坐标或位置值（英寸）
#define N_DECIMAL_COORDVALUE_MM   3//坐标或位置值（单位：mm）
#define N_DECIMAL_RATEVALUE_INCH  1//速率或速度值（单位：in/min）
#define N_DECIMAL_RATEVALUE_MM    0//速率或速度值，单位为mm/min
#define N_DECIMAL_SETTINGVALUE    3//浮点设置值的小数
#define N_DECIMAL_RPMVALUE        0//每分钟转数的RPM值。

//如果您的机器有两个平行于一个轴的限位开关，则需要启用此功能。
//由于两个开关共用一个管脚，Grbl无法判断哪一个已启用。 
//此选项仅影响回零，如果启用限位，Grbl将发出警报并强制用户手动断开限位开关。
//否则，如果每个轴都有一个限位开关，则不要启用此选项。 
//通过使其处于禁用状态，您可以在限位开关上执行复位循环，而不必将机器移出限位开关。
// #define LIMITS_TWO_SWITCHES_ON_AXES

//允许GRBL跟踪和报告gcode行号。启用这意味着计划缓冲区从16变为15，以便为计划块结构中的额外行号数据腾出空间
// #define USE_LINE_NUMBERS // Disabled by default. Uncomment to enable.

//探测循环成功后，此选项通过自动生成的消息立即提供探测坐标的反馈。
//如果禁用，用户仍可以通过Grbl“$#”打印参数访问最后一个探测器坐标。
#define MESSAGE_PROBE_COORDINATES // Enabled by default. Comment to disable.

//通过Arduino Uno模拟针脚4上的雾化冷却液g代码命令M7启用第二个冷却液控制针脚。
//仅当需要第二个冷却液控制引脚时才使用此选项。
//注意：不管怎样，模拟针脚3上的M8溢流冷却液控制针脚仍将正常工作。
// #define ENABLE_M7 // Disabled by default. Uncomment to enable.

// 此选项使进给保持输入充当安全门开关。
// 安全门一旦触发，将立即强制进给保持，然后安全断电。 
//在安全门重新接合之前，将阻止恢复。
//此时，Grbl将使机器重新通电，然后恢复上一条刀具路径，就像什么也没发生一样。
// #define ENABLE_SAFETY_DOOR_INPUT_PIN // 默认禁用。取消注释以启用。

//切换并恢复安全门开关后，此设置设置恢复主轴和冷却液与恢复循环之间的通电延迟。
#define SAFETY_DOOR_SPINDLE_DELAY 4.0 // Float (seconds)
#define SAFETY_DOOR_COOLANT_DELAY 1.0 // Float (seconds)

// 启用CoreXY运动学。仅与CoreXY机器一起使用。
// 重要提示：如果启用了归位，则必须重新配置归位循环。
// #defines 上面的 #define HOMING_CYCLE_0 (1<<X_AXIS) 和 #define HOMING_CYCLE_1 (1<<Y_AXIS)
// 注意：此配置选项将X轴和Y轴的运动更改为在(http://corexy.com/theory.html)中定义的工作原理 . 
//假设电机的位置和接线与所述完全一致，否则，运动可能会向奇怪的方向移动。
//Grbl要求CoreXY A和B电机内部每毫米的步数相同。
// #define COREXY // 默认禁用。取消注释以启用。

//基于掩码反转控制命令引脚的引脚逻辑。这基本上意味着您可以在指定的管脚上使用常闭开关，而不是默认的常开开关。
//注：顶部选项将掩码和反转所有控制引脚。底部选项是仅反转两个控制引脚（安全门和复位）的示例。参见cpu_map.h其他位定义。
// #define INVERT_CONTROL_PIN_MASK CONTROL_MASK // 默认禁用。取消注释以禁用。
// #define INVERT_CONTROL_PIN_MASK ((1<<CONTROL_SAFETY_DOOR_BIT)|(1<<CONTROL_RESET_BIT)) // 默认禁用。

//基于以下掩码反转选择限位引脚状态。这会影响所有限位引脚功能，例如硬限位和回零。但是，这与整体反转限位设置不同。
//此构建选项将仅反转此处定义的限制管脚，然后反转限制设置将应用于所有管脚。
//当用户的机器上安装有一组混合的限位引脚，且带有常开（NO）和常闭（NC）开关时，此功能非常有用。
//注意：请不要使用这个，除非你有需要它的情况。
// #define INVERT_LIMIT_PIN_MASK ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)) //默认禁用。取消注释以启用。


//将主轴启用引脚从低禁用/高启用反转为低启用/高禁用。适用于某些预制电子板。
//注意：如果启用可变_主轴（默认），此选项不起作用，因为PWM输出和主轴启用共用一个引脚。
//如果您同时需要此选项和主轴速度PWM，请取消注释下面的配置选项USE_spindle_DIR_AS_ENABLE_PIN。
// #define INVERT_SPINDLE_ENABLE_PIN //默认禁用。取消注释以启用。

//将选定的冷却液针脚从低禁用/高启用反转为低启用/高禁用。适用于某些预制电子板。
// #define INVERT_COOLANT_FLOOD_PIN // 默认禁用。取消注释以启用。
// #define INVERT_COOLANT_MIST_PIN // 默认禁用。注意：在config.h中启用M7喷雾冷却液

//当Grbl通电循环或使用Arduino重置按钮硬重置时，默认情况下，Grbl在无报警的情况下启动。
//这是为了使新用户开始使用Grbl尽可能简单。启用归位且用户已安装限位开关时，Grbl将在报警状态下启动，以指示
//Grbl不知道它的位置，因此无法在继续之前强制用户归位。这一选择迫使无论是否归位，Grbl始终初始化为报警状态。
//此选项更适合OEM和LinuxCNC用户希望这种功率循环行为。
// #define FORCE_INITIALIZATION_ALARM // Default disabled. Uncomment to enable.

//通电或复位时，Grbl将检查限位开关状态，以确保它们在初始化前未处于激活状态。
//如果检测到问题并且启用了硬限制设置，Grbl将简单地通知用户检查限制并进入报警状态，而不是空闲状态。Grbl不会抛出警报消息。
#define CHECK_LIMITS_AT_INIT

//---------------------------------------------------------------------------------------
//高级配置选项：

//启用用于调试目的的代码。不适用于一般用途。
// #define DEBUG // 取消注释以启用。默认禁用。

//配置快速、进给和主轴覆盖设置。这些值定义了允许的最大和最小覆盖值以及每个接收命令的粗略增量和精细增量。
//请注意以下各定义说明中的允许值。
#define DEFAULT_FEED_OVERRIDE           100// 100%. 不要更改此值。
#define MAX_FEED_RATE_OVERRIDE          200//编程进给速度的百分比（100-255）。通常为120%或200%
#define MIN_FEED_RATE_OVERRIDE           10//编程进给速度的百分比（1-100）。通常为50%或1%
#define FEED_OVERRIDE_COARSE_INCREMENT   10// (1-99). 通常是10%。
#define FEED_OVERRIDE_FINE_INCREMENT      1// (1-99). 通常为1%。

#define DEFAULT_RAPID_OVERRIDE  100// 100%. 不要更改此值。
#define RAPID_OVERRIDE_MEDIUM    50//快速的百分比（1-99）。通常是50%。
#define RAPID_OVERRIDE_LOW       25 //快速的百分比（1-99）。通常是25%。
// #define RAPID_OVERRIDE_EXTRA_LOW 5 // *不支持*rapid的百分比（1-99）。通常是5%。

#define DEFAULT_SPINDLE_SPEED_OVERRIDE    100// 100%. 不要更改此值。
#define MAX_SPINDLE_SPEED_OVERRIDE        200//编程主轴转速的百分比（100-255）。通常是200%。
#define MIN_SPINDLE_SPEED_OVERRIDE         10//编程主轴转速的百分比（1-100）。通常是10%。
#define SPINDLE_OVERRIDE_COARSE_INCREMENT  10// (1-99). 通常是10%。
#define SPINDLE_OVERRIDE_FINE_INCREMENT     1// (1-99). 通常为1%。

//当执行M2或M30程序结束命令时，大多数g代码状态将恢复为默认状态。
//此编译时选项包括在程序结束时将进给、快速和主轴速度覆盖值恢复为其默认值。
#define RESTORE_OVERRIDES_AFTER_PROGRAM_END//默认启用。注释后禁用

//Grbl v1.1及其后的状态报告更改还删除了从报告中禁用/启用大多数数据字段的功能。
//这给GUI开发人员带来了问题，他们不得不管理多个场景和配置。新报告样式效率的提高允许发送所有数据字段，而不会出现潜在的性能问题。
//注意：下面的选项仅在特殊情况需要时提供禁用某些数据字段的方法，但请注意GUI可能依赖于此数据。如果禁用，则可能不兼容。
#define REPORT_FIELD_BUFFER_STATE //默认启用。注释后禁用
#define REPORT_FIELD_PIN_STATE //默认启用。注释后禁用
#define REPORT_FIELD_CURRENT_FEED_SPEED //默认启用。注释后禁用
#define REPORT_FIELD_WORK_COORD_OFFSET //默认启用。注释后禁用
#define REPORT_FIELD_OVERRIDES //默认启用。注释后禁用
#define REPORT_FIELD_LINE_NUMBERS //默认启用。注释后禁用

//某些状态报告数据不是实时所必需的，只是间歇性的，因为这些值不会经常更改。
//以下宏配置在刷新关联数据并将其包含在状态报告中之前需要调用状态报告的次数。
//但是，如果其中一个值发生变化，Grbl将自动在下一个状态报告中包含该数据，而不管当时的计数是多少。
//这有助于减少高频报告和强流所涉及的通信开销。
//还有一个忙刷新计数和一个空闲刷新计数，它设置Grbl在不做任何重要事情时更频繁地发送刷新。
//一个好的GUI，这些数据不需要经常刷新，只需几秒钟。
// 注意：WCO刷新必须为2或更大。OVR刷新必须为1或更大。
#define REPORT_OVR_REFRESH_BUSY_COUNT 20  // (1-255)
#define REPORT_OVR_REFRESH_IDLE_COUNT 10//（1-255）必须小于或等于忙计数
#define REPORT_WCO_REFRESH_BUSY_COUNT 30  // (2-255)
#define REPORT_WCO_REFRESH_IDLE_COUNT 10  // (2-255) 必须小于或等于忙计数

//加速度管理子系统的时间分辨率。数值越大，加速越平稳，在进给速度非常高的机器上尤其明显，但可能会对性能产生负面影响。
//此参数的正确值取决于机器，因此建议仅将其设置为所需的最高值。成功的近似值范围很广，从50到200或更多。
//注意：更改此值也会更改步骤段缓冲区中段的执行时间。
//增加此值时，段缓冲区中存储的总时间会减少，反之亦然。确保增加/减少步长段缓冲区，以应对这些变化。
#define ACCELERATION_TICKS_PER_SECOND 100

//自适应多轴步进平滑（AMASS）是一种高级功能，它实现了其名称所暗示的多轴运动的步进平滑。此功能可平滑运动，尤其是在10kHz以下的低阶跃频率下，多轴运动轴之间的混叠可能会导致可听噪音并震动机器。在更低的阶跃频率下，AMASS可以适应并提供更好的阶跃平滑。见步进电机。c获取有关AMASS系统工作的更多详细信息。
#define ADAPTIVE_MULTI_AXIS_STEP_SMOOTHING  //默认启用。注释后禁用

//设置允许写入Grbl设置的最大步进速率。此选项启用设置模块中的错误检查，以防止设置值超过此限制。最大步进速率严格受CPU速度的限制，如果使用除16MHz运行的AVR以外的其他设备，则会发生变化。
//注意：现在禁用，如果闪存空间允许，将启用。
// #define MAX_STEP_RATE_HZ 30000 // Hz

// 默认情况下，Grbl将所有输入引脚设置为正常高电平操作，并启用其内部上拉电阻器。
//这就简化了用户的布线，只需要一个接地开关，尽管建议用户在低通滤波器中进行额外的布线，以减少引脚检测到的电气噪声。
//如果用户在Grbl设置中反转引脚，则只会翻转指示激活信号的高或低读数。
//在正常操作中，这意味着用户需要连接一个正常打开的开关，但如果反转，这意味着用户应该连接一个正常关闭的开关。
//以下选项禁用内部上拉电阻器，将引脚设置为正常低操作，开关现在必须连接到Vcc而不是接地。
//这也颠覆了反向引脚Grbl设置的含义，反向设置现在意味着用户应连接正常打开的开关，反之亦然。
//注：与该特征相关的所有引脚均被禁用，即XYZ限位引脚，而不是单个轴。
//警告：当上拉被禁用时，这需要使用下拉电阻器进行额外接线！
//#define DISABLE_LIMIT_PIN_PULL_UP
//#define DISABLE_PROBE_PIN_PULL_UP
//#define DISABLE_CONTROL_PIN_PULL_UP

//设置应用刀具长度偏移的轴。假设主轴始终与选定轴平行，刀具朝向负方向。换句话说，从当前位置减去正的刀具长度偏移值。
#define TOOL_LENGTH_OFFSET_AXIS Z_AXIS//默认z轴。有效值为X_轴、Y_轴或Z_轴。

//启用不同转速值的可变主轴输出电压。在Arduino Uno上，主轴启用引脚将在256个中间电平的最大转速下输出5V，禁用时输出0V。
//注意：对于Arduino Unos来说很重要！启用时，Z限制引脚D11和主轴启用引脚D12开关！
//可变主轴输出电压需要引脚D11上的硬件PWM输出。
#define VARIABLE_SPINDLE //默认启用。注释后禁用

// 仅用于可变主轴输出。这将强制PWM输出在启用时达到最小占空比。
// 当主轴停用时，PWM引脚仍将读取0V。 大多数用户不需要此选项，但在某些情况下它可能很有用。
//该最小PWM设置与主轴rpm最小设置一致，如rpm max 到 max PWM。
//如果您需要在0V禁用和最小PWM设置的电压之间有更大的电压差以达到最小转速，这将非常方便。
//该差值为每PWM值0.02V。
//因此，当最小PWM为1时，只有0.02伏单独启用和禁用。
//在PWM 5时，这将为0.1V。请记住，随着最小PWM值的增加，您将开始失去PWM分辨率，因为您在总共255个PWM电平上发出不同主轴转速信号的范围越来越小。
//注：通过以下等式计算最小脉宽调制下的占空比：（%占空比）=（SPINDLE_PWM_MIN_VALUE/255）*100
// #define SPINDLE_PWM_MIN_VALUE 5 // 默认禁用。取消注释以启用。必须大于零。整数（1-255）。

//默认情况下，在328p（Uno）上，Grbl将可变主轴PWM和启用共用为一个引脚，以帮助保留I/O引脚。
//对于某些设置，这些可能需要单独的引脚。此配置选项将主轴方向引脚（D13）与引脚D11上的主轴速度PWM一起用作单独的主轴启用引脚。
// 注意：此配置选项仅适用于启用可变_主轴和328p处理器（Uno）。
// 注：如果没有方向引脚，M4将没有指示与M3差异的引脚输出。
// 注意：小心！Arduino引导加载程序在通电时切换D13引脚。 
//如果您使用编程器烧录Grbl（您可以使用备用Arduino作为“Arduino作为ISP”）。
//在网站上搜索如何连接。），此D13 LED切换应消失。
//我们还没有测试过这个。请报告进展情况！
// #define USE_SPINDLE_DIR_AS_ENABLE_PIN // 默认禁用。取消注释以启用。

// Alters the behavior of the spindle enable pin with the USE_SPINDLE_DIR_AS_ENABLE_PIN option . By default,
// Grbl will not disable the enable pin if spindle speed is zero and M3/4 is active, but still sets the PWM 
// output to zero. This allows the users to know if the spindle is active and use it as an additional control
// input. However, in some use cases, user may want the enable pin to disable with a zero spindle speed and 
// re-enable when spindle speed is greater than zero. This option does that.
// NOTE: Requires USE_SPINDLE_DIR_AS_ENABLE_PIN to be enabled.
// #define SPINDLE_ENABLE_OFF_WITH_ZERO_SPEED // Default disabled. Uncomment to enable.

// With this enabled, Grbl sends back an echo of the line it has received, which has been pre-parsed (spaces
// removed, capitalized letters, no comments) and is to be immediately executed by Grbl. Echoes will not be
// sent upon a line buffer overflow, but should for all normal lines sent to Grbl. For example, if a user
// sendss the line 'g1 x1.032 y2.45 (test comment)', Grbl will echo back in the form '[echo: G1X1.032Y2.45]'.
// NOTE: Only use this for debugging purposes!! When echoing, this takes up valuable resources and can effect
// performance. If absolutely needed for normal operation, the serial write buffer should be greatly increased
// to help minimize transmission waiting within the serial write protocol.
// #define REPORT_ECHO_LINE_RECEIVED // Default disabled. Uncomment to enable.

// Minimum planner junction speed. Sets the default minimum junction speed the planner plans to at
// every buffer block junction, except for starting from rest and end of the buffer, which are always
// zero. This value controls how fast the machine moves through junctions with no regard for acceleration
// limits or angle between neighboring block line move directions. This is useful for machines that can't
// tolerate the tool dwelling for a split second, i.e. 3d printers or laser cutters. If used, this value
// should not be much greater than zero or to the minimum value necessary for the machine to work.
#define MINIMUM_JUNCTION_SPEED 0.0 // (mm/min)

// Sets the minimum feed rate the planner will allow. Any value below it will be set to this minimum
// value. This also ensures that a planned motion always completes and accounts for any floating-point
// round-off errors. Although not recommended, a lower value than 1.0 mm/min will likely work in smaller
// machines, perhaps to 0.1mm/min, but your success may vary based on multiple factors.
#define MINIMUM_FEED_RATE 1.0 // (mm/min)

// Number of arc generation iterations by small angle approximation before exact arc trajectory
// correction with expensive sin() and cos() calcualtions. This parameter maybe decreased if there
// are issues with the accuracy of the arc generations, or increased if arc execution is getting
// bogged down by too many trig calculations.
#define N_ARC_CORRECTION 12 // Integer (1-255)

// The arc G2/3 g-code standard is problematic by definition. Radius-based arcs have horrible numerical
// errors when arc at semi-circles(pi) or full-circles(2*pi). Offset-based arcs are much more accurate
// but still have a problem when arcs are full-circles (2*pi). This define accounts for the floating
// point issues when offset-based arcs are commanded as full circles, but get interpreted as extremely
// small arcs with around machine epsilon (1.2e-7rad) due to numerical round-off and precision issues.
// This define value sets the machine epsilon cutoff to determine if the arc is a full-circle or not.
// NOTE: Be very careful when adjusting this value. It should always be greater than 1.2e-7 but not too
// much greater than this. The default setting should capture most, if not all, full arc error situations.
#define ARC_ANGULAR_TRAVEL_EPSILON 5E-7 // Float (radians)

// Time delay increments performed during a dwell. The default value is set at 50ms, which provides
// a maximum time delay of roughly 55 minutes, more than enough for most any application. Increasing
// this delay will increase the maximum dwell time linearly, but also reduces the responsiveness of
// run-time command executions, like status reports, since these are performed between each dwell
// time step. Also, keep in mind that the Arduino delay timer is not very accurate for long delays.
#define DWELL_TIME_STEP 50 // Integer (1-255) (milliseconds)

// Creates a delay between the direction pin setting and corresponding step pulse by creating
// another interrupt (Timer2 compare) to manage it. The main Grbl interrupt (Timer1 compare)
// sets the direction pins, and does not immediately set the stepper pins, as it would in
// normal operation. The Timer2 compare fires next to set the stepper pins after the step
// pulse delay time, and Timer2 overflow will complete the step pulse, except now delayed
// by the step pulse time plus the step pulse delay. (Thanks langwadt for the idea!)
// NOTE: Uncomment to enable. The recommended delay must be > 3us, and, when added with the
// user-supplied step pulse time, the total time must not exceed 127us. Reported successful
// values for certain setups have ranged from 5 to 20us.
// #define STEP_PULSE_DELAY 10 // Step pulse delay in microseconds. Default disabled.

// The number of linear motions in the planner buffer to be planned at any give time. The vast
// majority of RAM that Grbl uses is based on this buffer size. Only increase if there is extra
// available RAM, like when re-compiling for a Mega2560. Or decrease if the Arduino begins to
// crash due to the lack of available RAM or if the CPU is having trouble keeping up with planning
// new incoming motions as they are executed.
// #define BLOCK_BUFFER_SIZE 16 // Uncomment to override default in planner.h.

// Governs the size of the intermediary step segment buffer between the step execution algorithm
// and the planner blocks. Each segment is set of steps executed at a constant velocity over a
// fixed time defined by ACCELERATION_TICKS_PER_SECOND. They are computed such that the planner
// block velocity profile is traced exactly. The size of this buffer governs how much step
// execution lead time there is for other Grbl processes have to compute and do their thing
// before having to come back and refill this buffer, currently at ~50msec of step moves.
// #define SEGMENT_BUFFER_SIZE 6 // Uncomment to override default in stepper.h.

// Line buffer size from the serial input stream to be executed. Also, governs the size of
// each of the startup blocks, as they are each stored as a string of this size. Make sure
// to account for the available EEPROM at the defined memory address in settings.h and for
// the number of desired startup blocks.
// NOTE: 80 characters is not a problem except for extreme cases, but the line buffer size
// can be too small and g-code blocks can get truncated. Officially, the g-code standards
// support up to 256 characters. In future versions, this default will be increased, when
// we know how much extra memory space we can re-invest into this.
// #define LINE_BUFFER_SIZE 80  // Uncomment to override default in protocol.h

// Serial send and receive buffer size. The receive buffer is often used as another streaming
// buffer to store incoming blocks to be processed by Grbl when its ready. Most streaming
// interfaces will character count and track each block send to each block response. So,
// increase the receive buffer if a deeper receive buffer is needed for streaming and avaiable
// memory allows. The send buffer primarily handles messages in Grbl. Only increase if large
// messages are sent and Grbl begins to stall, waiting to send the rest of the message.
// NOTE: Grbl generates an average status report in about 0.5msec, but the serial TX stream at
// 115200 baud will take 5 msec to transmit a typical 55 character report. Worst case reports are
// around 90-100 characters. As long as the serial TX buffer doesn't get continually maxed, Grbl
// will continue operating efficiently. Size the TX buffer around the size of a worst-case report.
// #define RX_BUFFER_SIZE 128 // (1-254) Uncomment to override defaults in serial.h
// #define TX_BUFFER_SIZE 100 // (1-254)

// A simple software debouncing feature for hard limit switches. When enabled, the interrupt 
// monitoring the hard limit switch pins will enable the Arduino's watchdog timer to re-check 
// the limit pin state after a delay of about 32msec. This can help with CNC machines with 
// problematic false triggering of their hard limit switches, but it WILL NOT fix issues with 
// electrical interference on the signal cables from external sources. It's recommended to first
// use shielded signal cables with their shielding connected to ground (old USB/computer cables 
// work well and are cheap to find) and wire in a low-pass circuit into each limit pin.
// #define ENABLE_SOFTWARE_DEBOUNCE // Default disabled. Uncomment to enable.

// Configures the position after a probing cycle during Grbl's check mode. Disabled sets
// the position to the probe target, when enabled sets the position to the start position.
// #define SET_CHECK_MODE_PROBE_TO_START // Default disabled. Uncomment to enable.

// Force Grbl to check the state of the hard limit switches when the processor detects a pin
// change inside the hard limit ISR routine. By default, Grbl will trigger the hard limits
// alarm upon any pin change, since bouncing switches can cause a state check like this to
// misread the pin. When hard limits are triggered, they should be 100% reliable, which is the
// reason that this option is disabled by default. Only if your system/electronics can guarantee
// that the switches don't bounce, we recommend enabling this option. This will help prevent
// triggering a hard limit when the machine disengages from the switch.
// NOTE: This option has no effect if SOFTWARE_DEBOUNCE is enabled.
// #define HARD_LIMIT_FORCE_STATE_CHECK // Default disabled. Uncomment to enable.

// Adjusts homing cycle search and locate scalars. These are the multipliers used by Grbl's
// homing cycle to ensure the limit switches are engaged and cleared through each phase of
// the cycle. The search phase uses the axes max-travel setting times the SEARCH_SCALAR to
// determine distance to look for the limit switch. Once found, the locate phase begins and
// uses the homing pull-off distance setting times the LOCATE_SCALAR to pull-off and re-engage
// the limit switch.
// NOTE: Both of these values must be greater than 1.0 to ensure proper function.
// #define HOMING_AXIS_SEARCH_SCALAR  1.5 // Uncomment to override defaults in limits.c.
// #define HOMING_AXIS_LOCATE_SCALAR  10.0 // Uncomment to override defaults in limits.c.

// Enable the '$RST=*', '$RST=$', and '$RST=#' eeprom restore commands. There are cases where
// these commands may be undesirable. Simply comment the desired macro to disable it.
// NOTE: See SETTINGS_RESTORE_ALL macro for customizing the `$RST=*` command.
#define ENABLE_RESTORE_EEPROM_WIPE_ALL         // '$RST=*' Default enabled. Comment to disable.
#define ENABLE_RESTORE_EEPROM_DEFAULT_SETTINGS // '$RST=$' Default enabled. Comment to disable.
#define ENABLE_RESTORE_EEPROM_CLEAR_PARAMETERS // '$RST=#' Default enabled. Comment to disable.

// Defines the EEPROM data restored upon a settings version change and `$RST=*` command. Whenever the
// the settings or other EEPROM data structure changes between Grbl versions, Grbl will automatically
// wipe and restore the EEPROM. This macro controls what data is wiped and restored. This is useful
// particularily for OEMs that need to retain certain data. For example, the BUILD_INFO string can be
// written into the Arduino EEPROM via a seperate .INO sketch to contain product data. Altering this
// macro to not restore the build info EEPROM will ensure this data is retained after firmware upgrades.
// NOTE: Uncomment to override defaults in settings.h
// #define SETTINGS_RESTORE_ALL (SETTINGS_RESTORE_DEFAULTS | SETTINGS_RESTORE_PARAMETERS | SETTINGS_RESTORE_STARTUP_LINES | SETTINGS_RESTORE_BUILD_INFO)

// Enable the '$I=(string)' build info write command. If disabled, any existing build info data must
// be placed into EEPROM via external means with a valid checksum value. This macro option is useful
// to prevent this data from being over-written by a user, when used to store OEM product data.
// NOTE: If disabled and to ensure Grbl can never alter the build info line, you'll also need to enable
// the SETTING_RESTORE_ALL macro above and remove SETTINGS_RESTORE_BUILD_INFO from the mask.
// NOTE: See the included grblWrite_BuildInfo.ino example file to write this string seperately.
#define ENABLE_BUILD_INFO_WRITE_COMMAND // '$I=' Default enabled. Comment to disable.

// AVR processors require all interrupts to be disabled during an EEPROM write. This includes both
// the stepper ISRs and serial comm ISRs. In the event of a long EEPROM write, this ISR pause can
// cause active stepping to lose position and serial receive data to be lost. This configuration
// option forces the planner buffer to completely empty whenever the EEPROM is written to prevent
// any chance of lost steps.
// However, this doesn't prevent issues with lost serial RX data during an EEPROM write, especially
// if a GUI is premptively filling up the serial RX buffer simultaneously. It's highly advised for
// GUIs to flag these gcodes (G10,G28.1,G30.1) to always wait for an 'ok' after a block containing
// one of these commands before sending more data to eliminate this issue.
// NOTE: Most EEPROM write commands are implicitly blocked during a job (all '$' commands). However,
// coordinate set g-code commands (G10,G28/30.1) are not, since they are part of an active streaming
// job. At this time, this option only forces a planner buffer sync with these g-code commands.
#define FORCE_BUFFER_SYNC_DURING_EEPROM_WRITE //默认启用。注释后禁用

// In Grbl v0.9 and prior, there is an old outstanding bug where the `WPos:` work position reported
// may not correlate to what is executing, because `WPos:` is based on the g-code parser state, which
// can be several motions behind. This option forces the planner buffer to empty, sync, and stop
// motion whenever there is a command that alters the work coordinate offsets `G10,G43.1,G92,G54-59`.
// This is the simplest way to ensure `WPos:` is always correct. Fortunately, it's exceedingly rare
// that any of these commands are used need continuous motions through them.
#define FORCE_BUFFER_SYNC_DURING_WCO_CHANGE //默认启用。注释后禁用

// By default, Grbl disables feed rate overrides for all G38.x probe cycle commands. Although this
// may be different than some pro-class machine control, it's arguable that it should be this way. 
// Most probe sensors produce different levels of error that is dependent on rate of speed. By 
// keeping probing cycles to their programmed feed rates, the probe sensor should be a lot more
// repeatable. If needed, you can disable this behavior by uncommenting the define below.
// #define ALLOW_FEED_OVERRIDE_DURING_PROBE_CYCLES // Default disabled. Uncomment to enable.

// Enables and configures parking motion methods upon a safety door state. Primarily for OEMs
// that desire this feature for their integrated machines. At the moment, Grbl assumes that
// the parking motion only involves one axis, although the parking implementation was written
// to be easily refactored for any number of motions on different axes by altering the parking
// source code. At this time, Grbl only supports parking one axis (typically the Z-axis) that
// moves in the positive direction upon retracting and negative direction upon restoring position.
// The motion executes with a slow pull-out retraction motion, power-down, and a fast park.
// Restoring to the resume position follows these set motions in reverse: fast restore to
// pull-out position, power-up with a time-out, and plunge back to the original position at the
// slower pull-out rate.
// NOTE: Still a work-in-progress. Machine coordinates must be in all negative space and
// does not work with HOMING_FORCE_SET_ORIGIN enabled. Parking motion also moves only in
// positive direction.
// #define PARKING_ENABLE  // Default disabled. Uncomment to enable

// Configure options for the parking motion, if enabled.
#define PARKING_AXIS Z_AXIS // Define which axis that performs the parking motion
#define PARKING_TARGET -5.0 // Parking axis target. In mm, as machine coordinate [-max_travel,0].
#define PARKING_RATE 500.0 // Parking fast rate after pull-out in mm/min.
#define PARKING_PULLOUT_RATE 100.0 // Pull-out/plunge slow feed rate in mm/min.
#define PARKING_PULLOUT_INCREMENT 5.0 // Spindle pull-out and plunge distance in mm. Incremental distance.
                                      // Must be positive value or equal to zero.

// Enables a special set of M-code commands that enables and disables the parking motion. 
// These are controlled by `M56`, `M56 P1`, or `M56 Px` to enable and `M56 P0` to disable. 
// The command is modal and will be set after a planner sync. Since it is g-code, it is 
// executed in sync with g-code commands. It is not a real-time command.
// NOTE: PARKING_ENABLE is required. By default, M56 is active upon initialization. Use 
// DEACTIVATE_PARKING_UPON_INIT to set M56 P0 as the power-up default.
// #define ENABLE_PARKING_OVERRIDE_CONTROL   // Default disabled. Uncomment to enable
// #define DEACTIVATE_PARKING_UPON_INIT // Default disabled. Uncomment to enable.

// This option will automatically disable the laser during a feed hold by invoking a spindle stop
// override immediately after coming to a stop. However, this also means that the laser still may
// be reenabled by disabling the spindle stop override, if needed. This is purely a safety feature
// to ensure the laser doesn't inadvertently remain powered while at a stop and cause a fire.
#define DISABLE_LASER_DURING_HOLD //默认启用。注释后禁用

// This feature alters the spindle PWM/speed to a nonlinear output with a simple piecewise linear
// curve. Useful for spindles that don't produce the right RPM from Grbl's standard spindle PWM 
// linear model. Requires a solution by the 'fit_nonlinear_spindle.py' script in the /doc/script
// folder of the repo. See file comments on how to gather spindle data and run the script to
// generate a solution.
// #define ENABLE_PIECEWISE_LINEAR_SPINDLE  // Default disabled. Uncomment to enable.

// N_PIECES, RPM_MAX, RPM_MIN, RPM_POINTxx, and RPM_LINE_XX constants are all set and given by
// the 'fit_nonlinear_spindle.py' script solution. Used only when ENABLE_PIECEWISE_LINEAR_SPINDLE
// is enabled. Make sure the constant values are exactly the same as the script solution.
// NOTE: When N_PIECES < 4, unused RPM_LINE and RPM_POINT defines are not required and omitted.
#define N_PIECES 4  // Integer (1-4). Number of piecewise lines used in script solution.
#define RPM_MAX  11686.4  // Max RPM of model. $30 > RPM_MAX will be limited to RPM_MAX.
#define RPM_MIN  202.5    // Min RPM of model. $31 < RPM_MIN will be limited to RPM_MIN.
#define RPM_POINT12  6145.4  // Used N_PIECES >=2. Junction point between lines 1 and 2.
#define RPM_POINT23  9627.8  // Used N_PIECES >=3. Junction point between lines 2 and 3.
#define RPM_POINT34  10813.9 // Used N_PIECES = 4. Junction point between lines 3 and 4.
#define RPM_LINE_A1  3.197101e-03  // Used N_PIECES >=1. A and B constants of line 1.
#define RPM_LINE_B1  -3.526076e-1
#define RPM_LINE_A2  1.722950e-2   // Used N_PIECES >=2. A and B constants of line 2.
#define RPM_LINE_B2  8.588176e+01
#define RPM_LINE_A3  5.901518e-02  // Used N_PIECES >=3. A and B constants of line 3.
#define RPM_LINE_B3  4.881851e+02
#define RPM_LINE_A4  1.203413e-01  // Used N_PIECES = 4. A and B constants of line 4.
#define RPM_LINE_B4  1.151360e+03

/* --------------------------------------------------------------------------------------- 
  This optional dual axis feature is primarily for the homing cycle to locate two sides of 
  a dual-motor gantry independently, i.e. self-squaring. This requires an additional limit
  switch for the cloned motor. To self square, both limit switches on the cloned axis must
  be physically positioned to trigger when the gantry is square. Highly recommend keeping  
  the motors always enabled to ensure the gantry stays square with the $1=255 setting.

  For Grbl on the Arduino Uno, the cloned axis limit switch must to be shared with and 
  wired with z-axis limit pin due to the lack of available pins. The homing cycle must home
  the z-axis and cloned axis in different cycles, which is already the default config.

  The dual axis feature works by cloning an axis step output onto another pair of step
  and direction pins. The step pulse and direction of the cloned motor can be set 
  independently of the main axis motor. However to save precious flash and memory, this
  dual axis feature must share the same settings (step/mm, max speed, acceleration) as the 
  parent motor. This is NOT a feature for an independent fourth axis. Only a motor clone.

  WARNING: Make sure to test the directions of your dual axis motors! They must be setup
  to move the same direction BEFORE running your first homing cycle or any long motion!
  Motors moving in opposite directions can cause serious damage to your machine! Use this 
  dual axis feature at your own risk.
*/
// NOTE: This feature requires approximately 400 bytes of flash. Certain configurations can
// run out of flash to fit on an Arduino 328p/Uno. Only X and Y axes are supported. Variable
// spindle/laser mode IS supported, but only for one config option. Core XY, spindle direction
// pin, and M7 mist coolant are disabled/not supported.
// #define ENABLE_DUAL_AXIS	// Default disabled. Uncomment to enable.

// Select the one axis to mirror another motor. Only X and Y axis is supported at this time.
#define DUAL_AXIS_SELECT  X_AXIS  // Must be either X_AXIS or Y_AXIS

// To prevent the homing cycle from racking the dual axis, when one limit triggers before the
// other due to switch failure or noise, the homing cycle will automatically abort if the second 
// motor's limit switch does not trigger within the three distance parameters defined below. 
// Axis length percent will automatically compute a fail distance as a percentage of the max
// travel of the other non-dual axis, i.e. if dual axis select is X_AXIS at 5.0%, then the fail 
// distance will be computed as 5.0% of y-axis max travel. Fail distance max and min are the 
// limits of how far or little a valid fail distance is.
#define DUAL_AXIS_HOMING_FAIL_AXIS_LENGTH_PERCENT  5.0  // Float (percent)
#define DUAL_AXIS_HOMING_FAIL_DISTANCE_MAX  25.0  // Float (mm)
#define DUAL_AXIS_HOMING_FAIL_DISTANCE_MIN  2.5  // Float (mm)

// Dual axis pin configuration currently supports two shields. Uncomment the shield you want,
// and comment out the other one(s).
// NOTE: Protoneer CNC Shield v3.51 has A.STP and A.DIR wired to pins A4 and A3 respectively.
// The variable spindle (i.e. laser mode) build option works and may be enabled or disabled.
// Coolant pin A3 is moved to D13, replacing spindle direction.
#define DUAL_AXIS_CONFIG_PROTONEER_V3_51    // Uncomment to select. Comment other configs.

// NOTE: Arduino CNC Shield Clone (Originally Protoneer v3.0) has A.STP and A.DIR wired to 
// D12 and D13, respectively. With the limit pins and stepper enable pin on this same port,
// the spindle enable pin had to be moved and spindle direction pin deleted. The spindle
// enable pin now resides on A3, replacing coolant enable. Coolant enable is bumped over to
// pin A4. Spindle enable is used far more and this pin setup helps facilitate users to 
// integrate this feature without arguably too much work. 
// Variable spindle (i.e. laser mode) does NOT work with this shield as configured. While
// variable spindle technically can work with this shield, it requires too many changes for
// most user setups to accomodate. It would best be implemented by sharing all limit switches
// on pins D9/D10 (as [X1,Z]/[X2,Y] or [X,Y2]/[Y1,Z]), home each axis independently, and 
// updating lots of code to ensure everything is running correctly.
// #define DUAL_AXIS_CONFIG_CNC_SHIELD_CLONE  // Uncomment to select. Comment other configs.


/* ---------------------------------------------------------------------------------------
   OEM Single File Configuration Option

   Instructions: Paste the cpu_map and default setting definitions below without an enclosing
   #ifdef. Comment out the CPU_MAP_xxx and DEFAULT_xxx defines at the top of this file, and
   the compiler will ignore the contents of defaults.h and cpu_map.h and use the definitions
   below.
*/

// Paste CPU_MAP definitions here.

// Paste default settings definitions here.


#endif
