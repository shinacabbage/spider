#include "ServoMovingList.h"
#include <Servo.h>
/*
 四つ足クモ型のサーボの角度パターン設定するクラス
 好きなモーションの関数をloop内で実行すればその通り各関節の角度を制御してくれる
 */

class Spider{
  private:
  //サーボを接続するピンの設定
  int rft_pin = 5;    //右前関節　95左-右0 ①
  int rff_pin = 4;    //右前足　　0全閉-全開150　②
  int rbt_pin = 7;    //右後関節　0左-右95　③
  int rbf_pin = 6;    //右後足　　150全閉-全開0　④
  int lft_pin = 9;    //左前関節　95左-右0　⑤
  int lff_pin = 8;    //左前足　　150全閉-全開0　⑥
  int lbt_pin = 11;   //左後関節　0左-右95　⑦
  int lbf_pin = 10;   //左後足　　0全閉-全開150　⑧

  ServoMovingList *sml;//サーボの動きリスト
  bool setting=false;
  int first_deg[8]={};
  public:
  Spider(int firstdeg_[])
  {
    int pin[]={rft_pin,rff_pin,rbt_pin,rbf_pin,lft_pin,lff_pin,lbt_pin,lbf_pin};
    for(int i=0;i<8;i++)first_deg[i]=firstdeg_[i];
    sml=new ServoMovingList(pin,first_deg);
    sml->servoSwich(true,true,true,true,true,true,true,true);
  }
  ~Spider();
  void MoveForward();
  void MoveBack();
  void StandUp();
  void SitDown();
  void ResetSetting(int[]);
  void Calibration();
};
void Spider::MoveForward()
{
  if(!setting){
    int deg[]={60,150,60,100,35,110,45,100};//①
    sml->appendNode(deg,0);
    int deg2[]={95,90,0,70,95,110,95,60};//②
    sml->appendNode(deg2,0);
    int deg3[]={70,60,20,100,0,20,60,100};//③
    sml->appendNode(deg3,0);
    int deg4[]={40,60,20,70,0,40,95,60};//④
    sml->appendNode(deg4,0);
    sml->loop_on();//ループする      
    setting=true;
  }
  sml->playNodes();
}
//サーボの位置調整用
void Spider::Calibration()
{
  if(!setting){
    int deg[]={95,150,0,0,0,0,95,150};//
    sml->appendNode(deg,0);
    setting=true;
  }
  sml->playNodes();
}
//立ち上がる
void Spider::StandUp()
{
  if(!setting){
//    sml->reset(first_deg);
    int deg6[]={90,75,45,75,35,110,45,75};//
    sml->appendNode(deg6,0);
    setting=true;
  }
  sml->playNodes();
}
//座る
void Spider::SitDown()
{
  if(!setting){
//    sml->reset(first_deg);
    int deg6[]={90,0,45,150,35,150,45,0};//
    sml->appendNode(deg6,0);
    setting=true;
  }
  sml->playNodes();
}
void Spider::MoveBack()
{
  if(!setting){
    int deg[]={40,60,20,70,0,40,95,30};//
    sml->appendNode(deg,0);
    int deg2[]={70,60,20,100,0,20,60,70};//
    sml->appendNode(deg2,0);
    int deg3[]={95,90,0,70,95,110,95,30};//
    sml->appendNode(deg3,0);
    int deg4[]={60,150,60,100,35,110,45,70};//
    sml->appendNode(deg4,0);
    sml->loop_on();//ループする      
    setting=true;
  }
  sml->playNodes();
}
//別モーションを切り替えるときに実行する。
void Spider::ResetSetting(int firstdeg_[])
{
   setting=false;
   sml->reset(firstdeg_);
   sml->servoSwich(true,true,true,true,true,true,true,true); 
}
