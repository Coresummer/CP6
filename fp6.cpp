#include "fp6.h"
#include "fp2.h"

void fp6_init(fp6_t *A){
  fp2_init(&A->x0);
  fp2_init(&A->x1);
  fp2_init(&A->x2);
}

void fp6_printf(std::string str ,fp6_t *A){
  gmp_printf("%s(",str.c_str());
  fp2_printf("",&A->x0);
  gmp_printf(",");
  fp2_printf("",&A->x1);
  gmp_printf(")");
  fp2_printf("",&A->x2);
  gmp_printf(")");
}

void fp6_println(std::string str ,fp6_t *A){
  gmp_printf("%s(",str.c_str());
  fp2_printf("",&A->x0);
  gmp_printf(",");
  fp2_printf("",&A->x1);
  gmp_printf(",");
  fp2_printf("",&A->x2);
  gmp_printf(")\n");
}

// void fp6_printf_montgomery(std::string str ,fp6_t *A){
//   gmp_printf("%s(",str.c_str());
//   fp2_printf_montgomery("",&A->x0);
//   gmp_printf(",");
//   fp2_printf_montgomery("",&A->x1);
//   gmp_printf(",");
//   fp2_printf_montgomery("",&A->x2);
//   gmp_printf(")");
// }

void fp6_set(fp6_t *ANS,fp6_t *A){
  fp2_set(&ANS->x0,&A->x0);
  fp2_set(&ANS->x1,&A->x1);
  fp2_set(&ANS->x2,&A->x2);

}

void fp6_set_ui(fp6_t *ANS,unsigned long int UI){
  fp2_set_ui(&ANS->x0,UI);
  fp2_set_ui(&ANS->x1,0);
  fp2_set_ui(&ANS->x2,0);
}

void fp6_set_ui_ui(fp6_t *ANS,unsigned long int UI){
  fp2_set_ui_ui(&ANS->x0,UI);
  fp2_set_ui_ui(&ANS->x1,UI);
  fp2_set_ui_ui(&ANS->x2,UI);
}

void fp6_set_mpn(fp6_t *ANS,mp_limb_t *A){
  fp2_set_mpn(&ANS->x0,A);
  fp2_set_ui(&ANS->x1,0);
  fp2_set_ui(&ANS->x2,0);

}

void fp6_set_neg(fp6_t *ANS,fp6_t *A){
  fp2_set_neg(&ANS->x0,&A->x0);
  fp2_set_neg(&ANS->x1,&A->x1);
  fp2_set_neg(&ANS->x2,&A->x2);
}

// void fp6_to_montgomery(fp6_t *ANS,fp6_t *A){
//   fp2_to_montgomery(&ANS->x0,&A->x0);
//   fp2_to_montgomery(&ANS->x1,&A->x1);
//   fp2_to_montgomery(&ANS->x2,&A->x2);
// }

// void fp6_mod_montgomery(fp6_t *ANS,fp6_t *A){
//   fp2_mod_montgomery(&ANS->x0,&A->x0);
//   fp2_mod_montgomery(&ANS->x1,&A->x1);
//   fp2_mod_montgomery(&ANS->x2,&A->x2);

// }

void fp6_set_random(fp6_t *ANS,gmp_randstate_t state){
  fp2_set_random(&ANS->x0,state);
  fp2_set_random(&ANS->x1,state);
  fp2_set_random(&ANS->x2,state);
}

