//====================================================================
//  工学部「情報環境実験１」(富永)  例題
//  多倍長整数の計算  除算と応用
//--------------------------------------------------------------------
//  BigNum  BigNumDiv.c
//  ライブラリ
//  Linux  GCC 4.4
//--------------------------------------------------------------------
//  富永研究室  tominaga 富永浩之
//  2015.05.20  v3.0
//====================================================================

//====================================================================
//  s15t200 高松太郎
//  2017.00.00  v3.0  
//====================================================================


//====================================================================
//  前処理
//====================================================================

//--------------------------------------------------------------------
//  ヘッダ指定
//--------------------------------------------------------------------

#include <stdio.h>
#include "BigNumLib.h"


//====================================================================
//  関数定義(除算)
//====================================================================

//--------------------------------------------------------------------
//  多倍長整数の除算(整商と剰余)
//--------------------------------------------------------------------

Bool bignum_div(BigNum b1, BigNum b2, BigNum *b3, BigNum *b4) 
{
    // return bignum_div3(b1, b2, b3, b4);
    return bignum_div4(b1, b2, b3, b4);
}

//--------------------------------------------------------------------
//  多倍長÷多倍長＝単倍長‥多倍長 (減算法)
//--------------------------------------------------------------------

Bool bignum_div0(BigNum b1, BigNum b2, int *a3, BigNum *b4) 
{
    //----  初期処理
    if ( bignum_zero(b2) ) { return FALSE; }    // 計算不可の除外
    *a3 = 0;     // 整商の初期化(減算の反復回数)
    *b4 = b1;    // 剰余の初期化(減算の最終結果)

    //----  除算処理
    // b2×a3 ≦ b1 ＜ b2×(a3+1) となるa3を減算法で求める 
    while ( *a3 < RAD ) {
        if ( bignum_cmp(*b4, b2) < 0 ) { break; }    // 除数と剰余の比較
        bignum_sub(*b4, b2, b4);                     // 剰余からの減算
        (*a3)++;                                     // 整商の増分
    }
    // 以下でもよい
    // if ( ! bignum_sub(*b4, b2, t) ) { break; } 
    // *b4 = t; (*a3)++;

    //----  事後処理
    if ( *a3 >= RAD ) { return FALSE; }    // 範囲超過
    return bignum_normal(b4);              // 正常に処理完了
    // 減算法は非効率なので、二分法で効率化する
}

//--------------------------------------------------------------------
//  多倍長÷多倍長＝単倍長‥多倍長 (二分法)
//--------------------------------------------------------------------

Bool bignum_div1(BigNum b1, BigNum b2, int *a3, BigNum *b4) 
{
    //----  局所宣言
    int low = 0;      // 下端値
    int hig = RAD;    // 上端値
    BigNum val;       // 乗算値
    int cmp;          // 比較結果

    //----  事前処理
    if ( bignum_zero(b2) ) { return FALSE; }    // 除数が0なら計算不可
 
    //----  二分法の適用
    while ( hig - low > 1 ) {       // hig‐low＝1 は事後処理に
        *a3 = (low+hig)/2;            // 中央値(切捨であることに注意)
        val = b2;                     // 
        bignum_scl(&val, *a3);        // 仮の整商で乗算 
        cmp = bignum_cmp(b1, val);    // 比較結果
        if ( cmp <= 0 ) { hig = *a3; } else { low = *a3; } 
    }
    // b2×a3＝val ≦ b1 ＜ b2×(a3+1) となるa3を二分法で求める 

    //----  結果の調整と吟味
    if ( bignum_cmp(b1, val) < 0 ) { (*a3)--; }    // 整商は切捨値
    bignum_scl(&b2, *a3);                          // 乗算値の再計算
    return bignum_sub(b1, b2, b4);                 // 剰余の計算
}

//--------------------------------------------------------------------
//  多倍長÷単倍長＝多倍長‥単倍長 (商立法)
//--------------------------------------------------------------------

