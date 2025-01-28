// ************************************************
// *** PSDセンサ 読み取りプログラム ***
// ************************************************
#define PIN_ANA   5   // 5番ピンからアナログ入力

#define T_val   300   // 閾値のアナログ値

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200) ;     // 115200bpsでシリアル通信のポートを開きます
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(PIN_ANA);
  val= PSDread(PIN_ANA);
  Serial.println(val);
/*  Serial.println("\n");  
  int val_100 = PSDread(PIN_ANA);   // PSDセンサから読込む
  Serial.println(val_100);    // シリアルモニタに表示させる
  Serial.println("\n"); 
*/
  delay(200) ;             // 200ms時間待ち
}
int PSDread(int PinNo){ //PSDの読み取り
  long ans=0;
  for(int i=0; i<100; i++){
    ans=ans +analogRead(PinNo);
  }
  return ans/100;
}