void fp6_mul(fp6_t *ANS,fp6_t *A,fp6_t *B){
  static fp6_t tmp_A,tmp_B;
  fp6_set(&tmp_A,A);
  fp6_set(&tmp_B,B);

  static fp2_t tmp1_fp2,tmp2_fp2,tmp3_fp2,tmp4_fp2,tmp5_fp2,tmp6_fp2;
  //(a+b)(d+e) = tmp4
  fp2_add(&tmp1_fp2,&tmp_A.x0,&tmp_A.x1);
  fp2_add(&tmp2_fp2,&tmp_B.x0,&tmp_B.x1);
  fp2_mul(&tmp4_fp2,&tmp1_fp2,&tmp2_fp2);

  //(a+b+c)(d+e+f) = tmp5
  fp2_add(&tmp1_fp2,&tmp1_fp2,&tmp_A.x2);
  fp2_add(&tmp2_fp2,&tmp2_fp2,&tmp_B.x2);
  fp2_mul(&tmp5_fp2,&tmp1_fp2,&tmp2_fp2);

  //(a+c)(d+f) = tmp6
  fp2_sub(&tmp1_fp2,&tmp1_fp2,&tmp_A.x1);
  fp2_sub(&tmp2_fp2,&tmp2_fp2,&tmp_B.x1);
  fp2_mul(&tmp6_fp2,&tmp1_fp2,&tmp2_fp2);

  //ad = tmp1
  fp2_mul(&tmp1_fp2,&tmp_A.x0,&tmp_B.x0);
  //be = tmp2
  fp2_mul(&tmp2_fp2,&tmp_A.x1,&tmp_B.x1);
  //cf = tmp3
  fp2_mul(&tmp3_fp2,&tmp_A.x2,&tmp_B.x2);

  //x0 = ad + θ^3((a+c+b)(d+e+f)-(a+c)(d+f)-(a+b)(d+e)+ad)
  // = tmp1 + mul_base(tmp5-tmp6-tmp2-tmp4+)
  fp2_sub(&ANS->x0,&tmp5_fp2,&tmp4_fp2);  //(a+b+c)(d+e+f) - (a+b)(d+e)
  fp2_sub(&ANS->x0,&ANS->x0,&tmp6_fp2);   //(a+b+c)(d+e+f) - (a+b)(d+e) - (a+c)(d+f)
  fp2_add(&ANS->x0,&ANS->x0,&tmp1_fp2);   //(a+b+c)(d+e+f) - (a+b)(d+e) - (a+c)(d+f) + ad
  fp2_mul_base(&ANS->x0,&ANS->x0);        //
  fp2_add(&ANS->x0,&ANS->x0,&tmp1_fp2);   //

  //x1 = (a+b)(d+e)-ad-be+cfθ^3
  fp2_sub(&ANS->x1,&tmp4_fp2,&tmp1_fp2);
  fp2_sub(&ANS->x1,&ANS->x1,&tmp2_fp2);
  fp2_mul_base(&ANS->x2,&tmp3_fp2);
  fp2_add(&ANS->x1,&ANS->x1,&ANS->x2);

  //x2 = be+(a+c)(d+f)-ad-cf
  fp2_add(&ANS->x2,&tmp2_fp2,&tmp6_fp2);
  fp2_sub(&ANS->x2,&ANS->x2,&tmp1_fp2);
  fp2_sub(&ANS->x2,&ANS->x2,&tmp3_fp2);
}


void fp6_mul_sparse_add(fp6_t *ANS,fp6_t *A,fp6_t *B){  //?000?? * ??????
  //fp2_mul*3 fp_mul*4 = 13 m
  static fp6_t tmp_A,tmp_B;
  fp6_set(&tmp_A,A);//?0 00 ?? a+b0+c0^2
  fp6_set(&tmp_B,B);//?? ?? ?? d+e0+f0^2

  static fp2_t tmp1_fp2,tmp2_fp2,tmp3_fp2,tmp4_fp2,tmp5_fp2,tmp6_fp2;

  // fp2_add(&tmp1_fp2,&tmp_A.x0,&tmp_A.x1);
  //(a)(d+e)
  fp2_add(&tmp2_fp2,&tmp_B.x0,&tmp_B.x1);
  // fp2_mul(&tmp4_fp2,&tmp_A.x0,&tmp2_fp2);
  fp_mul(&tmp4_fp2.x0,&tmp_A.x0.x0,&tmp2_fp2.x0);
  fp_mul(&tmp4_fp2.x1,&tmp_A.x0.x0,&tmp2_fp2.x1);

  //(a+c)(d+e+f) = tmp5
  // fp2_add(&tmp1_fp2,&tmp_A.x0,&tmp_A.x2);
  fp_add(&tmp1_fp2.x0,&tmp_A.x0.x0,&tmp_A.x2.x0);
  fp_set(&tmp1_fp2.x1,&tmp_A.x2.x1);
  fp2_add(&tmp2_fp2,&tmp2_fp2,&tmp_B.x2);
  fp2_mul(&tmp5_fp2,&tmp1_fp2,&tmp2_fp2);

  //(a+c)(d+f) = tmp6
  fp2_sub(&tmp2_fp2,&tmp2_fp2,&tmp_B.x1);
  fp2_mul(&tmp6_fp2,&tmp1_fp2,&tmp2_fp2);

  //ad = tmp1
  // fp2_mul(&tmp1_fp2,&tmp_A.x0,&tmp_B.x0);
  fp_mul(&tmp1_fp2.x0,&tmp_A.x0.x0,&tmp_B.x0.x0);
  fp_mul(&tmp1_fp2.x1,&tmp_A.x0.x0,&tmp_B.x0.x1);

  //cf = tmp3
  fp2_mul(&tmp3_fp2,&tmp_A.x2,&tmp_B.x2);

  //x0 = ad + θ^3((a+c)(d+e+f)-(a+c)(d+f)-(a)(d+e)+ad)
  // = tmp1 + mul_base(tmp5-tmp6-tmp2-tmp4+)
  fp2_sub(&ANS->x0,&tmp5_fp2,&tmp4_fp2);  //(a+c)(d+e+f) - (a)(d+e)
  fp2_sub(&ANS->x0,&ANS->x0,&tmp6_fp2);   //(a+c)(d+e+f) - (a)(d+e) - (a+c)(d+f)
  fp2_add(&ANS->x0,&ANS->x0,&tmp1_fp2);   //(a+c)(d+e+f) - (a)(d+e) - (a+c)(d+f) + ad
  fp2_mul_base(&ANS->x0,&ANS->x0);        //
  fp2_add(&ANS->x0,&ANS->x0,&tmp1_fp2);   //

  //x1 = (a)(d+e)-ad+cfθ^3
  fp2_sub(&ANS->x1,&tmp4_fp2,&tmp1_fp2);
  fp2_mul_base(&ANS->x2,&tmp3_fp2);
  fp2_add(&ANS->x1,&ANS->x1,&ANS->x2);

  //x2 = (a+c)(d+f)-ad-cf
  fp2_sub(&ANS->x2,&tmp6_fp2,&tmp1_fp2);
  fp2_sub(&ANS->x2,&ANS->x2,&tmp3_fp2);
}

