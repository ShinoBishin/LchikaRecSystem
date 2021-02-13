#include <U8g2lib.h>
#include <U8x8lib.h>

//必要なヘッダファイルを読み込み
#include <Arduino.h>
#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

//LEDとボタンを接続した端子を定義
#define LED 3
#define BTN1 4
#define BTN2 5

//OLEDディスプレイのライブラリ設定
U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(/* reset=*/U8X8_PIN_NONE, /* clock=*/SCL, /* data=*/SDA);

void setup()
{
  //各端子を入力、出力に設定
  pinMode(LED, OUTPUT);
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  //シリアル通信のボーレート指定
  Serial.begin(9600);
  //OLEDディスプレイ起動
  u8x8.begin();
  u8x8.setFont(u8x8_font_px437wyse700a_2x2_r);
  u8x8.drawString(0, 0, "WELCOME!");
  u8x8.drawString(0, 2, "PUSH1BTN");
  u8x8.refreshDisplay();
}

//必要な変数を宣言
int btn_push[120]; //ボタン入力をタイミング記録する所。0か1しか入らないのでメモリ節約のためにchar型でもよかったかも。
int mode = 0;      //モードチェンジ時に使います
int i;             //for文回すのに使います

void ready(void)
{
  for (i = 0; i < 2; i++)
  {
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
  }
}

int btn_read(int x[])
{
  unsigned long now_time = millis();
  Serial.println(now_time);
  i = 0;
  while (millis() - now_time < 10000UL)
  {
    x[i] = digitalRead(BTN2);
    i++;
    delay(100);
    Serial.println(millis());
  }
  u8x8.refreshDisplay();
}

int led_on(int x[])
{
  for (i = 0; i < (sizeof(btn_push) / sizeof(btn_push[0])); i++)
  {
    delay(100);
    if (btn_push[i] == 1)
    {
      digitalWrite(LED, HIGH);
    }
    else
    {
      digitalWrite(LED, LOW);
    }
  }
  u8x8.refreshDisplay();
}

void loop()
{

  if (digitalRead(BTN1) == 1)
  {
    delay(100);
    mode++;
  }

  switch (mode)
  {
  case 1:
    u8x8.drawString(0, 0, "STANDBY!");
    u8x8.drawString(0, 2, "PUSH1BTN");
    ready();
    mode++;
    break;
  case 2:
    if (digitalRead(BTN1) == 1)
    {
      u8x8.refreshDisplay();
      mode += 1;
    }
    break;
  case 3:
    u8x8.drawString(0, 0, "RECMODE!");
    u8x8.drawString(0, 2, "PUSH2BTN");
    btn_read(btn_push);
    u8x8.drawString(0, 0, "STOP!!!!");
    u8x8.drawString(0, 2, "PUSH1BTN");
    mode++;
    break;
  case 4:
    if (digitalRead(BTN1) == 1)
    {
      u8x8.refreshDisplay();
      mode++;
    }
    break;
  case 5:
    u8x8.drawString(0, 0, "PLAYBACK");
    u8x8.drawString(0, 2, "LEDFLASH");
    u8x8.refreshDisplay();
    led_on(btn_push);
    u8x8.drawString(0, 0, "END.....");
    u8x8.drawString(0, 2, "PUSH1BTN");
    for (i = 0; i < (sizeof(btn_push) / sizeof(btn_push[0])); i++)
    {
      btn_push[i] = 0;
    }
    mode = 0;
    break;
  }
  delay(500);
}
