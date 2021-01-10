#ifndef DEFINE_H
#define DEFINE_H

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#define DEBUG_COST_A
//#define DEBUG_ASSERT
#define CHECK_PAIRING_TIME_LOOP 10


/**************Option**************/
#define X64
/**********************************/


#ifdef X64
#define FPLIMB_BITS 512
#define FPLIMB 11 //??
#define FPLIMB2 23  //??
#endif

#ifdef X32
#define FPLIMB_BITS 512
#define FPLIMB 12
#define FPLIMB2 24
#endif

#define scalar_t mpz_t

/**************Multiplication Algorithm Option**************/
#define Karatsuba23
//Candidats:Mont22,SB49,Karatsuba40,Karatsuba30,Karatsuba23
/***********************************************************/

/**************Multiplication Algorithm Option**************/
#define OriginalExp
//Candidats:LoubnaExp, OriginalExp
/***********************************************************/

#define k6_X_length 128//////37??
int k6_X_binary[k6_X_length+1];  //CP-6

/*Field*/
typedef struct{
  mp_limb_t x0[FPLIMB];
}fp_t;
typedef struct{
  fp_t x0,x1,x2;
}fp3_t;
typedef struct{
  fp3_t x0,x1;
}fp6_t;

/*Field*/

typedef struct{
  mp_limb_t x0[FPLIMB2];
}fpd_t;
typedef struct{
  fpd_t x0,x1,x2;
}fpd3_t;
typedef struct{
  fpd3_t x0,x1;
}fpd6_t;
//tmp finite field
mp_limb_t buf[FPLIMB];


/*Elliptic Curve*/
typedef struct{
  fp_t x,y;
  int infinity;
}efp_t;
typedef struct{
  fp3_t x,y;
  int infinity;
}efp3_t;
typedef struct{
  fp6_t x,y;
  int infinity;
}efp6_t;

/*Projectiv Elliptic Curve*/
typedef struct{
  fp_t x,y,z;
  int infinity;
}efp_projective_t;
typedef struct{
  fp3_t x,y,z;
  int infinity;
}efp3_projective_t;
typedef struct{
  fp6_t x,y,z;
  int infinity;
}efp6_projective_t;

/*Jacobian Elliptic Curve*/
typedef struct{
  fp_t x,y,z;
  int infinity;
}efp_jacobian_t;
typedef struct{
  fp3_t x,y,z;
  int infinity;
}efp3_jacobian_t;
typedef struct{
  fp6_t x,y,z;
  int infinity;
}efp6_jacobian_t;

gmp_randstate_t state;//for random
int cost_add,cost_add_ui,cost_sub,cost_sub_ui,cost_mul,cost_mul_ui,cost_mul_base,cost_sqr,cost_inv,cost_mod;
mpz_t X_z,prime_z,order_z,trace_z;
mp_limb_t X,prime[FPLIMB];
mp_limb_t prime2[FPLIMB2];
fp_t base_c;//α^7=c, β^2=α となるように逐次拡大で拡大体を構成する
fp_t base_c_inv;//α^7=c, β^2=α に出てくるcの逆元の計算結果

// fp_t curve_a;
// fp_t curve_a_twist_x3;//twist_curve_aは(0,0,0,ac^-1,0)となる

// fp_t curve_a_3sparse_x3;//(ツイスト曲線の(az^-2))*(xP^4)*(yP^-4) を格納する
// fp_t curve_a_5sparse_x3;//(ツイスト曲線の(az^-2))*(xP^4)*(yP^-4) を格納する
// fp_t curve_a_7sparse_x3;//(ツイスト曲線の(az^-2))*(xP^4)*(yP^-4) を格納する

fp_t curve_b;
fp_t curve_b_twist_x3;//Type M twist_curve_bは(0,0,0,0,ac^-1,0)となる
                      //Type D twist_curve_bは(0,0,0,0,ac^-1,0)となる
fp_t curve_b_3sparse_x3;//(ツイスト曲線の(az^-2))*(xP^4)*(yP^-4) を格納する
fp_t curve_b_5sparse_x3;//(ツイスト曲線の(az^-2))*(xP^4)*(yP^-4) を格納する
fp_t curve_b_7sparse_x3;//(ツイスト曲線の(az^-2))*(xP^4)*(yP^-4) を格納する

fp_t line_3sparse_x0;//3sparse乗算をするときに使う (xP^-3)*(yP^2) を格納する
fp_t line_5sparse_x0;//5sparse乗算をするときに使う (xP^-3)*(yP^2) を格納する
fp_t line_7sparse_x0;//7sparse乗算をするときに使う (xP^-3)*(yP^2) を格納する

mpz_t efp_total,efp3_total,efp6_total;//#efp,#efp5,#efp10,#efp7,#efp14
mpz_t miller_loop_s;
mpz_t X_1_div2,X_1,X_2,X_2_1,four;//(kai +1)/2,(kai -1),(kai^2)をあらかじめ求めておく

//emb6
fp_t frobenius_1_6;//c^((p-1)/10)の計算結果
fp_t frobenius_2_6;//c^(2(p-1)/10)の計算結果
//c^(3(p-1)/6) = c^((p-1)/2) = -1
fp_t frobenius_4_6;//c^(6(p-1)/10)の計算結果
fp_t frobenius_5_6;//c^(7(p-1)/2)の計算結果

//montgomery
mp_limb_t R[FPLIMB],Ri[FPLIMB],R1[FPLIMB],RR[FPLIMB],Ni[FPLIMB];
int m;
mp_limb_t u[FPLIMB+1];
mp_limb_t N[FPLIMB2],R2[FPLIMB],R3[FPLIMB],RmodP[FPLIMB];

struct timeval tv_start,tv_end;
float MILLER_ATE_4SPARSE_TIME;
float MILLER_ATE_5SPARSE_TIME;
float MILLER_ATE_6SPARSE_TIME;
float MILLER_ATE_7SPARSE_TIME;
float FINAL_EXP_TIME;

#endif
