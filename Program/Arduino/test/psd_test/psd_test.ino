// ************************************************
// *** PSDセンサ 読み取りプログラム ***
// ************************************************
#define PIN_ANA   0   // 0番ピンからアナログ入力

#define LEDPIN  13    // 13番ピンに出力（LED点灯）
#define T_val   300   // 閾値のアナログ値

int val;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200) ;     // 115200bpsでシリアル通信のポートを開きます

  // for Output
  // LEDに接続ピンをデジタル出力に設定
  pinMode(LEDPIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //int val;
  val = analogRead(PIN_ANA);   // PSDセンサから読込む
  Serial.println(val);    // シリアルモニタに表示させる

  // 出力設定（LED）
  if (val >= T_val){
    digitalWrite(LEDPIN, HIGH);
  }
  else{
    digitalWrite(LEDPIN, LOW);
  }

  delay(200) ;             // 200ms時間待ち
}
