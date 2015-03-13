ArduinoHumidor is a simple program that takes 30 measurements over a
user defined timeframe and then trips a relay if the humidity is more than one percent below a user defined value. The relay will stay on until the humidity exceeds 1% above the target humidity. 

Hardware
The sketch uses the DHT22 temperature and humidity sensor though the CHT11 and probably others are supported with the same library. The display is a cheap .96" SSD1306 OLED display that is widely available on eBAy today in early 2015.

Possible alterations
Since this is designed to run in a retail store I do not have any mechanism for changing the target humidity on the unit but were I building one for home I would include butttons to adjust the target humidity. Right now the unit only works on relative humidity I may update it to take temperature into account.

Copyright Dan Todd
License: GPL version 3

DanTodd@dantodd.com

Libraries used by this project:

  DHT-sensor-library by Adafruit http://www.adafruit.com available at 
  https://github.com/adafruit/DHT-sensor-library/  
  
  u8glib The Universal Graphics Library: available at https://code.google.com/p/u8glib/
