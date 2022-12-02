# Multi-Function Button Dectection

Multi-Function Button Dectection，简称MFBD，是一个基于嵌入式C语言的按键库，自动消抖，支持单击、长按单次触发、长按连续触发、双击、多击（三击、四击等等，最大256击）按键操作，可以运行于裸机和各类RTOS。

## MFBD设计理念

MFBD尽可能的减少了RAM的使用，将能保存在Flash中的参数都保存在Flash中。  
当然还可以继续将变量放置在RAM中，这也会减少编写程序时候的工作量。  
MFBD通过类似电脑对按键的处理方式，将每个按键的不同操作分配不同的键值。  
同时mfbd采用了回调函数的机制，通过回调函数将键值上报到上层应用程序，当然用户也可以直接在回调函数中进行操作。  

## MFBD移植和配置

MFBD移植只需将文件添加到工程目录即可，配置项都已经汇总到mfbd_cfg.h中：

```c
/* mfbd_btn_code_t is the type of value of every button event. */
typedef uint16_t    mfbd_btn_code_t;

/* mfbd_btn_count_t is the type of count time for button scanning. */
typedef uint16_t    mfbd_btn_count_t;

/* mfbd_btn_index_t is the type of params when calling is_btn_down_func in mfbd_group_t. */
typedef uint32_t    mfbd_btn_index_t;

/* set MFBD_USE_TINY_BUTTON to 1 will enable tiny button functions. */
#define MFBD_USE_TINY_BUTTON                1

/* set MFBD_USE_NORMAL_BUTTON to 1 will enable normal button functions. */
#define MFBD_USE_NORMAL_BUTTON              1

/* set MFBD_USE_MULTIFUCNTION_BUTTON to 1 will enable multifunction button functions. */
#define MFBD_USE_MULTIFUCNTION_BUTTON       1

/* set MFBD_USE_BTN_SCAN_PRE_FUNC to 1 will enable running prepare_function before run button detection function */
#define MFBD_USE_BTN_SCAN_PRE_FUNC          0

/* set MFBD_USE_BTN_SCAN_AFTER_FUNC to 1 will enable running after_function after run button detection function */
#define MFBD_USE_BTN_SCAN_AFTER_FUNC        0

/*
 * @Note:
 * set MFBD_PARAMS_SAME_IN_GROUP to 1 means all key's filter_time/repeat_time/long_time/multiclick_time are same,
 * it will not save filter_time/repeat_time/long_time/multiclick_time in btn_info struct.
 * if MFBD_PARAMS_SAME_IN_GROUP is 1, btns cannot disable repeat count alone.
 */
#define MFBD_PARAMS_SAME_IN_GROUP           1

```

`mfbd_btn_code_t`：按键键值的类型。  
可以根据按键事件的多少进行设置。例如有127个按键，每个按键只有按下和松开两个事件，那么就可以设置为`uint8_t`。  
一般情况下推荐使用`uint16_t`。  

`mfbd_btn_count_t`：按键检测时时间计数参数的类型。  
如果为`uint8_t`，那么时间计数无法超过255，此时如果检测周期为10ms，那么将无法进行255*10ms以上的检测操作。  
一般情况下推荐使用`uint16_t`。  

`mfbd_btn_index_t`：调用按键检测函数时传入的参数类型。  
按键检测函数为每个按键组都有自己单独的一个按键检测函数，在检测按键时，函数需要传入一个参数，该参数类型可根据芯片需求自定。  

`MFBD_USE_TINY_BUTTON`：是否使用`tiny button`相关函数和定义。  
因在`mfbd_group_t`结构体中会存放`tiny button`,`normal button`,`multi-function button`三个地址数组的指针，所以会占用3个4字节，通过开关不同的宏定义，可以精简程序空间。

`MFBD_USE_NORMAL_BUTTON`：是否使用`normal button`相关函数和定义。  

`MFBD_USE_MULTIFUCNTION_BUTTON`：是否使用`multi-function button`相关函数和定义。  

`MFBD_USE_BTN_SCAN_PRE_FUNC`：是否使能在检测每组按键之前调用准备函数。  
该选项非常适合矩阵键盘，矩阵键盘每次扫描后都可以获得所有按键的状态，而无需每次检测一个按键。  
通过使能该函数，可以将矩阵按键的键值存入一个缓冲区，然后该按键组的按键读取函数只需要从缓冲区中读取相应的数值即可。  

`MFBD_USE_BTN_SCAN_AFTER_FUNC`：是否使能在检测每组按键之后调用结束函数。  
该选项可以用来进行低功耗管理，通过在检测后关闭相应外设的电源等操作。  

