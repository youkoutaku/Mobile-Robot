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
static int  All_black_flag, All_white_flag;  // ���C���g���[�X�p�t���O

ki_sharedType  Ki;
ct_sharedType  Ct;

// --------------
// --- �֐��Q ---
// --------------
static void  line_trace( void );

static void  traj_tracking( float x_f, float tht_f, float t_f );
static void  traj_plan_3( int jnt, float x_i, float x_f, float t_i, float t_f );
static void  make_traj( float t_i, float t_f );

static void  init_settings( void );
static void  initialize_para( void );
static void  cool_down( void );

static void  start_time( ct_timeType *p );
static void  reset_time( ct_timeType *p );
static int   inc_time(   ct_timeType *p );

static void  motor( int no, int duty );
static int   enco(  int no );  // 3069 H8�ް�ނł́Cint �� long �̈���

static void  save_data( int file_number );


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

              for( jnt= LW; jnt<= RW; jnt++ ){				

		    // --- �ԗւ̉�]�p���v�� ---
		    Ct.jnt[jnt].last.d   = Ct.jnt[jnt].present.d;
		    Ct.jnt[jnt].present.d= (float)Ct.enco_dir[jnt]*enco(jnt)*(Ct.WPulse2Rad)/(Ct.Tr);
	            Ct.jnt[jnt].present.v= (Ct.jnt[jnt].present.d - Ct.jnt[jnt].last.d)/RsampTime;

		    // --- �ԗւ̐��� ---
		    Ct.jnt[jnt].delta=  (Ct.start_present_d[jnt] + Ki.traj[jnt][Ct.dTime.val].d)
			                  - (Ct.jnt[jnt].present.d);

		    // --- �o�͓d�����v�Z ---
		    Ct.pwm[jnt] = (int)(  (Ct.jnt[jnt].gain.kp)*(Ct.jnt[jnt].delta)
			                    - (Ct.jnt[jnt].gain.kd)*(Ct.jnt[jnt].present.v) );	
	
		    motor( jnt, (Ct.mot_dir[jnt])*Ct.pwm[jnt] );
			
		    // --- �f�[�^�̕ۑ� ---
		    Ct.data[jnt][Ct.dTime.val].d= Ct.jnt[jnt].present.d - Ct.start_present_d[jnt];
	      }
		
	      // --- ���Ԃ�i�߂� ---
	      Ct.timeFlag= inc_time(&Ct.dTime);
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
        int  jnt;

	init_settings();    // H8�̏����ݒ�
	initialize_para();  // �p�����[�^�̐ݒ�
        All_black_flag = 0;  // �t���O��������

	START_FEED;	 // �t�B�[�h�o�b�N�֐��X�^�[�g�i���荞�ݏ����J�n�j

	printf("\r Press Start Button.\r\n");
	while(P6.DR.BIT.B0);




	// ----------------------
	// --- ���C���g���[�X ---
	// ----------------------

	for ( lap_flag=0 ; lap_flag<3 ; lap_flag++) //2���ŏI��	
	{
		Feed_flag = PHOTO;// �t�B�[�h�o�b�N�֐����ł̐���t���O

		while( All_black_flag == 0 )
		{	//�R�[�X1����-->line_trace�ŏ����iwhile�𔲂�������͏\���}�[�J�[�j
			sleep(1);// 1[ms] �l�͕ύX���Ȃ�
		}
                while(Photo_1 == Black && Photo_2 == Black &&
                      Photo_3 == Black && Photo_4 == Black){ 
                motor(LW, 200*LD); //�O�i
	        motor(RW, 200*RD);
	        }
		All_black_flag = 0;  // �t���O��߂�
        }

	Feed_flag = STOP ;// �t�B�[�h�o�b�N�֐����ł̐���t���O
