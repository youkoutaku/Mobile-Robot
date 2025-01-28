//***********************************************************
//***  閾値の調整するために測距モジュールセンサのデータを可視化するプログラム
//***  YANG(2022/11/30) バージョン1.1
//***********************************************************
//測距モジュールセンサは，10cmから80cmまで測れる．10cm内とデータが変になる．
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // put your main code here, to run repeatedly:
  int val;
  val = IDSread(0);
  
  Serial.print(val);
  Serial.print("  ");
  if( AnaToCm(val)!=0 ){
    Serial.print(AnaToCm(val)-10);
    Serial.print("~");
    Serial.print(AnaToCm(val));
    Serial.println("cm");
  
  }else{
    Serial.println("out of range");
  }

  delay(500);
}

//センサーからの読み込み処理（100回読み平均をとる）
int IDSread(int PinNo){
 long ans=0;
 int i;
 for(i=0; i<100; i++){
  ans=ans +analogRead(PinNo);
 }
 return ans/100;
}
//センサーから読込んだ値を距離(cm)に変換する処理
int AnaToCm(int analogValue){
  if(analogValue<85) return(0);
  if(analogValue>=470) return(10);
  if(analogValue>=260) return(20);
  if(analogValue>=165) return(30);
  if(analogValue>=126) return(40);
  if(analogValue>=105) return(50);
  if(analogValue>=85) return(60);
  return(-1);
}
}
