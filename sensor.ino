

#include <dht11.h>

dht11 DHT11;

#define DHT11PIN 2
#define HCSR501PIN 4

char buffer[9];
int length=8;
int n=0;

void printHCSR501(){
  int HCSR501State = digitalRead(HCSR501PIN);
  Serial.print("Read sensor: ");
  Serial.println(HCSR501State);
}
void printDHT11()
{
  int chk = DHT11.read(DHT11PIN);
  Serial.print("Read sensor: ");
  switch (chk)
  {
    case DHTLIB_OK: 
                Serial.println("OK"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.println("Checksum error"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.println("Time out error"); 
                break;
    default: 
                Serial.println("Unknown error"); 
                break;
  }
  if(DHTLIB_OK==chk){
    Serial.print("Temperature: ");
    Serial.println(DHT11.temperature);
    Serial.print("Humidity(%): ");
    Serial.println(DHT11.humidity);
  }
}
void setup()
{
  //open serial
  Serial.begin(9600);
  Serial.setTimeout(10);
  //close LED
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
}

void loop()
{
  n=Serial.readBytes(buffer,length);
  buffer[n]='\0';
  if(n>0){  
    
    //receive string is "DHT11"
    if(!strcmp(buffer,"DHT11")){
      printDHT11();
    }
    
    else if(!strcmp(buffer,"HCSR501")){
      printHCSR501();
    }
    else{
      Serial.println("error order!");
    }
    buffer[0]='\0';
  }
  //delay(10);
}
