/*
    Function: Temperature and Humidity Sensor

    Measures temperature and humidity using AM2302
    sensor, and an Arduino Mega. Peak values can be
    displayed using button controls.

    Parameters:

    Temp_Current - Current temperature.
    Temp_High - High temperature peak.
    Temp_Low - Low temperature peak.
    Hum_Current - Current Humidity.
    Hum_High - High humidity peak.
    Hum_Low - Low humidity peak.

    Returns:

    Current temperature and humdity are displayed.
    By pressing Button-1 high and low temperature peaks
    can be displayed.
    By pressing Button-2 high and low humidity peaks can
    be displayed.
    By pressing Button-3 peak values can be reset.
    
*/

#include "DHT.h"            // DHT library is included.
#define DHTPIN 50           // Data Pin
#define DHTTYPE DHT22       // Type AM2302
DHT dht(DHTPIN, DHTTYPE);

float Temp_Current = 3.5;   // Variables are set to initial
float Temp_High = 3.5;      // values, which are obtained
float Temp_Low = 3.5;       // by running tests.
float Hum_Current = 49;
float Hum_High = 49;
float Hum_Low = 49;
int Count = 0;
volatile int state = 0;     // Used in interrupt.
int But1_Pin =19;
int But1_Pos = 0;
int But2_Pin = 20;
int But2_Pos = 0;
int But3_Pin = 21;
int But3_Pos = 0;

void setup(){
  Serial.begin(9600);
  
  pinMode(But1_Pin, INPUT_PULLUP);
  pinMode(But2_Pin, INPUT_PULLUP);
  pinMode(But3_Pin, INPUT_PULLUP);
  
  digitalWrite(But1_Pin, HIGH);
  digitalWrite(But2_Pin, HIGH);
  digitalWrite(But3_Pin, HIGH);
  
  attachInterrupt(4,But1_Int,FALLING);
  attachInterrupt(3,But2_Int,FALLING);
  attachInterrupt(2,But3_Int,FALLING);
}

// Button-1 function (But1) displays temperature peaks.

int But1(){
    Serial.print("\nTemperature High: ");
    Serial.print(Temp_High);
    Serial.print(" *C\nTemperature Low: ");
    Serial.print(Temp_Low);
    Serial.print(" *C\n");
    delay(2000);
    state = 0;
return 0;}

// Button-2 function (But2) displays humidity peaks.

int But2(){
    Serial.print("\nHumidity High: ");
    Serial.print(Hum_High);
    Serial.print(" %\nHumidity Low: ");
    Serial.print(Hum_Low);
    Serial.print(" %\n");
    delay(2000);
    state = 0;
return 0;}

// Button-3 function (But3) resets all peaks to initial values.

int But3(){
    Temp_High = 3.5;
    Temp_Low = 3.5;
    Hum_High = 49;
    Hum_Low = 49;
    Serial.print("\nValues Reset..\n");
    delay(3000);
    state = 0;
return 0;}

// Temperature function (Temp) reads current temperature
// values using the AM2302 sensor. It compares the current
// value with peak values, and replaces if necessary.

float Temp(){
  delay(500);
  Temp_Current = dht.readTemperature();
  if(isnan(Temp_Current)){
    Serial.println("\nFailed to read temperature!\n");}
  if(Temp_Current > Temp_High){
    Temp_High = Temp_Current;}
  else if(Temp_Current < Temp_Low){
    Temp_Low = Temp_Current;}
return 0;}

// Humidity function (Hum) reads current humidity values
// using the AM2302 sensor. It compares the current
// value with peak values, and replaces if necessary.

float Hum(){
  delay(500);
  Hum_Current = dht.readHumidity();
  if(isnan(Hum_Current)){
    Serial.println("\nFailed to read humidity!\n");}
  if(Hum_Current > Hum_High){
    Hum_High = Hum_Current;}
  else if(Hum_Current < Hum_Low){
    Hum_Low = Hum_Current;}
return 0;}

// Display_Current function displays the current values
// of temperature and humidity, as well as the number
// of the reading. Every reading and displaying cycle
// takes about 1 second, so reading value helps keep
// track of and approximate time.

float Display_Current(){
  Count = Count + 1;
  Serial.print("\nReading ");
  Serial.print(Count);
  Serial.print("\nTemperature:  ");
  Serial.print(Temp_Current);
  Serial.print(" *C\nHumidity:  ");
  Serial.print(Hum_Current);
  Serial.print(" %\n");
return 0;}

// Loop checks if state value is changed by an interrupt,
// and runs the representative function. Otherwise, it
// continues checking temperature, humidity, and displaying
// them.

void loop(){
  
  if(state==1){
    But1();
  }
  else if(state==2){
    But2();
  }
  else if(state==3){
    But3();
  }
  else{
  Temp();
  Hum();
  Display_Current();}
}

// Following three functions are used when the interrupt
// takes place. It changes state value, which later will
// change the loop by running the requested function.

void But1_Int(){
  state = 1;
}
void But2_Int(){
  state = 2;
}
void But3_Int(){
  state = 3;
}

/*   
    These are some values from different environments.

    Room Values:
    
    Temp = 25 C
    Hum = 37 %

    Fridge values:
    
    Temp_High = 6 C
    Temp_Low = 1 C
    Hum_High = 65.3 %
    Hum_Low = 34 %
    
*/
