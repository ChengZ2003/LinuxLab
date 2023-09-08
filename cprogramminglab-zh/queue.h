/*
 * C 编程技能测试实验代码
 * 为课程预检验
 */

/*
 * 程序实现一个同时支持 FIFO 和 LIFO 的队列
 *
 * 使用单链表表示队列元素
 */

#include <stdbool.h>

/************** 数   据   结   构   声   明 ****************/

/* 链表中的元素 (你不需要修改这个) */
typedef struct ELE
{
  /* 指向存放字符串的数组空间的指针
     空间需要显式地分配和释放 */
  char *value;
  struct ELE *next;
} list_ele_t;

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

/************** 队   列   操   作 ************************/

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
