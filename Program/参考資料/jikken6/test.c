// **************************************************
// *** H8 ���C���g���[�X  �@�@�@�@�@�@�@�@�@�@�@�@***
// ***       �@�@�@�@�@�@ �@�@�@�@�@2012.7.4  �@�@***
// ***       �@�@�@�@�@�@ �@�@produced by Y. Mori ***
// ***	       special thanks to A. Ruike, S. Kido ***
// **************************************************
// �R�����g���̃y�[�W�ԍ��́C�n�[�h�E�F�A�}�j���A�� h8_3069f.pdf ���Q��
// #if 0: �R�����g�A�E�g�����C#if 1: �R�����g���O���

#include <mes2.h>
#include "r3069.h"
#include "common.h"

#define  LD  -1  // �����[�^�̉�]����
#define  RD   1  // �E���[�^�̉�]����

static int  End_flag;   // �v���O�����̏I���t���O
static int  Feed_flag;  // �t�B�[�h�o�b�N�֐����ł̐���t���O
static int  All_black_flag;  // ���C���g���[�X�p�t���O

ct_sharedType  Ct;

// --------------
// --- �֐��Q ---
// --------------
static void  line_trace( void );

static void  init_settings( void );
static void  initialize_para( void );
static void  cool_down( void );

static void  motor( int no, int duty );


// ************************
// *** ���荞�ݏ����֐� ***
// *** ���Z�b�g�{�^���p ***
// ************************
#pragma interrupt
void
prg_end(void)
{
	STOP_FEED;  // feedback�֐��̃N���b�N���͋֎~

	load_segment( 7 );
	cool_down();
	End_flag= 1;
}


// ************************
// *** ���荞�ݏ����֐� ***
// *** feedback �֐�    ***
// ************************
#pragma interrupt
void
feed( void )
{
	int jnt;

	load_segment(42);  // �x�N�^�ԍ��i���ϐ����g���Ƃ��ɕK�v�j�Cpp.143

	// --------------------------------------
	// --- �G���R�[�_��p�������[�v���� ---
	// --------------------------------------
	if( Feed_flag == ENCO ){
	}
	// ----------------------------------------------
	// --- �t�H�g�C���^���v�^��p�����J���[�v���� ---
	// ----------------------------------------------
	else if( Feed_flag == PHOTO ){

		// �G���R�[�_�̃J�E���g��~
		STOP_ENCO_COUNT;

		line_trace();
	}
	// ----------------
	// --- �Î~��� ---
	// ----------------
	else if( Feed_flag == STOP ){

		// ���[�^��~
		for( jnt=LW; jnt<=RW; jnt++ ){
			motor( jnt, 0 );
		}

		// �G���R�[�_�̃J�E���g��~
		STOP_ENCO_COUNT;
	}

	// TSCR:�^�C�}�R���g���[���X�e�[�^�X���W�X�^ pp.464
	// CMFA:TCORA �̃R���y�A�}�b�`�̔����������X�e�[�^�X�t���O pp.465
	OCT_ITU3.TCSR.BIT.CMFA = 0;
}


// *****************
// *** main �֐� ***
// *****************
int
main( void )
{
	int  lap_flag;//���J�E���^

	init_settings();    // H8�̏����ݒ�
	initialize_para();  // �p�����[�^�̐ݒ�
        All_black_flag = 0;  // �t���O��������

	START_FEED;	 // �t�B�[�h�o�b�N�֐��X�^�[�g�i���荞�ݏ����J�n�j

	printf("\r Press Start Button.\r\n");
	while(P6.DR.BIT.B0);



	// ----------------------
	// --- ���C���g���[�X ---
	// ----------------------

	for ( lap_flag=0 ; lap_flag<4 ; lap_flag++) //3���ŏI��
	{
		Feed_flag = PHOTO;// �t�B�[�h�o�b�N�֐����ł̐���t���O

		while( All_black_flag == 0 )
		{	//�R�[�X1����-->line_trace�ŏ����iwhile�𔲂�������͏\���}�[�J�[�j
			sleep(1);// 1[ms] �l�͕ύX���Ȃ�
		}

		if(lap_flag <3 ){
//			sleep(20);
			while( Photo_1 == Black && Photo_2 == Black && Photo_3 == Black && Photo_4 == Black){
				motor(LW, 250*LD);
				motor(RW, 250*RD);
			}
		}

		All_black_flag = 0;  // �t���O��߂�
	}

	Feed_flag = STOP ;// �t�B�[�h�o�b�N�֐����ł̐���t���O

	cool_down();
	exit(0);
}


