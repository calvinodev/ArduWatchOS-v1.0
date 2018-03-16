#define sclk 13
#define mosi 11
#define cs   10
#define rst  9
#define dc   8

#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>


Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, mosi, sclk, rst);  

float pie = 3.1415926;

void setup(void) {
  Serial.begin(9600);            //Start serial
  display.begin();               //Start display
  Serial.println("init");        //Print INIT in serial
  display.fillScreen(BLACK);     //Clear the screen
}

int seconds   = 10;               //The second
int minutes   = 10;               //The minute
int hours     = 10;               //The hour
bool am       = true;            //Is it AM or PM

String BTCVAL = "$00000.00";     //Value of Bitcoin to be recieved from BLE   
bool stringstop = false;
String msg1     = "";            //1st line message to be displayed from BLE
String msg2     = "";            //2st line message to be displayed from BLE
String msg3     = "";            //3st line message to be displayed from BLE
String instring = "";            //Whole imput string from BLE

int clockCenterX = 48;           //Clock center x
int clockCenterY = 32;           //Clock center y

String clockface = "2";           //What clockface is active

void loop() {  
  Serial.begin(9600);                      //Start serial
  if(Serial.available()) {                 //If message is recieved from serial (BLE)
    delay(500);                            //Wait for full message (.5 seconds)
    GetSerialBLE();                        //Split Message Recieved from Serial(BLE) into HOUR, MINUTE, SECOND, MSG(1,2,3), and BTCVAL
    //DisplayMessage(msg1, msg2, msg3, 5);   //Display the recieved data
  }
  UpdateTime();                            //Update the time
  if(clockface = "1") {                    //Is clock face digital
    DisplayTimeDig(5);                     //Display the time (Numbers)
  }
  if(clockface = "2") {                    //Is clock face traditional
    DisplayTimeClock();                    //Display the time (Clock)
  } 
  Serial.println(String(minutes));
  delay(10);
  Serial.println(clockface);
  delay(10);
}

void DisplayMessage (String m1, String m2, String m3, int delayt)    //Display a message
{ 
    display.setTextColor(WHITE);                                     //Set text to white
    display.setCursor(0,0);                                          //Write text in top left
      display.print(m1);                                             //Write first message
    display.setCursor(0,15);                                         //Write text 15 pixels lower
      display.print(m2);                                             //Write second message
    display.setCursor(0,30);                                         //Write text 15 pixels lower
      display.print(m3);                                             //Write third message

    delay(delayt*1000);                                              //Wait for desired amount of time

    display.setTextColor(BLACK);                                     //Set text to black (to work as a faster method to clear the screen
    display.setCursor(0,0);                                          //Clear scren
      display.print(m1);                                             //Clear scren
    display.setCursor(0,15);                                         //Clear scren
      display.print(m2);                                             //Clear scren
    display.setCursor(0,30);                                         //Clear scren
      display.print(m3);                                             //Clear scren
    
    seconds += delayt;                                               //Add desired delay time to actual time
}

