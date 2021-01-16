#include <Arduino.h>
#include <Servo.h>
/*
 　8つのサーボの角度をノードとして設定し、
 　順序角度を合わせていくクラス
 　ループフラグを立てておけば繰り返しも可能。
 */

unsigned long time;
unsigned long prev_time;

class ServoMovingList{
  private:
  int servo_sum=8;//サーボの個数
  struct ListNode
  {
    int number;  
    struct ListNode *next;  
    
    int deg[8]={};//目標とする角度
    int rot_time;//所要時間
  };
  //各サーボ
  Servo rft;
  Servo rff;
  Servo rbt;
  Servo rbf;
  Servo lft;
  Servo lff;
  Servo lbt;
  Servo lbf;
  
  ListNode *head;//先頭ノード
  ListNode *current_node;//現在のノード
  long passed_time=0;
  bool end_flg=true;//ノードの終了フラグ
  int servo_deg[8]={};//サーボの現状角度
  int tmp_servo_deg[8]={};//前のノードの現状角度
  int pin[8]={};//pin番号
  bool servo_reached[8]={};//到着フラグ
  bool servo_switch[8]={};//サーボのオンオフフラグ　falseにしておくと、動かない
  bool loopflg=false;//設定したノードの繰り返すかどうか
  int sp=4;//サーボのスピード
  
  public:
  ServoMovingList(int pin_[],int deg[])//double
  {
    head = NULL;
    for(int i=0;i<servo_sum;i++){
      //ピン番号の設定
      pin[i]=pin_[i];
      pinMode(pin[i],OUTPUT);
      //初期化
      servo_deg[i]=deg[i];
      tmp_servo_deg[i]=deg[i];
    }
    
    //初期角度に調整
    rft.attach(pin[0]);
    rff.attach(pin[1]);
    rbt.attach(pin[2]);
    rbf.attach(pin[3]);
    lft.attach(pin[4]);
    lff.attach(pin[5]);
    lbt.attach(pin[6]);
    lbf.attach(pin[7]);
    rft.write(servo_deg[0]);//95-0
    rff.write(servo_deg[1]);//0-95
    rbt.write(servo_deg[2]);//0-95
    rbf.write(servo_deg[3]);//95-0
    lft.write(servo_deg[4]);//95-0
    lff.write(servo_deg[5]);//95-0
    lbt.write(servo_deg[6]);//0-95
    lbf.write(servo_deg[7]);//0-95
    
    Serial.begin(9600);
  }
  ~ServoMovingList();

