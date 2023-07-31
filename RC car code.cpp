#include<Servo.h>
#include<IRremote.h>
int in1=6, in2=5;
int speed;
int trigpin=12;
int echopin=11;
int IRpin=3;
int steeringpin=9;
float time;
int dist;
IRrecv rc(IRpin);
decode_results sig;
Servo steering;

int headlight=4;
int leftlight=13;
bool leftblink=false;
int rightlight =7;
bool rightblink=false;
int buzzpin=2;
bool goforward=false;
bool gobackward=false;

void setup()
{
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(trigpin, OUTPUT);
  pinMode(IRpin, INPUT );
  Serial.begin(9600);
  steering.attach(steeringpin);
  rc.enableIRIn();
}

void loop()
{
  //ultrasonicsensor
  digitalWrite(trigpin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  time=pulseIn(echopin, HIGH);
  dist= 0.0343*time;
  if(dist<=25)
  {
    digitalWrite(rightlight, HIGH);
    steering.write(135);
    delay(2000);
    steering.write(0);
    digitalWrite(rightlight, LOW);
  }
  
  //indicator
   if(leftblink==true && rightblink==false) 
        {
          digitalWrite(leftlight, HIGH);
          delay(500);
          digitalWrite(leftlight, LOW);
          delay(500);
         }
   else if(rightblink)
   {
          digitalWrite(rightlight, HIGH);
          delay(500);
          digitalWrite(rightlight, LOW);
          delay(500);
    }
   else
       {
        digitalWrite(leftlight, LOW);
        digitalWrite(rightlight, LOW);
       } 
  
  //accelerate or decelerate
  if(goforward==true && gobackward==false)
  {
       digitalWrite(in1,HIGH);
       digitalWrite(in2,LOW);
  }
  else if(goforward==false && gobackward==true)
  {
    digitalWrite(in1,LOW);
       digitalWrite(in2,HIGH);
  }
  else {
       digitalWrite(in1,LOW);
        digitalWrite(in2,LOW);
  }
  
  //IR
  delay(50);
  Serial.println(sig.value);
  if (rc.decode(&sig)) {
    switch (sig.value) {
      case 0xFD00FF: // press power headlight on
        digitalWrite(headlight,HIGH);
        break;
      case 0xFD08F7://press 1 headlight off
        digitalWrite(headlight, LOW);
        break;
      case 0xFD042F:
      goforward= !goforward;
       break;
      case 0xFD906F: //backward
        gobackward=!gobackward;
        break;
      case 0xFFFFFFFF: //left
        steering.write(45);
        leftblink = !leftblink;
        break;
      case 0xFD609F://right
        steering.write(135);
        rightblink=!rightblink;
        break;
      case 0xFDA05F: // horn 
        digitalWrite(buzzpin, HIGH);
        delay(150);
        digitalWrite(buzzpin, LOW);
        delay(150);
        digitalWrite(buzzpin, HIGH);
        delay(150);
        digitalWrite(buzzpin, LOW);
        break;
      default: 
        steering.write(0);
    }
  } rc.resume();
}