Bool bignum_div2(BigNum b1, int a2, BigNum *b3, int *a4)
{
    //----  局所宣言
    int k;    // 反復変数

    //----  初期処理
    if ( a2 == 0 ) { return FALSE; }    // 計算不可の除外
    bignum_init(b3, 0, 0);              // 整商の初期化
    *a4 = 0;                            // 剰余の初期化

    //----  除算処理
    for ( k = b1.nsz-1; k >= 0; k-- ) {    // 上位節から計算
        //printf("node: %d\n", b1.node[k]);
        *a4 *= RAD;
        //printf("a41: %d\n", *a4);
        *a4 += b1.node[k];       // 一時的な被除数(繰下りを含む)
        //printf("a42: %d\n", *a4);
        b3->node[k] = *a4 / a2;              // 整商の節値
        //printf("%d\n", b3->node[k]);
        *a4 %= a2;                           // 剰余の更新
        //printf("a43: %d\n", *a4);
    }

    //----  事後処理
    return bignum_normal(b3);    // 節数と桁数の格納
}

//-------------------------------------------------------------------
//  多倍長÷多倍長＝多倍長‥多倍長 (商立法)
//--------------------------------------------------------------------

Bool bignum_div3(BigNum b1, BigNum b2, BigNum *b3, BigNum *b4) 
{
    //----  局所宣言
    int nsz;     // 節数
    int a3;      // 整商の節値
    int k;       // 反復変数

    //----  事前処理
    if ( bignum_zero(b2) ) { return FALSE; }     // 計算不可の除外
    bignum_init(b3, 0, 0);                       // 整商の初期化(0)
    nsz = b1.nsz - b2.nsz +1;                    // 節数の差(整商の節数)
    if ( nsz < 0 ) { *b4 = b1; return TRUE; }    // 整商が0のとき終了
    *b4 = b1;                                    // 
    bignum_shift(b4, -nsz);                      // 剰余の初期化

    //----  除算処理
    for ( k = nsz-1; k >= 0; k-- ) {
        bignum_shift(b4, 1);              // 剰余の候補のRAD倍(左移動)
        bignum_inc(b4, b1.node[k], 0);    // 部分節との加算
        bignum_div1(*b4, b2, &a3, b4);    // b4とb2に除算による剰余の更新
        b3->node[k] = a3;                 // 整商を節値の格納       
    }
 
   //----  事後処理
    return bignum_normal(b3);
}

//--------------------------------------------------------------------
//  多倍長÷多倍長＝多倍長‥多倍長 (二分法)
//--------------------------------------------------------------------

Bool bignum_div4(BigNum b1, BigNum b2, BigNum *b3, BigNum *b4) 
{
    //----  局所宣言
    BigNum low;    // 下端
    BigNum hig;    // 上端
    BigNum m0, m1;    // 中央
    BigNum v0, v1;    // 計算値(除数と仮整商の積)
    int nsz;       // 上端の節数
    int cmp;       // 大小比較の結果

    //----  初期処理
    if ( bignum_zero(b2) ) { return FALSE; }
    nsz = b1.nsz - b2.nsz +1;        // 上端の節数
    if ( nsz <= 0 ) { 
        *b4 = b1; 
         bignum_init(b3, 0, 0); return TRUE; 
    }
    bignum_init(&low, 0, 0);         // 下端の初期化(0)
    bignum_init(&hig, 1, nsz);       // 上端の初期化(節移動)

    while ( TRUE ) {
        //--  中央値m0と累乗値v0の計算
        bignum_add(low, hig, &m0);
        bignum_half(&m0);
        bignum_mlt(b2, m0, &v0);

        //--  中央値の増分m1と累乗値v1の計算
        m1 = m0; 
        bignum_inc(&m1, 1, 0);
        bignum_mlt(b2, m1, &v1);

        //-- 計算の終了判定と範囲の更新
        if ( bignum_within(v0, b1, v1) ) { break; }
        if ( bignum_cmp(v0, b1) <= 0 ) { low = m0; } else { hig = m0; }
    }

    //----  事後処理
    *b3 = m0; 
    return bignum_sub(b1, v0, b4);
}

//--------------------------------------------------------------------
//  多倍長整数の半分
//--------------------------------------------------------------------

Bool bignum_half(BigNum *b0) 
{
    //----  局所宣言
    int r;    // 剰余
    int k;    // 反復変数

    //---- 計算処理(ビット演算の利用)
    for ( k = b0->nsz -1; k >= 0; k-- ) {    // 上位節から
        r = b0->node[k] & 1;    // 奇偶
        b0->node[k] >>= 1;      // 半分
        if ( r == 1 && k >= 1 ) { 
            b0->node[k-1] += RAD;   // 桁送り
        }
    }

    //----  事後処理
    return bignum_normal(b0);    // 節数と桁数の格納
}


