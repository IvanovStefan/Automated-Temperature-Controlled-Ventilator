#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int sensorPin = A0; // pinul senzorului de temperatura
float sensorValue; // valoarea primită de la senzorul de temperatură
float voltageOut; // voltajul prin senzorul de temperatura

float temp; // temperatura în Celsius
float temperatureK; // temperatura în Kelvin

int fan = 11; // pinul ventilatorului
int led = 8; // pinul ledului
int tempMin = 10; // temperatura minimă unde ventilatorul e la 0%
int tempMax = 20; // temperatura maximă unde ventilatorul e la 100%
int fanSpeed; //viteza ventilator
int fanLCD; //viteza ventilatorului de afisat pe LCD

void setup()
{
  pinMode(fan, OUTPUT); //pinul ventilatorului primeste informatie
  pinMode(led, OUTPUT); //pinul ledului primeste informatie
  pinMode(sensorPin, INPUT); //pinul senzorului de temperatura ofera informatie
  Serial.begin(9600);
  
  lcd.begin();  // initializare LCD
  lcd.setBacklight((uint8_t)1); // Pornire blacklight
}

void loop()
{
  sensorValue = analogRead(sensorPin);
  voltageOut = (sensorValue * 5000) / 1024;
  temperatureK = voltageOut / 10; // calculate temperature for LM335  
  temp = temperatureK - 273; // transformare temperatura in Celsius

  Serial.print("Temperature(ºC): "); 
  Serial.print(temp);   
 Serial.print("  Voltage(mV): "); 
  Serial.println(voltageOut); 

  if(temp < tempMin) // daca temperatura este mai mica decat minimul
{
fanSpeed = 0; // ventilatorul nu este pornit
analogWrite(fan, fanSpeed); // ventilatorul nu este pornit
fanLCD=0; // viteza 0% de afisat pe LCD
}
if((temp >= tempMin) && (temp <= tempMax)) // daca temperatura depaseste minimul
{
fanSpeed = map(int(temp), tempMin, tempMax, 0, 255); // viteza efectiva a ventilatorului poate lua valori de la 0 la 255
fanLCD = map(int(temp), tempMin, tempMax, 0, 100); // viteza de afisat pe LCD
analogWrite(fan, fanSpeed); // trimite ventilatoului viteza calculata
}
 
if(temp > tempMax) // temperatura depaseste maximul
{
digitalWrite(led, HIGH); // porneste LED-ul
analogWrite(fan, 255); // ventilatorul are viteza maxima
fanLCD=100; // viteza de afisat pe LCD
}
else 
{
digitalWrite(led, LOW); // oprire LED in caz contrar
}
  
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Speed: ");
  lcd.print(fanLCD);
  lcd.print("%");
  delay(1000); // delay 1 secunda intre citirile datelor senzorului de temperatura
}