void fp6_mul_sparse_dbl(fp6_t *ANS,fp6_t *A,fp6_t *B){  //??0?00 * ??????
  //fpmul*2 fp2mul*4 = 14 m prob because 2->6
  static fp6_t tmp_A,tmp_B;
  fp6_set(&tmp_A,A);//?? 0? 00 = a b c
  fp6_set(&tmp_B,B);//??????
  static fp2_t tmp1_fp2,tmp2_fp2,tmp3_fp2,tmp4_fp2,tmp5_fp2,tmp6_fp2;
  //(a+b)(d+e) = tmp4
  fp_set(&tmp1_fp2.x0,&tmp_A.x0.x0);
  fp_add(&tmp1_fp2.x1,&tmp_A.x0.x1,&tmp_A.x1.x1);
  fp2_add(&tmp2_fp2,&tmp_B.x0,&tmp_B.x1);
  fp2_mul(&tmp4_fp2,&tmp1_fp2,&tmp2_fp2);

  //(a+b)(d+e+f) = tmp5
  fp2_add(&tmp2_fp2,&tmp2_fp2,&tmp_B.x2);
  fp2_mul(&tmp5_fp2,&tmp1_fp2,&tmp2_fp2);

  //(a)(d+f) = tmp6
  fp2_sub(&tmp2_fp2,&tmp2_fp2,&tmp_B.x1);
  fp2_mul(&tmp6_fp2,&tmp_A.x0,&tmp2_fp2);

  //ad = tmp1
  fp2_mul(&tmp1_fp2,&tmp_A.x0,&tmp_B.x0);

  //be = tmp2
  fp_mul(&tmp2_fp2.x1,&tmp_A.x1.x1,&tmp_B.x1.x0);
  fp_mul(&tmp2_fp2.x0,&tmp_A.x1.x1,&tmp_B.x1.x1);
  fp_mul_base(&tmp2_fp2.x0,&tmp2_fp2.x0);

  //x0 = ad + θ^3((a+b)(d+e+f)-(a)(d+f)-(a+b)(d+e)+ad)
  fp2_sub(&ANS->x0,&tmp5_fp2,&tmp4_fp2);  //(a+b)(d+e+f) - (a+b)(d+e)
  fp2_sub(&ANS->x0,&ANS->x0,&tmp6_fp2);   //(a+b)(d+e+f) - (a+b)(d+e) - (a)(d+f)
  fp2_add(&ANS->x0,&ANS->x0,&tmp1_fp2);   //(a+b)(d+e+f) - (a+b)(d+e) - (a)(d+f) + ad
  fp2_mul_base(&ANS->x0,&ANS->x0);        //
  fp2_add(&ANS->x0,&ANS->x0,&tmp1_fp2);   //

  //x1 = (a+b)(d+e)-ad-be
  fp2_sub(&ANS->x1,&tmp4_fp2,&tmp1_fp2);
  fp2_sub(&ANS->x1,&ANS->x1,&tmp2_fp2);

  //x2 = be+(a)(d+f)-ad
  fp2_add(&ANS->x2,&tmp2_fp2,&tmp6_fp2);
  fp2_sub(&ANS->x2,&ANS->x2,&tmp1_fp2);
}

// void fp6_mul_lazy_montgomery(fp6_t *ANS,fp6_t *A,fp6_t *B){
//   static fp2_t tmp1,tmp2;

