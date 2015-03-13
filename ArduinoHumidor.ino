/* ///////////////////////////////////////////////////////////////
  Humidifier control using DHT22 and single relay breakout board
  By Dan Todd for Burlingame Tobacconists March 2015

  User should set the desired humidity (at "target") and the amount 
  of time you want to wait between changing the status of your 
  humidifying unit "averagTime" a small wait might force your unit to 
  cycle on and off too frequently. It will also keep the humidifier 
  from coming on when the humidor is opened for a short period. 
  10 or 15 minutes is recommended.

  Hardware: (prices as of 3/11/2015
  ssd1306 .96" OLED display (about $5 on eBay)
  DHT22 humidity and Temperature sensor. (around $4.50)
  Signal output is via pin D3 I am using a small relay to switch the 
  AC mains for a humidifier.
  
  
  Required Libraries.
  
  DHT-sensor-library by Adafruit http://www.adafruit.com available at 
  https://github.com/adafruit/DHT-sensor-library/  
  
  u8glib The Universal Graphics Library available at https://code.google.com/p/u8glib/

   Copyright 2015 Dan Todd ( dantodd@dantodd.com )
   This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/ //////////////////////////////////////////////////////////////////

// OLED setup U8glib
#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);	// I2C / TWI 

//Humidity/Temperature Sensor Setup
#include "DHT.h"
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);
// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

int target = 70; // humidity setting. This is the target humidity.
float H_Correction = 0; // sensor error determined by 69% Boveda test. (number added to reading)
int averageTime = 10; // readings will be averaged over this time before the humidifier is turned on or off.
char* humStatus = "off";


void setup(){
  pinMode(3, OUTPUT);  // Relay pin
  

// Configure and initialize OLED display
u8g.setColorIndex(1);         // Set colr as "pixel on"
u8g.firstPage();              // Bootup screen will display until the first measurements are taken from the sensor
  do {
    u8g.setFont(u8g_font_courR12);
    u8g.setPrintPos(0,12);
    u8g.print("Burlingame");
    u8g.setPrintPos(0,26);
    u8g.print("Tobacconists");
    u8g.setPrintPos(0,40);
    u8g.print("Initializing");
    u8g.setPrintPos(0,54);
    u8g.print("Humidor");
  } while(u8g.nextPage());
}


  
void loop(){
  
  //set up local varibles
  int total = 0;
  int i;
  float HumidityAverage = 0;
  

  for(i = 0; i < 30; i++){
    delay(averageTime * 2000); // 30 measurements taken over "averageTime" in minutes. "average time * 2000 giver 1/30th of "averageTime" in milliseconds.
    total = total + dht.readHumidity() + H_Correction;
    checkHumStatus(); 
    updateDisplay(); //prevents display from going stale while we average measurements
    HumidityAverage = total / i;
  }
  if ((HumidityAverage > (target + 1)) && (digitalRead(3) == HIGH)) 
  digitalWrite(3, LOW);
  
  else if ((HumidityAverage < (target -1)) && (digitalRead(3) == LOW))
  digitalWrite(3, HIGH);

  else 
  updateDisplay(); // no wait 
}

void checkHumStatus(){     // check status of the relay only called in one place right now.
  if ((digitalRead(3)) == HIGH)
      humStatus = "On";
  else
      humStatus = "Off";
}


void updateDisplay(void){

  u8g.firstPage();  
  do {
    u8g.setFont(u8g_font_timB24);
    u8g.setPrintPos(0,24);
    // the display shows instant humidity plus correction factor and not the average that is used to determine if the humidifier is on. There may be a discrepency between the two 
    u8g.print(dht.readHumidity() + H_Correction,0); 
    u8g.print( "% ");
    u8g.print(dht.readTemperature(true),0);
    u8g.print("\xb0");
    u8g.setPrintPos(0,50);
    u8g.setFont(u8g_font_courR12);
    u8g.print("Target: ");
    u8g.print(target);
    u8g.print("%");
    u8g.setPrintPos(0,64);
    u8g.print("fan: ");
    u8g.print(humStatus);
  } while( u8g.nextPage() );
}    
