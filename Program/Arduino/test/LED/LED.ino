//setup - Arduinoを起動した際に最初に呼ばれる処理
void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT); //デジタル13pinを使う
}
//loop - setupの処理が終わった後，このloopの中の処理を電源が切れるまでずっと繰り返す
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13,HIGH);//デジタルpin13出力ON
  delay(1000);//1秒待つ
  digitalWrite(13,LOW);//デジタルpin13出力OFF
  delay(1000);//1秒待つ
  
}
