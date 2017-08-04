/*********************************************************
name:Color Detection
function: you may see the color RGB value in the serial monitor
connection:
color sensor     uno r3
S0                 3
S1                 4
S2                 5
S3                 6
OUT                2
GND               GND
VCC                5V
******************************************************/
//Email:support@sunfounder.com
//website:www.sunfounder.com
#include <TimerOne.h>
#define S0 3 // Please notice the Pin's define
#define S1 4
#define S2 5
#define S3 6
#define OUT 2
int g_count = 0; // count the frequecy
int g_array[3]; // store the RGB value
int g_flag = 0; // filter of RGB queue
float g_SF[3]; // save the RGB Scale factor
/**************ADDING BUZZER*****************************/
const int buzzerPin = 7;//the buzzer pin attach to
int fre = 31000;//set the variable to store the frequence value
// 31000 Hz is beyond human hearing range
/*******************************************/

// Init TSC230 and setting Frequency.
void TSC_Init()
{
  //set the pins as OUTPUT
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);
  pinMode(buzzerPin,OUTPUT);//set buzzerPin as OUTPUT * ADDING BUZZER *
  
  // OUTPUT Frequency Scaling 2%
  digitalWrite(S0, LOW);
  digitalWrite(S1, HIGH); 
}
// Select the filter color
void TSC_FilterColor(int Level01, int Level02)
{
  if(Level01 != 0)
    Level01 = HIGH;

  if(Level02 != 0)
    Level02 = HIGH;

  digitalWrite(S2, Level01); 
  digitalWrite(S3, Level02); 
}
void TSC_Count()
{
  g_count ++ ;
}
void TSC_Callback()
{
  switch(g_flag)
  {
    case 0:
      Serial.println("->WB Start");
      TSC_WB(LOW, LOW); //Filter output Red 
      break;
    case 1:
      Serial.print("->Frequency R=");
      Serial.println(g_count);
      g_array[0] = g_count;
      TSC_WB(HIGH, HIGH); //Filter output Green 
      break; 
    case 2:
      Serial.print("->Frequency G="); 
      Serial.println(g_count);
      g_array[1] = g_count;
      TSC_WB(LOW, HIGH); //Filter output Blue
      break; 
    case 3:
      Serial.print("->Frequency B=");
      Serial.println(g_count);
      Serial.println("->WB End");
      g_array[2] = g_count;
      TSC_WB(HIGH, LOW); //Clear(no filter) 
      break;
    default: 
      g_count = 0; 
      break; 
  }
}
void TSC_WB(int Level0, int Level1) //White Balance
{
  g_count = 0;
  g_flag ++;
  TSC_FilterColor(Level0, Level1);
  Timer1.setPeriod(1000000);//1000000 microseconds(1s)
}
void setup()
{
  TSC_Init();
  Serial.begin(9600);
  Timer1.initialize(); // defaulte is 1s
  Timer1.attachInterrupt(TSC_Callback); 
  attachInterrupt(0, TSC_Count, RISING); 
  delay(4000); 
  for(int i=0; i<3; i++)
  Serial.println(g_array[i]);
  g_SF[0] = 255.0/ g_array[0]; //R Scale factor
  g_SF[1] = 255.0/ g_array[1] ; //G Scale factor
  g_SF[2] = 255.0/ g_array[2] ; //B Scale factor
  // Serial.println(g_SF[0]);
  // Serial.println(g_SF[1]);
  // Serial.println(g_SF[2]); 
}
/******************ADD BUZZER START *************************/
void buzzer_loop()
{
  for(int i = 200;i <= 600;i++) //frequence loop from 200 to 800
  {
    tone(7,i); //in pin7 generate a tone,it frequence is i
    delay(3); //wait for 5 milliseconds 
  }
  // delay(4000); //wait for 4 seconds on highest frequence
  for(int i = 600;i >= 200;i--) //frequence loop from 800 downto 200
  {
    tone(7,i); //in pin7 generate a tone,it frequence is i
    delay(5); //delay 10ms
  }
  tone(7,31000); // pin 7, freq 31000 is beyond human hearing range
}
/****************ADD BUZZER END ***************************/

void loop()
{
  g_flag = 0;
  for(int i=0; i<3; i++)
  Serial.println(int(g_array[i] * g_SF[i]));
  if ((g_array[0] > 450) && (g_array[1] < 400) && (g_array[2] < 550))
    {
      Serial.println("RED detected");
      buzzer_loop();      
    }  
  delay(4000); 
}