//   //set
//   fp2_mul_lazy_montgomery(&tmp2,&A->x1,&B->x1);//a1*b1
//   fp2_add_nonmod_single(&tmp1,&A->x0,&A->x1);//a0+a1
//   fp2_add_nonmod_single(&ANS->x1,&B->x0,&B->x1);//b0+b1
//   fp2_mul_lazy_montgomery(&ANS->x1,&tmp1,&ANS->x1);//(a0+a1)(b0+b1)
//   fp2_mul_lazy_montgomery(&tmp1,&A->x0,&B->x0);//a0*b0

//   //x0
//   fp2_mul_base(&ANS->x0,&tmp2);//(a1*b1)*alpha
//   fp2_add(&ANS->x0,&ANS->x0,&tmp1);//(a0*b0)+(a1*b1)*alpha

//   //x1
//   fp2_sub(&ANS->x1,&ANS->x1,&tmp1);//(a0+a1)(b0+b1)-a0*b0
//   fp2_sub(&ANS->x1,&ANS->x1,&tmp2);//(a0+a1)(b0+b1)-a0*b0-a1*b1
// }

void fp6_mul_ui(fp6_t *ANS,fp6_t *A,unsigned long int UI){
  fp2_mul_ui(&ANS->x0,&A->x0,UI);
  fp2_mul_ui(&ANS->x1,&A->x1,UI);
  fp2_mul_ui(&ANS->x2,&A->x2,UI);

}

void fp6_mul_mpn(fp6_t *ANS,fp6_t *A,mp_limb_t *B){
  fp2_mul_mpn(&ANS->x0,&A->x0,B);
  fp2_mul_mpn(&ANS->x1,&A->x1,B);
  fp2_mul_mpn(&ANS->x2,&A->x2,B);

}

void fp6_sqr(fp6_t *ANS,fp6_t *A){

  // static fp6_t tmp_A;
  // fp6_set(&tmp_A,A);

  // static fp2_t tmp1_fp2,tmp2_fp2,tmp3_fp2,tmp4_fp2,tmp5_fp2,tmp6_fp2;
  // fp2_add(&tmp1_fp2,&tmp_A.x0,&tmp_A.x1);
  // fp2_sqr(&tmp4_fp2,&tmp1_fp2);

  // //(a+b+c)(d+e+f) = tmp5
  // fp2_add(&tmp1_fp2,&tmp1_fp2,&tmp_A.x2);
  // fp2_sqr(&tmp5_fp2,&tmp1_fp2);

  // //(a+c)(d+f) = tmp6
  // fp2_sub(&tmp1_fp2,&tmp1_fp2,&tmp_A.x1);
  // fp2_sqr(&tmp6_fp2,&tmp1_fp2);

  // //ad = tmp1
  // fp2_sqr(&tmp1_fp2,&tmp_A.x0);
  // //be = tmp2
  // fp2_sqr(&tmp2_fp2,&tmp_A.x1);
  // //cf = tmp3
  // fp2_sqr(&tmp3_fp2,&tmp_A.x2);

  // //x0 = ad + θ^3((a+c+b)(d+e+f)-(a+c)(d+f)-(a+b)(d+e)+ad)
  // // = tmp1 + mul_base(tmp5-tmp6-tmp2-tmp4+)
  // fp2_sub(&ANS->x0,&tmp5_fp2,&tmp4_fp2);
  // fp2_sub(&ANS->x0,&ANS->x0,&tmp6_fp2);
  // fp2_add(&ANS->x0,&ANS->x0,&tmp1_fp2);
  // fp2_mul_base(&ANS->x0,&ANS->x0);
  // fp2_add(&ANS->x0,&ANS->x0,&tmp1_fp2);

  // //x1 = (a+b)(b+e)-ad-be+cfθ^3
  // fp2_sub(&ANS->x1,&tmp4_fp2,&tmp1_fp2);
  // fp2_sub(&ANS->x1,&ANS->x1,&tmp2_fp2);
  // fp2_mul_base(&ANS->x2,&tmp3_fp2);
  // fp2_add(&ANS->x1,&ANS->x1,&ANS->x2);

  // //x2 = be+(a+c)(d+f)-ad-cf
  // fp2_add(&ANS->x2,&tmp2_fp2,&tmp6_fp2);
  // fp2_sub(&ANS->x2,&ANS->x2,&tmp1_fp2);
  // fp2_sub(&ANS->x2,&ANS->x2,&tmp3_fp2);
//ELiPS sqr
  static fp2_t tmp1_fp2,tmp2_fp2,tmp3_fp2,tmp4_fp2,tmp5_fp2;
  fp2_sqr(&tmp1_fp2,&A->x0);        //x0^2
  fp2_sqr(&tmp4_fp2,&A->x2);        //x2^2
  fp2_add(&tmp5_fp2,&A->x1,&A->x1);        //2x1
  fp2_mul(&tmp2_fp2,&tmp5_fp2,&A->x2);  //2x1x2
  fp2_mul(&tmp3_fp2,&A->x0,&tmp5_fp2);  //2x0x1
  fp2_add(&tmp5_fp2,&A->x0,&A->x1);        //x0+x1+x2
  fp2_add(&tmp5_fp2,&tmp5_fp2,&A->x2);

  //x0
  fp2_mul_base(&ANS->x0,&tmp2_fp2);
  fp2_add(&ANS->x0,&ANS->x0,&tmp1_fp2);
  //x1
  fp2_mul_base(&ANS->x1,&tmp4_fp2);
  fp2_add(&ANS->x1,&ANS->x1,&tmp3_fp2);
  //x2
  fp2_sqr(&ANS->x2,&tmp5_fp2);
  fp2_add(&tmp5_fp2,&tmp1_fp2,&tmp4_fp2);
  fp2_add(&tmp5_fp2,&tmp5_fp2,&tmp2_fp2);
  fp2_add(&tmp5_fp2,&tmp5_fp2,&tmp3_fp2);
  fp2_sub(&ANS->x2,&ANS->x2,&tmp5_fp2);
}

