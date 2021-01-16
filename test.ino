#include "Spider.h"
#include <Servo.h>

Spider *sp;//
void setup() { 
  //初期化
  int firstdeg[]={90,75,45,75,35,110,45,75};//90,0,45,150,35,150,45,0
  sp=new Spider(firstdeg);
}

void loop() {
    sp->MoveForward();//前進
    //sp->StandUp();//立ち上がる
    //sp->SitDown();//座る
    //sp->Calibration();//サーボの角度を調整　サーボのアームを外してこの部分だけ実行すればサーボの角度が調整される。
}
