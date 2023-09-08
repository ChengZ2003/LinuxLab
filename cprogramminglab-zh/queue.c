/*
 * C 编程技能测试实验代码
 * 为课程预检验
 */

/*
 * 程序实现一个同时支持 FIFO 和 LIFO 的队列
 *
 * 使用单链表表示队列元素
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

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
    q->head = q->head->next;    // 移动队列头部指针到下一个元素
    if (temp->value)
    {
      free(temp->value); // 如果元素中的字符串非空，则释放字符串内存
    }
    free(temp); // 释放当前元素节点的内存
  }
  free(q); // 释放队列结构的内存
}

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