`MFBD_PARAMS_SAME_IN_GROUP`：当所有按键的扫描滤波时间、长按时间、重复上报时间、连击释放时间一致的时候，可以将该宏置为`1`。此时，每个按键的info结构体中将不再存放上述事件参数，而把时间参数存放在group结构体中。当该选项使能的时候，无法在启用长按事件(long count)的时候单独禁用某个按键的重复上报事件(repeat count)，但可以通过设置`repeat_time = 0`禁用全部按键的重复上报事件。

## MFBD按键事件

### 单击事件

单击事件分为按下事件和松开事件。  
按下事件发生后，如果按下的按键值不为0，就会上报按下按键值。  
松开事件发生后，如果松开的按键值不为0，就会上报松开按键值。  

### 长按事件

单击事件分为长按连续触发事件和长按单次触发事件。  
**长按连续触发事件**效果为当按键持续按下时，持续按下达到一个特定的时间后，上报一次长按按键值，然后开始每隔特定的时间后，继续上报按键值。  
**长按单次触发事件**效果为当按键持续按下时，只检测一个特定的长时间后，上报一次长按按键值。如果不想上报单独的长按按键值，长按只重复上报按键值的话，将长按按键值设置和按下按键值一样即可。  

### 连击事件

连击事件指的是在按键松开后的指定时间后，再一次检测到按键按下。  
连击事件中，每次按键按下后，都会上报指定次数的连击按键值。  
其实连击事件是不应该由按键驱动层进行检测的，但是嵌入式环境资源紧张，不可以像电脑那样交给应用层处理。
**注意：多次连击事件和长按事件是冲突的，当长按事件发生，不会进行多次连击的检测。当触发多次连击检测后，也不会进行长按事件的检测**

## MFBD按键组结构体

MFBD的函数调用的参数都是MFBD按键组结构体指针，用户需要独立编写给其赋值。

```c
/* mfbd group struct */
typedef struct _mfbd_group_struct
{
    /* used to read whether button is down. */
    unsigned char (*is_btn_down_func)(mfbd_btn_index_t btn_index);

    /* used to report btn_value, must have a legal value, must not be NULL. */
    void (*btn_value_report)(mfbd_btn_code_t btn_value);

#if MFBD_USE_TINY_BUTTON
    /* pointer to the head of tiny buttons array */
    mfbd_tbtn_t **tbtns;
#endif

#if MFBD_USE_NORMAL_BUTTON
    /* pointer to the head of normal buttons array */
    mfbd_nbtn_t **nbtns;
#endif

#if MFBD_USE_MULTIFUCNTION_BUTTON
    /* pointer to the head of multifunction buttons array */
    mfbd_mbtn_t **mbtns;
#endif

/* if set MFBD_PARAMS_SAME_IN_GROUP to 1, all btns in group has same params. */
#if MFBD_PARAMS_SAME_IN_GROUP

#if MFBD_USE_TINY_BUTTON || MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON
    mfbd_btn_count_t   filter_time;             /* filter time when button state changed, please do not use 0. */
#endif /*  MFBD_USE_TINY_BUTTON || MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON */

#if MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON

    mfbd_btn_count_t   repeat_time;             /* repeat time when button still down for over long_time, set 0 will disable repeat time count. */

    mfbd_btn_count_t   long_time;               /* long time when button still down, set 0 will disable long time and repeat time count. */

#endif /* MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON */

#if MFBD_USE_MULTIFUCNTION_BUTTON

    mfbd_btn_count_t   multiclick_time;         /* multi-click time when button still up, set 0 will disable multi-click time count. */

#endif /* MFBD_USE_MULTIFUCNTION_BUTTON */

#endif

#if MFBD_USE_BTN_SCAN_PRE_FUNC
    /* prepare function when start to scan buttons for each group. */
    void (*btn_scan_prepare)(void);
#endif

#if MFBD_USE_BTN_SCAN_AFTER_FUNC
    /* function after scanning buttons for each group. */
    void (*btn_scan_after)(void);
#endif

} mfbd_group_t;
```

**按键组结构体成员介绍：**

