// ************************************************
// *** 超音波センサ(HC-SR04) 読み取りプログラム ***
// ************************************************
#define PIN_TRG   2
#define PIN_ECHO  3
#define PIN_PWR   4

#define LEDPIN  13    // 13番ピンに出力（LED点灯）
#define T_dist  10.0  // [cm] 閾値の距離

int interval;
double distance;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_TRG,OUTPUT);
  pinMode(PIN_ECHO,INPUT);
  pinMode(PIN_PWR,OUTPUT);
  Serial.begin(115200);
  
  digitalWrite(PIN_TRG,LOW);

  // for Output
  // LEDに接続ピンをデジタル出力に設定
  pinMode(LEDPIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(PIN_PWR,HIGH);

  // 距離計測
  // trigger pulse output (10us)
  digitalWrite(PIN_TRG, HIGH);
  delayMicroseconds(9);
  digitalWrite(PIN_TRG, LOW);
  // echo pulse width measure
  interval = pulseIn(PIN_ECHO, HIGH, 30000);  // 時間計測
  distance = interval * 0.017;  // 時間=>距離に変換 [cm]
                                // 340[m/sec]/2 = 0.017[cm/μs]
  // send result
  Serial.print("pulse width =");
  Serial.print(interval);
  Serial.print("[us] distance =");
  Serial.print(distance);
  Serial.print("[cm]\n");

  delay(20);  // adjustable  

  digitalWrite(PIN_PWR,LOW);
  // 出力設定（LED）
  if( distance <= T_dist ){
    digitalWrite( LEDPIN, HIGH );
  }
  else{
    digitalWrite( LEDPIN, LOW );
  }
}
