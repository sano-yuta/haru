#ifndef DEFINE_h
#define DEFINE_h

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

// >>> Controllerまわりで使用 >>>>>>>>>>>>>>>>>>>>>
#define CON_ADACHI    ( 0 ) //安達&宇井が2021大会の時に作った自作コントローラ用のヤツ
#define CON_ELECOM    ( 1 )
#define CON_DS4       ( 2 ) //基本コレ

#define CON_TYPE  ( CON_DS4 )
//
#if CON_TYPE == CON_ADACHI
#define MASK_BUTTON_UP    0x01
#define MASK_BUTTON_RIGHT 0x02
#define MASK_BUTTON_DOWN  0x04
#define MASK_BUTTON_LEFT  0x08
#define MASK_BUTTON_R1    0x10
#define MASK_BUTTON_R2    0x20
#define MASK_BUTTON_L1    0x40
#define MASK_BUTTON_L2    0x80

#define BUTTON_UP    1
#define BUTTON_RIGHT 2
#define BUTTON_DOWN  3
#define BUTTON_LEFT  4
#define BUTTON_R1    5
#define BUTTON_R2    6
#define BUTTON_L1    7
#define BUTTON_L2    8

#elif CON_TYPE == CON_ELECOM || CON_TYPE == CON_DS4
#define MASK_BUTTON_X       0x0001//ELECOMコントローラ用
#define MASK_BUTTON_Y       0x0002//ELECOMコントローラ用
#define MASK_BUTTON_A       0x0004//ELECOMコントローラ用
#define MASK_BUTTON_B       0x0008//ELECOMコントローラ用

#define MASK_BUTTON_SHIKAKU 0x0001
#define MASK_BUTTON_SANKAKU 0x0002
#define MASK_BUTTON_BATU    0x0004
#define MASK_BUTTON_MARU    0x0008

#define MASK_BUTTON_L1      0x0010
#define MASK_BUTTON_R1      0x0020
#define MASK_BUTTON_L2      0x0040
#define MASK_BUTTON_R2      0x0080
#define MASK_BUTTON_L1L2    0x0050

#define MASK_BUTTON_PAD     0x0100
#define MASK_BUTTON_PS      0x0200 
#define MASK_BUTTON_JOY_L   0x0100//ELECOMコントローラ用
#define MASK_BUTTON_JOY_R   0x0200//ELECOMコントローラ用
#define MASK_BUTTON_BACK    0x0400//ELECOMコントローラ用
#define MASK_BUTTON_START   0x0800//ELECOMコントローラ用
#define MASK_BUTTON_SHARE   0x0400
#define MASK_BUTTON_OPTION  0x0800

#define MASK_BUTTON_UP     0x1000
#define MASK_BUTTON_RIGHT  0x2000
#define MASK_BUTTON_DOWN   0x4000
#define MASK_BUTTON_LEFT   0x8000

#define MASK_BUTTON_0     0x10000//↓赤崎追加分，ポールに対応．
#define MASK_BUTTON_1     0x20000
#define MASK_BUTTON_2     0x40000
#define MASK_BUTTON_3     0x80000

#define MASK_BUTTON_4    0x100000
#define MASK_BUTTON_5    0x200000
#define MASK_BUTTON_6    0x400000
#define MASK_BUTTON_7    0x800000

#define MASK_BUTTON_8   0x1000000
#define MASK_BUTTON_9   0x2000000
#define MASK_BUTTON_10  0x4000000

#define SHIKAKU  1
#define SANKAKU  2
#define BATU     3
#define MARU     4
#define L1       5
#define R1       6
#define L2       7
#define R2       8
#define JOY_L    9
#define JOY_R   10
#define BACK    11
#define START   12
#define PAD      9
#define PS      10
#define SHARE   11
#define OPTION  12
#define UP      13
#define RIGHT   14
#define DOWN    15
#define LEFT    16

#define SW0     17//↙赤崎追加分
#define SW1     18          //　　|　8⃣　　　9⃣　　　10　　|
#define SW2     19          //　　|　　　6⃣　　　7⃣　　　　|
#define SW3     20          //　　|　　　　　5⃣　　　　　　|
#define SW4     21          //　　|　　　3⃣　　　4⃣　　　　|
#define SW5     22          //　　|　0⃣　　　1⃣　　　2⃣　　|
#define SW6     23          //　　∟______________________」
#define SW7     24          //
#define SW8     25          //0~10のスイッチの配置は↑のようになっている
#define SW9     26          //
#define SW10    27          //

#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#define STATE_READY 0x10
#define STATE_SETUP 0x10

#define LED_CYCLE 1

// 基板スイッチ
#define MASK_SW_1 0x0001
#define MASK_SW_2 0x0002
#define MASK_SW_3 0x0004
#define MASK_SW_4 0x0008
#define MASK_ADDRIGHT 0x0010
#define MASK_ADDLEFT 0x0020

#define LEDBLINKING_TIME 2      //回数
#define LEDBLINKING_INTERVAL 30 // ms

