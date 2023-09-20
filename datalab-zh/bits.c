/*
 * CS:APP Data Lab
 *
 * <请在此输入你的学号和姓名>
 *
 * bits.c - 修改此源文件，按注释要求完成函数功能，提交此文件给老师。
 *
 * 注意: 不要包含 <stdio.h> 头文件; 它会干扰 dlc 编译器。不包含 <stdio.h>
 * 你仍然可以使用 printf 用于调试，虽然你会得到一个编译器警告。一般情况下
 * 最好不要忽略警告，不过这种情况下是OK的。
 */

#if 0
/*
 * 实验步骤:
 *
 * 第一步: 仔细阅读下面的说明。
 */

你将通过修改这个源文件里面的一系列函数来完成此次实验。

整数编程规则:
 
  将每个函数中的 return 语句替换成一条或者多条实现函数功能的C代码。
  你的代码必须符合下面的风格：
 
  int Funct(arg1, arg2, ...) {
      /* 简要描述你是如何实现的 */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  每个 Expr 是一个表达式，只能使用:
    1. 整数常量 0 到 255 (0xFF), 不允许使用大的常量，例如 0xffffffff。
    2. 函数参数和局部变量（不能使用全局变量）。
    3. 一元整数运算符 ! ~
    4. 二元整数运算符 & ^ | + << >>
    
  有些问题中对允许使用的运算符有更严格的限制。
  每个表达式 Expr 可以包含多个运算符，不做限制。

  明确禁止的是:
    1. 使用任何控制结构，例如 if, do, while, for, switch, 等.
    2. 定义或使用宏。
    3. 定义任何额外的函数。
    4. 调用任何函数。
    5. 使用任何其他的运算符, 例如 &&, ||, -, or ?:
    6. 使用任何形式的强制类型转换。
    7. 使用 int 之外的任何其他数据类型，意味着你不能使用数组或者结构等。

 
  你需要假定你的机器:
    1. 使用32位补码表示整数。
    2. 执行算术右移。
    3. 当移位次数小于 0 或者大于 31时，行为是不可预料的。


可接受的代码风格例子:
  /*
   * pow2plus1 - 返回 2^x + 1, 其中 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* 用移位实现计算2的幂 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - 返回 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* 用移位实现计算2的幂 */
     int result = (1 << x);
     result += 4;
     return result;
  }

浮点数编程规则

对于要求实现浮点操作的问题，编程限制会宽松一些. 可以使用循环和条件控制，
可以使用int 和 unsigned，可以使用任意的整型和无符号整型常量，
在整型和无符号整型数据上可以使用任何的算术、逻辑、关系运算符。

明确禁止的是:

  1. 定义或使用宏。
  2. 定义任何额外的函数。
  3. 调用任何函数。
  4. 使用任何形式的强制类型转换。
  5. 使用 int 和 unsigned 之外的任何其他数据类型，意味着你不能使用数组或者结构等。
  6. 使用任何的浮点数据类型、运算符或常量。


提醒:
  1. 使用 dlc (data lab checker)编译器(使用方法请查看说明文档)来检查你的程序的合法性。
  2. 在实现函数功能时，每个函数都有最多运算符数量限制 (整型, 逻辑或关系). 
    最多运算符数量可以用 dlc 检查.
    注意赋值运算符 (=) 不计算在内，尽管使用。
  3. 使用 btest 测试框架来检查你的函数是否正确性.
  4. 使用 BDD 检查工具来正式地验证你的函数
  5. 最多运算符数量在每个函数之前的注释中给出。

/*
 * 第二步: 按照编程规则修改下面的各函数。
 * 
 *   重要提醒:
 *   1. 使用 dlc 编译器检查你的代码遵循编程规则。
 *   2. 使用 BDD 检查工具正式地验证你的函数是否得到正确结果。
 */

#endif
// 1
/*
 * bitXor - 只使用 ~ 和 & 实现 x^y
 *   例如: bitXor(4, 5) = 1
 *   合法运算符: ~ &
 *   最多运算符数量: 14
 *   分值: 1
 */
int bitXor(int x, int y)
{
    // 德摩根律    x^y=
    // 1.(x|y)&(~x|~y)
    // 2.~(~x&~y)&~(x&y)
    // 3.(x&~y)|(~x&y)
    return ~(~x & ~y) & ~(x & y);
}
/*
 * tmin - 返回补码表示的最小值
 *   合法运算符: ! ~ & ^ | + << >>
 *   最多运算符数量: 4
 *   分值: 1
 */
int tmin(void)
{
    // 补码表示的最小值为100 .... 0(32位)，即将1左移31位
    return 1 << 31;
}
// 2
/*
 * isTmax - 如果 x 是补码表示的最大值，则返回 1
 *   合法运算符: ! ~ & ^ | +
 *   最多运算符数量: 10
 *   分值: 1
 */
