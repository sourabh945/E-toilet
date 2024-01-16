#include <Servo.h>

Servo motor;  

int servo_pin = 9;
int triggerPin = 10;
int echoPin = 11;


bool opened = false;
int pos;


void setup() {

    motor.attach(servo_pin);  
    pinMode(triggerPin,OUTPUT);
    pinMode(echoPin,INPUT);
}


void loop() {

  motor.write(0);

  if ((distance() < 20) && (!opened)){
        opened = true;
        motor.write(90);
        delay(2000);
        motor.write(0);
  }

  if((distance() > 20) && (opened)){
    opened = false;
  }
  if(distance() > 20){
    opened = false;
  }
}

double distance(){

    digitalWrite(triggerPin,LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin,LOW);

    double duration = pulseIn(echoPin,HIGH);

    return duration*(0.034/2);
}