// void fp6_sqr_lazy_montgomery(fp6_t *ANS,fp6_t *A){
//   static fp2_t tmp1,tmp2,tmp3;

//   fp2_add_nonmod_single(&tmp1,&A->x0,&A->x1);
//   fp2_mul_base(&tmp2,&A->x1);
//   fp2_add_nonmod_single(&tmp2,&tmp2,&A->x0);
//   fp2_mul_lazy_montgomery(&tmp3,&A->x0,&A->x1);

//   //x0
//   fp2_mul_lazy_montgomery(&ANS->x0,&tmp1,&tmp2);
//   fp2_sub(&ANS->x0,&ANS->x0,&tmp3);
//   fp2_mul_base(&tmp1,&tmp3);
//   fp2_sub(&ANS->x0,&ANS->x0,&tmp1);

//   //x1
//   fp2_add(&ANS->x1,&tmp3,&tmp3);
// }

void fp6_add(fp6_t *ANS,fp6_t *A,fp6_t *B){
  fp2_add(&ANS->x0,&A->x0,&B->x0);
  fp2_add(&ANS->x1,&A->x1,&B->x1);
  fp2_add(&ANS->x2,&A->x2,&B->x2);

}

void fp6_add_lazy(fp6_t *ANS,fp6_t *A,fp6_t *B){
  fp2_add_nonmod_single(&ANS->x0,&A->x0,&B->x0);
  fp2_add_nonmod_single(&ANS->x1,&A->x1,&B->x1);
  fp2_add_nonmod_single(&ANS->x2,&A->x2,&B->x2);

}
void fp6_add_fp(fp6_t *ANS,fp6_t *A,fp_t *B){
  fp6_set(ANS,A);
  fp_add(&ANS->x0.x0,&ANS->x0.x0,B);
}
void fp6_add_ui(fp6_t *ANS,fp6_t *A,unsigned long int UI){
  fp2_add_ui(&ANS->x0,&A->x0,UI);
  fp2_set(&ANS->x1,&A->x1);
  fp2_set(&ANS->x2,&A->x2);
}

void fp6_add_ui_ui(fp6_t *ANS,fp6_t *A,unsigned long int UI){
  fp2_add_ui_ui(&ANS->x0,&A->x0,UI);
  fp2_add_ui_ui(&ANS->x1,&A->x1,UI);
  fp2_add_ui_ui(&ANS->x2,&A->x2,UI);
}

void fp6_add_mpn(fp6_t *ANS,fp6_t *A,mp_limb_t *B){
  fp2_add_mpn(&ANS->x0,&A->x0,B);
  fp2_add_mpn(&ANS->x1,&A->x1,B);
  fp2_add_mpn(&ANS->x2,&A->x2,B);

}

void fp6_sub(fp6_t *ANS,fp6_t *A,fp6_t *B){
  fp2_sub(&ANS->x0,&A->x0,&B->x0);
  fp2_sub(&ANS->x1,&A->x1,&B->x1);
  fp2_sub(&ANS->x2,&A->x2,&B->x2);
}

void fp6_sub_lazy(fp6_t *ANS,fp6_t *A,fp6_t *B){
  fp2_sub_nonmod_single(&ANS->x0,&A->x0,&B->x0);
  fp2_sub_nonmod_single(&ANS->x1,&A->x1,&B->x1);
  fp2_sub_nonmod_single(&ANS->x2,&A->x2,&B->x2);

}

