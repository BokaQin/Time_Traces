#include<LiquidCrystal.h>
#include "DHT.h"
#define DHTPIN 11
#define DHTTYPE DHT11
#include "FastLED.h"        
#include <stdio.h>

#define Trig 7 
#define Echo 8 
float cm; //距离变量
float temp; // 
DHT dht(DHTPIN,DHTTYPE);
#include <DS3231.h>
DS3231  rtc(SDA, SCL);
LiquidCrystal lcd(13,12,2,3,4,5);
int leapyear(int year)
{
 if((year%4==0 && year%100!=0) || year%400==0)
  return 1;
 else
  return 0;
}
int days(int *day1, int *day2)
{
 int i=0;
 int *tmp;
 int diff = 0;
 const int month[13]={0,31,29,31,30,31,30,31,31,30,31,30,31};
 if(day1[0] == day2[0])
 {
  if(day1[1] == day2[1])
  {
   diff = day1[2] - day2[2];
   diff = (diff < 0)?(-diff):diff;
  }
  else
  {
   if(day1[1] < day2[1])    //day1=1991-5-8   day2=1991-6-2
   {
    tmp = day1;      //day1=1991-6-2   day2=1991-5-8
    day1 = day2;
    day2 = tmp;
   }
   for(i=day2[1]+1; i<day1[1]; i++)
   {
    diff +=  month[i];
   }
   diff += month[day2[1]] - day2[2] + day1[2];
   if(day2[1] <= 2 && day1[1] >2)
    if(leapyear(day2[0]))
     diff++;
  }
 }
 else
 {
  if(day1[0] < day2[0])
  {
   tmp = day1; 
   day1 = day2;
   day2 = tmp;
  }
  for(i=day2[0]+1; i<day1[0]; i++)
  {
   if(leapyear(i))
    diff += 366;
   else
    diff += 365;
  }
  for(i=day2[1]+1; i<=12; i++)     //day1=1992-1-1   day2=1991-1-1
  {
   diff += month[i];
  }
  diff += (month[day2[1]] - day2[2]);
  if(day2[1] <= 2)
   if(leapyear(day2[0]))
    diff++;
  for(i=1; i<day1[1]; i++)
  {
   diff += month[i];
  }
  diff += day1[2];
  if(day1[1] > 2)
   if(leapyear(day1[0]))
    diff++;
 }

 return diff;
}

void setup(){
    lcd.begin(16,2);
    dht.begin();
    rtc.begin();
    pinMode(Trig, OUTPUT);
    pinMode(Echo, INPUT);
}
void loop(){


    lcd.clear(); 
    int day1[3], day2[3];
    int day = 0;
      /*
    day1[0] = 2020;
    day1[1] = 2;
    day1[2] = 26;
    */
    day1[0] = rtc.getTime().year, DEC;
    day1[1] = rtc.getTime().mon, DEC;
    day1[2] = rtc.getTime().date, DEC;
  
    day2[0] = 2020;
    day2[1] = 6;
    day2[2] = 24;
    day = days(day1, day2);
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);
    float hif = dht.computeHeatIndex(f, h);
    float hic = dht.computeHeatIndex(t, h, false);
    
    digitalWrite(Trig, LOW); 
    delayMicroseconds(2);    
    digitalWrite(Trig,HIGH); 
    delayMicroseconds(10);  
    digitalWrite(Trig, LOW); 
    
    temp = float(pulseIn(Echo, HIGH)); 
    float cf;

    cf = 331.45 + 0.61 * t;
    cm = temp * (cf/10000)/2;
    float tempt;
    tempt = temp / 10000;
    
      {
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print(rtc.getMonthStr());
        lcd.setCursor(11,0);
 
        
        lcd.print(rtc.getTime().date, DEC);
        lcd.setCursor(0,1);
        lcd.print("*&");
        lcd.print(rtc.getTime().dow, DEC);

              int jij = 15;
              while( jij < 20 )
               {
              lcd.setCursor(7,1);
              lcd.print(rtc.getTimeStr());
              delay(1000);
              jij++;
               }
               jij = 30;

      }
      

  
      {
  
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Tem:");
    lcd.setCursor(6,0);
    lcd.print(t);
    lcd.print((char)223);
    lcd.print("C");
    lcd.setCursor(2,1);
    lcd.print("Hum:");
    lcd.setCursor(6,1);
    lcd.print(f);
    lcd.setCursor(12,1);
    lcd.print((char)37);
    delay(3000);

  }
  
{
   
   lcd.clear();
   lcd.setCursor(1,0);
   lcd.print("DS Tem:");
   lcd.print(rtc.getTemp());
   lcd.print((char)223);
   lcd.print("C");
   lcd.setCursor(0,1);
   //lcd.print("S=vt=");
   //int cfsg;
   //cfsg = cf;
   //lcd.print(cfsg);
   //lcd.setCursor(0,1);
   //lcd.print("x");
   int cmji;
   int cfji;
   cfji = cf;

   
   cmji = cm;

              lcd.setCursor(1,1);
              lcd.print("Vs");
              lcd.print("=");
              lcd.print(cfji);
              lcd.setCursor(8,1);
              lcd.print("Ss");
              lcd.print("=");
              lcd.print(cmji);
              if (cmji < 1000)
              lcd.print("cm");
              else
              lcd.print(" ");
              delay(5000);
           
  }



    {
    
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Only");
    lcd.setCursor(6,0);
    lcd.print(day);
    lcd.setCursor(10,0);
    lcd.print("days");
    lcd.setCursor(0,1);
    lcd.print("remained b4 exam");
    delay(3000);

    }
        {
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("Years ");
      lcd.print(rtc.getTime().year, DEC);
      lcd.setCursor(0,1);
      lcd.print("So I Love You 19");
      delay(3000);

      
    }

  
}