//足回りとの通信
#define UNDER_CARRIAGE_TX  P2_14//P8_14//
#define UNDER_CARRIAGE_RX  P2_15//P8_15//

#define UP_CARRIAGE_TX  P5_6//P8_14//
#define UP_CARRIAGE_RX  P5_7//P8_15//

// スイッチ，LED
#define SW_UP ( P3_12 ) 
#define SW_RIGHT ( P3_15 ) 
#define SW_DOWN ( P3_14 ) 
#define SW_LEFT ( P3_13 ) 
#define LED1 ( P5_4 ) 
#define LED2 ( P5_5 ) 
#define LED3 ( P5_6 ) 
#define LED4 ( P5_7 ) 
#define LED5 ( P5_4 ) 
#define LED6 ( P5_5 ) 
#define LED7 ( P5_6 ) 
#define LED8 ( P5_7 ) 
//#define DIP_SW1 ( P3_12 )
//#define DIP_SW2 ( P2_2 )
//#define DIP_SW3 ( P3_13 )
//#define DIP_SW4 ( P2_3 )

#define PIN_LED_USER ( P6_12 )
#define PIN_LED_RED ( P6_13 )
#define PIN_LED_GREEN ( P6_14 )
#define PIN_LED_BLUE ( P6_15 )
#define USER_BUTTON ( P6_0 )

// シリアル
//#define P4_SERIALTX ( P2_14 )//Serial0
//#define P4_SERIALRX ( P2_15 )//Serial0
//#define BEN_SERIALTX ( P5_3 )//Serial3
//#define BEN_SERIALRX ( P5_4 )//Serial3
//#define UMS_TX ( P5_3 )//Serial4
//#define UMS_RX ( P5_4 )//Serial4
//#define COM_SERIALTX (  )//Serial5_P3
//#define COM_SERIALRX (  )//Serial5_P3
#define ROBOCLAW1_SERIALTX ( P2_5 )//Serial6
#define ROBOCLAW1_SERIALRX ( P2_6 )//Serial6
#define ROBOCLAW2_SERIALTX ( P8_13 )//Serial6
#define ROBOCLAW2_SERIALRX ( P8_11 )

//#define UMS_ID ( 25 )

// 光電センサ
#define KOUDEN1 ( P1_7 ) 
#define KOUDEN2 ( P1_6 ) 
#define KOUDEN3 ( P2_7 )
#define KOUDEN4 ( P3_15 ) 
#define KOUDEN5 ( P3_14 ) 
#define KOUDEN6 ( P3_13 )
//#define KOUDEN4 ( P2_9 )
//#define KOUDEN5 ( P2_10 )

// リミットスイッチ
#define LIMIT_SW1 ( P5_0 ) //台座右　手前
#define LIMIT_SW2 ( P5_1 ) //台座右　奥
#define LIMIT_SW3 ( P5_2 ) //台座左　奥
#define LIMIT_SW4 ( P5_3 ) //台座左　手前
#define LIMIT_SW5 ( P5_4 ) //ひな上
#define LIMIT_SW6 ( P5_5 ) //ひな下

//電磁弁
#define DEN_BEN1 ( P7_15 ) //ひな 7_15
#define DEN_BEN2 ( P8_1 ) //ひな 8_1
#define DEN_BEN3 ( P10_15 ) //ひきづる 10_12 
#define DEN_BEN4 ( P10_12 ) //たおす 1_2
#define DEN_BEN5 ( P10_14 ) //ととのえる 8_1
#define DEN_BEN6 ( P10_13 )

// AMT10系
#define ENC_A_1 (  ) 
#define ENC_A_2 (  ) 
#define ENC_A_3 (  ) 
#define ENC_A_4 (  ) 
#define ENC_M_1 (  ) 
//#define ENC_M_2 (  ) 
//#define ENC_S_1 (  ) 
//#define ENC_S_2 (  ) 
// AMT22系
//#define ABSENC_CSB1 ( P3_8 )
//#define ABSENC_CSB2 ( P3_9 )
//#define ABSENC_MISO ( P10_15 )
//#define ABSENC_MOSI ( P10_14 )
//#define ABSENC_SCK ( P10_12 )

// CANの設定
#define CANRD P11_12
#define CANTD P11_13
#define CANID_MAINUP 0x01
#define CANID_MAINDOWN 0x02

#define MAIN_DISFALL 30500 //メイン落下
#define MAIN_ONRING 16250 // メイン装填
#define CHAIN 68424 // 一周 

#define MASK_RIGHT 0x0001
#define MASK_LEFT 0x0001

// comm_foot
#define FOOT_DATANUM 9
//controller
#define CONCOM_INTERVAL (5*1000) // ms
#define CONCOM_AVAILABLETIME 850 // ms

// rad
#define DEG_TO_RAD(deg) (deg) * 3.14159265358979 / 180
#define RAD_TO_DEG(rad) (rad) * 180 / 3.14159265358979

#endif
