# 实验0 课程先导实验说明文档

> **练习完成方式：独立完成**
>
> **尽早开始，请在课程开始时提交代码和实验文档，最晚在课程开始一周内提交**
>
> **保证是自己动手做出来的**
>
> **请不要直接搜索或者讨论答案本身，鼓励交流讨论非答案内容**
>
> **实验源码：仔细阅读此文档，编写程序完成指定功能，并运行测试通过，将修改过的源文件上传 [Gitee](https://gitee.com/)** **和 学习通作业**
>
> **实验文档撰写：要求使用 markdown格式书写，放在源码文件夹下面，推荐使用 [Typora](https://typora.io/)**

[TOC]

## 概述

​	这个实验帮助你找到编程状态，为后续的实验做准备，也可以看作是编程能力的一次检验。

### 实验目标

- C的显式内存管理。
- 通过指针创建和操作数据。
- 处理字符串。
- 在数据结构中存储冗余信息以提高关键操作的性能。
- 实现健壮的代码能处理非法的参数，包括NULL指针。

### 提供的资料

- 开始代码包

  starter code [link](https://cnchen2000.oss-cn-shanghai.aliyuncs.com/csapp/cprogramminglab-zh.zip) (直接按住ctrl点击打开此超链接)下载后解压，请**先按照文档后面所附的实验环境配置安装好环境**，通过mobaXterm连接WSL，把解压文件夹拖动到WSL中，用VS Code打开项目文件夹。

  需要编写代码的源文件包括：

​				**queue.c**

​				**queue.h**

​		需要使用到的函数包括：

​			内存函数： **mallloc** 和 **free**

​			字符串函数：**strlen** 和 **strcpy**

​		函数描述文档查询网站：[cppreference.com](https://en.cppreference.com/w/c/string/byte/strcpy)

- 此说明文档

 

## 需提交的材料清单

### 1 修改后的queue.c 及其说明文档

​	完全实现以下函数，更多细节查看代码文件注释，包括非法操作说明（例如从空队列或者NULL中删除元素操作），返回值等说明：

- q_new: 创建新的空队列。
- q_free: 释放队列使用的所有内存。
- q_insert_head: 将新元素插入队首 (LIFO)。
- q_insert_tail: 将新元素插入队尾 (FIFO)。
- q_remove_head: 移除队首元素。
- q_size: 计算队列元素数量。
- q_reverse: 将队列元素顺序反转。注意函数不能直接或者间接调用其他函数为队列元素分配任何额外的空间，通过重排现有元素实现。

### 2 修改后的 queue.h 

​	头文件根据函数功能要求修改queue_t 结构，并注释说明修改原因。

### 3 实验说明文档

​	做实验的同时别忘了写文档记录，记录实验过程（关于函数功能，实现过程，测试情况，bug调试）并写下实验==心得体会==（关于实验心里活动、碰到的难点、解决后的收获、能力成长或其他）。



## 实验内容

### 实验说明

​	这部分实验做为热身练习涉及了单链表、内存分配和释放、功能测试。包括了 插入队首、插入队尾、移除队首、反转链表等常见链表操作，还包括测试和调试代码。在 **queue.c** 文件中编写程序并提交该代码文件。

- 开始代码中的数据结构：

```c
/* 链表中的元素 */
typedef struct ELE {
	char *value;
	struct ELE *next;
} list_ele_t;
/* 队列结构 */
typedef struct {
	list_ele_t *head; /* 链表 */
} queue_t;
```

​	list_ele_t 结构和 queue_t 结构一起实现如下图所示的字符串队列。在开始代码中，queue_t 结构定义中只有一个域 head，存储指向队列的指针，你将需要添加其他的域；队列内容由单链表表示，每个元素用 list_ele_t 结构表示，有 value 和 next 两个域，存储指向一个字符串的指针和指向下一个链表元素的指针，字符用ASCII编码，最后一个元素的next 指针设置为 NULL。list_ele_t 不需要添加额外的域。

![image-20210823153936250](https://cnchen2000.oss-cn-shanghai.aliyuncs.com/img/image-20210823153936250.png)

- C中的字符串


​    在绝大多数机器中，char 数据类型用一个字节表示。要存储长度为 n 的字符串， 字符数组需要有 n+1 个元素，最后一个额外的字符用来存储字符串结束符 '\0'（也即整数0）, 在list_ele_t 结构中用 value 指针指向该字符串。图中表示的字符串列表是 [“cab”, “bead”, “cab”]， 字符是用16机制数表示的（2个16机制数正好表示1个字节）。注意两个“cab” 字符串分别用数组单独表示，每个都有单独的拷贝。在构造链表元素时，别忘了为字符串动态分配内存空间。

- 队列的特殊情况

​	队列用 queue_t * 指针表示，注意区别两种特殊情况： NULL队列为指针值为NULL的队列，空队列为指针指向某个具体存在的合法结构，但是head 域 的值为 NULL。你的代码在处理包含一个或者多个元素的队列时，也需要恰当地处理好这两种队列特例。	

### 背景知识   

1. ####  布尔类型 

​	绝大多数高级编程语言都支持显式地表示真和假这两个值，它们的类型称为布尔类型。在C语言中，添加头文件 #include <stdbool.h> 即可直接使用布尔类型（bool），值要么为真（true）要么为假（false），注意字母全小写。计算机硬件能存储的是1和0，这里布尔值 true 存储为 1，false 存储为 0。更一般地，值为0的被认为是 false，只要值不是0都被认为是true。	

### 任务步骤

#### 任务1 按开始代码中的注释要求实现各函数功能	

```c
/*
  创建空队列
  如果没有分配空间则返回 NULL
*/
queue_t *q_new();

/*
  释放队列所用到的所有空间
  如果 q 为 NULL 则没有效果
*/
void q_free(queue_t *q);

/*
  在队首插入一个元素
  成功则返回 true
  q 为 NULL 或者 无法分配空间则返回 false 
  参数 s 指向待存储的字符串
  函数必须显式地分配空间并拷贝字符串到空间里面
 */
bool q_insert_head(queue_t *q, char *s);

/*
  在队尾插入一个元素
  成功则返回 true
  q 为 NULL 或者 无法分配空间则返回 false 
  参数 s 指向待存储的字符串
  函数必须显式地分配空间并拷贝字符串到空间里面
 */
bool q_insert_tail(queue_t *q, char *s);

/*
  移除队首元素Attempt to remove element from head of queue.
  成功则返回 true
  队列为 NULL 或者为 空 则返回 false 
  如果 sp 非空 并移除了一个元素，则将移除的字符串拷贝给 *sp
  (至多 bufsize-1 个字符, 加上1个 null 结束符（'\0',0）)
  元素和字符串使用的空间需要释放
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize);

/*
  返回队列的元素数量
  如果队列为 NULL 或者为 空 则返回 0
 */
int q_size(queue_t *q);

/*
  反转队列元素
  如果 q 为 NULL 则没有效果
  该函数不能直接或者间接调用其他函数为队列元素分配任何额外的空间
  (例如, 不能调用 q_insert_head, q_insert_tail, 或者 q_remove_head).
  应该是通过重排现有元素实现
 */
void q_reverse(queue_t *q);
```

包括修改 queue_t结构

```c
/* 队列结构 */
typedef struct {
    list_ele_t *head;  /* 链表 */
    /*
      你需要为这个结构添加额外的域来有效地实现 q_size 和 q_insert_tail
    */
} queue_t;
```

​	对于提供字符串做参数的函数，需要调用 malloc 创建和存储字符串的拷贝（记得包括结束符所需的空间），然后字符串从源拷贝到新分配的空间。到释放链表元素的时候，也要释放字符串的空间。你不能假定字符串的长度为一个固定的上限值，必须根据字符串的长度分配合适的空间。

​	q_insert_tail  和 q_size 要求你做些改变来满足性能要求。对于n个元素的队列来说，原生的实现可能需要 O(n) 步操作，现在要求在 O(1) 步实现，也即实现操作只需要固定数量的操作步骤，和队列大小无关。要实现新要求你可以在queue_t 数据结构添加额外的域并在链表元素插入、去除、反转时恰当地管理这些域的值。

​	你的程序将在超过1,000,000个元素的队列中测试。你会发现无法使用递归函数来操纵这么长的链表，因为这会需要太多的栈空间（课程讲到过程时会详细介绍）。因此，你需要使用循环遍历链表的元素。

#### 任务2 测试你的代码

​	通过 make 程序编译：

```shell
linux> make
```

​	如果没有错误，编译器会生成一个可执行程序 qtest，它为你提供了创建、修改、测试队列的命令行接口。使用文档可以通过运行程序并执行 help 命令实现：

```shell
linux> ./qtest
cmd>help
```

help的完整内容如下：

```xml
        #        ...            | 显式提示
        free                    | 删除队列
        help                    | 显式帮助文档
        ih       str [n]        | 在队首插入 str字符串 n 次 (缺省: n == 1)
        it       str [n]        | 在队尾插入 str字符串 n 次 (缺省: n == 1)
        log      file           | 复制输出到文件 file
        new                     | 创建新的队列
        option   [name val]     | 显示或者设置 选项 Option
        quit                    | 退出程序
        reverse                 | 反转队列
        rh       [str]          | 移除队首元素，可选与 字符串 str 比较
        rhq                     | 移除队首元素，不报告字符串值
        show                    | 显示队列内容
        size     [n]            | 计算队列大小 n 次 (缺省: n == 1)
        source   file           | 读取文件 file 的命令
        time     cmd arg ...    | 执行时间命令
Options:
        echo    1       是否回显命令
        error   5       达到多少错误数量后程序退出
        fail    30      允许队列操作返回 false 的次数
        length  1024    显示的字符串最大长度
        malloc  0       Malloc 失败概率百分比
        verbose 4       Verbosity level
```

​	在 traces 文件夹中有许多预先定义好的命令序列文件，下面是 traces/trace-eg.cmd 文件的内容：

```shell
# Demonstration of queue testing framework
# Use help command to see list of commands and options
# Initial queue is NULL.
show
# Create empty queue
new
# Fill it with some values.  First at the head
ih dolphin
ih bear
ih gerbil
# Now at the tail
it meerkat
it bear
# Reverse it
reverse
# See how long it is
size
# Delete queue.  Goes back to a NULL queue.
free
# Exit program
quit
```

​	要执行该命令序列文件，在终端使用如下命令：

```shell
linux> ./qtest -f traces/trace-eg.cmd
```

​	执行开始代码，你会发现很多操作都还没有恰当地实现（这是自然😁）

​	traces文件夹包括 15个 trace 文件， 命名形式 trace-k-cat.cmd,  k表示trace编号， cat指定测试的类别。每个trace文件内容和上面的例子类似，它们从正确性、鲁棒性、性能等各方面测试你的程序。你也可以编写自己的trace文件并用qtest测试和调试你的程序。

#### 任务3 评估

```shell
linux> make test
```

​	执行make test后，你的程序将被上面的15个trace文件全面评估。如果每个trace都执行正确，你将得到6到7个成绩点，以及最高100分的分数。

![image-20210823220609376](https://cnchen2000.oss-cn-shanghai.aliyuncs.com/img/image-20210823220609376.png)

​	加油实现吧，坚持就是胜利！

### Tips

- 测试是关键，尽量用好测试trace 文件。
- 如果测试未通过，可以通过 # ... 显示提示信息，查看错误描述。
- malloc 分配字符串空间时注意申请的空间大小， 复制字符串时注意最大bufsize，记得添加字符串结束符



------



## 附：课程实验环境配置

​	目前个人电脑上的操作系统普遍使用的是win10，本课程的实验环境推荐 WSL + MobaXterm + VS Code + Typora：

<img src="https://cnchen2000.oss-cn-shanghai.aliyuncs.com/img/image-20210823231007455.png" alt="image-20210823231007455" style="zoom:50%;" /> ![image-20210823231053223](https://cnchen2000.oss-cn-shanghai.aliyuncs.com/img/image-20210823231053223.png) <img src="https://cnchen2000.oss-cn-shanghai.aliyuncs.com/img/image-20210823231209137.png" alt="image-20210823231209137" style="zoom:50%;" /> <img src="https://cnchen2000.oss-cn-shanghai.aliyuncs.com/img/image-20210823231444396.png" alt="image-20210823231444396" style="zoom: 80%;" />

### WSL（Windows Subsystem for Linux）安装，参考[官方指南](https://docs.microsoft.com/zh-cn/windows/wsl/install-win10#manual-installation-steps)

#### 1. 启用WSL

​	需要先启用“适用于 Linux 的 Windows 子系统”可选功能，然后才能在 Windows 上安装 Linux 分发。以管理员身份打开 PowerShell 并运行：

```
dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
```

#### 2. 检查运行 WSL 2 的要求

​	若要更新到 WSL 2，需要运行 Windows 10。

- 对于 x64 系统：**版本 1903** 或更高版本，采用 **内部版本 18362** 或更高版本。
- 对于 ARM64 系统：**版本 2004** 或更高版本，采用 **内部版本 19041** 或更高版本。

​	如果运行的是 Windows 10 版本1903 或 1909，请在 Windows  菜单中打开“设置”，导航到“更新和安全性”，然后选择“检查更新”。 内部版本号必须是 18362.1049+ 或  18363.1049+，次要内部版本号需要高于 .1049。

#### 3. 启用虚拟机功能

​	安装 WSL 2 之前，必须启用“虚拟机平台”可选功能。 计算机需要[虚拟化功能](https://docs.microsoft.com/zh-cn/windows/wsl/troubleshooting#error-0x80370102-the-virtual-machine-could-not-be-started-because-a-required-feature-is-not-installed)才能使用此功能。以管理员身份打开 PowerShell 并运行：

```
dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
```

#### 4. 下载 Linux 内核更新包并安装

​	下载[适用于 x64 计算机的 WSL2 Linux 内核更新包](https://wslstorestorage.blob.core.windows.net/wslblob/wsl_update_x64.msi)，如果使用的是 ARM64 计算机，请下载 [ARM64 包](https://wslstorestorage.blob.core.windows.net/wslblob/wsl_update_arm64.msi)

#### 5. 将 WSL 2 设置为默认版本

​	打开 PowerShell，然后在安装新的 Linux 发行版时运行以下命令，将 WSL 2 设置为默认版本：

```
wsl --set-default-version 2
```

#### 6. 安装所选的 Linux 分发

打开 [Microsoft Store](https://aka.ms/wslstore)，并选择你偏好的 Linux 分发版[Ubuntu 20.04 LTS](https://www.microsoft.com/store/apps/9n6svws3rx71)，在分发版的页面中，选择“获取”。

首次启动新安装的 Linux 分发版时，将打开一个控制台窗口，系统会要求你等待一分钟或两分钟，以便文件解压缩并存储到电脑上。 未来的所有启动时间应不到一秒。然后，需要[为新的 Linux 分发版创建用户帐户和密码](https://docs.microsoft.com/zh-cn/windows/wsl/user-support)。

![Windows 控制台中的 Ubuntu 解包](https://docs.microsoft.com/zh-cn/windows/wsl/media/ubuntuinstall.png)

### MobaXterm安装

​	功能强大的终端工具，安装好WSL后，在官网直接下载[免费版本](https://mobaxterm.mobatek.net/download-home-edition.html)安装。安装好后打开缺省就有wsl会话，双击连接WSL。

### 编译环境安装

​	推荐通过 MobaXterm 连接WSL使用ubuntu，可以直接把代码文件夹拖动到MobaXterm左边显示的文件列表中，免去敲命令的麻烦。

​	更新源：

```
sudo apt-get update
```

​	安装编译器 gcc，调试器gdb，python（本实验中运行make test 需要用到）：

```
sudo apt install gcc-multilib
sudo apt install gdb
sudo apt install python
```

​	其余包根据需要自行选择安装

### VS Code 安装

​	VS Code 大家应该比较熟悉， 为使用WSL，在VS Code中安装 Remote - WSL 插件，连接WSL。

### Typora安装

​	实验文档撰写请使用[Typora](https://typora.io/#download)。
