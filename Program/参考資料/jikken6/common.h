// **************************************************
// *** common.h (���ʕϐ��C���ʒ萔)              ***
// ***                        produced by Y. Mori ***
// ***	      special thanks to A. Ruike, S. Kido ***
// **************************************************

#ifndef __COMMON_H
#define __COMMON_H

#include <math.h>

// **************
// *** For H8 ***
// **************

// --- motor ---
#define  MOTOR_0_CW		P4.DR.BIT.B0= 1; P4.DR.BIT.B2= 0;
#define  MOTOR_0_CCW	P4.DR.BIT.B0= 0; P4.DR.BIT.B2= 1;
#define  MOTOR_0_RUN	P4.DR.BIT.B0= 0; P4.DR.BIT.B2= 0;
#define  MOTOR_0_BREAK	P4.DR.BIT.B0= 1; P4.DR.BIT.B2= 1;
#define  MOTOR_0_DUTY	OCT_ITU0.TCORB

#define  MOTOR_1_CW		P4.DR.BIT.B1= 1; P4.DR.BIT.B3= 0;
#define  MOTOR_1_CCW	P4.DR.BIT.B1= 0; P4.DR.BIT.B3= 1;
#define  MOTOR_1_RUN	P4.DR.BIT.B1= 0; P4.DR.BIT.B3= 0;
#define  MOTOR_1_BREAK	P4.DR.BIT.B1= 1; P4.DR.BIT.B3= 1;
#define  MOTOR_1_DUTY	OCT_ITU1.TCORB

// �^�C�}�[�J�n 011: CK2=0, CK1=1, CK0=1 -> ��/8192, pp.463
#define  START_FEED  OCT_ITU3.TCR.BIT.CKS= 3;
// �^�C�}�[��~ 000: CK2=0, CK1=0, CK0=0 -> pp.463
#define   STOP_FEED  OCT_ITU3.TCR.BIT.CKS= 0;

// �G���R�[�_�J�E���g�J�n
#define  START_ENCO_COUNT  HEX_ITU.TSTR.BIT.STR0= 1; HEX_ITU.TSTR.BIT.STR1= 1; HEX_ITU.TSTR.BIT.STR2= 1;
// �G���R�[�_�J�E���g��~
#define  STOP_ENCO_COUNT   HEX_ITU.TSTR.BIT.STR0= 0; HEX_ITU.TSTR.BIT.STR1= 0; HEX_ITU.TSTR.BIT.STR2= 0;

// --- Photo reflector ---
#define  Photo_1  P5.DR.BIT.B0  // 1�ԍ�
#define  Photo_2  P5.DR.BIT.B1  // 2�Ԗ�
#define  Photo_3  P5.DR.BIT.B2  // 3�Ԗ�
#define  Photo_4  P5.DR.BIT.B3  // 4�Ԗ�

#define  Black  0
#define  White  1

// --- Motor ---
#define  Max_duty    250
#define  Limit_duty  250
#define  Ratio_duty (Max_duty/Limit_duty)

// --- Feedback control ---
#define  STOP   0
#define  ENCO   1
#define  PHOTO  2


// ************************
// *** Common Parameter ***
// ************************

// --- �T���v�����O�^�C�� ---
#define  SampTime   20      // [ms]
#define  RsampTime   0.02f  // [s]

// --- Joint Number ---
#define  Jnum  2
#define  LW  0  //  Left wheel
#define  RW  1  // Right wheel


// **********************
// *** For Controller ***
// **********************

// --- control shared type ---
typedef struct{

	int    pwm[Jnum];      // ���[�^��PWM�l�i�ő�l=Max_duty�j
    
} ct_sharedType;

#endif
