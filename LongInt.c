//====================================================================
//  工学部「情報算法２」(富永)  例題
//  多倍長整数の四則演算  超長精度整数の確認
//--------------------------------------------------------------------
//  BigNum  LongInt.c
//  ソース本体
//  Linux  GCC
//--------------------------------------------------------------------
//  富永研究室  tominaga 富永浩之
//  2016.04.26  v2.0
//====================================================================


//====================================================================
//  概略仕様
/*====================================================================

● GCCでの64ビット整数 long int

C言語のint型は、その処理系で自然に扱えるビット数での整数の範囲を指す。
これをワード長という。16ビットOSなら2バイト、32ビットOSなら4バイトである。
以前のMS DOSなど、16ビットOSの時代は、int型は16ビット(2バイト)の整数を指していた。

現在のMS Linux  GCC 4.4
そのため、従来の16ビット整数を区別するため、short int型が用意されている。

しかし、古いプログラムでは、int型が16ビットのままを前提として書かれているものがある。
このようなプログラムでは、逆に32ビットの整数をlong int型としていることがある。

なお、GCCなど、long int型を64ビット(8バイト)の整数のために用意している処理系もある。
これを超長精度整数と呼ぶ。符号無で約1800京、符号付で約±900京が扱える。
これは、19桁までに相当する。二乗については、30億まで計算できる。30億の二乗は900京である。
したがって、多倍長整数の実装において、各節を8桁(1億進数)とすることができる。

scanf() や printf() では、%ld と指定する。符号無は、%lu である。
定数の場合は、符号付で 123456789012L、符号無 123456789012UL のように、
末尾に L または UL と付ける。

char ＜ short int ≦    int ≦ long int ≦ long long int
   8           16    16～32      32～64               64

扱える最大・最小の値は、ヘッダファイル limits.h で、マクロ定数に格納されている。
実際には、マクロ定数の値は、処理系によって異なる。ここでは、GCC の場合を示す。

                     ±32,768  SHRT_MIN SHRT_MAX    short int型 2バイト (16ビット)
                       65,536  USHRT_MAX

              ±2,147,483,647  INT_MIN INT_MAX            int型 4バイト (32ビット)
                4,294,967,295  UINT_MAX

  ±9,223,372,036,854,775,807  LONG_MIN LONG_MAX     long int型 8バイト (64ビット)
   18,446,744,073,709,551,615  ULONG_MAX

● C99での64ビット整数 long long int

ところで、64ビットOSが普及の段階にきている。64ビットOS上の言語処理系では、
ワード長は8バイトであり、64ビット(8バイト)の整数が自然に使えるはずである。
そこで、新しいC言語の規格 C99 では、64ビットの整数に対応した lon long int型が
用意された。long int型は、処理系によっては、32ビット整数を表すが、long long int型は、
64ビット整数が保障されている。

定数の場合は、符号付で 123456789012LL、符号無 123456789012ULL のように、
末尾に LL または ULL と付ける。
scanf() や printf() では、%lld と指定する。符号無は、%llu である。
定数の場合は、123456789012LL のように、末尾に LL と付ける。
ヘッダファイル limits.h には、以下のマクロ定数が追加される。

  ±9,223,372,036,854,775,807  LLONG_MIN LLONG_MAX
   18,446,744,073,709,551,615  ULLONG_MAX

もちろん、これらは、C99に対応した処理系でないと使えない。BCCは、対応していない。
GCCもC99には対応していないが、独自の機能として、long int型を64ビット整数として扱っている。
また、拡張性を保つため、%lld や LL にも対応している。

---------------------------------------------------------------------*/


//====================================================================
//  事前処理
//====================================================================

#include <stdio.h>
#include <limits.h>


//====================================================================
//  大域宣言
//====================================================================


//====================================================================
//  本体関数
//====================================================================

int main(void)
{
    //----  局所宣言
    // C99ではlong long intを用意
    // GCCではlong intでもよい(64ビット整数を指す)
    // BCCにはlong long intはない
    long long int a;

    //----  開始処理
    puts("超長精度整数の範囲");
    printf("short型              : %d %d %u\n", SHRT_MIN, SHRT_MAX, USHRT_MAX);
    printf("int型                : %d %d %u\n", INT_MIN, INT_MAX, UINT_MAX);
    printf("long型               : %lld %lld %llu\n", LONG_MIN, LONG_MAX, ULONG_MAX);
    // 下記はGCCでは定義されていない
    // printf("long long int型 : %lld %lld %llu\n", LLONG_MIN, LLONG_MAX, ULLONG_MAX);

    puts("超長精度整数の定数");
    // C99ではLLを用意
    // GCCではLでもよい(64ビット整数を指す)
    // BCCにはLLはない
    printf("123456789012345678LL : %lld\n", 123456789012345678LL);

    //----  入力処理
    puts("30億以下の二乗計算");
    printf("A = ?  ");
    // C99では%lldを用意
    // GCCでは%ldでもよい(64ビット整数を指す)
    // BCCには%lldはない
    scanf("%lld", &a);
    printf("A^2 ＝ %lld\n", a*a);

    //----  終了
    return 0;
}

