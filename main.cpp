


#include <cstring>
#define TTT_INSTANCE_HERE
// #include "field_test.h"
#include <cstdio>
#include <cstdint>
#include <cybozu/benchmark.hpp>
#include <gmp.h>
#include <sys/types.h>
#include "define.h"
// #include "./time.h"
#include "scalar.h"
#include "mpn.h"
#include "fp.h"
// #include "fp2.h"
// #include "fp6.h"
// #include "test_fp.h"
// #include "field_test.h"
// #include "efp.h"
// #include "efp6.h"
// #include "test_efp.h"
#include "create.h"
// #include "miller.h"
// #include "final_exp.h"
// #include "test_pairing.h"


int main(){
  mcl_init();
  gmp_randinit_default(state);
  gmp_randseed_ui(state,(unsigned long int)time(NULL));
  tmp_init();
  create_prt();
  // check_base();
  pre_montgomery();
  // frobenius_precalculation();
  // curve_search();
  // create_weil();

  printf("*********************************************************************************************\n\n");
  //check 
  // check_fp();

  //各関数の動作確認、コスト計算、時間計測など
  // test_fp_montgomery(CHECK_PAIRING_TIME_LOOP);
  // test_field(0, CHECK_PAIRING_TIME_LOOP, CHECK_PAIRING_TIME_LOOP, CHECK_PAIRING_TIME_LOOP);
  // test_fp(CHECK_PAIRING_TIME_LOOP);

  // test_fp2(CHECK_PAIRING_TIME_LOOP);
  // test_fp6(CHECK_PAIRING_TIME_LOOP);

  // check_fp_with_montgomery();
  // check_fp2_with_montgomery();
  // check_fp6_with_montgomery();

  // BENCH_fp2_fp6_mul_lazy_montgomery(CHECK_PAIRING_TIME_LOOP);
  // BENCH_miller_lazy_montgomery(CHECK_PAIRING_TIME_LOOP);
  // BENCH_finalexp_lazy_montgomery(CHECK_PAIRING_TIME_LOOP);
  // BENCH_Pairingn_lazy_montgomery(CHECK_PAIRING_TIME_LOOP);
  // check_efp();
  // check_efp2();
  // check_efp6();
  // check_g1_g2();

  // check_pairing_2NAF();
  // check_pairing_static();
  // check_pairing_count_2NAF_lazy_montgomery();
  // check_pairing_count_2NAF();
  // check_pairing_time_2NAF();

  printf("*********************************************************************************************\n\n");
  //free space

  fp_t A1,A2;
  fpd_t ANS1,ANS2;
  fp_init(&A1);
  fp_init(&A2);

  fpd_init(&ANS1);
  fpd_init(&ANS2);

  uint64_t B = 0x33;
  // fp_set_random(&A1, state);
  std::string str = "7132031ec392544bf25be4b471619971903fa24451f1d1b06e10241ba99d6bd94c607b234d2324c850a60d84081c252ee5921d1dc762bae64f3252ead5b2e6c438752c3491bdeb7f5a992e246176847d8c4f91d7";
  fp_set_str(&A1,str);
  fp_set(&A2, &A1);
  fp_println("A:", &A1);


  // fp_mul11_1((uint64_t*)&ANS1, (uint64_t*)&A1, &B);
  // fpd_println("ANS1: ", &ANS1);
  memset(&ANS2, 0, sizeof(ANS2));
  fp_mul11_1_asm((uint64_t*)&ANS2, (uint64_t*)&A2, &B);

  fpd_println("ANS2: ", &ANS2);
  return 0;
}
