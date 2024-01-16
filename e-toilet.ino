#include <dht.h>

dht air_moisture_sensor;

#define tank_level_sensor A0
#define water_level_sensor A1 

int dht_pin = 7;
int ir_sensor = 6; 

int max_tank_level = 600;
int min_water_level = 120;
int max_air_moisture = 100;
int max_temperature = 20;

int service_tank_level = 450;
int service_water_level = 100;

int LED_green = 13;
int LED_occupied = 12; 
int LED_maintenance = 11;
int LED_fan = 10;
int LED_flush = 9; 

int people = 0;
bool inside = false; 

void setup(){
  // INPUT sensors 
  pinMode(tank_level_sensor,INPUT);
  pinMode(water_level_sensor,INPUT);
  pinMode(ir_sensor,INPUT);

  // OUTPUT LEDs 
  pinMode(LED_green,OUTPUT);
  pinMode(LED_occupied,OUTPUT);
  pinMode(LED_maintenance,OUTPUT);
  pinMode(LED_fan,OUTPUT);
  pinMode(LED_flush,OUTPUT);

  //serial output
  Serial.begin(9600);
}

void loop(){
  if(Tank() && Water()){
    digitalWrite(LED_green,HIGH);
  }
  else{
    digitalWrite(LED_green,LOW);
  }
  if(digitalRead(ir_sensor)==1){
     people = people + 1;
     if(inside){
        digitalWrite(LED_occupied,LOW);
        digitalWrite(LED_flush,HIGH);
        delay(1);
        digitalWrite(LED_flush,LOW);
        inside = false;
     }
     else{
        digitalWrite(LED_occupied,HIGH);
        inside = true;
     }
  }
}


bool Tank(){
    int val = analogRead(tank_level_sensor);
    if(val < max_tank_level){
        if(val > service_tank_level){
            digitalWrite(LED_maintenance,HIGH);
            Serial.println("t1");
        }
        else{
            digitalWrite(LED_maintenance,LOW);
            Serial.println("t0");
        }
        return true;
    }
    return false;
}

bool Water(){
    int val = analogRead(water_level_sensor);
    if(val < min_water_level){
        if(val > service_water_level){
          digitalWrite(LED_maintenance,HIGH);
          Serial.println("w1");
        }
        else{
          digitalWrite(LED_maintenance,LOW);
          Serial.println("w0");
        }
        return true;
    }
    return false;
}

bool Air_moisture(){
    int chk = air_moisture_sensor.read11(dht_pin);
    int val = air_moisture_sensor.humidity;
    int temp = air_moisture_sensor.temperature;
    if(val < max_air_moisture && temp < max_temperature){
        digitalWrite(LED_fan,LOW);
        return true;
    }
    digitalWrite(LED_fan,HIGH);
    return false;
}