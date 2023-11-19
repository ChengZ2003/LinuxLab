## ![img](https://cnchen2000.oss-cn-shanghai.aliyuncs.com/img/logo.png)      大数据专业Linux系统编程实验报告📝

| 专业                 | 学号         | 姓名         |
| :------------------- | ------------ | ------------ |
| 数据科学与大数据技术 | 210424071    | 江成         |
| **课程名称**         | **实验名称** | **完成日期** |
| Linux系统编程        | Shell-lab    | 2023.11.20   |

- [      大数据专业Linux系统编程实验报告📝](#------大数据专业linux系统编程实验报告)
  - [一 实验目标](#一-实验目标)
  - [二 实验要求](#二-实验要求)
  - [三 实验环境](#三-实验环境)
  - [四 前置知识](#四-前置知识)
    - [signal信号处理](#signal信号处理)
    - [waitpid函数](#waitpid函数)
    - [kill函数](#kill函数)
    - [sigprocmask函数](#sigprocmask函数)
  - [五 实验内容](#五-实验内容)
    - [1  sigint\_handler](#1--sigint_handler)
    - [2  sigtstp\_handler](#2--sigtstp_handler)
    - [3  sigchld\_handler](#3--sigchld_handler)
    - [4  waitfg](#4--waitfg)
    - [5  builtin\_cmd](#5--builtin_cmd)
    - [6  do\_bgfg](#6--do_bgfg)
    - [7  sigtstp\_handler](#7--sigtstp_handler)
    - [测试](#测试)
  - [六 实验心得](#六-实验心得)


### 一 实验目标

这项任务的目的是更加熟悉过程控制和信号的概念。为此，你将编写一个支持作业控制的简单 Unix shell 程序。

### 二 实验要求

查看 `tsh.c`（tiny shell）文件，你会发现它包含了一个简单 Unix shell 的功能框架。为了帮助你入门，我们已经实现了一些不太有趣的函数。你的任务是完成下面列出的其余空函数。我们在参考解决方案（包括大量注释）中列出了每个函数的大致代码行数，作为对你的合理性的检查。

- `eval`： 解析和解释命令行的主要例程。[70行］
- `builtin cmd`： 识别并解释内置命令：quit、fg、bg 和 jobs。[25行］
- `do bgfg`: 执行 bg 和 fg 内置命令。[50 行］
- `waitfg`：等待前台作业完成。[20 行］
- `sigchld handler`： 捕捉 SIGCHILD 信号。80 行］
- `sigint_handler`：捕获 SIGINT（ctrl-c）信号。[15 行］
- `sigtstp_handler`： 捕捉 SIGTSTP (ctrl-z) 信号。[15 行］

### 三 实验环境

- Vs Code、Github、Typora；
- 书：《深入理解计算机系统》
- 网站：
https://wdxtub.com/csapp/thick-csapp-lab-5/2016/04/16/
https://www.jianshu.com/p/7f5e78e83a0e

### 四 前置知识

#### signal信号处理

在终端里输入 `man 7 signal`命令， 查看信号的总览。

[![19b546bd72a06fce6c526dd478bd2c8d.png](https://s1.imagehub.cc/images/2023/11/19/19b546bd72a06fce6c526dd478bd2c8d.png)](https://www.imagehub.cc/image/1OPU5R)

当我使用 `man 7 signal` 查看信号的手册时，我获得了深入了解信号处理的信息。以下是我发现对CSAPP的shell lab实验可能有帮助的关键点：

1. **信号列表：** 我了解到有各种不同的信号，每个信号都有一个唯一的标识符，如`SIGHUP`、`SIGINT`、`SIGSEGV`等。这些信号在Shell编程中是非常重要的，因为它们与进程的不同状态和事件相关联。
2. **默认行为：** 对于每个信号，手册详细描述了默认的信号处理行为。例如，`SIGINT` 的默认行为是终止进程。这对于我理解在不同情况下发生信号时系统的默认行为很有帮助。
3. **信号处理函数：** 手册介绍了如何设置信号处理函数，这对于在Shell中处理异步信号是至关重要的。我学到了可以使用 `signal` 函数或更为灵活的 `sigaction` 函数来注册信号处理函数。
4. **信号的产生和传递：** 手册解释了信号是如何产生和传递的，这有助于我理解在不同情况下何时会发生信号。这对于正确地设计Shell程序以处理异步事件非常关键。
5. **可靠信号处理：** 我了解到一些信号可以被设置为“可靠”处理，这意味着如果在信号处理函数执行期间再次发生该信号，系统会等待处理函数完成后再调用处理函数。这对于确保信号处理的可靠性和一致性很重要。

#### waitpid函数

使用`man waitpid`查看`wait`的手册时，映入眼帘的就是`wait,waitpid,waitid`的定义：`wait for process to change state`，即 `等待进程改变状态`。在这里，“状态”指的是进程可能处于的几种状态，包括 `terminated`（结束）和 `stopped`（终止）。
[![74535a8514975cb39a40e0974681e5e6.png](https://s1.imagehub.cc/images/2023/11/19/74535a8514975cb39a40e0974681e5e6.png)](https://www.imagehub.cc/image/1Os2gT)
具体而言，我找到了 `waitpid` 函数的原型：

```c
pid_t waitpid(pid_t pid, int *wstatus, int options);
```

这个函数的目的是等待特定进程的状态发生变化。其中参数的含义如下：

- `pid`：要等待的进程的进程ID，如果为负值，则等待任何子进程的状态发生变化。
- `wstatus`：用于存储子进程的退出状态或终止信号的整数指针。
- `options`：用于指定等待的行为，例如是否阻塞等。

通过调用 `waitpid`，可以使当前进程暂停执行，直到指定的子进程改变其状态。这在处理子进程的退出状态时是非常有用的，特别是在编写Shell或类似的进程控制程序时。

#### kill函数

[![26369db6b06ecf83016bd22dc99496e7.png](https://s1.imagehub.cc/images/2023/11/19/26369db6b06ecf83016bd22dc99496e7.png)](https://www.imagehub.cc/image/1OsE37)

使用`man kill`查看`kill`的手册时，可以看到：`kill`的定义为`send a signal to a process`，即`发送一个信号給进程`。并不是字面意义上的`杀死某个进程`。`kill`向进程发送的默认信号是`TERM`，就是结束某个进程，但它也可以发送别的信号，例如`HUP, INT, KILL, STOP, CONT, and 0`。

#### sigprocmask函数

[![7fdca3ad846279db74ff4bb53accca7d.png](https://s1.imagehub.cc/images/2023/11/19/7fdca3ad846279db74ff4bb53accca7d.png)](https://www.imagehub.cc/image/1OsZKR)

通过查看 `sigprocmask` 函数的手册，我可以看到，这个函数是用于`检查并改变被阻塞的信号的`。`sigprocmask` 函数的原型如下：

```c
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
```

其中，第一个参数 `how` 可以取以下几种值：

- `SIG_BLOCK`：将指定的信号集添加到当前的信号屏蔽字（被阻塞的信号集）中。
- `SIG_UNBLOCK`：从当前的信号屏蔽字中移除指定的信号集。
- `SIG_SETMASK`：将当前的信号屏蔽字设置为指定的信号集。

这个函数的作用是在进程中管理信号的阻塞状态。通过调用 `sigprocmask`，可以控制哪些信号在进程执行期间被阻塞，以及哪些信号不被阻塞。

此外，函数的第二个参数 `set` 是一个指向要操作的信号集的指针，而第三个参数 `oldset` 则是一个指向保存之前信号屏蔽字的指针。通过传递这两个参数，可以获取或修改当前的信号屏蔽字，从而实现对信号的灵活管理。

这对于需要精细控制信号处理的场景，比如在信号处理程序执行期间阻塞某些信号以避免中断，是非常有用的。





### 五 实验内容

#### 1  sigint_handler

- 任务描述

  `sigint_handler`：捕获 SIGINT（ctrl-c）信号。[15 行］

- 实现方法

  1. **获取前台作业的进程ID：** 使用 `fgpid` 函数获取当前前台作业的进程ID。
  2. **检查进程ID的有效性：** 检查获取的进程ID是否有效（非零）。如果为零，表示没有前台作业需要处理，直接返回。
  3. **向进程组发送SIGINT信号：** 使用 `kill` 函数向具有给定进程ID的进程组发送 `SIGINT` 信号。这里使用负号 `-` 来将信号发送给整个进程组，以终止前台作业中的所有进程。
  4. **返回：** 从信号处理程序返回。

  ```c
  void sigint_handler(int sig)
  {
      // 使用'fgpid'函数获取前台作业的进程ID
      pid_t pid = fgpid(jobs);
  
      // 检查获取的进程ID是否有效（不等于0）
      if (pid != 0)
      {
          // 向具有给定进程ID的进程组发送SIGINT信号
          // 使用负号（-）发送信号到整个进程组
          // 这是为了终止前台作业中的所有进程
          kill(-pid, sig);
      }
  
      // 从信号处理程序返回
      return;
  }
  ```

整个思路就是通过获取前台作业的进程ID，然后向该进程ID所在的进程组发送 `SIGINT` 信号，从而实现终止前台作业中的所有进程。这样，当用户按下 `Ctrl+C` 时，会触发 `SIGINT` 信号，进而调用该处理函数，终止前台作业中的所有相关进程。

#### 2  sigtstp_handler

- 任务描述

  `sigtstp_handler`： 捕捉 SIGTSTP (ctrl-z) 信号。[15 行］

- 实现方法

  1. **获取前台作业的进程ID：** 使用 `fgpid` 函数获取当前前台作业的进程ID。
  2. **检查进程ID的有效性：** 检查获取的进程ID是否有效（非零）。如果为零，表示没有前台作业需要处理，直接返回。
  3. **向进程组发送SIGTSTP信号：** 使用 `kill` 函数向具有给定进程ID的进程组发送 `SIGTSTP` 信号。这里使用负号 `-` 来将信号发送给整个进程组，以停止前台作业中的所有进程。
  4. **返回：** 从信号处理程序返回。

  ```c
  void sigtstp_handler(int sig)
  {
      // 使用'fgpid'函数获取前台作业的进程ID
      pid_t pid = fgpid(jobs);
  
      // 检查获取的进程ID是否有效（不等于0）
      if (pid != 0)
      {
          // 向具有给定进程ID的进程组发送SIGTSTP信号
          // 使用负号（-）发送信号到整个进程组
          // 这是为了停止前台作业中的所有进程
          kill(-pid, sig);
      }
  
      // 从信号处理程序返回
      return;
  }
  ```

整个思路就是通过获取前台作业的进程ID，然后向该进程ID所在的进程组发送 `SIGTSTP` 信号，从而实现停止前台作业中的所有进程。这样，当用户按下 `Ctrl+Z` 时，会触发 `SIGTSTP` 信号，进而调用该处理函数，停止前台作业中的所有相关进程。

#### 3  sigchld_handler

- 任务描述

  `sigchld handler`： 捕捉 SIGCHILD 信号。80 行］

- 实现方法

  1. **循环等待所有子进程的状态变化：** 使用 `waitpid` 函数以非阻塞的方式循环等待所有子进程的状态变化。`fgpid(jobs)` 用于获取前台作业的进程ID。
  2. **处理被暂停的子进程：** 如果子进程被暂停（`WIFSTOPPED(status)`为真），则修改相应作业的状态为停止（`ST`）。然后，获取作业ID，并打印相关信息，包括作业ID、进程ID和导致停止的信号。
  3. **处理被信号终止的子进程：** 如果子进程被信号终止（`WIFSIGNALED(status)`为真），则获取作业ID，并打印相关信息，包括作业ID、进程ID和导致终止的信号。然后，使用 `deletejob` 函数删除该作业。
  4. **处理正常退出的子进程：** 如果子进程正常退出（`WIFEXITED(status)`为真），则使用 `deletejob` 函数删除该作业。
  5. **循环直到没有子进程状态需要处理：** 继续循环，直到 `waitpid` 不再返回任何子进程状态。这是通过检查 `pid > 0` 条件来实现的。
  6. **返回：** 从信号处理程序返回

  ```c
  void sigchld_handler(int sig)
  {
      int status;
      pid_t pid;
  
      // 循环等待所有子进程的状态变化
      while ((pid = waitpid(fgpid(jobs), &status, WNOHANG | WUNTRACED)) > 0)
      {
          if (WIFSTOPPED(status))
          {
              // 如果子进程被暂停，则修改作业状态为停止
              getjobpid(jobs, pid)->state = ST;
  
              // 获取作业ID并打印相关信息
              int jid = pid2jid(pid);
              printf("Job [%d] (%d) Stopped by signal %d\n", jid, pid, WSTOPSIG(status));
          }
          else if (WIFSIGNALED(status))
          {
              // 如果子进程被信号终止，则删除作业
              int jid = pid2jid(pid);
              printf("Job [%d] (%d) terminated by signal %d\n", jid, pid, WTERMSIG(status));
              deletejob(jobs, pid);
          }
          else if (WIFEXITED(status))
          {
              // 如果子进程正常退出，则删除作业
              deletejob(jobs, pid);
          }
      }
  
      // 从信号处理程序返回
      return;
  }
  ```

整体思路是通过循环等待所有子进程的状态变化，并根据不同的状态进行相应的处理。如果子进程被暂停，修改作业状态并打印相关信息；如果子进程被信号终止，打印相关信息并删除作业；如果子进程正常退出，直接删除作业。最后，循环直到没有子进程状态需要处理。

#### 4  waitfg

- 任务描述

  `waitfg`：等待前台作业完成。[20 行］

- 实现方法

  1. **获取具有给定进程ID的作业信息：** 使用 `getjobpid` 函数获取具有给定进程ID的作业信息。
  2. **检查进程ID的有效性：** 检查获取的进程ID是否有效（非零）。如果为零，表示没有前台作业需要等待，直接返回。
  3. **等待前台作业结束：** 进入一个循环，持续检查前台作业的进程ID是否仍然等于给定的进程ID。这是通过使用 `fgpid(jobs)` 函数来获取当前前台作业的进程ID，并与传入的进程ID进行比较。
  4. **返回：** 一旦前台作业的进程ID不再等于给定的进程ID，跳出循环，从函数返回。

  ```c
  void waitfg(pid_t pid)
  {
      struct job_t *job;
  
      // 获取具有给定进程ID的作业信息
      job = getjobpid(jobs, pid);
  
      // 检查进程ID是否有效
      if (pid == 0)
      {
          return;
      }
  
      // 如果作业信息存在
      if (job != NULL)
      {
          // 等待前台作业结束
          while (pid == fgpid(jobs))
          {
              // 空循环，等待前台作业结束
              // 此循环通过持续检查前台作业的进程ID是否仍然等于给定的进程ID来等待作业结束
          }
      }
  
      // 从函数返回
      return;
  }
  ```

整个实现思路就是通过不断检查前台作业的进程ID是否仍然等于给定的进程ID，直到前台作业结束。这样，当 `waitfg` 被调用时，它会一直等待前台作业结束后才返回。这通常用于确保 shell 在前台作业执行完毕之前不会继续执行其他命令。

#### 5  builtin_cmd

- 任务描述

  `builtin cmd`： 识别并解释内置命令：quit、fg、bg 和 jobs。[25行］

- 实现方法

  1. **"quit" 命令：** 如果输入的命令是 "quit"，则调用 `exit(0)` 来退出 shell。
  2. **后台执行命令判断：** 如果命令以 "&" 结尾，表示要在后台执行。在 shell 中，"&" 通常用于表示后台执行。返回 1，表示这是一个后台执行的命令。
  3. **"jobs" 命令：** 如果命令是 "jobs"，调用 `listjobs` 函数列出当前运行的作业信息。然后返回 1，表示这是一个 "jobs" 命令。
  4. **"bg" 或 "fg" 命令：** 如果命令是 "bg" 或 "fg"，调用 `do_bgfg` 函数进行作业控制操作。这两个命令通常用于将作业移至后台或前台执行。然后返回 1，表示这是一个 "bg" 或 "fg" 命令。
  5. **其他命令：** 如果命令不是内置命令，则返回 0，表示这不是一个内置命令。这将提示 shell 执行外部命令的逻辑。

  ```c
  int builtin_cmd(char **argv)
  {
      // 如果命令是 "quit"，退出 shell
      if (!strcmp(argv[0], "quit"))
      {
          exit(0);
      }
      // 如果命令以 "&" 结尾，表示后台执行，返回 1
      else if (!strcmp("&", argv[0]))
      {
          return 1;
      }
      // 如果命令是 "jobs"，列出当前运行的作业信息，然后返回 1
      else if (!strcmp("jobs", argv[0]))
      {
          listjobs(jobs);
          return 1;
      }
      // 如果命令是 "bg" 或 "fg"，调用 do_bgfg 函数进行作业控制操作，然后返回 1
      else if (!strcmp("bg", argv[0]) || !strcmp("fg", argv[0]))
      {
          // 调用 do_bgfg 函数进行作业控制操作
          do_bgfg(argv);
          return 1;
      }
  
      // 如果命令不是内置命令，返回 0
      return 0;
  }
  ```

这个函数的目的是判断输入的命令是否是内置命令，如果是，则执行相应的操作，并返回一个标志来指示 shell 应该如何处理该命令。

#### 6  do_bgfg

- 任务描述

  `do bgfg`: 执行 bg 和 fg 内置命令。[50 行］

- 实现方法

  1. **获取参数：** 获取 `bg` 或 `fg` 命令中的参数，即作业号（以 "%" 开头）或进程号。
  2. **参数有效性检查：** 检查参数是否存在。如果参数不存在，则输出错误信息并返回。
  3. **作业号和进程号的处理：**
     - 如果参数以 "%" 开头，表示为作业号（jid）。使用 `getjobjid` 函数获取指定作业的信息，并获取作业的进程ID（pid）。
     - 如果参数为数字，表示为进程号（pid）。使用 `getjobpid` 函数获取指定进程的作业信息。
  4. **参数格式检查：** 检查参数格式是否正确。如果不正确，输出错误信息并返回。
  5. **发送 SIGCONT 信号：** 使用 `kill` 函数向指定进程组发送 `SIGCONT` 信号，以恢复作业的执行。
  6. **执行 fg 命令：** 如果是 `fg` 命令，将作业状态设置为前台运行（`FG`），然后调用 `waitfg` 函数等待前台作业结束。
  7. **执行 bg 命令：** 如果是 `bg` 命令，打印后台运行的作业信息，并将作业状态设置为后台运行（`BG`）。

  ```c
  void do_bgfg(char **argv)
  {
      struct job_t *job;
      char *tmp;
      int jid;
      pid_t pid;
  
      // 获取命令中的参数
      tmp = argv[1];
  
      // 如果参数不存在
      if(tmp == NULL) {
          printf("%s command requires PID or %%jobid argument\n", argv[0]);
          return;
      }
  
      // 如果是作业号（jid）
      if(tmp[0] == '%') {
          jid = atoi(&tmp[1]);
          // 获取指定作业
          job = getjobjid(jobs, jid);
          // 如果作业不存在
          if(job == NULL){
              printf("%s: No such job\n", tmp);
              return;
          } else {
              // 获取该作业的进程ID，以备后续使用
              pid = job->pid;
          }
      }
      // 如果是进程ID（pid）
      else if(isdigit(tmp[0])) {
          // 获取进程ID
          pid = atoi(tmp);
          // 获取指定进程的作业信息
          job = getjobpid(jobs, pid);
          // 如果作业不存在
          if(job == NULL){
              printf("(%d): No such process\n", pid);
              return;
          }
      }
      // 如果参数格式不正确
      else {
          printf("%s: argument must be a PID or %%jobid\n", argv[0]);
          return;
      }
  
      // 向指定进程组发送 SIGCONT 信号，以恢复作业执行
      if (kill(-pid, SIGCONT) < 0) {
          perror("kill");
          return;
      }
  
      // 如果是 fg 命令
      if(!strcmp("fg", argv[0])) {
          // 将作业状态设置为前台运行
          job->state = FG;
          // 等待前台作业结束
          waitfg(job->pid);
      }
      // 如果是 bg 命令
      else {
          // 打印后台运行的作业信息
          printf("[%d] (%d) %s", job->jid, job->pid, job->cmdline);
          // 将作业状态设置为后台运行
          job->state = BG;
      }
  }
  ```

#### 7  sigtstp_handler

- 任务描述

  `sigtstp_handler`： 捕捉 SIGTSTP (ctrl-z) 信号。[15 行］

- 实现方法

  1. **参数解析：**
     使用 `parseline` 函数解析命令行，填充 `argv` 数组，并返回是否在后台运行的标志 `bg`。
  2. **内置命令处理：**
     调用 `builtin_cmd` 函数检查命令是否是内置命令，如果是，则执行相应的内置命令并返回。
  3. **信号阻塞：**
     使用 `sigset_t` 类型的信号集合 `mask` 阻塞 `SIGCHLD` 信号，防止在添加作业前子进程就绪导致的竞态条件。
  4. **创建子进程：**
     使用 `fork` 函数创建一个新的子进程。
  5. **子进程操作：**
     如果是子进程，解除对 `SIGCHLD` 信号的阻塞。
     将子进程设置为新的进程组的组长。
     执行命令，如果失败则输出错误信息并退出。
  6. **父进程操作：**
     如果是父进程，添加作业到作业列表。
     解除对 `SIGCHLD` 信号的阻塞。
     根据命令在前台还是后台运行，采取相应的等待或打印操作。

  ```c
  void sigtstp_handler(int sig)
  {
      // 使用'fgpid'函数获取前台作业的进程ID
      pid_t pid = fgpid(jobs);
  
      // 检查获取的进程ID是否有效（不等于0）
      if (pid != 0)
      {
          // 向具有给定进程ID的进程组发送SIGTSTP信号
          // 使用负号（-）发送信号到整个进程组
          // 这是为了停止前台作业中的所有进程
          kill(-pid, sig);
      }
  
      // 从信号处理程序返回
      return;
  }
  ```

#### 测试

- 测试结果

  ***trace01***
  [![b825c1ee80671e3ea64b9955c573afbc.png](https://s1.imagehub.cc/images/2023/11/19/b825c1ee80671e3ea64b9955c573afbc.png)](https://www.imagehub.cc/image/1OEAph)
  ***trace02***
  [![f16bc81b38fa19055ea3db318a76d6ac.png](https://s1.imagehub.cc/images/2023/11/19/f16bc81b38fa19055ea3db318a76d6ac.png)](https://www.imagehub.cc/image/1OESbG)
  ***trace03***
  [![e10990d1373e2d6a1e3e17b55e705631.png](https://s1.imagehub.cc/images/2023/11/19/e10990d1373e2d6a1e3e17b55e705631.png)](https://www.imagehub.cc/image/1OEYEr)
  ***trace04***
  [![077619e7cd70e7433b3d558a5bcc505a.png](https://s1.imagehub.cc/images/2023/11/19/077619e7cd70e7433b3d558a5bcc505a.png)](https://www.imagehub.cc/image/1OGduv)
  ***trace05***
  [![cd21bd082e7e1b513e1a2a9d3e7c5789.png](https://s1.imagehub.cc/images/2023/11/19/cd21bd082e7e1b513e1a2a9d3e7c5789.png)](https://www.imagehub.cc/image/1OGfSt)
  ***trace06***
  [![e634b426e74a864b114b1e45944172fd.png](https://s1.imagehub.cc/images/2023/11/19/e634b426e74a864b114b1e45944172fd.png)](https://www.imagehub.cc/image/1OG1BS)
  ***trace07***
  [![738ffb13d69055e369e756f430271deb.png](https://s1.imagehub.cc/images/2023/11/19/738ffb13d69055e369e756f430271deb.png)](https://www.imagehub.cc/image/1OGbiL)
  ***trace08***
  [![36bf47f32ce6a2b4e379977c621a1770.png](https://s1.imagehub.cc/images/2023/11/19/36bf47f32ce6a2b4e379977c621a1770.png)](https://www.imagehub.cc/image/1OGC7B)
  ***trace09***
  [![77125fc7e8ad3162f19d3296cc0e9c24.png](https://s1.imagehub.cc/images/2023/11/19/77125fc7e8ad3162f19d3296cc0e9c24.png)](https://www.imagehub.cc/image/1OGMeq)
  ***trace10***
  [![29bb7ea22c3b32965f5173186a5f9f86.png](https://s1.imagehub.cc/images/2023/11/19/29bb7ea22c3b32965f5173186a5f9f86.png)](https://www.imagehub.cc/image/1OGgHU)
  ***trace11***
  [![4cc5304f8d1480c9065499fdbe1b4d80.png](https://s1.imagehub.cc/images/2023/11/19/4cc5304f8d1480c9065499fdbe1b4d80.png)](https://www.imagehub.cc/image/1OGwo0)
  ***trace12***
  [![c7e9eae5754830790317f5457beb7039.png](https://s1.imagehub.cc/images/2023/11/19/c7e9eae5754830790317f5457beb7039.png)](https://www.imagehub.cc/image/1OGlbz)
  ***trace13***
  [![a31ab78b46c774e7e0355160af746cd3.png](https://s1.imagehub.cc/images/2023/11/19/a31ab78b46c774e7e0355160af746cd3.png)](https://www.imagehub.cc/image/1OG0Ej)
  [![5d55b081d58439855a66fbdd73a61fb8.png](https://s1.imagehub.cc/images/2023/11/19/5d55b081d58439855a66fbdd73a61fb8.png)](https://www.imagehub.cc/image/1OG3vg)
  ***trace14***
  [![c64fdfe04fda7eb95f57b48b0dd48f23.png](https://s1.imagehub.cc/images/2023/11/19/c64fdfe04fda7eb95f57b48b0dd48f23.png)](https://www.imagehub.cc/image/1OG6So)
  ***trace15***
  [![9f1beb9b7832bf0d0369d56516600092.png](https://s1.imagehub.cc/images/2023/11/19/9f1beb9b7832bf0d0369d56516600092.png)](https://www.imagehub.cc/image/1OGBNR)
  ***trace16***
  [![e6cc5c6ce9934c524027734b9e6abd05.png](https://s1.imagehub.cc/images/2023/11/19/e6cc5c6ce9934c524027734b9e6abd05.png)](https://www.imagehub.cc/image/1OGNkb)

- 实现过程

  下面是此任务时间线：

​			[![3e983c332aac794685627d620fb82a7f.png](https://s1.imagehub.cc/images/2023/11/19/3e983c332aac794685627d620fb82a7f.png)](https://www.imagehub.cc/image/1OG9kS)

### 六 实验心得

在完成这个实验的过程中，我对操作系统中的作业控制和信号处理有了更深入的理解。以下是我在实验中获得的一些心得体会：

1. **信号处理的重要性：** 通过实现信号处理函数，特别是 `SIGCHLD`、`SIGINT`、`SIGTSTP` 的处理函数，我更加理解信号在操作系统中的重要性。合理地处理信号，特别是与作业控制相关的信号，是实现一个稳定、可靠的 shell 的关键。
2. **作业控制的复杂性：** 实现作业控制并确保正确地管理前台作业和后台作业之间的状态转换是一项相对复杂的任务。需要谨慎处理作业的添加、删除以及状态的变化，以确保 shell 的行为符合用户的预期。
3. **细节决定成败：** 实验中有许多细节需要处理，如处理作业状态、正确地解析命令行、处理内置命令等。这些细节的处理直接影响了 shell 的正确性和稳定性。在实现过程中，仔细审查每个函数、每个逻辑分支，确保它们能够正确地完成预期的功能。
4. **代码的模块化设计：** 在实验中，采用模块化的设计方式对代码进行组织，使得各个功能块分离开来，便于理解和维护。函数的独立性和可复用性对于整个项目的可维护性非常重要。
5. **调试的重要性：** 实现一个功能完善的 shell 是一个复杂的任务，难免会出现各种 bug 和问题。在调试过程中，通过运用调试工具、输出调试信息以及逐步验证代码，我成功地解决了一些潜在问题，提高了代码的健壮性。

实验让我深刻认识到我们每天都在使用 Shell，而它的一个最基础的作业控制就要考虑如此多的因素：回收进程、避免竞争等。读 CSAPP 第 8 章时，我对很多地方都不甚理解。通过亲自编写多线程的代码、考虑如何避免竞争、不断调试由于并发带来的 BUG，我对书上的例程一下子就领悟了，感谢 CMU 提供的实验材料！

这是我计算机生涯中第一次真正接触并发，所以很多地方只会照猫画虎，希望以后我的水平能不断提高，甚至能写出像 zsh 这样强大的终端工具。

总的来说，这个实验使我更深入地了解了操作系统中的一些核心概念和技术，并提升了我的编程和调试能力。通过不断克服问题和挑战，我更加自信地处理了复杂的系统编程任务。





