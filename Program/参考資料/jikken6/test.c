// **************************************************
// *** H8 ライントレース  　　　　　　　　　　　　***
// ***       　　　　　　 　　　　　2012.7.4  　　***
// ***       　　　　　　 　　produced by Y. Mori ***
// ***	       special thanks to A. Ruike, S. Kido ***
// **************************************************
// コメント文のページ番号は，ハードウェアマニュアル h8_3069f.pdf を参照
// #if 0: コメントアウトされる，#if 1: コメントが外れる

#include <mes2.h>
#include "r3069.h"
#include "common.h"

#define  LD  -1  // 左モータの回転方向
#define  RD   1  // 右モータの回転方向

static int  End_flag;   // プログラムの終了フラグ
static int  Feed_flag;  // フィードバック関数内での制御フラグ
static int  All_black_flag;  // ライントレース用フラグ

ct_sharedType  Ct;

// --------------
// --- 関数群 ---
// --------------
static void  line_trace( void );

static void  init_settings( void );
static void  initialize_para( void );
static void  cool_down( void );

static void  motor( int no, int duty );


// ************************
// *** 割り込み処理関数 ***
// *** リセットボタン用 ***
// ************************
#pragma interrupt
void
prg_end(void)
{
	STOP_FEED;  // feedback関数のクロック入力禁止

	load_segment( 7 );
	cool_down();
	End_flag= 1;
}


// ************************
// *** 割り込み処理関数 ***
// *** feedback 関数    ***
// ************************
#pragma interrupt
void
feed( void )
{
	int jnt;

	load_segment(42);  // ベクタ番号（大域変数を使うときに必要），pp.143

	// --------------------------------------
	// --- エンコーダを用いた閉ループ制御 ---
	// --------------------------------------
	if( Feed_flag == ENCO ){
	}
	// ----------------------------------------------
	// --- フォトインタラプタを用いた開ループ制御 ---
	// ----------------------------------------------
	else if( Feed_flag == PHOTO ){

		// エンコーダのカウント停止
		STOP_ENCO_COUNT;

		line_trace();
	}
	// ----------------
	// --- 静止状態 ---
	// ----------------
	else if( Feed_flag == STOP ){

		// モータ停止
		for( jnt=LW; jnt<=RW; jnt++ ){
			motor( jnt, 0 );
		}

		// エンコーダのカウント停止
		STOP_ENCO_COUNT;
	}

	// TSCR:タイマコントロールステータスレジスタ pp.464
	// CMFA:TCORA のコンペアマッチの発生を示すステータスフラグ pp.465
	OCT_ITU3.TCSR.BIT.CMFA = 0;
}


// *****************
// *** main 関数 ***
// *****************
int
main( void )
{
	int  lap_flag;//周カウンタ

	init_settings();    // H8の初期設定
	initialize_para();  // パラメータの設定
        All_black_flag = 0;  // フラグを初期化

	START_FEED;	 // フィードバック関数スタート（割り込み処理開始）

	printf("\r Press Start Button.\r\n");
	while(P6.DR.BIT.B0);



	// ----------------------
	// --- ライントレース ---
	// ----------------------

	for ( lap_flag=0 ; lap_flag<4 ; lap_flag++) //3周で終了
	{
		Feed_flag = PHOTO;// フィードバック関数内での制御フラグ

		while( All_black_flag == 0 )
		{	//コース1周内-->line_traceで処理（whileを抜ける条件は十字マーカー）
			sleep(1);// 1[ms] 値は変更しない
		}

		if(lap_flag <3 ){
//			sleep(20);
			while( Photo_1 == Black && Photo_2 == Black && Photo_3 == Black && Photo_4 == Black){
				motor(LW, 250*LD);
				motor(RW, 250*RD);
			}
		}

		All_black_flag = 0;  // フラグを戻す
	}

	Feed_flag = STOP ;// フィードバック関数内での制御フラグ

	cool_down();
	exit(0);
}


// ----------------------
// --- ライントレース ---
// ----------------------
static void
line_trace( void )
{

	// --- ロボットの移動 ---1周分

	// 注意： motor関数出力時に，モータの向き(LD, RD) を掛けること
	// 例 motor( LW, 20*LD ), motor( RW, 20*RD )

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
	All_black_flag = 1 ; //1周の終わり（※条件の判定後、代入すること）
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

 //1周の終わり（※条件の判定後、代入すること）
}

