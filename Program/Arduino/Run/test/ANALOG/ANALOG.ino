int val=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9800);//モニターに出力するための設定
}

void loop() {
  // put your main code here, to run repeatedly:
  //ANALOG INの0番ピンからデータを受け付ける.
  val=analogRead(5); 
  Serial.println(val/4);//入力された値をモニターに出力;
  analogWrite(3,val/4);//pin3にval/4のアナログを出力する
  //一部のデジタルpinがアナログ信号を扱えるから．pin3,5,6,9,10,11
  //アナログの出力と入力はsetup関数に設定は必要ない.入力は-~1023,出力0~255
  delay(1000);
}