//====================================================================
//  関数定義  累乗根
//====================================================================

//--------------------------------------------------------------------
//  二分法による平方根の切捨整数値
//--------------------------------------------------------------------

Bool bignum_sqrt(BigNum b1, BigNum *b0)
{
    return bignum_plrt(b1, 2, b0);
}

//--------------------------------------------------------------------
//  二分法による累乗根の切捨整数値
//--------------------------------------------------------------------

Bool bignum_plrt(BigNum b1, int e, BigNum *b0)
{
    //----  局所宣言
    BigNum low;       // 下端
    BigNum hig;       // 上端
    BigNum m0, m1;    // 中央
    BigNum v0, v1;    // 累乗値
    int nsz;          // 
    // b0^e ≦ b1 ＜ (b0+1)^e となるb0を二分法で求める 
    // 比較が面倒なので、m0 と m1＝m0+1 を用意して、
    // m0^e ≦ b1 ＜ m1^e を終了条件にして反復処理を行う
    // 不等号を調整すれば、m1で切上整数値が求められる

    //----  初期処理
    nsz = b1.nsz/e +1;            // higの節数
    bignum_init(&low, 0, 0);      // lowを最下端0に初期化
    bignum_init(&hig, 1, nsz);    // higを最上端に初期化

    //----  二分法の計算処理
    while ( TRUE ) {
        //--  中央値m0と累乗値v0の計算
        bignum_add(low, hig, &m0);
        bignum_half(&m0);
        bignum_pow(m0, e, &v0);

        //--  中央値の増分m1と累乗値v1の計算
        m1 = m0; 
        bignum_inc(&m1, 1, 0);
        bignum_pow(m1, e, &v1);

        //-- 計算の終了判定と範囲の更新
        if ( bignum_within(v0, b1, v1) ) { break; }
        if ( bignum_cmp(v0, b1) <= 0 ) { low = m0; } else { hig = m0; }
    }

    //----  事後処理
    *b0 = m0;                    // 中央値m0を結果b0に格納
    return bignum_normal(b0);    // 正常に処理完了
}


//====================================================================
//  関数定義(最大公約数)
//====================================================================

//--------------------------------------------------------------------
//  最大公約数
//--------------------------------------------------------------------

Bool bignum_gcd(BigNum b1, BigNum b2, BigNum *b0)
{
    // return bignum_gcd1(b1, b2, b0);
    return bignum_gcd2(b1, b2, b0);
}

//--------------------------------------------------------------------
//  ブレントの算法による最大公約数
//--------------------------------------------------------------------

Bool bignum_gcd1(BigNum b1, BigNum b2, BigNum *b0)
{
    //----  局所宣言
    int e;      // 2の指数
    int cmp;    // 比較結果

    //----  事前処理 
    while ( b1.node[0] %2 == 0 && b2.node[0] %2 == 0 ) {
        bignum_half(&b1); bignum_half(&b2); e++;  // ともに偶数なら割り続ける
    }

    //----  計算処理
    while ( 1 ) {
        //--  ともに奇数になるまで2で割っておく
        while ( b1.node[0] %2 == 0 ) { bignum_half(&b1); }
        while ( b2.node[0] %2 == 0 ) { bignum_half(&b2); }

        //--  相等なら終了、そうでなければ大きい方から小さい方を引いて更新
        cmp = bignum_cmp(b1, b2);
        if ( cmp == 0 ) { *b0 = b1; break; }
        if ( cmp > 0 ) { bignum_sub(b1, b2, &b1); }
        if ( cmp < 0 ) { bignum_sub(b2, b1, &b2); }
    }

    //----  事後処理
    while ( e-- > 0 ) { bignum_scl(b0, 2); }    // 指数分だけ2を乗算
    return TRUE;    // 正常に処理完了
}

//--------------------------------------------------------------------
//  ユークリッドの互除法による最大公約数
//--------------------------------------------------------------------

Bool bignum_gcd2(BigNum b1, BigNum b2, BigNum *b0)
{
    //----  局所宣言
    BigNum t;    // 除算における整商の捨値

    //----  計算処理
    while ( ! bignum_zero(b2) ) {
        bignum_div(b1, b2, &t, &b1);
        t = b1; b1 = b2; b2 = t;
    } 

    //----  返却処理
    *b0 = b1;
    return TRUE;    // 正常に処理完了
}

