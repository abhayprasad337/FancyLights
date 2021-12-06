#include "ESP8266WiFi.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRutils.h>

typedef enum {
  POWER = 0xFF02FD,  //p
  BRIGHTNESS_UP = 0xFF3AC5,
  BRIGHTNESS_DOWN = 0xFFBA45,
  RED1 = 0xFF1AE5,
  ORANGE1 = 0xFF2AD5,
  ORANGE2 = 0xFF0AF5,
  ORANGE3 = 0xFF38C7,
  YELLOW1 = 0xFF18E7,
  GREEN1 = 0xFF9A65,
  GREEN2 = 0xFFAA55,
  GREEN3 = 0xFF8A75,
  GREEN4 = 0xFFB847,
  GREEN5 = 0xFF9867,
  BLUE1 =  0xFFA25D,
  BLUE2 =  0xFF926D,
  PURPLE1 = 0xFFB24D,
  PURPLE2 =  0xFF7887,
  PURPLE3 =  0xFF58A7,
  WHITE1 =  0xFF22DD,
  PINK1 =  0xFF12ED,
  PINK2 =  0xFF32CD,
  LIGHT_BLUE1 = 0xFFF807,
  LIGHT_BLUE2 = 0xFFD827,
} switch_code_t;


// === LED IR controller global definitions
#define PIN_D2 (4)
IRsend lightController(PIN_D2);


// ==== Wifi connection globals
const char* ssid = ""; // SSID
const char* password = ""; // Password
// IP address setting of this node
IPAddress ip(192, 168, 0, 204); 
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);


// Buffers for sending and receiving data
#define TCP_DATA_SIZE (40)

// IP of server to connect
const int PORT_NUMBER = 5659;  //Port number

// Wifi server object
WiFiServer server(PORT_NUMBER);

#define DEBUG


void init_IR(void)
{
  // Start IR for LED
  lightController.begin();
}

void init_serial(void)
{
  #ifdef DEBUG
  Serial.begin(115200);
  #endif
}

void init_wifi(void)
{
  // Setup Wifi connection first and do not proceed until its good
  // Wifi connection isnt established if LED is blinking
  pinMode(LED_BUILTIN, OUTPUT);
  
  digitalWrite(LED_BUILTIN, LOW);  
  
  // Connect to WiFi
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
     delay(500);
     Serial.print("*");
     digitalWrite(LED_BUILTIN, LOW);
     delay(10);
     digitalWrite(LED_BUILTIN, HIGH);
  }

  server.begin();

  digitalWrite(LED_BUILTIN, HIGH);
  
#ifdef DEBUG
  Serial.println("");
  Serial.println("WiFi connection Successful");
  Serial.println("The IP Address of ESP8266 Module is: ");
  Serial.println(WiFi.localIP());// Print the IP address
#endif
}


void send_IR(char cmd_str)
{
   switch (cmd_str)
  {
    case 'p':
      lightController.sendNEC(POWER,32);
      break;
    case 'g':
       lightController.sendNEC(GREEN2,32);
       break;
     case 'b':
       lightController.sendNEC(BLUE1,32); 
       break;
      case 'w':
       lightController.sendNEC(WHITE1,32); 
       break;
      case 'r':
       lightController.sendNEC(RED1,32); 
       break;
      case 'k':
       lightController.sendNEC(PINK1,32); 
       break;
      case 'y':
       lightController.sendNEC(YELLOW1,32); 
       break;
      case 'e':
       lightController.sendNEC(PURPLE1,32); 
       break;
      case 'o':
       lightController.sendNEC(ORANGE1,32); 
       break;
      case 'l':
       lightController.sendNEC(LIGHT_BLUE1,32); 
       break;
      case 'u':
       lightController.sendNEC(BRIGHTNESS_UP,32); 
       break;
      case 'd':
       lightController.sendNEC(BRIGHTNESS_DOWN,32); 
       break;
  }  
}


void setup() {
  init_serial();
  init_IR();
  init_wifi();
}

void loop() {
  char output_str[TCP_DATA_SIZE];
  
  WiFiClient client = server.available();
  if (client) {
    if(client.connected())
    {
      Serial.println("Client Connected");
    }
    
    while(client.connected()){      
      while(client.available()>0){
        // read data from the connected client
         client.read(output_str, TCP_DATA_SIZE);
         Serial.println(output_str);
         send_IR(output_str[0]);
      }
      //Send Data to connected client
      while(Serial.available()>0)
      {
        client.write(output_str);
      } 
    }
    client.stop();
    Serial.println("Client disconnected");    
  }

 
}
