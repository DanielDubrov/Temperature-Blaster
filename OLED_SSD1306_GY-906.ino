#include <ESP8266WiFi.h>

#include <SPI.h>                        // Підключення бібліотеки SPI
#include <Wire.h>                       // Підключення бібліотеки Wire
#include <Adafruit_GFX.h>               // Підключення бібліотеки Adafruit_GFX
#include <Adafruit_SSD1306.h>           // Підключення бібліотеки Adafruit_SSD1306
#define OLED_RESET                      16//16
#include <Adafruit_MLX90614.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,&Wire,OLED_RESET);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

double temp_amb;
double temp_obj;
double calibration = 2.36;


//---------------------------------------------------------------------------------------------------
String apiKey = "SLVILOT90E90JKDQ";

const char* ssid = "SunFlower";
const char* password = "MARS2707";

const char* server = "api.thingspeak.com";
WiFiClient client;
//--------------------------------------------------------------------------------------------------

void setup() 
{
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c); // 0x3c Вказує адресу екрану
  mlx.begin(); 
 //-----------------------Connecting to site-------------------------- 
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
//--------------------------------------------------------------------

 Serial.println("Temperature Sensor MLX90614");
 
  display.clearDisplay();
  display.setRotation(270);
  display.setCursor(25,15);  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(" Thermometer");
  display.setCursor(25,35);
  display.setTextSize(1);
  display.print("Initializing");
  display.display();
  delay(2500);
} 

void loop() 
{
  temp_amb = mlx.readAmbientTempC();
  temp_obj = mlx.readObjectTempC();

//Serial Monitor
  Serial.print("Room Temp = ");
  Serial.println(temp_amb);
  Serial.print("Object temp = ");
  Serial.println(temp_obj);

  display.clearDisplay();
  display.setRotation(270);
  display.setCursor(25,0);  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(" Temperature");

//display.clearDisplay();                  // Очищаемо дисплей
display.setTextSize(1);                    // Встановлюємо розмір шрифта
display.setTextColor(WHITE);               // Колір фона
display.setCursor(10,10);                  // Встановлюємо позицію x, y для відображення
display.print(" Ambient = ");              // Відправка тексту
display.print(mlx.readAmbientTempC());  
display.print((char)247);
display.println("C");    
display.setCursor(10,20);                  // Встановлюємо позицію x, y для відображення 
display.setTextSize(1);                    // Встановлюємо розмір шрифта
display.setTextColor(WHITE);               // Встановлюємо білыи текст, чорний фон
display.print(" Object = ");                // Відправка тексту
display.print(mlx.readObjectTempC()+ calibration); 
display.print((char)247);
display.println("C");
 delay(200);
//display.clearDisplay();

//======================================================================
    if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
    {
      String postStr = apiKey;

      postStr += "&field1=";
      postStr += String(mlx.readAmbientTempC());

      postStr += "&field2=";
      postStr += String(mlx.readObjectTempC()+ calibration);

//      postStr += "&field3=";
//     postStr += String(ntu);

//      postStr += "&field4=";
//      postStr += String(volt);
      
      postStr += "\r\n\r\n";

      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);
    }
    client.stop();
//=========================================================================

display.display();                       
}
