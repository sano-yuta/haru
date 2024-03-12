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

 bool readButton_bin(unsigned int ButtonNum);

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
static void daiza_up(int x){
    static bool daiza_sw_1 = true;
    if(daiza_sw_1){
      pc.printf("dai_up_ok\t");
      wait_us(x);
      den_ben_3 = 1;
      pc.printf("dai_up_fin\t");
      wait_us(3000000);//1000000 = 1秒//現在3秒
      flag_h_go = true;
      daiza_sw_1 = false; 
    }
}
static void daiza_down(int x,int y){
    static bool daiza_sw_2 = true;
    if(daiza_sw_2){
      pc.printf("dai_down_ok\t");
      wait_us(x);
      den_ben_4 = 1;
      wait_us(y);
      den_ben_4 = 0;
      den_ben_5 = 1;
      wait_us(y);
      den_ben_5 = 0;
      pc.printf("dai_down_fin\t");
      daiza_sw_2 = false; 
    }
}
static void hina_up(int x,int y){
    static bool hina_sw_1 = true;
    if(hina_sw_1){
      pc.printf("hina_up_ok\t");
      wait_us(x);
      den_ben_1 = 1;
      den_ben_2 = 1;
      pc.printf("hina_up_fin\t");
      wait_us(3000000);//1000000 = 1秒//現在3秒
      flag_d_go = true;
      hina_sw_1 = false; 
    }
}
static void hina_open(int x){
    static bool hina_sw_2 = true;
    if(hina_sw_2){
      pc.printf("hina_open_ok\t");
      den_ben_1 = 0;
      den_ben_2 = 0;
      wait_us(x);
      led_1 = 1;
      pc.printf("bon_ok\t");
      wait_us(3000000);
      hina_sw_2 = false;
    }
}
int main() {
  fripper.attach(&warikomi, 0.01);
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

      /*  if(but == 0){
            test_led(1000000);
        } */
      // pc.printf("iii\t");
      
     //  pc.printf("%d,%d\t",(int)den_ben_4,(int)den_ben_5);

    // pc.printf("%d,%d,%d\t",(int)kouden4,(int)kouden5,(int)kouden6);

    // pc.printf("%d,%d,%d\t",(bool)kouden1,(bool)kouden2,(bool)kouden3);//kouden1 = 1, kouden2 = 0
  //  pc.printf("%d\t",(bool)limit5);
    // pc.printf("%d,%d,%d,%d,%d\t",(bool)limit1,(bool)limit2,(bool)limit3,(bool)limit4,(bool)limit6); //ok
     //１＝左リミット開，２＝，３＝ひな下，４＝右リミット閉，５＝６＝ひな上
    // pc.printf("%d,%d,%d\t",(int)Controller.readButton_bin(MARU),(int)Controller.readButton_bin(BATU),(int)Controller.readButton_bin(SHIKAKU));

     //   pc.printf("%d\t",(int)but);
    //  pc.printf("%d\t",(bool)limit5);
     // pc.printf ("%d,%d,%d,%d\t",fall_button_sankaku,fall_button_shikaku,fall_button_maru,fall_button_batu);
    //  pc.printf ("%d,%d,%d,%d\t",(int)fall_button_up,(int)fall_button_down,(int)fall_button_left,(int)fall_button_right);
    //  pc.printf("%d\t",(int)fall_button_R1);
    // pc.printf("%d\t",);
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////     
      /////////////////////////////台座////////////////////////////////////////
      if (Controller.readButton_bin(MARU) == 1){//(kouden3 == 1) { //台座アーム昇降//速度は100で向きが逆
         if(limit4 == 1){
          roboclaw_2.SpeedM1(-100);
         }else if(limit4 == 0){
          roboclaw_2.SpeedM1(0);
         }
         if(limit2 == 1){
          roboclaw_2.SpeedM2(100);
         }else if(limit2 == 0){
          roboclaw_2.SpeedM2(0);
         }
         if(limit2 == 0 && limit4 == 0){
        //  pc.printf("dai_up_go\t");
          daiza_up(3000000);
         }
      }

      if (flag_h_go == true) { //おろす
        if(limit3 == 1){
         roboclaw_2.SpeedM1(100);
        }else if(limit3 == 0){
         roboclaw_2.SpeedM1(0);
        }
        if(limit1 == 1){
            roboclaw_2.SpeedM2(-100);
        }else if(limit1 == 0){
        roboclaw_2.SpeedM2(0);
        }
        if(limit1 == 0 && limit3 == 0){
     //    pc.printf("dai_down_go\t");
         daiza_down(1000000,1000000);
        }
      }
      
  //////////////////////////////////////ひなアーム////////////////////////////////////////////////////
     if(Controller.readButton_bin(SANKAKU) == 1){//(kouden1 == 0 && kouden2 == 1){ //ひなアーム上昇
       if(limit6 == 1){
         pc.printf("hina_up\t");
         roboclaw_1.SpeedM1(2500);
       }else if(limit6 == 0){
         roboclaw_1.SpeedM1(0);
    //     pc.printf("hina_up_go\t");
         hina_up(1000000,1000000);
         pc.printf("hina_111\t");
       }
     }
     if(flag_d_go == true){//ひなアーム開く
   //    pc.printf("hina_open_go\t");
       hina_open(2000000);
     }

     flag_10ms = false;
    
     ///////////////////////////////////受け取るデータ//////////////////////////////////////////////////
   /*  asi = (int)underCarriageSerial.getc();
     if(asi == 97){
      souko = 1;
     }else if(asi == 98){
      tenzi_1 = 1;
     }else if(asi == 99){
      tansu = 1;
     }else if(asi ==100){
      tenzi_2 = 1; 
     }*/
     /////////////////////////////////////送るデータ///////////////////////////////
 /*   receive = underCarriageSerial.getc();
     data = (int)receive;

     pc.printf("%d",data);
     if(data == 97){
      souko = 1;
      pc.printf("souko");
     }else if(data == 98){
      tenzi_1 = 1;
      pc.printf("tenzi_1");
     }else if(data == 99){
      tansu = 1;
      pc.printf("tansu");
     }else if(data == 100){
      tenzi_2 = 1; 
      pc.printf("tennzi_2");
     }
     */
    }
  }
}