  int node_sum=0;
  void appendNode(int[],int);//double double
  void playNodes();
  void servoSwich(bool,bool,bool,bool,bool,bool,bool,bool);
  double getServoDeg(int);
  bool getEndFlg();
  void loop_on();
  void loop_off();
  void reset(int[]);
};
void ServoMovingList::reset(int deg[])
{
    head = NULL;

    for(int i=0;i<servo_sum;i++){
      //初期化
      servo_deg[i]=deg[i];
      tmp_servo_deg[i]=deg[i];
    }
    //最初の角度に
    rft.attach(pin[0]);
    rff.attach(pin[1]);
    rbt.attach(pin[2]);
    rbf.attach(pin[3]);
    lft.attach(pin[4]);
    lff.attach(pin[5]);
    lbt.attach(pin[6]);
    lbf.attach(pin[7]);
    rft.write(servo_deg[0]);//95-0
    rff.write(servo_deg[1]);//0-95
    rbt.write(servo_deg[2]);//0-95
    rbf.write(servo_deg[3]);//95-0
    lft.write(servo_deg[4]);//95-0
    lff.write(servo_deg[5]);//95-0
    lbt.write(servo_deg[6]);//0-95
    lbf.write(servo_deg[7]);//0-95
}
void ServoMovingList::loop_on()
{
  loopflg=true;
}
void ServoMovingList::loop_off()
{
  loopflg=false;
}
void ServoMovingList::servoSwich(bool f1,bool f2,bool f3,bool f4,bool f5,bool f6,bool f7,bool f8)
{
  servo_switch[0]=f1;
  servo_switch[1]=f2;
  servo_switch[2]=f3;
  servo_switch[3]=f4;
  servo_switch[4]=f5;
  servo_switch[5]=f6;
  servo_switch[6]=f7;
  servo_switch[7]=f8;
}
bool ServoMovingList::getEndFlg()
{
  return end_flg;
}
double ServoMovingList::getServoDeg(int num)
{
  return servo_deg[num];
}
void ServoMovingList::playNodes()
{
  //実行時間計測
   //Serial.print("Time: ");
   //time = millis();
   //passed_time+=time-prev_time;
   //Serial.println(passed_time);
   //prev_time=time;

   //各サーボの更新
   if(!end_flg){
    for(int i=0;i<servo_sum;i++){
      if(tmp_servo_deg[i]<(current_node->deg[i]))
      {
      if(!servo_reached[i])servo_deg[i]+=sp;
      if(servo_deg[i]>=(current_node->deg[0]))
      {
        servo_deg[i]=current_node->deg[i];
        servo_reached[i]=true;
      }
      }else if(tmp_servo_deg[i]>(current_node->deg[i]))
      {
        if(!servo_reached[i])servo_deg[i]-=sp;
        if(servo_deg[i]<=(current_node->deg[i]))
        {
          servo_deg[i]=current_node->deg[i];
          servo_reached[i]=true;
        }
      }
    }
    //書き込む
    rft.write(servo_deg[0]);//95-0
    rff.write(servo_deg[1]);//0-95
    rbt.write(servo_deg[2]);//0-95
    rbf.write(servo_deg[3]);//95-0
    lft.write(servo_deg[4]);//95-0
    lff.write(servo_deg[5]);//95-0
    lbt.write(servo_deg[6]);//0-95
    lbf.write(servo_deg[7]);//0-95

    delay(30);

    //ずれが生じた場合は修正
    for(int i=0;i<servo_sum;i++){
      if(abs((current_node->deg[i])-servo_deg[i])<0.001){
        servo_deg[i]=current_node->deg[i];
        servo_reached[i]=true;
        }
    }
        
   }//end_flg


   //スイッチ オフのサーボのフラグは常に立てる
   for(int i=0;i<servo_sum;i++){
      if(!servo_switch[i])servo_reached[i]=true;
    }

   //次のノードへ
   if(servo_reached[0]&&
      servo_reached[1]&&
      servo_reached[2]&&
      servo_reached[3]&&
      servo_reached[4]&&
      servo_reached[5]&&
      servo_reached[6]&&
      servo_reached[7])//(abs((current_node->deg[5])-servo_deg[5])<0.1)
   {

      if(current_node->next!=NULL){
      current_node=current_node->next;
      passed_time=0;
        for(int i=0;i<servo_sum;i++){
          tmp_servo_deg[i]=servo_deg[i];
          servo_reached[i]=false;
        }
      //Serial.println("next node ");
      }else{//次のノードがない
        if(loopflg)//ループの場合
        {
          current_node=head;//先頭ノードに移動
          passed_time=0;
          for(int i=0;i<servo_sum;i++){
            tmp_servo_deg[i]=servo_deg[i];
            servo_reached[i]=false;
          }
        }else{//一回限りの場合
          end_flg=true;
          for(int i=0;i<servo_sum;i++){
            tmp_servo_deg[i]=servo_deg[i];
          }
        Serial.println("oshimai!");
        }
      }
   }
}
void ServoMovingList::appendNode(int deg[],int time)
{
  ListNode *newNode;  
  ListNode *nodePtr;  

  // 新たなノードのメモリを割り当て、
  // valueにnumを代入、
  // nextにNULLをセット
  newNode = new ListNode;
  newNode->number = node_sum;//id登録
  newNode->next = NULL;

  end_flg=false;
  
  //設定角度の初期化
  for(int i=0;i<servo_sum;i++){
    newNode->deg[i]=deg[i];
    servo_reached[i]=false;
  }
  newNode->rot_time=time;  
  node_sum++;//ノード数を追加

  // リストにノードがなければ、
  // newNodeを最初のノードにする
  if (!head)
  {
     head = newNode;
     current_node=newNode;
     //if(loopflg)newNode->next = newNode;
  }
  else  // そうでなければ、最後にnewNodeを挿入する
  {
    // nodePtrをリストの先頭として初期化する
    nodePtr = head;

    // リストの最後のノード（nodePtr->nextがnull）を見つける
    // nodePtr->nextがnullであるとき、ループを出る
    while (nodePtr->next)
    {
      nodePtr = nodePtr->next;
    }
    // 最後のノードとしてnewNodeを挿入する
    nodePtr->next = newNode;
  }
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