// kidoutijuuseigyo

        // --------------------
	// --- �O���Ǐ]���� ---
	// --------------------
	// traj_tracking( x_f[cm],  tht_f[deg],  t_f[s] );
	Feed_flag = ENCO;				
	traj_tracking( 65.0f,  0.0f, 3.0f );
	Feed_flag = STOP;
	#if 1
	save_data(1);  // �f�[�^�̃Z�[�u
	#endif	
	
	Feed_flag = ENCO;				
	// ( x_f[cm],  tht_f[deg],  t_f[s] );
	traj_tracking( -25.0f,  -90.0f,  2.0f );
	Feed_flag = STOP;	
	#if 1
	save_data(2);  // �f�[�^�̃Z�[�u
	#endif

	Feed_flag = ENCO;				
	// ( x_f[cm],  tht_f[deg],  t_f[s] );
	traj_tracking( -25.0f,  0.0f,  2.0f );
	Feed_flag = STOP;	
	#if 1
	save_data(3);  // �f�[�^�̃Z�[�u
	#endif
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
	if( Photo_1 == White && Photo_2 == Black &&
            Photo_3 == Black && Photo_4 == White ){
	motor(LW, 200*LD); //�O�i
	motor(RW, 200*RD);
	}
	else if( Photo_1 == Black && Photo_2 == Black &&
                 Photo_3 == White && Photo_4 == White ){
	motor(LW, 180*LD); //������
	motor(RW, 200*RD);
	}
	else if( Photo_1 == Black && Photo_2 == White &&
                 Photo_3 == White && Photo_4 == White ){
	motor(LW, 170*LD); //������
	motor(RW, 200*RD);
	}
        else if( Photo_1 == Black && Photo_2 == Black &&
                 Photo_3 == Black && Photo_4 == Black ){
        motor(LW, 200*LD); //�O�i
        motor(RW, 200*RD);
	All_black_flag = 1 ; //1���̏I���i�������̔����A������邱�Ɓj
        }	
	else if(( Photo_1 == White && Photo_2 == White &&
                 Photo_3 == Black && Photo_4 == Black ) ||
               ( Photo_1 == White && Photo_2 == White &&
                 Photo_3 == Black && Photo_4 == White ) ||
 	       ( Photo_1 == White && Photo_2 == White &&
                 Photo_3 == White && Photo_4 == Black )){
	motor(LW, 200*LD); //�E����
	motor(RW, 180*RD);
	}

	else if(( Photo_1 == Black && Photo_2 == Black &&
                 Photo_3 == Black && Photo_4 == White ) ||
 	       ( Photo_1 == White && Photo_2 == White &&
                 Photo_3 == White && Photo_4 == White )){
	motor(LW, 0*LD); //����(���p�R�[�i�[)
	motor(RW, 100*RD);
	}

}

// --------------------------------------
// --- �G���R�[�_��p�����O���Ǐ]���� ---
// --------------------------------------
static void
traj_tracking( float x_f, float tht_f, float t_f ){

	int  confirm;
	int  i, jnt, d;
	float  x_i, tht_i, t_i;	

	static unsigned short  start_times[150];
	static unsigned short    end_times[150];

	x_i  = 0.0f;
	tht_i= 0.0f;
	t_i  = 0.0f;

	traj_plan_3( DDis,   x_i,   x_f*0.01f,   t_i, t_f );
	traj_plan_3( DTht, tht_i, tht_f*Deg2Rad, t_i, t_f );

	make_traj( t_i, t_f );

	Ct.dTime.stopVal= (int)( (t_f-t_i)*1000/SampTime );
	start_time( &Ct.dTime );
	
	START_ENCO_COUNT;  // �G���R�[�_�̃J�E���g�J�n
	
	while( Ct.timeFlag != Reached ){

		#if 0
		printf("\n");
		printf("%d[ms]  LW_traj=%d[mm]  LW=%d[mm]  RW_traj=%d[mm]  RW=%d[mm] \r", 
			(Ct.dTime.val)*SampTime,
			(int)(1000*( Ki.traj[LW][Ct.dTime.val].d )*Ki.r),  
			(int)(1000*( Ct.jnt[LW].present.d - Ct.start_present_d[LW] )*Ki.r ),
			(int)(1000*( Ki.traj[RW][Ct.dTime.val].d )*Ki.r), 
			(int)(1000*( Ct.jnt[RW].present.d - Ct.start_present_d[RW] )*Ki.r )
		);
		#endif

		if( End_flag==1 ){
			exit(0);
		}
	}

	reset_time( &Ct.dTime );

	// ���݈ʒu���擾
	for( jnt= LW; jnt<= RW; jnt++ ){
		Ct.start_present_d[jnt] = Ct.jnt[jnt].present.d;
	}

	#if 1
	printf(" traj_tracking_END: (x_f, tht_f)=(%d, %d)\n\r",(int)x_f, (int)tht_f); // ����I��
	#endif
}