int isTmax(int x)
{   // v1
    // 补码表示的最大值为0111 .... 1(32位)，Tmax = 0x7FFFFFFF, Tmax+1 = Tmin = 0x80000000。
    // ~(Tmax+1) = Tmax，只需判断 ~(x+1) 与 x 是否相等即可得出结论。但是，题目要求不能用等于号，于是利用 x^x = 0 的性质进行判断，即 return !~(x+1)^x。
    // 但是 -1，即 0xFFFFFFFF，进行上述操作后也满足要求，因为 0xFFFFFFFF + 1后发生了溢出，结果成了0x00000000，也满足!~(x+1)^x = 1。故要对 -1 进行单独处理。
    return !(~(x+1)^x) & (!!((x+1) ^ 0x0));
    // v2
    // return !((~x) ^ (1<<31));(但是<<在此题不能使用，dlc:bits.c:158:isTmax: Illegal operator (<<))
}
/*
 * allOddBits - 如果所有奇数位都是 1，则返回 1
 *   位的编号是从 0 (最低有效位 LSB) to 31 (最高有效位 MSB)
 *   例如： allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   合法运算符: ! ~ & ^ | + << >>
 *   最多运算符数量: 12
 *   分值: 2
 */
int allOddBits(int x)
{
    return 2;
}
/*
 * negate - 返回 -x
 *   例如: negate(1) = -1.
 *   合法运算符: ! ~ & ^ | + << >>
 *   最多运算符数量: 5
 *   分值: 2
 */
int negate(int x)
{
    return 2;
}
// 3
/*
 * isAsciiDigit - 如果 0x30 <= x <= 0x39 （'0' 和 '9'的 ASCII值）则返回 1
 *   例如: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   合法运算符: ! ~ & ^ | + << >>
 *   最多运算符数量: 15
 *   分值: 3
 */
int isAsciiDigit(int x)
{
    return 2;
}
/*
 * conditional - 实现条件运算符 x ? y : z
 *   例如: conditional(2,4,5) = 4
 *   合法运算符: ! ~ & ^ | + << >>
 *   最多运算符数量: 16
 *   分值: 3
 */
int conditional(int x, int y, int z)
{
    return 2;
}
/*
 * isLessOrEqual - 如果 x <= y  返回 1, 否则返回 0
 *   例如: isLessOrEqual(4,5) = 1.
 *   合法运算符: ! ~ & ^ | + << >>
 *   最多运算符数量: 24
 *   分值: 3
 */
int isLessOrEqual(int x, int y)
{
    return 2;
}
// 4
/*
 * logicalNeg - 实现逻辑非 ! 运算符, 使用除 ! 之外的其他允许使用的运算符
 *   例如: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   合法运算符: ~ & ^ | + << >>
 *   最多运算符数量: 12
 *   分值: 4
 */
int logicalNeg(int x)
{
    return 2;
}
/* howManyBits - 返回补码表示 x 所需要的最少位数（复杂题，选做）
 *  例如: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  合法运算符: ! ~ & ^ | + << >>
 *  最多运算符数量: 90
 *  分值: 4
 */
int howManyBits(int x)
{
    return 0;
}
// float
/*
 * floatScale2 - 为浮点型参数 f 返回与表达式 2*f 等价的位级表示
 *   参数和返回值都以 unsigned int 传递, 但是它们将被解释成位级表示的单精度浮点值
 *   当参数是 NaN, 返回参数
 *   合法运算符: 任何 int/unsigned 支持的运算符包括 || && 还有 if, while
 *   最多运算符数量: 30
 *   分值: 4
 */
unsigned floatScale2(unsigned uf)
{
    return 2;
}
/*
 * floatFloat2Int - 为浮点型参数 f 返回与表达式 (int)f 等价的位级表示 （复杂题，选做）
 *   参数和返回值都以 unsigned int 传递, 但是它们将被解释成位级表示的单精度浮点值
 *   超出表示范围的(包括 NaN 和 infinity) 应该返回 0x80000000u
 *   合法运算符: 任何 int/unsigned 支持的运算符包括 || && 还有 if, while
 *   最多运算符数量: 30
 *   分值: 4
 */
int floatFloat2Int(unsigned uf)
{
    return 2;
}
/*
 * floatPower2 - 为任意的 32位 int x 返回与表达式 2.0^x (2.0 的 x 次幂) 等价的位级表示
 *   返回的 unsigned 值应该有和单精度浮点值 2.0^x 一模一样的位级表示
 *   如果结果太小无法表示成 denorm 时，则返回 0
 *   如果太大，则返回 +INF.
 *
 *   合法运算符: 任何 int/unsigned 支持的运算符包括 || && 还有 if, while
 *   最多运算符数量: 30
 *   分值: 4
 */
unsigned floatPower2(int x)
{
    return 2;
}
