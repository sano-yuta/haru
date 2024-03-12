#include "Controller.h"
#include "RoboClaw_Mbed_Studio.h"
#include "haru_define.h"
#include "mbed.h"
#include "platform/mbed_thread.h"
#include <cstdint>
#include"CommTalk.h"


Serial pc(USBTX, USBRX, 115200);
Serial roboclaw1(ROBOCLAW1_SERIALTX, ROBOCLAW1_SERIALRX, 115200);

Controller Controller(P7_4, P7_5, 115200);

Serial upCarriageSerial(UP_CARRIAGE_TX, UP_CARRIAGE_RX, 115200);
Serial underCarriageSerial(UNDER_CARRIAGE_TX, UNDER_CARRIAGE_RX, 115200); 

DigitalOut led_1(LED1);
DigitalOut led_2(LED2);
DigitalOut led_3(LED3);
DigitalOut led_4(LED4);

DigitalOut den_ben_1(DEN_BEN1);
DigitalOut den_ben_2(DEN_BEN2);
DigitalOut den_ben_3(DEN_BEN3);
DigitalOut den_ben_4(DEN_BEN4);
DigitalOut den_ben_5(DEN_BEN5);

DigitalOut p_led_user(PIN_LED_USER);
DigitalOut p_led_red(PIN_LED_RED);
DigitalOut p_led_green(PIN_LED_GREEN);
DigitalOut p_led_brue(PIN_LED_BLUE);

DigitalIn sw_up(SW_UP);
DigitalIn sw_down(SW_DOWN);
DigitalIn sw_right(SW_RIGHT);
DigitalIn sw_left(SW_LEFT);
DigitalIn but(USER_BUTTON);

DigitalIn limit1(LIMIT_SW1);
DigitalIn limit2(LIMIT_SW2);
DigitalIn limit3(LIMIT_SW3);
DigitalIn limit4(LIMIT_SW4);
DigitalIn limit5(LIMIT_SW5);//下のリミット
DigitalIn limit6(LIMIT_SW6);

DigitalIn kouden1(KOUDEN1);
DigitalIn kouden2(KOUDEN2);
DigitalIn kouden3(KOUDEN3);

RoboClaw roboclaw_1(129, &roboclaw1, 115200);
RoboClaw roboclaw_2(128, &roboclaw1, 115200);

Ticker fripper;

char receive;
char send;
int i = 0;
int data = 0;
int asi = 0;
int souko = 0;
int tenzi_1 = 0;
int tansu = 0;
int tenzi_2 = 0;
int bonbori = 0;

int speed1 = 0;
int speed2 = 0;
int speed3 = 0;

int posi = 0;

bool flag_h_go = false;
bool flag_d_go = false;
bool flag_S_go = false;
bool flag_M_go = false;
bool flag_10ms = false;

bool fall_button_maru = false;
bool fall_button_batu = false;
bool fall_button_shikaku = false;
bool fall_button_sankaku = false;
bool fall_button_right = false;
bool fall_button_down = false;
bool fall_button_left = false;
bool fall_button_up = false;
bool fall_button_R1 = false;

unsigned int button_state;
unsigned int add_button_state;
static uint32_t preswstate;


void warikomi() {
  static uint8_t count = 0;
  //  count += 10;
  //  if (!(count % 10)) {
  //    flag_10ms = true;
  //    count = 0;
  //  }
  flag_10ms = true;
}

void test_led(int x){
    p_led_red = 1;
    wait_us(x);
    p_led_red = 0;
}
void air_1(int x){
    den_ben_1 = 1;
    wait_us(x);
    den_ben_1 = 0;
}
void air_2(int x){
    den_ben_2 = 1;
    wait_us(x);
    den_ben_2 = 0;
}
void air_3(int x){
    den_ben_3 = 1;
    wait_us(x);
    den_ben_3 = 0;
}
void air_4(int x){
    den_ben_4 = 1;
    wait_us(x);
    den_ben_4 = 0;
}
void air_5(int x){
    den_ben_5 = 1;
    wait_us(x);
    den_ben_5 = 0;
}