// --------------------------------
// --- �ڕW�̖{�̑��x�O���𐶐� ---
// --------------------------------
static void
traj_plan_3(int jnt, float x_i, float x_f, float t_i, float t_f)
{
	char  buf[150];
	int   i;
	int   imti, imtf;
	float j;    
	float a0, a2, a3;
	float pos_present=0.0f, pos_last=0.0f;
    
	a0=  x_i;
	a2=  3.0f*(x_f-x_i)/((t_f-t_i)*(t_f-t_i));
	a3= -2.0f*(x_f-x_i)/((t_f-t_i)*(t_f-t_i)*(t_f-t_i));
    
	imti = (int)(t_i/ RsampTime);
	imtf = (int)(t_f/ RsampTime);

	// ���x�̐ϕ��l�����傤�ǖڕW�ʒu�ɂȂ�悤�ɁC�����Ē��ڑ��x�����߂Ȃ��D
	// �������C���x���狁�߂��ʒu�́C�����킸���ɂ���邱�Ƃ��m�F���ꂽ�D
	for (i=imti; i<=imtf; i++){
		pos_last= pos_present; 
        
		j= ((float)i-(float)imti)*RsampTime;

		pos_present= a0 + a2*j*j + a3*j*j*j;

		if (i==imti || i==imtf ){
			Ki.in[jnt][i]= 0.0f;
		}
		else {
			Ki.in[jnt][i]= (pos_present - pos_last)/RsampTime;
		}        
	
		// for debug
		#if 0
		if( jnt == DDis ){
			printf("%d \n\r", (int)(1000*pos_present) );
		}
		#endif
	}
}