// ----------------------
// --- ���C���g���[�X ---
// ----------------------
static void
line_trace( void )
{

	// --- ���{�b�g�̈ړ� ---1����

	// ���ӁF motor�֐��o�͎��ɁC���[�^�̌���(LD, RD) ���|���邱��
	// �� motor( LW, 20*LD ), motor( RW, 20*RD )

	//1
	if( Photo_1 == White && Photo_2 == Black && Photo_3 == Black && Photo_4 == White){
		motor(LW, 250*LD);
		motor(RW, 250*RD);
	}

	//2
	if( Photo_1 == Black && Photo_2 == Black && Photo_3 == White && Photo_4 == White){
		motor(LW, 200*LD);
		motor(RW, 250*RD);
	}

	//3
//	if( Photo_1 == White && Photo_2 == White && Photo_3 == Black && Photo_4 ==Black){
//		motor(LW, 200*LD);
//		motor(RW, 40*RD);
//	}

	//4
	if( Photo_1 == White && Photo_2 ==  White && Photo_3 ==  White && Photo_4 == White){
		motor(LW, 0*LD);
		motor(RW, 100*RD);
	}

	//5
	if( Photo_1 == Black && Photo_2 == Black && Photo_3 == Black && Photo_4 == Black){
		motor(LW, 250*LD);
		motor(RW, 250*RD);
	All_black_flag = 1 ; //1���̏I���i�������̔����A������邱�Ɓj
	}

	//6
	if( Photo_1 == Black && Photo_2 == Black && Photo_3 == Black && Photo_4 == White){
		motor(LW, 0*LD);
		motor(RW, 100*RD);
	}

	//7
	if( Photo_1 == White && Photo_2 == Black && Photo_3 == Black && Photo_4 == Black){
		motor(LW, 150*LD);
		motor(RW, 0*RD);
	}

	//11
	if( Photo_1 == Black && Photo_2 == White && Photo_3 == White && Photo_4 == White){
		motor(LW, 200*LD);
		motor(RW, 250*RD);
	}

	//12
	if( Photo_1 == White && Photo_2 == Black && Photo_3 == White && Photo_4 == White){
		motor(LW, 200*LD);
		motor(RW, 250*RD);
	}

	//13
	if( Photo_1 == White && Photo_2 == White && Photo_3 == Black && Photo_4 == White){
		motor(LW, 150*LD);
		motor(RW, 50*RD);
}

	//14
	if( Photo_1 == White && Photo_2 == White && Photo_3 == White && Photo_4 == Black){
		motor(LW, 150*LD);
		motor(RW, 30*RD);
}

 //1���̏I���i�������̔����A������邱�Ɓj
}

// ------------------------
// --- H8�̏����ݒ�֐� ---
// ------------------------
static void
init_settings( void )
{
	// *** NMI �ݒ�i���Z�b�g�p�j***
	SYSCR.BIT.NMIEG= 0;
	set_handler( 7, prg_end );

	// *** �|�[�g���o�͐ݒ� (for motor, photo-sensor) ***
	P4.DDR=0xff;  // output (for motor)
	P5.DDR=0x00;  // input  (for photo-sensor)

	// *** 8bit timer pwm settings (for motor) 3069�}�j���A�� pp.482 �Q�� ***
	// [��/8192 ->3] ,[��/64 ->2] ,[��/8 ->1]
	OCT_ITU0.TCR.BIT.CKS= 2;
	OCT_ITU1.TCR.BIT.CKS= 2;
	//OCT_ITU2.TCR.BIT.CKS= 2;  // �\���|�[�g

	// TCNT���R���y�A�}�b�`A�ŃN���A: CCLR0=1, CCLR1=0, pp.462
	OCT_ITU0.TCR.BIT.CCLR= 1;
	OCT_ITU1.TCR.BIT.CCLR= 1;
	//OCT_ITU2.TCR.BIT.CCLR= 1;  // �\���|�[�g

	// �R���y�A�}�b�`B��1�o��: OIS3 �� OIS2 �� 01 �ɐݒ�, pp.467
	OCT_ITU0.TCSR.BIT.OIS32= 1;
	OCT_ITU1.TCSR.BIT.OIS32= 1;
	//OCT_ITU2.TCSR.BIT.OIS32= 1;  // �\���|�[�g

	// �R���y�A�}�b�`A��0�o��: OS1 �� OS0 �� 10 �ɐݒ�, pp.468
	OCT_ITU0.TCSR.BIT.OS10= 2;
	OCT_ITU1.TCSR.BIT.OS10= 2;
	//OCT_ITU2.TCSR.BIT.OS10= 2;  // �\���|�[�g

	// Duty
	OCT_ITU0.TCORB= 0;
	OCT_ITU1.TCORB= 0;
	//OCT_ITU2.TCORB= 0;  // �\���|�[�g

	// TCRB: �^�C���R���X�^���g���W�X�^A
	// ���g��: 20MHz(Clock)/64(��)/250(TCORA)= 1.25kHz
	OCT_ITU0.TCORA= Max_duty;
	OCT_ITU1.TCORA= Max_duty;
	//OCT_ITU2.TCORA= 250;  // �\���|�[�g

	// *** ITU3 �^�C�}���荞�� 8bit (for timer) ***
	// OCT_ITU3.TCORA: �^�C���R���X�^���g���W�X�^A pp.465
	// 1�b�ԂɌĂяo�����feed�֐��̉񐔂Ɠ����D
	// 20000000:�V�X�e���N���b�N�C8192:�����l(r3069.h��197�s�� CKS:3 (CKS2=0, CKS1=1, CKS0=1))
	// �����N���b�N=�V�X�e���N���b�N/�����l pp.463
	OCT_ITU3.TCORA= (20000000/8192)*SampTime/1000;
	OCT_ITU3.TCNT = 0;  // �J�E���^, TCORA��8TCNT�̒l�͏�ɔ�r����Ă���Dpp.460
	OCT_ITU3.TCSR.BYTE= 0x00;
	OCT_ITU3.TCR.BIT.CCLR = 1;  // �J�E���^�N���A 1:�R���y�A�}�b�`A�ɂ��N���A pp.462
	OCT_ITU3.TCR.BIT.CMIEA= 1;
	OCT_ITU3.TCR.BIT.CKS  = 3;  // �^�C�}�[�J�n 011: CK2=0, CK1=1, CK0=1 -> ��/8192, pp.463
	                            // �^�C�}�[��~ 000: CK2=0, CK1=0, CK0=0 -> pp.463
	set_handler( 42, feed );
}