void fp6_sub_ui(fp6_t *ANS,fp6_t *A,unsigned long int UI){
  fp2_sub_ui(&ANS->x0,&A->x0,UI);
  fp2_set(&ANS->x1,&A->x1);
  fp2_set(&ANS->x2,&A->x2);

}

void fp6_sub_ui_ui(fp6_t *ANS,fp6_t *A,unsigned long int UI){
  fp2_sub_ui_ui(&ANS->x0,&A->x0,UI);
  fp2_sub_ui_ui(&ANS->x1,&A->x1,UI);
  fp2_sub_ui_ui(&ANS->x2,&A->x2,UI);
}

void fp6_sub_mpn(fp6_t *ANS,fp6_t *A,mp_limb_t *B){
  fp2_sub_mpn(&ANS->x0,&A->x0,B);
  fp2_sub_mpn(&ANS->x1,&A->x1,B);
  fp2_sub_mpn(&ANS->x2,&A->x2,B);

}

void fp6_inv(fp6_t *ANS,fp6_t *A){
  static fp2_t tmp1_fp2,tmp2_fp2,tmp3_fp2,tmp4_fp2,tmp5_fp2,tmp6_fp2,tmp7_fp2,tmp8_fp2;

  fp2_sqr(&tmp1_fp2,&A->x0);
  fp2_sqr(&tmp2_fp2,&A->x1);
  fp2_sqr(&tmp3_fp2,&A->x2);
  
  fp2_mul(&tmp4_fp2,&A->x1,&A->x2);
  fp2_mul_base(&tmp4_fp2,&tmp4_fp2);
  fp2_sub(&tmp6_fp2,&tmp1_fp2,&tmp4_fp2);
  
  fp2_mul(&tmp4_fp2,&A->x0,&A->x1);
  fp2_mul_base(&tmp7_fp2,&tmp3_fp2);
  fp2_sub(&tmp7_fp2,&tmp7_fp2,&tmp4_fp2);
  
  fp2_mul(&tmp4_fp2,&A->x0,&A->x2);
  fp2_sub(&tmp8_fp2,&tmp2_fp2,&tmp4_fp2);
  
  fp2_mul(&tmp1_fp2,&tmp1_fp2,&A->x0);
  fp2_mul(&tmp3_fp2,&tmp3_fp2,&A->x2);
  fp2_mul_base(&tmp3_fp2,&tmp3_fp2);
  
  fp2_add(&tmp5_fp2,&tmp4_fp2,&tmp4_fp2);
  fp2_add(&tmp5_fp2,&tmp5_fp2,&tmp4_fp2);
  fp2_sub(&tmp5_fp2,&tmp2_fp2,&tmp5_fp2);
  fp2_mul(&tmp5_fp2,&tmp5_fp2,&A->x1);
  fp2_add(&tmp5_fp2,&tmp5_fp2,&tmp3_fp2);
  fp2_mul_base(&tmp5_fp2,&tmp5_fp2);
  fp2_add(&tmp5_fp2,&tmp5_fp2,&tmp1_fp2);
  
  fp2_inv(&tmp5_fp2,&tmp5_fp2);
  
  fp2_mul(&ANS->x0,&tmp6_fp2,&tmp5_fp2);
  fp2_mul(&ANS->x1,&tmp7_fp2,&tmp5_fp2);
  fp2_mul(&ANS->x2,&tmp8_fp2,&tmp5_fp2);
}

// void fp6_inv_lazy_montgomery(fp6_t *ANS,fp6_t *A){
//   static fp2_t tmp1,tmp2;

//   fp2_sqr_lazy_montgomery(&tmp1,&A->x0);
//   fp2_sqr_lazy_montgomery(&tmp2,&A->x1);
//   fp2_mul_base(&tmp2,&tmp2);
//   fp2_sub(&tmp1,&tmp1,&tmp2);//a0^2 -a1^2*alpha
//   fp2_inv_lazy_montgomery(&tmp1,&tmp1);

//   fp2_mul_lazy_montgomery(&ANS->x0,&A->x0,&tmp1);
//   fp2_set_neg(&tmp2,&A->x1);
//   fp2_mul_lazy_montgomery(&ANS->x1,&tmp2,&tmp1);
// }

int fp6_legendre(fp6_t *A){
  mpz_t expo;
  mpz_init(expo);
  fp6_t tmp;
  fp6_init(&tmp);

  mpz_pow_ui(expo,prime_z,6);
  mpz_sub_ui(expo,expo,1);
  mpz_tdiv_q_ui(expo,expo,2);
  fp6_pow(&tmp,A,expo);

  if(fp6_cmp_one(&tmp)==0){
    mpz_clear(expo);
    return 1;
  }
  else{
    mpz_clear(expo);
    return -1;
  }
}