| 数据成员 | 说明 |
| :---- | :---- |
| `is_btn_down_func`   | 按键组中按键的获取按键状态的函数。 |
| `btn_value_report`   | 按键组中按键事件触发后上报按键值的函数。 |
| `tbtns`      | tbtn按键地址数组的首地址，扫描程序通过该指针为入口，扫描tbtn组的按键，通过宏定义`MFBD_USE_TINY_BUTTON`开启。 |
| `nbtns`      | nbtn按键地址数组的首地址，扫描程序通过该指针为入口，扫描nbtn组的按键，通过宏定义`MFBD_USE_TINY_BUTTON`开启。 |
| `mbtns`      | mbtn按键地址数组的首地址，扫描程序通过该指针为入口，扫描mbtn组的按键，通过宏定义`MFBD_USE_TINY_BUTTON`开启。 |
| `filter_time`     | 按键按下和松开的滤波时间，当MFBD_PARAMS_SAME_IN_GROUP为1时，该时间参数存放到GROUP结构体中。 |
| `repeat_time`     | 按键长按事件发生后，重复上报btn_down_code的时间周期，如果设置为0，不会检测长按后的重复事件。当MFBD_PARAMS_SAME_IN_GROUP为1时，该时间参数存放到GROUP结构体中。 |
| `long_time`       | 按键长按事件触发时间，如果设置为0，不会检测长按事件（包括长按后的重复事件）。当MFBD_PARAMS_SAME_IN_GROUP为1时，该时间参数存放到GROUP结构体中。 |
| `multiclick_time` | 按键连按事件的终止时间。当MFBD_PARAMS_SAME_IN_GROUP为1时，该时间参数存放到GROUP结构体中。 |
| `btn_scan_prepare`   | 按键组开始扫描前的准备函数，通过宏定义`MFBD_USE_BTN_SCAN_PRE_FUNC`开启，设置为NULL时不会调用。 |
| `btn_scan_after`     | 按键组完成扫描后的函数，通过宏定义`MFBD_USE_BTN_SCAN_AFTER_FUNC`开启，设置为NULL时不会调用。 |

## MFBD按键结构体

MFBD提供了三种按键处理，之所以提供三种按键处理，是为了开发中可以根据不同项目中不同按键的不同情况，自由的减少RAM的占用。  

1. **tiny button**：只支持Button单击的操作，不支持按键其他操作，在普通的场景中应用广泛。  
2. **normal button**：normal button支持Button单击、长按的操作，不支持多次连击操作。normal button是很类似于电脑按键处理的，只是多了一个长按单次触发事件（可以通过将btn_long_code设置为0，来禁用长按单次触发事件）。  
3. **multi-function button**：multi-function button支持单击、长按、多次连击的操作。  
三种按键的结构体中同名的成员功能是基本相同的。  

**按键信息结构体成员介绍：**

| 数据成员 |  说明 |
| :---- | :---- |
| `filter_time`     | 按键按下和松开的滤波时间，当MFBD_PARAMS_SAME_IN_GROUP为0时，该时间参数存放到每个按键信息结构体中。 |
| `repeat_time`     | 按键长按事件发生后，重复上报btn_down_code的时间周期，如果设置为0，不会检测长按后的重复事件。当MFBD_PARAMS_SAME_IN_GROUP为0时，该时间参数存放到每个按键信息结构体中。 |
| `long_time`       | 按键长按事件触发时间，如果设置为0，不会检测长按事件（包括长按后的重复事件）。当MFBD_PARAMS_SAME_IN_GROUP为0时，该时间参数存放到每个按键信息结构体中。 |
| `multiclick_time` | 按键连按事件的终止时间。当MFBD_PARAMS_SAME_IN_GROUP为0时，该时间参数存放到每个按键信息结构体中。 |
| `btn_down_code`   | tbtn和nbtn按键按下后，需要上报的键值。如果设置为0，就不会上报。 |
| `*btn_down_code`  | mbtn中，按键多次按下后，需要上报的键值数组指针，不可以为NULL。数组中按键值为0的，不会上报。 |
| `btn_up_code`     | 按键按下后再松开后，需要上报的键值。如果设置为0，就不会上报。 |
| `btn_long_code`   | 按键长按后，需要上报的键值。如果设置为0，就会禁用长按检测和重复上报检测。 |
| `btn_index`       | 按键组扫描时，调用按键组结构体中`is_btn_down_func`所传入的参数。 |
| `max_multiclick_state`| mbtn按键最大支持的连击次数。 |

**按键控制结构体成员介绍：**

| 数据成员 | 说明 |
| :---- | :---- |
| `*btn_info`         | 指向每一个按键的键值和扫描参数信息。 |
| `filter_count`      | 用于记录按键滤波时间和内部判断按键状态。 |
| `long_count`        | 用于记录按键长按事件触发时间。 |
| `repeat_count`      | 用于记录按键长按事件发生后的重复上报时间。 |
| `multiclick_count`  | 用于记录按键松开事件发生后，重置多次连击状态multiclick_state的时间。 |
| `multiclick_state`  | 用于记录按键连击状态。 |
| `state`             | 用于记录当前按键的状态和内部判断。 |

