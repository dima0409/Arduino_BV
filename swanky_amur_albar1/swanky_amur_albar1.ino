#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

#define PIN 6

#define NUMPIXELS 7 

const int hp = 2;
const int hm = 3;
const int mp = 4;
const int mm = 5;
const int ww = 7;

int r;
int g;
bool wake = false;
bool flag = true;
bool ledFlag = false;
unsigned long lightTime = 0;
unsigned long rlightTime = 0;
unsigned long long time_ = 0;
long hours = 0; 
long minutes = 0;
long hours_wake = 0; 
long minutes_wake = 0;
// Инициализация экрана LCD
LiquidCrystal_I2C lcd_1(0x20,16,2);

//NeoPixel
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

 
void setup() {
  pinMode(hp, INPUT_PULLUP);
  pinMode(hm, INPUT_PULLUP);
  pinMode(mp, INPUT_PULLUP);
  pinMode(mm, INPUT_PULLUP);
  pinMode(ww, INPUT_PULLUP);
  Serial.begin(9600);
  lcd_1.init();
  lcd_1.clear();         
  lcd_1.backlight();
  pixels.begin();
}

    
 void button(){
   if (flag)
   {
    if (digitalRead(ww) == LOW)
    {
     wake = !wake;
      flag = false;
    }
    if (digitalRead(hp) == LOW && wake == false) 
    {
      hours = (hours + 1) % 24;
      flag = false;
    }
    if (digitalRead(hp) == LOW && wake == true)
    {
       hours_wake = (hours_wake + 1) % 24;
      flag = false;
    }


    if (digitalRead(hm) == LOW && wake == false) 
    {
      hours = (hours - 1) % 24;
      if (hours < 0) hours += 24;
      flag = false;
    }
    if (digitalRead(hm) == LOW && wake == true)
    {
       hours_wake = (hours_wake - 1) % 24;
       if (hours_wake < 0) hours_wake += 24;
       flag = false;
    }


    if (digitalRead(mp) == LOW && wake == false) 
    {
      minutes = (minutes + 1) % 60;
      flag = false;
    }
     if (digitalRead(mp) == LOW && wake == true)
     {
       minutes_wake = (minutes_wake + 1) % 60;
       flag = false;
     }


    if (digitalRead(mm) == LOW && wake == false) 
    {
      minutes = (minutes - 1);
      if (minutes < 0) minutes += 60;
      flag = false;
    }
    if (digitalRead(mm) == LOW && wake == true)
    {
       minutes_wake = (minutes_wake - 1);
       if (minutes_wake < 0) minutes_wake += 60;
      flag = false;
    }
     
   }
   else
   {
     if (!(digitalRead(mm) == LOW || digitalRead(mp) == LOW || digitalRead(hm) == LOW || digitalRead(hp) == LOW || digitalRead(ww) == LOW)) 
     {flag = true;}
   }
  }

  // Выводим время на экран LCD
void LCD(){
  lcd_1.setCursor(6, 0);
  if(wake == false){
  if (hours < 10) lcd_1.print('0');
  lcd_1.print(hours);
  lcd_1.print(":");
  if (minutes < 10) lcd_1.print('0');
  lcd_1.print(minutes);
  }
  if(wake == true){
  if (hours_wake < 10) lcd_1.print('0');
  lcd_1.print(hours_wake);
  lcd_1.print(":");
  if (minutes_wake < 10) lcd_1.print('0');
  lcd_1.print(minutes_wake);
  }
    
  
  lcd_1.setBacklight(1);
  }
  
  // диоды
  void NeoPixel(){
for(int i=0; i<NUMPIXELS; i++) {
  pixels.setPixelColor(i, pixels.Color(r, g , 0));
}

// Плавное включение зелёного света
if (r == 0 && g < 255) {
  if (millis() - lightTime > 400) {
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(r, g, 0));
    g++;
    lightTime = millis();
    }
  }
}
    // Плавное включение красного света

    if (g == 255 && r < 255){
      if (millis() == rlightTime > 400) {
        pixels.Color(r, g, 0);
      r++;
        rlightTime = millis();
      }
      if (ledFlag == false){
        pixels.Color(0,0,0);}
}
pixels.show();
}

void loop() 
{
  button();
  LCD();
  // за 5 минут до будильника
  if(hours == hours_wake && minutes == (minutes_wake - 5) || hours == (hours_wake - 1) && minutes == (minutes_wake + 55)  % 60){
  ledFlag = true;
}
  // проверяем время пробуждения
  if (hours >= hours_wake && minutes >= minutes_wake){
  ledFlag = false;
  }
// включение
  if (ledFlag){
    NeoPixel();}
  if (millis() - time_ >= 60000) 
  {
	time_ = millis();
    minutes++;
    if (minutes / 60 == 1) 
    {
      hours++; 
      hours %= 24;
      minutes %= 60;
    }
  }
}
