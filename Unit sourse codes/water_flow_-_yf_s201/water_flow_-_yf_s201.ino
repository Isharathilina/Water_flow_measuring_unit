/*YF- S201 water Flow sensor code for Arduino */

volatile int  Pulse_Count;
volatile int  Pulse_Count2;
unsigned int  Liter_per_hour;
unsigned int  TotalLiters;
unsigned long Current_Time, Loop_Time;

void setup()
{ 
   Serial.begin(9600); 
   attachInterrupt(digitalPinToInterrupt(23), Detect_Rising_Edge, RISING);
                                     
   Current_Time = millis();
   Loop_Time = Current_Time;
} 

void loop ()    
{
   Current_Time = millis();
   if(Current_Time >= (Loop_Time + 1000))
   {
      Loop_Time = Current_Time;
      Liter_per_hour = (Pulse_Count * 60 / 7.5);
      TotalLiters = (Pulse_Count2 / (7.5*60));
      Pulse_Count = 0;
      Serial.print(Liter_per_hour, DEC);
      Serial.println(" Liter/hour");
      Serial.print(TotalLiters, DEC);
      Serial.println(" Total Liters");
   }
}
void Detect_Rising_Edge()
{ 
   Pulse_Count++;
   Pulse_Count2++;
} 