// --------------------------
// --- �p�����[�^�̏����� ---
// --------------------------
static void
initialize_para( void )
{
	int  jnt;

	// --- �t���O�̏����� ---
	End_flag= 0;
	Feed_flag= STOP;
	All_black_flag= 0;

	// -------------------
	// --- motor�̐ݒ� ---
	// -------------------
	MOTOR_0_RUN;
	MOTOR_1_RUN;
}


// ------------------------------
// --- �v���O�����I�����̏��� ---
// ------------------------------
static void
cool_down( void )
{
	STOP_FEED;  // feedback�֐��̃N���b�N���͋֎~  pp.462

	motor(LW, 0);
	motor(RW, 0);

	MOTOR_0_BREAK;
	MOTOR_1_BREAK;
}


// ----------------------
// --- ���[�^�쓮�֐� ---
// ----------------------
static void
motor( int no, int duty )
{
	int  real_duty= 0;

	switch( no ){

	// for Left
	case LW:
	if( duty > 0 ){
		real_duty= duty*Ratio_duty;

		if( real_duty >= Max_duty ){
			MOTOR_0_CW;
			MOTOR_0_DUTY= Max_duty;
		}
		else{
			MOTOR_0_CW;
			MOTOR_0_DUTY= real_duty;
		}
	}
	else if(duty == 0){
		MOTOR_0_DUTY = Max_duty;  // 0:�X�g�b�v�i�������~�܂�j, Max_duty:�u���[�L�i�}�Ɏ~�܂�j
		MOTOR_0_BREAK;
	}
	else{
		real_duty= - duty*Ratio_duty;

		if( real_duty >= Max_duty ){
			MOTOR_0_CCW;
			MOTOR_0_DUTY= Max_duty;
		}
		else{
		   	MOTOR_0_CCW;
			MOTOR_0_DUTY= real_duty;
		}
	}
	break;

	// for Right
	case RW:
	if( duty > 0 ){
		real_duty= duty*Ratio_duty;

		if( real_duty >= Max_duty ){
			MOTOR_1_CW;
			MOTOR_1_DUTY= Max_duty;
		}
		else{
			MOTOR_1_CW;
			MOTOR_1_DUTY= real_duty;
		}
	}
	else if(duty == 0){
		MOTOR_1_DUTY = Max_duty;  // 0:�X�g�b�v�i�������~�܂�j, Max_duty:�u���[�L�i�}�Ɏ~�܂�j
		MOTOR_1_BREAK;
	}
	else{
		real_duty= - duty*Ratio_duty;

		if( real_duty >= Max_duty ){
			MOTOR_1_CCW;
			MOTOR_1_DUTY= Max_duty;
		}
		else{
		   	MOTOR_1_CCW;
			MOTOR_1_DUTY= real_duty;
		}
	}
	break;

	default: break;
	}
}