void GetSerialBLE ()                                                 //Get and split text from serial (ble)
{
    instring = Serial.readString();                                  //Set a string as the serial (ble) input
    for(int i = 0; i < 250; i++)                                     //Read string for at most 250 characters
    {
      if(instring.substring(i, i+1) == "H")                          //If the character H is found
      {
         hours = instring.substring(i+1, i+3).toInt();               //Read just the hours text and set to hours
         instring.remove(i+1, i+3);                                  //Cut out the hours text
      }

      if(instring.substring(i, i+1) == "M")                          //If the character M is found
      {
         minutes = instring.substring(i+1, i+3).toInt();             //Read just the minutes text and set to minutes
         instring.remove(i+1, i+3);
      }

      if(instring.substring(i, i+1) == "S")                          //If the character S is found
      {
        seconds = instring.substring(i+1, i+3).toInt();              //Read just the seconds text and set to seconds
        instring.remove(i+1, i+3);
      }

      if(instring.substring(i, i+2) == "Q1")                         //If the characters Q1 are found
      {
        msg1 = instring.substring(i+2, i+17);                        //Read just Message1 and set to msg2
        instring.remove(i+2, i+17);                                  //Cut out msg2 text
      }

      if(instring.substring(i, i+2) == "Q2")                         //If the characters Q2 are found
      {
        msg2 = instring.substring(i+2, i+17);                        //Read just Message2 and set to msg2
        instring.remove(i+2, i+17);                                  //Cut out msg2 text
      }

      if(instring.substring(i, i+2) == "Q3")                         //If the characters Q3 are found
      {
        msg3 = instring.substring(i+2, i+17);                        //Read just Message3 and set to msg3
        instring.remove(i+2, i+17);                                  //Cut out msg3 text
      }

      if(instring.substring(i, i+1) == "B")                          //If the character B is found
      {
        BTCVAL = instring.substring(i+1, i+10);                      //Read just the BTC text and set to BTCVAL
        instring.remove(i+1, i+10);                                  //Cut out the BTC text
      }

      if(instring.substring(i, i+1) == "C")                         //If the characters C is found
      {
        clockface = "2";                                            //Set clockface to 2
      }
      
      if(instring.substring(i, i+1) == "D")                         //If the characters D is found
      {
        clockface = "1";                                            //Set clockface to 2
      }
    }

    Serial.println(BTCVAL);
}

void UpdateTime ()          //Update the time
{
  if(seconds > 60) {       //If more than 1 minute has passed
    seconds = 0;           //Reset seconds
    minutes++;             //Add one minute
  }

  if(minutes > 60) {       //If more than 1 hour has passed
    minutes = 0;           //Reset minutes
    hours++;               //Add one hour
  }

  if(hours > 12) {         //If more than 12 hours have passed
    hours = 0;             //Reset hours
    if(am) {               //If it is in the AM 
      am = false;          //Set the time to PM
    } else {               //If it is in the PM
      am = true;           //Set the time to AM
    } 
  }

  seconds++;
}

void DisplayTimeDig (int delaytime)        //Display the time on screen
{ 
  Serial.println(minutes);              //Print minutes to serial (BLE)
  Serial.println(seconds);              //Print seconds to serial (BLE)

  display.setTextColor(WHITE);          //Set text to white (WRITE)
  display.setCursor(0,0);               //Set text to top left
    display.print(hours);               //Write hours
  display.setCursor(30,0);              //Set text to 30 pixels right
    display.print("HOUR");              //Write HOUR
  display.setCursor(0,15);              //Set text 15 pixels down
    display.print(minutes);             //Write minutes
  display.setCursor(30,15);             //Set text 30 pixels right
    display.print("MINUTE");            //Write MINUTE
  display.setCursor(0,30);              //Set text 30 pixels down
    display.print(seconds);             //Write seconds
  display.setCursor(30,30);             //Set text 30 pixels right
    display.print("SECOND");            //Write SECOND
  display.setCursor(0,45);              //Set text 45 pixels down
    display.print("BTC: ");             //Write BTC:
  display.setCursor(30,45);             //Set text 30 pixels right
    display.print(BTCVAL);              //Write BTCVAL
  
  delay(delaytime*1000);                //Wait for desired delay time
  
  display.setTextColor(BLACK);          //Set text to black (ERASE)
  display.setCursor(0,0);               //Set text to top left
    display.print(hours);               //Write hours
  display.setCursor(30,0);              //Set text to 30 pixels right
    display.print("HOUR");              //Write HOUR
  display.setCursor(0,15);              //Set text 15 pixels down
    display.print(minutes);             //Write minutes
  display.setCursor(30,15);             //Set text 30 pixels right
    display.print("MINUTE");            //Write MINUTE
  display.setCursor(0,30);              //Set text 30 pixels down
    display.print(seconds);             //Write seconds
  display.setCursor(30,30);             //Set text 30 pixels right
    display.print("SECOND");            //Write SECOND
  display.setCursor(0,45);              //Set text 45 pixels down
    display.print("BTC: ");             //Write BTC:
  display.setCursor(30,45);             //Set text 30 pixels right
    display.print(BTCVAL);              //Write BTCVAL

  seconds += delaytime;                 //Add desired delay time to actual time 
}

