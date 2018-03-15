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

float p = 3.1415926;

void setup(void) {
  Serial.begin(9600);            //Start serial
  display.begin();               //Start display
  Serial.println("init");        //Print INIT in serial
  display.fillScreen(BLACK);     //Clear the screen
}

int seconds   = 0;               //The second
int minutes   = 0;               //The minute
int hours     = 0;               //The hour
bool am       = true;            //Is it AM or PM

String BTCVAL = "$00000.00";     //Value of Bitcoin to be recieved from BLE   
bool stringstop = false;
String msg1     = "";            //1st line message to be displayed from BLE
String msg2     = "";            //2st line message to be displayed from BLE
String msg3     = "";            //3st line message to be displayed from BLE
String instring = "";            //Whole imput string from BLE

void loop() {  
  Serial.begin(9600);                      //Start serial
  if(Serial.available()) {                 //If message is recieved from serial (BLE)
    delay(500);                            //Wait for full message (.5 seconds)
    GetSerialBLE();                        //Split Message Recieved from Serial(BLE) into HOUR, MINUTE, SECOND, MSG(1,2,3), and BTCVAL
    DisplayMessage(msg1, msg2, msg3, 5);   //Display the recieved data
  }
  UpdateTime();                            //Update the time
  //DisplayTimeDig(5);                     //Display the time (Numbers)
  DisplayTimeClock();                      //Display the time (Clock)
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
  int outlinemin[2] = {0, 0};                                       //Position on the circumfrence of clock circle for minute line
  int outlinehor[2] = {0, 0};                                       //Position on the circumfrence of clock circle for hours line
  int outlinesec[2] = {0, 0};                                       //Position on the circumfrence of clock circle for seconds line

  int minangle = 0;                                                 //Angle for minute hand
  int horangle = 0;                                                 //Angle for hours hand
  int secangle = 0;                                                 //Angle for second hand

  360/minutes = minangle;                                           //Calculate angle for minutes
  360/hours = horangle;                                             //Calculate angle for minutes
  360/seconds = secangle;                                           //Calculate angle for minutes

  outlinemin[0] = 27*cos(minangle)+48;                              //Calculate x position for minute hand
  outlinemin[1] = 27*cos(minangle)+32;                              //Calculate y position for minute hand 
  outlinehor[0] = 15*cos(horangle)+48;                              //Calculate x position for minute hand
  outlinehor[1] = 15*cos(horangle)+32;                              //Calculate y position for minute hand
  outlinesec[0] = 21*cos(secangle)+48;                              //Calculate x position for minute hand
  outlinesec[1] = 21*cos(secangle)+32;                              //Calculate y position for minute hand
  
  display.drawCircle(48,32,30,WHITE);                               //Draws clock outside
  display.drawLine(outlinemin[0], outlinemin[1], 48, 32, WHITE);    //Draws minute line
  display.drawLine(outlinehor[0], outlinehor[1], 48, 32, WHITE);    //Draws hour line
  display.drawLine(outlinesec[0], outlinesec[1], 48, 32, RED);      //Draws seconds line

  delay(1);

  display.drawLine(outlinemin[0], outlinemin[1], 48, 32, WHITE);    //Erases minute line
  display.drawLine(outlinehor[0], outlinehor[1], 48, 32, WHITE);    //Erases hour line
  display.drawLine(outlinesec[0], outlinesec[1], 48, 32, RED);      //Erases seconds line  
  
}