## MFBD button定义

MFBD使用略微复杂，但是给开发者提供了更大的自由度。  
但是在按键程序编写完成后，其他应用程序获取按键值将变得十分容易，无需关注按键硬件层，只需要根据不同的按键值做出相应的操作即可。  
ringbuf环形缓冲区是MFBD的好搭档，推荐移植使用时和ringbuf配合，来上报和获取按键值。  
各类RTOS中的MailBox邮箱机制也是一个搭配使用不错的选择。  
为了方便定义按键结构体，`mfbd.h`中提供了宏定义供用户定义按键使用。用户也可以自行通过结构体定义。  
宏定义`MFBD_PARAMS_SAME_IN_GROUP`是否为0，定义方式的参数是不一样的。  

### tiny button定义示例

```c
#if MFBD_BTN_PARAMS_SAME
/* MFBD_TBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, BTN_DOWN_CODE, BTN_UP_CODE) */
MFBD_TBTN_DEFINE(test_tbtn, 1, 0x1201, 0x1200);
#else
/* MFBD_TBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, BTN_DOWN_CODE, BTN_UP_CODE) */
MFBD_TBTN_DEFINE(test_tbtn, 1, 3, 0x1201, 0x1200);
#endif /*MFBD_BTN_PARAMS_SAME*/
```

### normal button定义示例

```c
#if MFBD_BTN_PARAMS_SAME
/* MFBD_NBTN_DEFINE(NAME, BTN_INDEX, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE) */
MFBD_NBTN_DEFINE(test_nbtn1, 3,  0x1401, 0x1400, 0x1402);
#else
/* MFBD_NBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE) */
MFBD_NBTN_DEFINE(test_nbtn1, 3, 3, 0, 150, 0x1401, 0x1400, 0x1402);
#endif /*MFBD_BTN_PARAMS_SAME*/
```

### multi-function button定义示例

multi-function button和其他按键不同，它的宏定义中使用了可变参数，由于可变参数只可以在末尾，所以一并将BTN_DOWN_CODE参数放到了后边  
可变参数可以方便的定义出不同数量的按键值数组，方便连击事件的按键值赋值  

```c
#if MFBD_BTN_PARAMS_SAME
/* MFBD_NBTN_DEFINE(NAME, BTN_INDEX, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE) */
MFBD_MBTN_DEFINE(test_mbtn, 4, 3, 0x1501, 0x1500, 0, 0x1511, 0x1521, 0x1531);
#else
/* MFBD_NBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE) */
MFBD_MBTN_DEFINE(test_mbtn, 4, 3, 30, 150, 75, 3, 0x1501, 0x1500, 0, 0x1511, 0x1521, 0x1531);
#endif /*MFBD_BTN_PARAMS_SAME*/
```

### 默认定义

为了方便定义，还引入了默认定义宏

```c
MFBD_TBTN_DEFAULT_DEFINE(NAME, ...);
MFBD_NBTN_DEFAULT_DEFINE(NAME, ...);
MFBD_MBTN_DEFAULT_DEFINE(NAME, ...);
```

在使用DEFAULT_DEFINE时，会自动使用按键名称为扩展的枚举或宏变量值。

```MFBD_TBTN_DEFAULT_DEFINE(HI, ...)```，将会默认使用```HI_DOWN_CODE```和```HI_UP_CODE```作为按键值。所以程序中需要准备好该两个名称的按键值枚举变量或宏定义。

```MFBD_NBTN_DEFAULT_DEFINE(HI, ...)```，将会默认使用```HI_DOWN_CODE```、```HI_UP_CODE```和```HI_LONG_CODE```作为按键值。所以程序中需要准备好该三个名称的按键值枚举变量或宏定义。

```MFBD_MBTN_DEFAULT_DEFINE(HI, ...)```，将会默认使用```HI_UP_CODE```和```HI_LONG_CODE```作为按键值。所以程序中需要准备好该两个名称的按键值枚举变量或宏定义。另外MBTN需要提供一个变量名为```HI_DOWN_CODES```的按键值数组，作为多次连击的按键值。

## MFBD 使用示例

这里使用上方MFBD定义示例中宏定义的方式定义按键进行操作  
示例中：按键按下低电平，按键弹起高电平  
按键上报函数和读取函数统一如下  

### 按键组GROUP中读取按键的接口函数