void fp6_sqrt(fp6_t *ANS,fp6_t *A){
  fp6_t x,y,t,k,n,tmp;
  fp6_init(&x);
  fp6_init(&y);
  fp6_init(&t);
  fp6_init(&k);
  fp6_init(&n);
  fp6_init(&tmp);
  unsigned long int e,m;
  mpz_t exp,q,z,result;
  mpz_init(exp);
  mpz_init(q);
  mpz_init(z);
  mpz_init(result);
  //gmp_randstate_t state;
  //gmp_randinit_default (state);
  //gmp_randseed_ui(state,(unsigned long)time(NULL));

  fp6_set_random(&n,state);
  while(fp6_legendre(&n)!=-1){
    fp6_set_random(&n,state);
  }
  mpz_pow_ui(q,prime_z,6);
  mpz_sub_ui(q,q,1);
  mpz_mod_ui(result,q,2);
  e=0;
  while(mpz_cmp_ui(result,0)==0){
    mpz_tdiv_q_ui(q,q,2);
    mpz_mod_ui(result,q,2);
    e++;
  }
  fp6_pow(&y,&n,q);
  mpz_set_ui(z,e);
  mpz_sub_ui(exp,q,1);
  mpz_tdiv_q_ui(exp,exp,2);
  fp6_pow(&x,A,exp);
  fp6_mul(&tmp,&x,&x);
  fp6_mul(&k,&tmp,A);
  fp6_mul(&x,&x,A);
  while(fp6_cmp_one(&k)!=0){
    m=1;
    mpz_ui_pow_ui(exp,2,m);
    fp6_pow(&tmp,&k,exp);
    while(fp6_cmp_one(&tmp)!=0){
      m++;
      mpz_ui_pow_ui(exp,2,m);
      fp6_pow(&tmp,&k,exp);
    }
    mpz_sub_ui(exp,z,m);
    mpz_sub_ui(exp,exp,1);
    mpz_ui_pow_ui(result,2,mpz_get_ui(exp));
    fp6_pow(&t,&y,result);
    fp6_mul(&y,&t,&t);
    mpz_set_ui(z,m);
    fp6_mul(&x,&x,&t);
    fp6_mul(&k,&k,&y);
  }
  fp6_set(ANS,&x);

  mpz_clear(q);
  mpz_clear(z);
  mpz_clear(exp);
  mpz_clear(result);
}

void fp6_pow(fp6_t *ANS,fp6_t *A,mpz_t scalar){
  int i,length;
  length=(int)mpz_sizeinbase(scalar,2);
  char binary[length+1];
  mpz_get_str(binary,2,scalar);
  fp6_t tmp;
  fp6_init(&tmp);
  fp6_set(&tmp,A);

  for(i=1;i<length; i++){
    fp6_sqr(&tmp,&tmp);
    if(binary[i]=='1')  fp6_mul(&tmp,A,&tmp);
  }
  fp6_set(ANS,&tmp);
}

void fp6_finalexpow_x_2NAF(fp6_t *ANS,fp6_t *A){
  static fp6_t tmp_A;
  fp6_set(&tmp_A,A);

  static fp6_t A_inv;
  fp6_frobenius_map_p3(&A_inv, A);
  fp6_set(ANS,&tmp_A);
  for(int i=(finalexp_pow_x.size() -2);i!=-1;i--){
    switch(finalexp_pow_x[i]){
      case 0:
        fp6_sqr(ANS, ANS);
        break;
      case 1:
        fp6_sqr(ANS, ANS);
        fp6_mul(ANS,ANS,&tmp_A);
        break;
      case -1:
        fp6_sqr(ANS, ANS);
        fp6_mul(ANS, ANS,&A_inv);
        break;
      default:
          break;
    }
  }
}
void fp6_finalexpow_x_1_2NAF(fp6_t *ANS,fp6_t *A){
  static fp6_t tmp_A;
  fp6_set(&tmp_A,A);

  static fp6_t A_inv;
  fp6_frobenius_map_p3(&A_inv, A);
  fp6_set(ANS,&tmp_A);
  for(int i=(finalexp_pow_x_1.size() -2);i!=-1;i--){
    switch(finalexp_pow_x_1[i]){
      case 0:
        fp6_sqr(ANS, ANS);
        break;
      case 1:
        fp6_sqr(ANS, ANS);
        fp6_mul(ANS,ANS,&tmp_A);
        break;
      case -1:
        fp6_sqr(ANS, ANS);
        fp6_mul(ANS, ANS,&A_inv);
        break;
      default:
          break;
    }
  }
}
void fp6_finalexpow_3w_2NAF(fp6_t *ANS,fp6_t *A){
    static fp6_t tmp_A;
  fp6_set(&tmp_A,A);

  static fp6_t A_inv;
  fp6_frobenius_map_p3(&A_inv, A);
  fp6_set(ANS,&tmp_A);
  for(int i=(finalexp_pow_3w.size() -2);i!=-1;i--){
    switch(finalexp_pow_3w[i]){
      case 0:
        fp6_sqr(ANS, ANS);
        break;
      case 1:
        fp6_sqr(ANS, ANS);
        fp6_mul(ANS,ANS,&tmp_A);
        break;
      case -1:
        fp6_sqr(ANS, ANS);
        fp6_mul(ANS, ANS,&A_inv);
        break;
      default:
          break;
    }
  }
}

