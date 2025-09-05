# 匿名飞控舵机驱动
## 使用方法
### 1.添加舵机驱动
将本仓库下的`Drv_Servo.c` 和 `Drv_Servo.h`文件添加到源代码的/drivers文件夹下

路径：
```
\drivers
```
### 2.替换PWM输出驱动
将本仓库下的`Drv_pwm_out.c`文件替换源代码的/drivers/Drv_pwm_out.c文件
路径：
```
\drivers\Drv_pwm_out.c
```
### 3.修改飞控初始化文件（关键步骤）
找到源代码的 `BSP_Init.c` 文件，其路径为：
```
\applications\BSP_Init.c
```
找到`All_Init()`函数，在该函数中添加一段代码：
```
ServoInit();					//初始化舵机端口
Delay_ms(50);					//延时
```
![photo](./photo.png)
这段代码的作用是告诉飞控初始化舵机PA3引脚的定时器相关配置

