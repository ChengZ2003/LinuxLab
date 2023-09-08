## ![img](https://cnchen2000.oss-cn-shanghai.aliyuncs.com/img/logo.png)      大数据专业Linux系统编程 实验报告📝

| 专业                 | 学号                 | 姓名         |
| :------------------- | -------------------- | ------------ |
| 数据科学与大数据技术 | 2104240713           | 江成         |
| **课程名称**         | **实验名称**         | **完成日期** |
| Linux系统编程        | Lab0 cprogramminglab | 2023.09.08   |



- [      大数据专业Linux系统编程 实验报告📝](#------大数据专业linux系统编程-实验报告)
  - [一 实验目标](#一-实验目标)
  - [二 实验要求](#二-实验要求)
  - [四 实验内容](#四-实验内容)
    - [1. 修改 queue.h 结构](#1-修改-queueh-结构)
    - [2. 实现 queue.c 中的函数](#2-实现-queuec-中的函数)
  - [五 实验心得](#五-实验心得)


### 一 实验目标

- C的显式内存管理。
- 通过指针创建和操作数据。
- 处理字符串。
- 在数据结构中存储冗余信息以提高关键操作的性能。
- 实现健壮的代码能处理非法的参数，包括NULL指针。

### 二 实验要求

- 个人独立完成，积极动手编程；
- 鼓励与同学交流，但不能抄袭源码；
- 能完成实验说明文档的各个步骤并撰写此实验报告；
- 能演示实验过程并阐述功能的主要代码模块。
- 实验报告请突出自己的**想法**、**做法**、**心得体会**；

- ## 三、实验环境

- 操作系统：Linux
- 编程工具：Visual Studio Code
- 文档编辑器：Typora
- 网站：https://en.cppreference.com/w/c/string/byte/strcpy



### 四 实验内容

#### 1. 修改 queue.h 结构

- **任务描述**：在头文件 `queue.h` 中修改 `queue_t` 结构，并添加必要的注释来说明修改的原因。
- **实现方法**：具体实现如下所示，修改原因见注释。

- 

  ```c
  /* 队列结构 */
  typedef struct
  {
    list_ele_t *head; /* 链表 */
    /*
      你需要为这个结构添加额外的域来有效地实现 q_size 和 q_insert_tail
    */
    list_ele_t *tail; /*队尾指针*/
    int qSize;        /*当前队列中元素的个数*/
  } queue_t;
  ```


#### 2. 实现 queue.c 中的函数

- 任务描述

  - 完全实现以下函数，包括非法操作的处理（例如，从空队列或者NULL指针中删除元素操作），返回值等说明：
    - `q_new`: 创建新的空队列。
    - `q_free`: 释放队列使用的所有内存。
    - `q_insert_head`: 将新元素插入队首 (LIFO)。
    - `q_insert_tail`: 将新元素插入队尾 (FIFO)。
    - `q_remove_head`: 移除队首元素。
    - `q_size`: 计算队列元素数量。
    - `q_reverse`: 将队列元素顺序反转。注意函数不能直接或者间接调用其他函数为队列元素分配任何额外的空间，通过重排现有元素实现。
  
- 实现方法与步骤

  **q_new**

  ```c
  /*
    创建空队列
    如果没有分配空间则返回 NULL
   */
  queue_t *q_new()
  {
    // 分配内存以存储队列结构
    queue_t *q = malloc(sizeof(queue_t));
  
    // 检查内存分配是否成功
    if (!q)
      return NULL; // 如果 malloc 返回 NULL，则代表申请内存失败，因此返回 NULL 表示创建队列失败
  
    // 初始化队列元素
    q->head = NULL; // 队列头指针初始化为 NULL，表示队列为空
    q->tail = NULL; // 队列尾指针初始化为 NULL，表示队列为空
    q->qSize = 0;   // 队列大小计数器初始化为 0，表示队列中没有元素
  
    // 返回指向新创建队列的指针
    return q;
  }
  ```

  函数首先使用 `malloc` 分配足够的内存来存储队列结构，并检查内存分配是否成功。如果内存分配失败，函数返回 `NULL`，表示创建队列失败。然后，初始化队列的头部指针、尾部指针和队列大小计数器。最后，函数返回指向新创建队列的指针。

**q_free**

```c
/*
  释放队列所用到的所有空间
  如果 q 为 NULL 则没有效果
*/
void q_free(queue_t *q)
{
  /* 检查队列指针是否为空，如果为空则无需释放 */
  if (!q)
    return;

  /* 释放队列结构 */
  for (int i = 0; i < q->qSize; i++)
  {
    list_ele_t *temp = q->head; // 临时指针指向队列头部

    q->head = q->head->next; // 移动队列头部指针到下一个元素

    if (temp->value)
    {
      free(temp->value); // 如果元素中的字符串非空，则释放字符串内存
    }

    free(temp); // 释放当前元素节点的内存
  }

  free(q); // 释放队列结构的内存
}
```

具体的步骤如下：

1. 首先，检查 `q` 是否为空，如果为空则直接返回，无需进行释放操作。
2. 进入循环，循环次数为队列的大小 `q->qSize`，每次迭代处理一个元素节点。
3. 在循环内部，创建一个临时指针 `temp`，将其指向队列头部（`q->head`）。
4. 移动队列头部指针 `q->head` 到下一个元素，以便在下一次迭代中释放下一个节点。
5. 检查 `temp` 中的 `value` 字段是否为空，如果非空，说明元素节点中存储了一个字符串，需要释放字符串的内存空间。
6. 释放当前元素节点 `temp` 所占用的内存空间。
7. 循环继续，直到释放完所有的元素节点。
8. 最后，释放队列结构 `q` 所占用的内存空间。

**q_insert_head**

```c
/*
  在队首插入一个元素
  成功则返回 true
  q 为 NULL 或者 无法分配空间则返回 false
  参数 s 指向待存储的字符串
  函数必须显式地分配空间并拷贝字符串到空间里面
 */
bool q_insert_head(queue_t *q, char *s)
{
  /* 如果 q 为 NULL 该怎么办? */
  if (!q)
    return false; /* 如果队列指针为空，则队列不存在，返回 false */

  list_ele_t *newh = (list_ele_t *)malloc(sizeof(list_ele_t)); /* 为新节点分配内存 */
  if (!newh)
    return false; /* 如果申请内存失败，也返回 false */

  /* 别忘了为字符串分配空间并拷贝它 */
  newh->value = (char *)malloc(strlen(s) + 1);
  /* 如果其中一个 malloc 调用返回 NULL 该怎么办? */
  if (!newh->value)
  {
    free(newh);   // 释放新节点内存
    return false; // 如果字符串内存申请失败，返回 false
  }
  /* 将 s 拷贝到申请的内存中 */
  strcpy(newh->value, s);

  // 如果队列当前为空
  if (q->qSize == 0)
  {
    q->head = newh;    // 设置队列头部指针指向新节点
    q->tail = newh;    // 设置队列尾部指针指向新节点
    newh->next = NULL; // 新节点的下一个指针为空，因为它是队列的最后一个元素
  }
  else
  {
    newh->next = q->head; // 新节点的下一个指针指向当前队列头部元素
    q->head = newh;       // 更新队列头部指针，使其指向新节点，成为新的队首元素
  }

  q->qSize++; // 增加队列的大小计数

  return true; // 插入成功，返回 true
}
```

函数首先检查传入的队列指针 `q` 是否为空，如果为空，则队列不存在，直接返回 false。然后分配内存用于新的队列元素，并为字符串分配内存，将传入的字符串 `s` 复制到新分配的内存中。接着，根据队列的当前状态，将新元素插入到队列的头部，并更新队列的大小计数。最后，函数返回 true 表示插入成功，或者返回 false 表示插入失败（内存分配失败）。

**q_insert_tail**

```c
/*
  在队尾插入一个元素
  成功则返回 true
  q 为 NULL 或者 无法分配空间则返回 false
  参数 s 指向待存储的字符串
  函数必须显式地分配空间并拷贝字符串到空间里面
 */
bool q_insert_tail(queue_t *q, char *s)
{
  /* 检查队列指针是否为 NULL */
  if (q == NULL)
  {
    return false; // 如果队列指针为空，则返回 false，表示插入失败
  }

  /* 为新元素节点分配内存 */
  list_ele_t *newt = (list_ele_t *)malloc(sizeof(list_ele_t));
  if (newt == NULL)
  {
    return false; // 如果内存分配失败，返回 false，表示插入失败
  }

  /* 为新元素节点中的字符串分配内存并拷贝字符串内容 */
  newt->value = (char *)malloc(strlen(s) + 1);
  if (newt->value == NULL)
  {
    free(newt);   // 如果字符串内存分配失败，释放新元素节点的内存
    return false; // 返回 false，表示插入失败
  }
  strcpy(newt->value, s);

  /* 插入新元素节点到队列尾部 */
  if (q->tail == NULL) // 当前队列中没有元素
  {
    q->head = newt; // 设置队列头部和尾部都指向新元素节点
    q->tail = newt;
  }
  else
  {
    q->tail->next = newt; // 将新元素节点连接到当前队尾元素的后面
    q->tail = newt;       // 更新队尾指针为新元素节点
  }
  newt->next = NULL; // 设置新元素节点的下一个指针为 NULL，表示它是队尾元素

  q->qSize++; // 增加队列的大小计数

  return true; // 插入成功，返回 true
}
```

具体原理同上，先检查传入的队列指针 `q` 是否为空，如果为空，则队列不存在，直接返回 false。然后分配内存用于新的队列元素，并为字符串分配内存，将传入的字符串 `s` 复制到新分配的内存中。接着，根据队列的当前状态，将新元素插入到队列的尾部，并更新队列的大小计数。最后，函数返回 true 表示插入成功，或者返回 false 表示插入失败（内存分配失败）。

**q_remove_head**

```c
/*
  移除队首元素Attempt to remove element from head of queue.
  成功则返回 true
  队列为 NULL 或者为空则返回 false
  如果 sp 非空并移除了一个元素，则将移除的字符串拷贝给 *sp
  (至多 bufsize-1 个字符, 加上1个 null 结束符（'\0',0）)
  元素和字符串使用的空间需要释放
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
  /* 检查队列指针是否为空或队列是否为空 */
  if (!q || !q->qSize)
  {
    return false; // 如果队列指针为空或队列为空，返回 false，表示移除失败
  }

  list_ele_t *oldhead = q->head; // 保存指向队首元素的指针
  q->head = q->head->next;       // 更新队首指针，将其指向下一个元素
  oldhead->next = NULL;          // 将被移除元素的下一个指针置为 NULL，断开与队列的联系

  if (sp != NULL)
  {
    // 如果 sp 不为空，将被移除的字符串拷贝给 *sp
    strncpy(sp, oldhead->value, bufsize - 1); // 使用 strncpy 防止字符串溢出
    sp[bufsize - 1] = '\0';                   // 手动添加字符串的结束符
  }

  if (oldhead->value != NULL)
  {
    free(oldhead->value); // 释放被移除元素的字符串内存
  }

  free(oldhead); // 释放被移除元素的节点内存
  q->qSize--;    // 减少队列的大小计数

  return true; // 移除成功，返回 true
}
```

首先检查传入的队列指针 `q` 是否为空或队列是否为空，如果为空或队列为空，则无法进行移除操作，直接返回 false。然后，从队列中摘下队首元素，将队首指针指向下一个元素，并释放被移除元素所占用的内存空间。如果参数 `sp` 非空，则将被移除的字符串拷贝给 `sp`，并确保不会溢出指定的缓冲区大小。

**q_size**

```c
/*
  返回队列的元素数量
  如果队列为 NULL 或者为空则返回 0
 */
int q_size(queue_t *q)
{
  /* 检查队列指针是否为 NULL */
  if (!q)
    return 0; // 如果队列指针为空，返回 0，表示队列元素数量为 0

  return q->qSize; // 返回队列的元素数量
}
```

首先检查传入的队列指针 `q` 是否为空，如果为空，则队列不存在或为空，直接返回 0 表示队列元素数量为 0。否则，返回队列结构中的 `qSize` 字段的值，表示队列中元素的数量。

**q_reverse**

```c
/*
  反转队列元素
  如果 q 为 NULL 则没有效果
  该函数不能直接或者间接调用其他函数为队列元素分配任何额外的空间
  (例如, 不能调用 q_insert_head, q_insert_tail, 或者 q_remove_head).
  应该是通过重排现有元素实现
 */
void q_reverse(queue_t *q)
{
  /* 检查队列指针是否为 NULL 或队列是否为空 */
  if (!q || !q->qSize)
  {
    return; // 如果队列指针为空或队列为空，无需进行反转操作，直接返回
  }

  list_ele_t *oldhead = q->head; // 保存原始队首指针
  list_ele_t *oldtail = q->tail; // 保存原始队尾指针

  // 使用迭代方法反转队列元素
  for (int i = 0; i < q->qSize - 1; i++)
  {
    list_ele_t *h = q->head; // 保存当前队首元素
    q->head = q->head->next; // 移动队首指针到下一个元素
    h->next = q->tail->next; // 将当前元素的下一个指针指向新队尾的下一个元素
    q->tail->next = h;       // 更新队尾的下一个指针，连接当前元素到新队尾
  }

  // 更新队列头部和尾部指针，完成反转
  q->head = oldtail;
  q->tail = oldhead;
}
```

首先检查传入的队列指针 `q` 是否为空或队列是否为空，如果为空或队列为空，则无需进行反转操作，直接返回。然后，它使用迭代方法将队列中的元素逐个反转，不需要额外的空间分配。最后，更新队列的头部和尾部指针以完成反转。

- 测试说明

  使用`show`，`new`，`ih`，`it`，`reverse`，`size`，`free`等命令进行测试

  运行结果为：

  ![Screen-Shot-2023-09-08-at-13.10.37.png](https://s1.imagehub.cc/images/2023/09/08/Screen-Shot-2023-09-08-at-13.10.37.png)

  在执行`make clean`之后，执行`make test`命令对程序进行评估

  运行结果为：

  ![Screen-Shot-2023-09-08-at-09.30.07.png](https://s1.imagehub.cc/images/2023/09/08/Screen-Shot-2023-09-08-at-09.30.07.png)

- 实现过程

  下面是此任务时间线：

![Screen-Shot-2023-09-08-at-13.13.40.png](https://s1.imagehub.cc/images/2023/09/08/Screen-Shot-2023-09-08-at-13.13.40.png)
![Screen-Shot-2023-09-08-at-13.13.28.png](https://s1.imagehub.cc/images/2023/09/08/Screen-Shot-2023-09-08-at-13.13.28.png)



### 五 实验心得

在完成本次实验的过程中，我积累了许多宝贵的经验和深刻的感受。下面我将分享一些我的心得体会：

首先，通过实验，我更加深入地理解了C语言的内存管理机制。特别是在编写 `q_new`、`q_insert_head`、`q_insert_tail` 等函数时，我学会了如何使用 `malloc` 和 `free` 来手动分配和释放内存，这让我对内存管理有了更深刻的认识。我也学到了处理内存泄漏和错误释放的重要性，这将有助于我编写更稳定和可靠的程序。

其次，实验让我更好地理解了指针和数据结构的应用。在队列的实现中，我使用了指针来管理链表结构，这让我意识到指针是C语言中非常强大的工具，可以用于构建复杂的数据结构和实现高效的算法。这个经验将对我今后的编程工作产生深远的影响。

此外，实验也教会了我如何编写健壮的代码。通过添加各种边界条件和错误处理机制，我确保了代码在面对非法参数和操作时不会崩溃或产生不可预测的行为。这种谨慎和注重细节的编程习惯是写出高质量代码的关键。

最后，实验让我体验到了编程的乐趣和挑战。虽然有时候遇到了困难和错误，但通过不断的尝试和调试，最终找到了解决问题的方法。这种克服困难的过程让我更加坚定了学习编程的决心。

总之，本次实验是一次非常有价值的经历，让我不仅学到了很多关于C语言和内存管理的知识，还提高了我的编程能力和问题解决能力。我相信这些经验将在今后的学习和工作中发挥重要作用，我会继续努力学习和提升自己的编程技能。

