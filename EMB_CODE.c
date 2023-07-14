#include "DHT.h"

#define DHT2PIN 2     // INPUT1
#define DHT3PIN 3     // IINPUT2
#define DHT4PIN 4     // IINPUT3
#define DHTTYPE DHT22 // DHT 22  (AM2302)

#define HEATER_LOW_PIN 5
#define HEATER_HIGH_PIN 6
#define COOLER_LOW_PIN 7
#define COOLER_HIGH_PIN 8
#define HUMIDITY_LOW_PIN 9
#define HUMIDITY_HIGH_PIN 10

DHT dht2(DHT2PIN, DHTTYPE,4);
DHT dht3(DHT3PIN, DHTTYPE,4);
DHT dht4(DHT4PIN, DHTTYPE,4);


int TEMPERATURE_STATE = 0;
int HUMIDITY_STATE= 0;
int HEATER_SUPER_STATE= 0; 
int COOLER_SUPER_STATE= 0;
int HUMIDITY_SUPER_STATE= 0;



void setup() {
    Serial.begin(9600);
    Serial.println("HELLO!!!!");
    dht2.begin();
    dht3.begin();
    dht4.begin();
     //PINS
    pinMode(HEATER_LOW_PIN, OUTPUT);
    pinMode(HEATER_HIGH_PIN, OUTPUT);
    pinMode(COOLER_LOW_PIN, OUTPUT);
    pinMode(COOLER_HIGH_PIN, OUTPUT);
    pinMode(HUMIDITY_LOW_PIN, OUTPUT);
    pinMode(HUMIDITY_HIGH_PIN, OUTPUT);
}


void loop() {

    delay(2000);
  
   float h0 = dht2.readHumidity();            //SENSOR1
   float t0 = dht2.readTemperature();        
   
   float h1 = dht3.readHumidity();          //SENSOR2
   float t1 = dht3.readTemperature();     
   
   float h2 = dht4.readHumidity();        //SENSOR3
   float t2 = dht4.readTemperature();    
   
   if (isnan(h0) || isnan(t0) || isnan(h1) || isnan(t1)|| isnan(h2) || isnan(t2)  ) { 	//ERROR_STATE
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    
     //AVERAGE OF 3 SENSOR
    float h = (h0 + h1 + h2) / 3; 
    float t = (t0 + t1 + t2) / 3;
    
    // PRINT THE OUTPUT
        Serial.println(" ");
        Serial.print("HUMIDITY : ");
        Serial.print(h);
        Serial.print(" %");
	Serial.print("     ");
        Serial.print("TEMP : ");
        Serial.print(t);
        Serial.println(" *C");

    // TEMPERATURE_STATE 
    if (TEMPERATURE_STATE  == 0) {
	if (t <= 20) {
            TEMPERATURE_STATE  = 1;
	    HEATER_SUPER_STATE = 1;
        }
	if (t >= 32) {
            TEMPERATURE_STATE  = 2;
	     COOLER_SUPER_STATE = 1;
        }
    } 
    

    // TEMPERATURE_SUPER_STATE
   
   //BOTH OFF
    if (TEMPERATURE_STATE  == 0) {
        digitalWrite(HEATER_LOW_PIN, LOW);
        digitalWrite(HEATER_HIGH_PIN, LOW);
        digitalWrite(COOLER_LOW_PIN, LOW);
        digitalWrite(COOLER_HIGH_PIN, LOW);
	
	Serial.println("HEATER : OFF");
        Serial.println("COOLER : OFF");
    }
   
   //HEATER_SUPER_STATE
    else if (TEMPERATURE_STATE  == 1) {
	//HEATER LOW
        if (HEATER_SUPER_STATE == 1) {
            digitalWrite(HEATER_LOW_PIN, HIGH);
            digitalWrite(HEATER_HIGH_PIN, LOW);
	    Serial.println("HEATER : LOW");
	    
	    if (t <= 15) {
                HEATER_SUPER_STATE = 2;
            }
           else if (t >= 23) {
                TEMPERATURE_STATE  = 0;
            } 
        }
	//HEATER HIGH
        else if (HEATER_SUPER_STATE == 2) {
            digitalWrite(HEATER_LOW_PIN, LOW);
            digitalWrite(HEATER_HIGH_PIN, HIGH);
	    Serial.println("HEATER : HIGH");
	    
            if (t >= 17) {
                HEATER_SUPER_STATE = 1;
            }
        }
	digitalWrite(COOLER_LOW_PIN, LOW);
        digitalWrite(COOLER_HIGH_PIN, LOW);
	Serial.println("COOLER : OFF");
    }
    
    //COOLER_SUPER_STATE
   else if (TEMPERATURE_STATE == 2) {
        digitalWrite(HEATER_LOW_PIN, LOW);
        digitalWrite(HEATER_HIGH_PIN, LOW);
	Serial.println("HEATER : OFF");

	//COOLER LOW
        if (COOLER_SUPER_STATE == 1) {
            digitalWrite(COOLER_LOW_PIN, HIGH);
            digitalWrite(COOLER_HIGH_PIN, LOW);
	      Serial.println("COOLER : LOW");
	      
            if (t >= 38) {
                COOLER_SUPER_STATE = 2;
            }
	    else if (t <= 28) {
                TEMPERATURE_STATE = 0;
            } 
        }
	 //COOLER HIGH
        else if (COOLER_SUPER_STATE == 2) {
            digitalWrite(COOLER_LOW_PIN, LOW);
            digitalWrite(COOLER_HIGH_PIN, HIGH);
	      Serial.println("COOLER : HIGH");
	      
            if (t <= 35) {
                COOLER_SUPER_STATE = 1;
            }
        }
    }

     // HUMIDITY_STATE
    if (HUMIDITY_STATE == 0) {
        if (h <= 80) {
            HUMIDITY_STATE = 1;
	    HUMIDITY_SUPER_STATE = 1;
        }
    }
    
    // HUMIDITY_SUPER_STATE
    //HUMIDITY OFF
    if (HUMIDITY_STATE == 0) {
        digitalWrite(HUMIDITY_LOW_PIN, LOW);
        digitalWrite(HUMIDITY_HIGH_PIN, LOW);
	Serial.println("HUMIDIFIER : OFF");
    }
    else if (HUMIDITY_STATE == 1) {
	//HUMIDITY LOW
        if (HUMIDITY_SUPER_STATE == 1) {
            digitalWrite(HUMIDITY_LOW_PIN, HIGH);
            digitalWrite(HUMIDITY_HIGH_PIN, LOW);
	    Serial.println("HUMIDIFIER : LOW");
	    
            if (h >= 85) {
                HUMIDITY_STATE = 0;
            } else if (h <= 70) {
                HUMIDITY_SUPER_STATE = 2;
            }
        }
	//HUMIDITY HIGH
        else if (HUMIDITY_SUPER_STATE == 2) {
            digitalWrite(HUMIDITY_LOW_PIN, LOW);
            digitalWrite(HUMIDITY_HIGH_PIN, HIGH);
	    Serial.println("HUMIDIFIER : HIGH");
	    
            if (h >= 75) {
                HUMIDITY_SUPER_STATE = 1;
            }
        }
    }
}