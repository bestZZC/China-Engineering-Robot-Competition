#ifndef PINOUT_H
#define PINOUT_H

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3
#define X_MAX_PIN           2

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

#define E0_STEP_PIN         26
#define E0_DIR_PIN          28
#define E0_ENABLE_PIN       24

#define E1_STEP_PIN         36
#define E1_DIR_PIN          34
#define E1_ENABLE_PIN       30

//微动开关按下：0（亮灯）
//微动开关弹起：1
#define weidong_1 Y_MAX_PIN
#define weidong_2 Z_MIN_PIN
#define weidong_3 Z_MAX_PIN

#define ENA 45
#define IN1 47
#define IN2 32

#define input 25    //红16
#define output1 23  //棕17
#define output2 17  //黑23
#define output3 16  //白25
#define output 27   //粉27

#define S0 41
#define S1 39
#define S2 X_MAX_PIN
#define S3 Y_MIN_PIN
#define OUT X_MIN_PIN
#define LED 43

#define green_in -3*PI/5
#define green_out 0
#define white_in -PI
#define white_out -2*PI/5
#define red_in 3*PI/5
#define red_out -4*PI/5
#define black_in PI/5
#define black_out 4*PI/5
#define blue_in -PI/5
#define blue_out 2*PI/5

#define xiaozhuanpan_one 2*PI/5
#define dazhuanpan_one 2*PI/5
#define dazhuanpan_half PI/5
#endif
