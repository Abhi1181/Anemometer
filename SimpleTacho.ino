#include <LiquidCrystal_I2C.h>
#define DISPLAY_W 16

LiquidCrystal_I2C lcd(0x27,DISPLAY_W,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
 //SDA is Analog pin 4 on UNO
 //SCL is Analog pin 5 on UNO

float revolutions=0;
int rpm=0; // max value 32,767 16 bit
long  startTime=0;
long  elapsedTime;
 
void setup() 
{
    pinMode(2, INPUT_PULLUP);           // set pin to input
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("RPM :"); 
    Serial.begin(9600);
}
 
void loop() {
revolutions=0; rpm=0;
startTime=millis();         
attachInterrupt(digitalPinToInterrupt(2),interruptFunction,RISING);
delay(1000);
detachInterrupt(2);                

//now let's see how many counts we've had from the hall effect sensor and calc the RPM
elapsedTime=millis()-startTime;     //finds the time, should be very close to 1 sec

if(revolutions>0)
{
  rpm=(max(1, revolutions) * 60000) / elapsedTime;        //calculates rpm
}
lcd.setCursor(0,0);
String outMsg = String("RPM :") + rpm;
fillMessage2DisplayWidth(outMsg);
lcd.print(outMsg);
Serial.println(outMsg);

}

void interruptFunction() //interrupt service routine
{  
  revolutions++;
}

void fillMessage2DisplayWidth(String & message)
{
  if(message.length()<DISPLAY_W+1)
  {
    while(message.length()<DISPLAY_W)
    {
      message+=" "; 
    }
     return;
  }
   //message is too wide for 1 line of the display, truncate it
    message = message.substring(0, DISPLAY_W-1); 
}
