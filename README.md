# Bootloader开发

## 目录

[toc]

## 正文

## 1. IAP简介

​	IAP(In Application Programmer)，即在应用编程，旨在产品发布后，给用户留有接口进行固件的升级，而无需使用ST-Link或者JLINK等烧录工具。设计固件程序时需要编写两个项目代码，第一个项目程序不执行正常的功能操作，而是通过某种方式来接收程序或数据，来对第二部分代码进行更新。第二部分代码则为真正的功能代码。

​	第一部分代码称为Bootloader，需通过烧录设备(如XLINK)录入芯片。

​	第二部分代码成为APP程序，即我们平时写的程序，只是程序存放和执行的地址需要进行修改，其余的与一般代码无恙。

**优势**

* *烧录方便*

​		烧录代码方式可以选择串口，SD卡或者外部FLASH，而无需使用烧录设备，且烧录的速度与XLINK差不多

* *操作方便*

​		Bootloader程序只需烧录进去一遍，以后都无需再管，APP程序生成简单，只需配置两个参数以及添加一句代码

* *轻量级产品*

​		Bootloader目前所占空间即为7K左右，相比于F4内部FLASH的1024K容量可忽略不计，对代码运行无影响

* *使用灵活*

​		由于实验室机器人的代码文件大小一般在60K-80K之间，对于外部FLASH或者SD卡，一次可存储好多好多个程序(如W25Q16大小为2M，理论上可承载30个左右的代码，不过考虑到预留余量，会控制写入的程序数目)，之后会添加程序命名以及用户交互的代码，方便一次性对多个代码(如比较不同的控制器的控制能力)进行调试。

* *拓展性强*

​		可以预留按键等交互接口，可以根据板子实际板载的外设来调整Bootloader的作用方式。

**不足**

* 型号受限

​		目前仅适用于内部FLASH大小为1M的板子(受限于程序存储地址不一致)，实验室的F405是适用的，在之后的版本会尝试改进。

## 2. Struture框架

### 1. 用户交互

[![sD0tlF.png](https://s3.ax1x.com/2021/01/16/sD0tlF.png)](https://imgchr.com/i/sD0tlF)

### 2. 程序实现

[![sDcH5d.md.png](https://s3.ax1x.com/2021/01/16/sDcH5d.md.png)](https://imgchr.com/i/sDcH5d)

## 3. How to use使用方法

### (1)Bootloader

* 使用烧录设备(X-LINK等)将该代码文件烧录进板子
* 使用串口助手进行信息交互

### (2)APP程序生成

> 以下的操作对于一个APP程序只需要一次，之后每次编译都会生成bin文件

* **内部FLASH大小为1M(0X100000)的板子**

  * 修改芯片的程序存放地址
    [![sDBTER.png](https://s3.ax1x.com/2021/01/16/sDBTER.png)](https://imgchr.com/i/sDBTER)

  * 添加BIN文件生成功能
    [![sDBIb9.png](https://s3.ax1x.com/2021/01/16/sDBIb9.png)](https://imgchr.com/i/sDBIb9)

    即选择自己电脑安装MDK位置里面的fromelf.exe(MDK自带的bin生成软件)，再在后面添加以下字符串
    --bin -o "$L@L.bin" "#L"

  * 修改中断向量表的偏移量

    在main函数的起始位置添加以下代码

    ```
    SCB->VTOR = FLASH_BASE | 0x10000;
    ```
    
  * 编译
    然后你会发现，在**MDK-ARM/你的项目名/**的路径下会生成一个bin文件，这就是我们需要的程序文件啦

  

### (3)烧录

* 配置串口助手，与芯片的串口参数相匹配
* 按照交互的信息进行操作
  [![sDcQjf.md.png](https://s3.ax1x.com/2021/01/16/sDcQjf.md.png)](https://imgchr.com/i/sDcQjf)



## 4. 版本

* V1.0
  实现了串口交互形式的更新固件，SD卡和外部FLASH暂不支持。
* V2.0
  优化了代码框架，实现了运用FLASH存储多程序的功能，优化了代码框架



## 5. 改进

** **未完待续** **

欢迎各位实验室的大鸽们提出宝贵意见😘