int main() {
  fripper.attach(&warikomi, 0.1);
  pc.printf("init\n");
  Controller.init(CONCOM_AVAILABLETIME, CONCOM_INTERVAL);
  pc.printf("con_ok\n");
  Controller.init(800,100);
  /*
  pc.printf("a\n");
  data = 1;
  send = (char)data;
   underCarriageSerial.putc(send);
    p_led_red = 1;
    pc.printf("%d",send);
    wait_us(3000000);
    p_led_red = 0;
    */
  while (true) { // 条件式がtrueの間、処理を繰り返す  
    bool update(uint32_t);
    static unsigned int pre_button_state = 0, preAddButtonState = 0;
    if (flag_10ms) {
      Controller.update(); //コントローラー情報アップデート
      if (Controller.getComCheck()) { //コントローラー通信チェック
        button_state = Controller.getButtonState();
      } else {
        button_state = pre_button_state;
      }
      // button_state = Controller.getButtonState();

      fall_button_maru =
          (((button_state & MASK_BUTTON_MARU) !=      //違うとき
            (pre_button_state & MASK_BUTTON_MARU)) && //変わったかどうか
           ((pre_button_state & MASK_BUTTON_MARU) ==
            MASK_BUTTON_MARU)) //最初が１だったのか０だったのか
              ? true
              : false;

      fall_button_batu =
          (((button_state & MASK_BUTTON_BATU) !=      //違うとき
            (pre_button_state & MASK_BUTTON_BATU)) && //変わったかどうか
           ((pre_button_state & MASK_BUTTON_BATU) ==
            MASK_BUTTON_BATU)) //最初が１だったのか０だったのか
              ? true
              : false;

      fall_button_shikaku =
          (((button_state & MASK_BUTTON_SHIKAKU) !=      //違うとき
            (pre_button_state & MASK_BUTTON_SHIKAKU)) && //変わったかどうか
           ((pre_button_state & MASK_BUTTON_SHIKAKU) ==
            MASK_BUTTON_SHIKAKU)) //最初が１だったのか０だったのか
              ? true
              : false;

      fall_button_sankaku =
          (((button_state & MASK_BUTTON_SANKAKU) !=      //違うとき
            (pre_button_state & MASK_BUTTON_SANKAKU)) && //変わったかどうか
           ((pre_button_state & MASK_BUTTON_SANKAKU) ==
            MASK_BUTTON_SANKAKU)) //最初が１だったのか０だったのか
              ? true
              : false;

      fall_button_right =
          (((button_state & MASK_BUTTON_RIGHT) !=      //違うとき
            (pre_button_state & MASK_BUTTON_RIGHT)) && //変わったかどうか
           ((pre_button_state & MASK_BUTTON_RIGHT) ==
            MASK_BUTTON_RIGHT)) //最初が１だったのか０だったのか
              ? true
              : false;

      fall_button_down =
          (((button_state & MASK_BUTTON_DOWN) !=      //違うとき
            (pre_button_state & MASK_BUTTON_DOWN)) && //変わったかどうか
           ((pre_button_state & MASK_BUTTON_DOWN) ==
            MASK_BUTTON_DOWN)) //最初が１だったのか０だったのか
              ? true
              : false;

      fall_button_left =
          (((button_state & MASK_BUTTON_LEFT) !=      //違うとき
            (pre_button_state & MASK_BUTTON_LEFT)) && //変わったかどうか
           ((pre_button_state & MASK_BUTTON_LEFT) ==
            MASK_BUTTON_LEFT)) //最初が１だったのか０だったのか
              ? true
              : false;

      fall_button_up =
          (((button_state & MASK_BUTTON_UP) !=      //違うとき
            (pre_button_state & MASK_BUTTON_UP)) && //変わったかどうか
           ((pre_button_state & MASK_BUTTON_UP) ==
            MASK_BUTTON_UP)) //最初が１だったのか０だったのか
              ? true
              : false;

      fall_button_R1 =
          (((button_state & MASK_BUTTON_R1) !=      //違うとき
            (pre_button_state & MASK_BUTTON_R1)) && //変わったかどうか
           ((pre_button_state & MASK_BUTTON_R1) ==
            MASK_BUTTON_R1)) //最初が１だったのか０だったのか
              ? true
              : false;
                   

      pre_button_state = button_state;

     //////////////////////////////////////////テスト用////////////////////////////////////////////////////
    /* 
      if(limit5 == 0){//動作テスト
          pc.printf("");
          den_ben_1 = 0;
      }else{
        //  pc.printf("back");
      }
      if(fall_button_up == 1){
          pc.printf("ok");
      }    
       pc.printf("ok");
       */
     //   pc.printf("%d\t",(int)but);

      //  if(but == 0){
       //     test_led(1000000);
       // }
      // pc.printf("iii\t");
      
      //   pc.printf("%d,%d\t",(int)kouden1,(int)kouden2);//kouden1 = 1, kouden2 = 0
     //  pc.printf("%d,%d\t",(int)den_ben_4,(int)den_ben_5);

    // pc.printf("%d,%d,%d,%d,%d,%d\n",(bool)limit1,(bool)limit2,(bool)limit3,(bool)limit4,(bool)limit5,(bool)limit6); //ok
     //１＝左リミット開，２＝死，３＝ひな上(1→0になるが0に張り付く)，４＝右リミット閉，５＝右リミット開，６＝左リミット閉

     //   pc.printf("%d\t",(int)but);
    //  pc.printf("%d\t",(bool)limit5);
    //  pc.printf ("%d,%d,%d,%d\t",fall_button_sankaku,fall_button_shikaku,fall_button_maru,fall_button_batu);
    //  pc.printf ("%d,%d,%d,%d\t",(int)fall_button_up,(int)fall_button_down,(int)fall_button_left,(int)fall_button_right);
    //  pc.printf("%d\t",(int)fall_button_R1);
      
      
      if ((int)fall_button_maru == 1) { //台座アーム昇降//速度は100で向きが逆
        flag_d_go = true;
        pc.printf("Daiza.UP\n");
        speed1 = -100;
      }else if ((int)fall_button_batu == 1) {
        flag_d_go = true;
        pc.printf("Daiza.DOWN\n");
        speed1 = 100;
      }else if ((int)limit2 == 0) {
        flag_d_go = false;
      }else if ((int)limit3 == 0) { 
        flag_d_go = false;

      if ((int)fall_button_maru == 1) { //台座アーム昇降//速度は100で向きが逆
        flag_d_go = true;
        pc.printf("Daiza.UP\n");
        speed2 = 100;
      }else if ((int)fall_button_batu == 1) {
        flag_d_go = true;
        pc.printf("Daiza.DOWN\n");
        speed2 = -100;
      }else if ((int)limit1 == 0) { 
        flag_d_go = false;
      }else if ((int)limit4 == 0) {
        flag_d_go = false;
      }
     if (flag_d_go == false) {
        roboclaw_2.SpeedM1(0);
        roboclaw_2.SpeedM2(0);
      }
     if (flag_d_go == true) {
        roboclaw_2.SpeedM1(speed1);
        roboclaw_2.SpeedM2(speed2);
      }

     if((int)fall_button_sankaku == 1){ //ひな人形アーム昇降//速度は2500で振動あり
        flag_h_go = true;//ひな人形
        pc.printf("Hina.UP\n");
        speed3 = 2500;
     }else if ((int)fall_button_shikaku == 1) {
        flag_h_go = true;//
        pc.printf("Hina.DOWN\n");
        speed3 = -2500;
      }else if ((int)limit6 == 0) { 
        flag_h_go = false;
      }else if ((int)limit5 == 0) {
        flag_h_go = false;
      }
     if (flag_h_go == false) {
        roboclaw_1.SpeedM1(0);
      }
     if (flag_h_go == true) {
        roboclaw_1.SpeedM1(speed3);
      }
     
     if((int)fall_button_right == 1){ //ひな人形アーム　開 //電圧不足
        den_ben_1 = 0;//右
        den_ben_2 = 0;//左
        pc.printf("nairihina.OPEN\n");
     }else if((int)fall_button_left == 1){ //ひな人形アーム　閉　//電圧不足
        den_ben_1 = 1;//右
        den_ben_2 = 1;//左
        pc.printf("nairihina.CLOSE\n");//
     }
/*
     if(kouden1 == 0){
        den_ben_1 = 1;
     }else{
         den_ben_1 = 0;
     }
     if(kouden2 == 1){
         den_ben_2 = 1;
     }else{
         den_ben_2 = 0;
     }
*/
  //   if((int)fall_button_left == 1){ //ひな人形三人官女側アーム開閉
   //     den_ben_2 = 0;
    //    pc.printf("sannninnkanjyo.OPEN\n");
    // }else{
     //   den_ben_2 = 1;
   //  }
   
     if((int)fall_button_up == 1) { //倒すエアシリ　
        pc.printf("knock down.OPEN\n");
        air_3(3000000);//右上
     }else{
       den_ben_3 = 0;
     }
     if ((int)fall_button_down == 1) { //整えるエアシリ　
       pc.printf("make up.OPEN\n");
       air_4(3000000);//右下
     }else{
        den_ben_4 = 0;
     }
     if((int)fall_button_R1 == 1){ //ひきづるエアシリ　
        den_ben_3 = 1;//左
     }else{
      //   den_ben_3 = 1;
     }
     
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
      if((int)fall_button_maru == 1){//右開く
        den_ben_1 = 0;
      }
      if((int)fall_button_batu == 1){//右閉じる
        den_ben_1 = 1;
      }
      if((int)fall_button_sankaku == 1){//左開く
        den_ben_2 = 0;
      }
      if((int)fall_button_shikaku == 1){//左閉じる
        den_ben_2 = 1;
      }

       if ((int)fall_button_up == 1) { //ひな人形　上昇
        flag_S_go = true;
        pc.printf("S.UP\n");
        speed1 = -1000;
      } 
      
      // else if ((int)fall_button_down == 1) { //ひな人形　下降
       // flag_S_go = true;
       // pc.printf("S.DOWN\n");
       // speed1 = 1000;
        
      } else if ((int)limit6 == 0) { //リミットストップ
        flag_S_go = false;
      }

      if (flag_S_go == false) { //STOP
        roboclaw1.SpeedM1(0);
        // pc.printf("S.STOP\n");
        p_led_user = 0;
      }
      if (flag_S_go == true) {//GO
        roboclaw1.SpeedM1(speed1);
       // pc.printf("go");
        p_led_user = 1;
      }
     */
       /*
      if(limit1 == 1){//送信テスト
      
      pc.printf("send_ok");
      p_led_user = 1;
       int data;
       char send;
       data = 97;
       send = (char)data;
       upCarriageSerial.putc(send);
       
      bool sendData({1,2,3,4,5},5);
      } 
       */
       /*
      if(limit2 == 0){//受信テスト
      
      p_led_user = 1;
      pc.printf("recive_ok");
      
       char receive;
       int data = 0;
       receive = underCarriageSerial.getc();
       data = (int)receive;
       pc.printf("%d", data);
      if(data == 97){  
        pc.printf("check_ok");
       }else if(data > 0){
         pc.printf("%d", data);
       }else{
           pc.printf("bad");
       }
       
       pc.printf("%d",data);
     }
     */
    // pc.printf ("%d,%d,%d,%d\t",fall_button_right,fall_button_down,fall_button_maru,fall_button_batu);

     /*

      if ((int)fall_button_maru == 1) { //サブ昇降
        flag_S_go = true;
        pc.printf("S.UP\n");
        speed1 = -1000;
      } else if ((int)fall_button_batu == 1) {
        flag_S_go = true;
        pc.printf("S.DOWN\n");
        speed1 = 1000;
      } else if ((int)limit1 == 0) { 
        flag_S_go = false;
      }
      else if ((int)limit2 == 0) {
        flag_S_go = false;
      }

      if (flag_S_go == false) {
        roboclaw1.SpeedM2(0);
        // pc.printf("S.STOP\n");
        p_led_user = 0;
      }
      if (flag_S_go == true) {
        roboclaw1.SpeedM2(speed1);
        pc.printf("go");
        p_led_user = 1;
      }

      if ((int)fall_button_shikaku == 1) { //サブ開閉
        den_ben_1 = 0;
        p_led_red = 1;
        pc.printf("S_1.OPEN\n");
      }
      if ((int)fall_button_sankaku == 1) {
        den_ben_1 = 1;
        p_led_red = 0;
        pc.printf("S_1.CLOSE\n");
      }
      if ((int)fall_button_up == 1) { //サブ開閉
        den_ben_2 = 0;
        p_led_green = 1;
        pc.printf("S_2.OPEN\n");
      }
      if ((int)fall_button_left == 1) {
        den_ben_2 = 1;
        p_led_green = 0;
        pc.printf("S_2.CLOSE\n");
      } */
     /*  if ((int)limit3 == 0) { //メイン昇降
            flag_M_go = false;
          }
       if ((int)limit4 == 0) {
            flag_M_go = false;
          } 
       if ((int)fall_button_right == 1) {
        flag_M_go = true;
        pc.printf("M.UP\n");
        posi = 500;
      } else if ((int)fall_button_down == 1) {
        flag_M_go = true;
        pc.printf("M.DOWN\n");
        posi = 0;
      } 
      
       if (flag_M_go == false) {
            roboclaw1.SpeedAccelDeccelPositionM1(0,100,0,0,true);
            pc.printf("M.STOP\n");
            p_led_green = 0;
          }
       if (flag_M_go == true) {
        roboclaw1.SpeedAccelDeccelPositionM1(0, 500, 0, posi, false);
        p_led_green = 1;
        pc.printf("ok\n");
      }

      if ((int)fall_button_up == 1) { //メイン開閉
        den_ben_3 = 1;
       // p_led_brue = 1;
       // pc.printf("M.OPEN\n");
      }
      if ((int)fall_button_left == 1) {
        den_ben_3 = 0;
        p_led_brue = 0;
        pc.printf("M.CLOSE\n");
      }
     
      // if (limit1 == 1) {
      //   pc.printf("1");
      // }

      // if (limit2 == 1) {
      // pc.printf("8");
      //  led_3 = 0;
      // led_2 = 0;
      // led_1 = 0;

      // else{
      // pc.printf("4");
      // led_3 = 1;
      // led_2 = 1;
      // led_1 = 1;

      // if (limit3 == 1) {
      //  pc.printf("3");
      //}
      // if (limit4 == 1) {
      //  pc.printf("4");
      // }

       // pc.printf
       // ("%d,%d,%d,%d\t",fall_button_right,fall_button_down,fall_button_maru,fall_button_batu);
      pc.printf("%d,%d,%d,%d\t", (bool)limit1, (bool)limit2, (bool)limit3,
                (bool)limit4);
      //pc.printf("s_go:%d,m_go:%d,speed:%d,possi:%d\n", flag_S_go, flag_M_go,
      //          speed1, posi);
      
      
      flag_10ms = false;
      */
     /*
     pc.printf("Daiza.UP\n");
        wait_us(5000000); //1000000で1秒
        pc.printf("knock down.OPEN\n");
        wait_us(5000000);
        pc.printf("make up.OPEN\n");
        wait_us(3000000);
     */
     /*
     receive = underCarriageSerial.getc();
     data = (int)receive;

     pc.printf("%d",data);
     if(data == 1){
      souko = 1;
      pc.printf("souko");
     }else if(data == 2){
      tenzi_1 = 1;
      pc.printf("tenzi_1");
     }else if(data == 3){
      tansu = 1;
      pc.printf("tansu");
     }else if(data ==4){
      tenzi_2 = 1; 
      pc.printf("tennzi_2");
     }
     */


     flag_10ms = false;
    }
    }
  }
} 