int  fp6_cmp(fp6_t *A,fp6_t *B){
  if(fp2_cmp(&A->x0,&B->x0)==0 && fp2_cmp(&A->x1,&B->x1)==0 && fp2_cmp(&A->x2,&B->x2)==0){
    return 0;
  }
  return 1;
}

int  fp6_cmp_ui(fp6_t *A,unsigned long int UI){
  if(fp2_cmp_ui(&A->x0,UI)==0 && fp2_cmp_ui(&A->x1,UI)==0&& fp2_cmp_ui(&A->x2,UI)==0){
    return 0;
  }
  return 1;
}

int  fp6_cmp_mpn(fp6_t *A,mp_limb_t *B){
  if(fp2_cmp_mpn(&A->x0,B)==0 && fp2_cmp_mpn(&A->x1,B)==0 && fp2_cmp_mpn(&A->x2,B)==0){
    return 0;
  }
  return 1;
}

int  fp6_cmp_zero(fp6_t *A){
  if(fp2_cmp_zero(&A->x0)==0 && fp2_cmp_zero(&A->x1)==0 && fp2_cmp_zero(&A->x2)==0){
    return 0;
  }
  return 1;
}

int  fp6_cmp_one(fp6_t *A){
  if(fp2_cmp_one(&A->x0)==0 && fp2_cmp_zero(&A->x1)==0 && fp2_cmp_zero(&A->x2)==0){
    return 0;
  }
  return 1;
}

// int fp6_montgomery_trick_montgomery(fp6_t *A_inv,fp6_t *A,int n){
//   int i;
//   fp6_t ANS[n],ALL_inv;
//   fp6_set(&ANS[0],&A[0]);
//   fp6_t check;

//   for(i=1;i<n;i++){
//     fp6_mul_lazy_montgomery(&ANS[i],&ANS[i-1],&A[i]);
//   }
//   fp6_inv_lazy_montgomery(&ALL_inv,&ANS[n-1]);
//   for(i=n-1;i>0;i--){
//     fp6_mul_lazy_montgomery(&A_inv[i],&ALL_inv,&ANS[i-1]);
//     fp6_mul_lazy_montgomery(&ALL_inv,&ALL_inv,&A[i]);
//   }

//   fp6_set(&A_inv[0],&ALL_inv);
//   return 0;
// }

void fp6_frobenius_map_p1(fp6_t *ANS,fp6_t *A){
  fp_set(&ANS->x0.x0,&A->x0.x0);
  fp_set_neg(&ANS->x0.x1,&A->x0.x1);

  fp_mul(&ANS->x1.x0,&A->x1.x0,&frobenius_1_6);
  fp_mul(&ANS->x1.x1,&A->x1.x1,&frobenius_4_6);

  fp_mul(&ANS->x2.x0,&A->x2.x0,&frobenius_2_6);
  fp_mul(&ANS->x2.x1,&A->x2.x1,&frobenius_5_6);
}

void fp6_frobenius_map_p2(fp6_t *ANS,fp6_t *A){ //not sure ...
  fp_set(&ANS->x0.x0,&A->x0.x0);
  fp_set(&ANS->x0.x1,&A->x0.x1);
  
  fp_mul(&ANS->x1.x0,&A->x1.x0,&frobenius_2_6);
  fp_mul(&ANS->x1.x1,&A->x1.x1,&frobenius_2_6);

  fp_mul(&ANS->x2.x0,&A->x2.x0,&frobenius_4_6);
  fp_mul(&ANS->x2.x1,&A->x2.x1,&frobenius_4_6);
}

void fp6_frobenius_map_p3(fp6_t *ANS,fp6_t *A){ //not sure ...
  fp_set(&ANS->x0.x0,&A->x0.x0);
  fp_set_neg(&ANS->x0.x1,&A->x0.x1);

  fp_set_neg(&ANS->x1.x0,&A->x1.x0);
  fp_set(&ANS->x1.x1,&A->x1.x1);

  fp_set(&ANS->x2.x0,&A->x2.x0);
  fp_set_neg(&ANS->x2.x1,&A->x2.x1);
}