// ------------------------
// --- H8の初期設定関数 ---
// ------------------------
static void
init_settings( void )
{
	// *** NMI 設定（リセット用）***
	SYSCR.BIT.NMIEG= 0;
	set_handler( 7, prg_end );

	// *** ポート入出力設定 (for motor, photo-sensor) ***
	P4.DDR=0xff;  // output (for motor)
	P5.DDR=0x00;  // input  (for photo-sensor)

	// *** 8bit timer pwm settings (for motor) 3069マニュアル pp.482 参照 ***
	// [φ/8192 ->3] ,[φ/64 ->2] ,[φ/8 ->1]
	OCT_ITU0.TCR.BIT.CKS= 2;
	OCT_ITU1.TCR.BIT.CKS= 2;
	//OCT_ITU2.TCR.BIT.CKS= 2;  // 予備ポート

	// TCNTをコンペアマッチAでクリア: CCLR0=1, CCLR1=0, pp.462
	OCT_ITU0.TCR.BIT.CCLR= 1;
	OCT_ITU1.TCR.BIT.CCLR= 1;
	//OCT_ITU2.TCR.BIT.CCLR= 1;  // 予備ポート

	// コンペアマッチBで1出力: OIS3 と OIS2 を 01 に設定, pp.467
	OCT_ITU0.TCSR.BIT.OIS32= 1;
	OCT_ITU1.TCSR.BIT.OIS32= 1;
	//OCT_ITU2.TCSR.BIT.OIS32= 1;  // 予備ポート

	// コンペアマッチAで0出力: OS1 と OS0 を 10 に設定, pp.468
	OCT_ITU0.TCSR.BIT.OS10= 2;
	OCT_ITU1.TCSR.BIT.OS10= 2;
	//OCT_ITU2.TCSR.BIT.OS10= 2;  // 予備ポート

	// Duty
	OCT_ITU0.TCORB= 0;
	OCT_ITU1.TCORB= 0;
	//OCT_ITU2.TCORB= 0;  // 予備ポート

	// TCRB: タイムコンスタントレジスタA
	// 周波数: 20MHz(Clock)/64(φ)/250(TCORA)= 1.25kHz
	OCT_ITU0.TCORA= Max_duty;
	OCT_ITU1.TCORA= Max_duty;
	//OCT_ITU2.TCORA= 250;  // 予備ポート

	// *** ITU3 タイマ割り込み 8bit (for timer) ***
	// OCT_ITU3.TCORA: タイムコンスタントレジスタA pp.465
	// 1秒間に呼び出されるfeed関数の回数と同じ．
	// 20000000:システムクロック，8192:分周値(r3069.hの197行目 CKS:3 (CKS2=0, CKS1=1, CKS0=1))
	// 内部クロック=システムクロック/分周値 pp.463
	OCT_ITU3.TCORA= (20000000/8192)*SampTime/1000;
	OCT_ITU3.TCNT = 0;  // カウンタ, TCORAと8TCNTの値は常に比較されている．pp.460
	OCT_ITU3.TCSR.BYTE= 0x00;
	OCT_ITU3.TCR.BIT.CCLR = 1;  // カウンタクリア 1:コンペアマッチAによりクリア pp.462
	OCT_ITU3.TCR.BIT.CMIEA= 1;
	OCT_ITU3.TCR.BIT.CKS  = 3;  // タイマー開始 011: CK2=0, CK1=1, CK0=1 -> φ/8192, pp.463
	                            // タイマー停止 000: CK2=0, CK1=0, CK0=0 -> pp.463
	set_handler( 42, feed );
}


// --------------------------
// --- パラメータの初期化 ---
// --------------------------
static void
initialize_para( void )
{
	int  jnt;

	// --- フラグの初期化 ---
	End_flag= 0;
	Feed_flag= STOP;
	All_black_flag= 0;

	// -------------------
	// --- motorの設定 ---
	// -------------------
	MOTOR_0_RUN;
	MOTOR_1_RUN;
}


// ------------------------------
// --- プログラム終了時の処理 ---
// ------------------------------
static void
cool_down( void )
{
	STOP_FEED;  // feedback関数のクロック入力禁止  pp.462

	motor(LW, 0);
	motor(RW, 0);

	MOTOR_0_BREAK;
	MOTOR_1_BREAK;
}


// ----------------------
// --- モータ駆動関数 ---
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
		MOTOR_0_DUTY = Max_duty;  // 0:ストップ（ゆっくり止まる）, Max_duty:ブレーキ（急に止まる）
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
		MOTOR_1_DUTY = Max_duty;  // 0:ストップ（ゆっくり止まる）, Max_duty:ブレーキ（急に止まる）
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