```c
unsigned char bsp_btn_check(mfbd_btn_index_t btn_index)
{
    switch (btn_index)
    {
    case 1:
        if (rt_pin_read(BTN_KEY0) == 0)
        {
            return 1;
        }
        break;
    case 2:
        if (rt_pin_read(BTN_KEY1) == 0)
        {
            return 1;
        }
        break;
    case 3:
        if (rt_pin_read(BTN_KEY2) == 0)
        {
            return 1;
        }
        break;
    case 4:
        if (rt_pin_read(BTN_WK_UP) == 1)
        {
            return 1;
        }
        break;
    default:
        break;
    }
    return 0;
}
```

### 按键组GROUP中上报按键的接口函数

```c
void bsp_btn_value_report(mfbd_btn_code_t btn_value)
{
    rt_kprintf("%04x\n", btn_value);
}
```

### 按键组GROUP定义示例

按键组GROUP需要将组内的tbtn、nbtn、mbtn分别组成数组，然后将数组头指针赋值到结构体中

```c
MFBD_TBTN_ARRAYLIST(test_tbtn_list, &test_tbtn);

MFBD_NBTN_ARRAYLIST(test_nbtn_list, &test_nbtn1, &test_nbtn);

MFBD_MBTN_ARRAYLIST(test_mbtn_list, &test_mbtn);

const mfbd_group_t test_btn_group =
{
    bsp_btn_check,
    bsp_btn_value_report,
    test_tbtn_list,
    test_nbtn_list,
    test_mbtn_list,

#if MFBD_PARAMS_SAME_IN_GROUP

#if MFBD_USE_TINY_BUTTON || MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON
    3,
#endif /*  MFBD_USE_TINY_BUTTON || MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON */

#if MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON
    30,
    150,
#endif /* MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON */

#if MFBD_USE_MULTIFUCNTION_BUTTON
    75,
#endif /* MFBD_USE_MULTIFUCNTION_BUTTON */

#endif /*MFBD_PARAMS_SAME_IN_GROUP*/

};
```

### 按键检测函数调用

在其他配置都已完成后，只需定时调用检测函数即可完成对按键组的检测并上报兼职。

```c
extern void mfbd_group_scan(const mfbd_group_t *_pbtn_group);
```

### 停止按键检测

在某些场景下可能需要临时停止按键检测，可以调用下面的函数复位按键组内所有的信息。

```c
extern void mfbd_group_reset(const mfbd_group_t *_pbtn_group);
```

## 移植使用示例工程

MFBD提供了下面的测试例程，如果你使用其他开发板和其他RTOS，可以参考例程移植即可。

### stm32f429-atk-apollo(rt-thread)

[examples/mfbd_demo_rtt.c](https://github.com/smartmx/mfbd/tree/main/examples/mfbd_demo_rtt.c)，该例程是基于rt-thread os，开发板为正点原子的[stm32f429-atk-apollo](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f429-atk-apollo)。

### [stm32f429-atk-apollo](https://github.com/smartmx/mfbd-examples/tree/main/STM32F429IGT6_MFBD)

该例程为**stm32f429-atk-apollo**的裸机移植使用例程。

### [CH573-EVT](https://github.com/smartmx/mfbd-examples/tree/main/CH573_MFBD)

该例程为基于沁恒微电子蓝牙芯片CH573的基于tmos的移植使用例程。

## 其他功能

### 矩阵键盘

矩阵键盘可以通过使能`MFBD_USE_BTN_SCAN_PRE_FUNC`，将矩阵键盘上的所有按键通过链表链接到同一个按键组。  
即可在每次轮询检测时，通过准备函数将所有的按键值扫描出来存放到缓存中。然后在获取函数中根据缓存获取按键值即可。  

### 组合按键

组合按键其实不应该由驱动层进行处理，应该由应用层处理。  
不同的应用需求不一样，有的需要组合按键，有的不需要组合按键。  
组合按键生效的应用中，普通按键按下是有应用处理延迟的。  
可以在上报函数中，将时间参数和按键值一起上报至应用层，交由应用层判断。  
应用层在接收到一个按键的键值后，在没有接收到该按键松开的按键值时，收到了组合按键的键值，则判定组合按键按下。

### 低功耗

低功耗可以通过使能`MFBD_USE_BTN_SCAN_AFTER_FUNC`和`MFBD_USE_BTN_SCAN_PRE_FUNC`，在每次检测完每一组的按键后，在结束函数中，将每组的外设关闭，在准备函数中,将每组的外设打开，以达到低功耗的目的。  
需要根据不同的芯片的情况分好组，不然可能导致无法正常运行。  

## [博客主页](https://blog.maxiang.vip/)
