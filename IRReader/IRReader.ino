#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
     
int RECV_PIN = 5; // the pin where you connect the output pin of IR sensor
   
IRrecv irrecv(RECV_PIN);
    
decode_results results;     
void setup()     
{     
  Serial.begin(115200);
  Serial.println();
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
  irrecv.enableIRIn();  // Start the receiver
//  Serial.println(RECV_PIN);
}

void loop()     
{     
  if (irrecv.decode(&results)) { // decode the received signal and store it in results
    serialPrintUint64(results.value, HEX);
    Serial.println()
    irrecv.resume(); // reset the receiver for the next code
  }
  delay(100);
}    