// ----------------------------
// --- �ڕW�̎ԗ֋O���𐶐� ---
// ----------------------------
static void
make_traj( float t_i, float t_f )
{
	int  i, jnt;
	int  imti, imtf;

	imti= (int)(t_i/ RsampTime);
	imtf= (int)(t_f/ RsampTime);

	for( i=imti; i<=imtf; i++ ){

		for( jnt= LW; jnt<= RW; jnt++ ){
				
			// --- �e�ԗւ̖ڕW�p���x�����߂� ---
			if( jnt == LW ){
				Ki.traj[jnt][i].v=
				( Ki.in[DDis][i] - (Ki.W)*(Ki.in[DTht][i])/2.0f )/(Ki.r);
			}
			else if( jnt == RW ){
				Ki.traj[jnt][i].v=
				( Ki.in[DDis][i] + (Ki.W)*(Ki.in[DTht][i])/2.0f )/(Ki.r);
			}
			if( i>=1 ){
				Ki.traj[jnt][i].d= (Ki.traj[jnt][i-1].d) + (Ki.traj[jnt][i].v)*RsampTime;
			}
			else{
				Ki.traj[jnt][i].d= 0;
			}

			// for debug
			#if 0
			if( jnt == LW ){
				printf(" %d[ms]  %d[mm] \r", i*SampTime, (int)(1000*Ki.traj[jnt][i].d*(Ki.r)+0.5f) );
			}
			#endif
		}
	}
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

        // *** ITU2 �ʑ��W�����[�h�ݒ� (for Left encoder) ***
	HEX_ITU.TMDR.BIT.MDF=  1;  // �ʑ��W�����[�h
	HEX_ITU.TMDR.BIT.FDIR= 1;  // �I�[�o�t���[�E�A���_�[�t���[���m
	HEX_ITU.TSTR.BIT.STR2= 1;  // 1:�^�C�}�X�^�[�g, 0:�X�g�b�v
	HEX_ITU2.TCNT =0;				

	// *** ITU0, ITU1 �J�E���^ (for Right encoder) ***
	// --- ITU0: �J�E���g ---
	HEX_ITU0.TCR.BIT.CCLR= 0;  // �N���A�֎~
	HEX_ITU0.TCR.BIT.CKEG= 2;  // 0:�����オ��C1:������C2:���G�b�W
	HEX_ITU0.TCR.BIT.TPSC= 6;  // �O���N���b�NC: TCLKC�[�q���͂ŃJ�E���g 110
	HEX_ITU0.TCNT= 0;
	HEX_ITU.TSTR.BIT.STR0= 1;  // 1:�^�C�}�X�^�[�g�C0:�X�g�b�v
	// --- ITU1: �J�E���g ---
	HEX_ITU1.TCR.BIT.CCLR= 0;  // �N���A�֎~
	HEX_ITU1.TCR.BIT.CKEG= 2;  // 0:�����オ��C1:������C2:���G�b�W
	HEX_ITU1.TCR.BIT.TPSC= 7;  // �O���N���b�ND: TCLKD�[�q���͂ŃJ�E���g 111
	HEX_ITU1.TCNT= 0;
	HEX_ITU.TSTR.BIT.STR1= 1;  // 1:�^�C�}�X�^�[�g�C0:�X�g�b�v

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
        All_white_flag= 0;

        // --- �������Ԃ��Z�b�g ---
	Ct.dTime.startVal = 0;  

	// --- �ϐ��̏����� ---
	for( jnt=LW; jnt<=RW; jnt++ ){
		Ct.jnt[jnt].present.d = 0;
	}

	// --- �n�[�h�E�F�A�p�����[�^���Z�b�g ---
	// �z�C�[�������]���Ƃ� (Ki.r)*2*PI [cm] <-> (Ct.Tr)*(128*4) [pulse] 
	// 0.0013725 [cm/pulse]
	Ki.W= 0.209f;    // �z�C�[���ԋ��� 0.209   [m]
	Ki.r= 0.02115f;  // �z�C�[�����a   0.02115 [m]
	Ct.WPulse2Rad= 2.0f*PI/512.0f;  // �G���R�[�_�̃p���X����]�p[rad]�ɕϊ�, 512=128[pulse]*4[���{]
	Ct.Tr= 29.0f;    // ���[�^������  

	// --- ���{�b�g�̃Q�C�����Z�b�g ---
	Ct.jnt[LW].gain.kp= 80.0f*1.0f;
	Ct.jnt[LW].gain.kd= 80.0f*RsampTime*1.0f;

	Ct.jnt[RW].gain.kp= 80.0f*1.0f;
	Ct.jnt[RW].gain.kd= 80.0f*RsampTime*1.0f;

	// -------------------
	// --- motor�̐ݒ� ---
	// -------------------
        // ��]�������Z�b�g
	Ct.mot_dir[LW]= -1;
	Ct.mot_dir[RW]=  1;
	MOTOR_0_RUN;
	MOTOR_1_RUN;
	// ----------------------
	// --- encoder �̐ݒ� ---
	// ----------------------
	// ��]�������Z�b�g
	Ct.enco_dir[LW]= -1;
	Ct.enco_dir[RW]=  1;

	// �ϐ��̏�����
	for( jnt=LW; jnt<=RW; jnt++ ){
		Ct.enco[jnt].present.d = 0;
		Ct.tmp_enco_val[jnt]   = 0;
		Ct.start_present_d[jnt]= 0;
	}
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

// ------------------------
// --- ���ԃX�^�[�g�֐� ---
// ------------------------
static void
start_time( ct_timeType *p )
{
	p->val = p->startVal;
	p->inc = UntilInc;
} 


// ----------------------
// --- ���Ԃ����Z�b�g ---
// ----------------------
static void
reset_time( ct_timeType *p )
{
	p->val = p->startVal;
	p->inc = Stopped;
} 


// --------------------
// --- ���ԍX�V�֐� ---
// --------------------
static int
inc_time( ct_timeType *p )
{
	int ret;

	ret = !Reached;          

	if( p->inc != Stopped ){
		if( p->val < p->stopVal ){
			p->val ++;
		}
		else{
			ret= Reached;
		}
	}
	return(ret);
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

// ------------------------------
// --- �G���R�[�_�l�̓Ǎ��֐� ---
// ------------------------------
static int
enco(int no)
{
	static int  r0_present_d, r0_last_d, r0_present_delta=0;
	static int  r1_present_d, r1_last_d, r1_present_delta=0;

	if( no==LW ){
		Ct.enco[no].last.d= Ct.enco[no].present.d;

		Ct.enco[no].present.d= - HEX_ITU2.TCNT;  // 4���{

		if( Ct.enco[no].present.d > 32767 ){
			Ct.enco[no].present.d -= 65536;
		}	
	
		Ct.enco[no].present.delta= Ct.enco[no].present.d - Ct.enco[no].last.d;

		if( Ct.enco[no].present.delta < -30000 ){
			Ct.enco[no].present.delta += 65536;
		}
		else if( Ct.enco[no].present.delta > 30000 ){
			Ct.enco[no].present.delta -= 65536;
		}

		Ct.tmp_enco_val[no] += Ct.enco[no].present.delta;
	}

	else if( no==RW ){

		//Ct.enco[no].present.d= (HEX_ITU0.TCNT - HEX_ITU1.TCNT)*2;  // 2���{      
		// -------------
		// --- r0 �p ---
		// -------------
		r0_last_d = r0_present_d;

		r0_present_d = HEX_ITU0.TCNT;

		if( r0_present_d > 32767 ){
			r0_present_d -= 65536;
		}	
	
		r0_present_delta = r0_present_d - r0_last_d;

		if( r0_present_delta < -30000 ){
			r0_present_delta += 65536;
		}
		else if( r0_present_delta > 30000 ){
			r0_present_delta -= 65536;
		}

		// -------------
		// --- r1 �p ---
		// -------------
		r1_last_d = r1_present_d;

		r1_present_d = HEX_ITU1.TCNT;

		if( r1_present_d > 32767 ){
			r1_present_d -= 65536;
		}	
	
		r1_present_delta = r1_present_d - r1_last_d;

		if( r1_present_delta < -30000 ){
			r1_present_delta += 65536;
		}
		else if( r1_present_delta > 30000 ){
			r1_present_delta -= 65536;
		}

		Ct.tmp_enco_val[no] += (r0_present_delta - r1_present_delta)*2;
	}

	return( Ct.tmp_enco_val[no] );
}


// -------------------------- //
// --- �f�[�^�Z�[�u�p�֐� --- //
// -------------------------- //
// �Etftp - 192.168.1.1 data*.txt (*�́Cfile_number) �ŁC
// �@PC�̃v���O�����̂���t�H���_�Ƀt�@�C�����]�������D
// �Edir �ŁCH8�̒��̃t�@�C����������D
// �Etype data*.txt �ŁCdata*.txt�̓��e���Q�Ƃł���D
static void
save_data(int file_number)
{
	static int   fd;
	static char  buf[256];  // static �����Ȃ��ƁC�T�C�Y���傫���Ƃ��ɃR���p�C���G���[���N����
	int  i;
	char filename[20];
	
	sprintf(filename, "data%d.txt", file_number);
	fd= open(filename, OptWrite);

	if( fd == -1 ){
		printf(" File open error! \n");
		exit(-1);
	}
	
	sprintf(buf, "time[ms]  LW_traj[mm]  LW[mm]  RW_traj[mm]  RW[mm] \r\n");
	write( fd, buf, strlen(buf) );
	
	for( i=0; i<=Ct.dTime.stopVal; i++ ){
		sprintf(buf, "%d  %d  %d  %d  %d \r\n", 
			i*SampTime,
			(int)(1000*( Ki.traj[LW][i].d )*Ki.r),  
			(int)(1000*( Ct.data[LW][i].d )*Ki.r),
			(int)(1000*( Ki.traj[RW][i].d )*Ki.r), 
			(int)(1000*( Ct.data[RW][i].d )*Ki.r)
		);
		write( fd, buf, strlen(buf) );
	}
	
	close(fd);
}