void DisplayTimeClock ()
{


  for (int i=0; i<2; i++)
  {
    display.drawCircle(clockCenterX, clockCenterY, 31-i, WHITE);
  }
  for (int i=0; i<3; i++)
  {
    display.drawCircle(clockCenterX, clockCenterY, i, WHITE);
  }
  
  // Draw a small mark for every hour
  for (int i=0; i<12; i++)
  {
    drawMark(i, WHITE);
  }

  drawSec(seconds, RED);
  drawMin(minutes, WHITE);
  drawHour(hours, minutes, WHITE);

  delay(2500);


  for (int i=0; i<2; i++)
  {
    display.drawCircle(clockCenterX, clockCenterY, 31-i, BLACK);
  }
  for (int i=0; i<3; i++)
  {
    display.drawCircle(clockCenterX, clockCenterY, i, BLACK);
  }
  
  // Draw a small mark for every hour
  for (int i=0; i<12; i++)
  {
    drawMark(i, BLACK);
  }

  drawSec(seconds, BLACK);
  drawMin(minutes, BLACK);
  drawHour(hours, minutes, BLACK);
}


void drawMark(int h, uint16_t col)
{
  float x1, y1, x2, y2;
  
  h=h*30;
  h=h+270;
  
  x1=29*cos(h*0.0175);
  y1=29*sin(h*0.0175);
  x2=26*cos(h*0.0175);
  y2=26*sin(h*0.0175);
  
  display.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY, col);
}

void drawSec(int s, uint16_t col)
{
  float x1, y1, x2, y2;

  s=s*6;
  s=s+270;
  
  x1=34*cos(s*0.0175);
  y1=34*sin(s*0.0175);
  x2=26*cos(s*0.0175);
  y2=26*sin(s*0.0175);
  
  //if ((s % 5) == 0)
  //  display.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY, BLACK);
  //else
    display.drawLine(x1+clockCenterX, y1+clockCenterY, x2+clockCenterX, y2+clockCenterY, col);
}

void drawMin(int m, uint16_t col)
{
  float x1, y1, x2, y2, x3, y3, x4, y4;

  m=m*6;
  m=m+270;
  
  x1=25*cos(m*0.0175);
  y1=25*sin(m*0.0175);
  x2=3*cos(m*0.0175);
  y2=3*sin(m*0.0175);
  x3=10*cos((m+8)*0.0175);
  y3=10*sin((m+8)*0.0175);
  x4=10*cos((m-8)*0.0175);
  y4=10*sin((m-8)*0.0175);
  
  display.drawLine(x1+clockCenterX, y1+clockCenterY, x3+clockCenterX, y3+clockCenterY, col);
  display.drawLine(x3+clockCenterX, y3+clockCenterY, x2+clockCenterX, y2+clockCenterY, col);
  display.drawLine(x2+clockCenterX, y2+clockCenterY, x4+clockCenterX, y4+clockCenterY, col);
  display.drawLine(x4+clockCenterX, y4+clockCenterY, x1+clockCenterX, y1+clockCenterY, col);
}

void drawHour(int h, int m, uint16_t col)
{
  float x1, y1, x2, y2, x3, y3, x4, y4;

  h=(h*30)+(m/2);
  h=h+270;
  
  x1=20*cos(h*0.0175);
  y1=20*sin(h*0.0175);
  x2=3*cos(h*0.0175);
  y2=3*sin(h*0.0175);
  x3=8*cos((h+12)*0.0175);
  y3=8*sin((h+12)*0.0175);
  x4=8*cos((h-12)*0.0175);
  y4=8*sin((h-12)*0.0175);
  
  display.drawLine(x1+clockCenterX, y1+clockCenterY, x3+clockCenterX, y3+clockCenterY, col);
  display.drawLine(x3+clockCenterX, y3+clockCenterY, x2+clockCenterX, y2+clockCenterY, col);
  display.drawLine(x2+clockCenterX, y2+clockCenterY, x4+clockCenterX, y4+clockCenterY, col);
  display.drawLine(x4+clockCenterX, y4+clockCenterY, x1+clockCenterX, y1+clockCenterY, col);
}

