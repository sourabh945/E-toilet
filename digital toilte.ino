// service code is 1 

#include <dth.h> // library for air moisture sensor 

dth air_moisture_sensor;

// value of the pins of the input analog sensors 
#define tank_level_sensor A0
#define water_level_sensor A1 
#define floor_moisture_sensor A2 

// input pins for digital sensors 
int air_moisture_sensor_pin = 7;
int ir_sensor = 6;

//  value for setting the levels 
int max_tank_level = 450;
int min_water_level = 100;
int max_floor_moisture = 500; 
int max_air_moisture = 60;
int max_temperature = 35;
int service_tank_level = 400;
int service_water_level = 300;

// value of output pins for digital sensors
int LED_green = 13;
int LED_red = 12;
int LED_yellow = 11; 
int LED_fan = 10;
int Cleaning_floor = 9;

int people = 0;
bool inside = false;

void setup(){

    // INPUT sensors and devices
    pinMode(tank_level_sensor,INPUT);
    pinMode(water_level_sensor,INPUT);
    pinMode(floor_moisture_sensor,INPUT);
    pinMode(ir_sensor,INPUT);
    // air moisture sensor doesn't need to defined 

    // OUTPUT sensor and devices
    pinMode(LED_green,OUTPUT);
    pinMode(LED_red,OUTPUT);
    pinMode(LED_yellow,OUTPUT);
    pinMode(LED_fan,OUTPUT);
    pinMode(Cleaning_floor,OUTPUT);

    // serial output
    Serial.begin(9600);

}

void loop(){
    if(digitalRead(ir_sensor)){
        people = people+1;
        if(inside){
            digitalWrite(LED_red,LOW);
            inside = false;
        }
        else{
            inside = true;
        }
    }
    run();
}

bool Tank(){
    delay(10);
    int val = analogRead(tank_level_sensor);
    if(val < max_tank_level){
        if(val > service_tank_level){
            Message("Tank service required",1);
        }
        return true;
    }
    return false;
}

bool Water(){
    delay(10);
    int val = analogRead(water_level_sensor);
    if(val > min_water_level){
        if(val < service_water_level){
            Message("Water service required",1);
        }
        return true;
    }
    return false;
}

bool Floor_moisture(){
    delay(10);
    int val = analogRead(floor_moisture_sensor);
    if(val < max_floor_moisture){
        return true;
    }
    return false;
}

bool Air_moisture(){
    delay(10);
    int chk = air_moisture_sensor.read11(air_moisture_sensor_pin);
    int val = air_moisture_sensor.humidity;
    if(val < max_air_moisture){
        digitalWrite(LED_fan,LOW);
        return true;
    }
    digitalWrite(LED_fan,HIGH);
    return false;
}

bool Temperature(){
    int chk = air_moisture_sensor.read11(air_moisture_sensor_pin);
    int val= air_moisture_sensor.temperature;
    if(val < max_temperature){
        return true;
        digitalWrite(LED_fan,LOW);
    }
    digitalWrite(LED_fan,HIGH);
    return false;
}

void Message(string message,integer code){
    if(code == 1){
        digitalWrite(LED_yellow);
    }

}

void people_entered(){
    Serial.print("Total number of person entered = ");
    Serial.println(people/2);
}


void run(){
    Air_moisture();
    Temperature();
    if((Water())&&(Tank())&&(Floor_moisture())){
        digitalWrite(LED_green,HIGH);
        if(inside){
            digitalWrite(LED_red,HIGH);
        }
    }
    else{
        digitalWrite(LED_green,LOW);
    }

}