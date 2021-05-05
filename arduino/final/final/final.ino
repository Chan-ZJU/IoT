#include "Wire.h"
#include <ArduinoJson.h>
#define COUNT_MAX_LIMIT 1000000
#define interval_upload 8000 //上传和下发间隔

void upload(int alcohol_whether, char* gps);
//初始化3个参数
void initialization_3param(void);
//喇叭
void call(void);

void alcoholTest(void);
//get GPS
void getGPS(void);


int ID = 1;
int id = 1;
int count = 0;
char GPS[512];
int gas_din=46;
int gas_ain=A0;
int ad_value;
int alcohol_flag = 0;

void setup() {
  Wire.begin();
  pinMode(gas_din,INPUT);
  pinMode(gas_ain,INPUT);
  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // initialize device
  Serial.println("Initializing I2C devices...");
  // verify connection
  Serial.println("Testing device connections...");
  //Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  // set the data rate for the SoftwareSerial port
  Serial3.begin(115200);
  // configure Arduino LED for
  Serial3.println("AT");
  while (Serial3.available() <= 0);
  while (Serial3.available() > 0)
    Serial.println(Serial3.readString());
  Serial3.println("AT+CGNSPWR=1");
  while (Serial3.available() <= 0);
  while (Serial3.available() > 0) Serial.println(Serial3.readString());
  Serial.println("start"); 
  delay(120000);//等待接收卫星信号
}

void loop()
{
  getGPS();
  alcoholTest();
  upload(alcohol_flag,GPS);
}


void upload(int alcohol_flag, char* gps)//我们只有上行 而并未有下行操作
{
  char buffer[512] = {};
  sprintf(buffer, "AT+HTTPPARA=\"URL\",\"http://8.140.9.53:3389/device/addinfo?id=%d&alcohol=%d&gps=%s\"", id, alcohol_flag, GPS);
  Serial3.println("AT");
  while (Serial3.available() <= 0);
  while (Serial3.available() > 0) Serial.println(Serial3.readString());
  Serial3.println("AT+SAPBR=3,1,\"APN\",\"CMNET\"");
  while (Serial3.available() <= 0);
  while (Serial3.available() > 0) Serial.println(Serial3.readString());
  Serial3.println("AT+SAPBR=1,1");
  while (Serial3.available() <= 0);
  while (Serial3.available() > 0) Serial.println(Serial3.readString());
  Serial3.println("AT+HTTPINIT");
  while (Serial3.available() <= 0);
  while (Serial3.available() > 0) Serial.println(Serial3.readString());
  Serial3.println(buffer);
 
  
  while (Serial3.available() <= 0);
  while (Serial3.available() > 0) Serial.println(Serial3.readString());
  Serial3.println("AT+HTTPACTION=0");//get
  String bufferData;
  //Serial.println("I'm here0");
  delay(3000);
  while (Serial3.available() <= 0);
  while (Serial3.available() > 0) Serial.println(Serial3.readString());


  
  Serial.println("I'm here1");

  
  Serial3.println("AT+HTTPREAD=0,512");
  while (Serial3.available() <= 0);
  while (Serial3.available() > 0)
  {
    Serial.println("I'm here2");
    bufferData = Serial3.readString();
    Serial.println(bufferData);
  }

  
  Serial3.println("AT+HTTPTERM");
  while (Serial3.available() <= 0);
  while (Serial3.available() > 0) Serial.println(Serial3.readString());
}

void initialization_3param(void)
{
  id = 1;
  alcohol_flag = 0;
  count = 0;
}


void call(void)//call
{
  if(alcohol_flag==0){
    Serial.println("no emergency");
    return ;
  }
  Serial3.println("AT");
  while (Serial3.available() <= 0);
  while (Serial3.available() > 0) 
    Serial.println(Serial3.readString());
  Serial3.println("ATD+8613773469345;");
  delay(30000);
}

void getGPS(void)
{
  const char *temp;
  double a, b, useless;
  String bufferData;
  Serial3.println("AT+CGNSINF");
  while (1)
  {
    if (Serial3.available() > 0)
    {
      bufferData = Serial3.readString(); temp = bufferData.c_str();
      Serial.println("-------BUFFFERDATA-------");
      Serial.println(bufferData);
      Serial.println("-------temp-------");
      Serial.println(temp);
      
      //Serial.println(bufferData);
      int i = 0;
      while (bufferData[i++] != ',');
      while (bufferData[i++] != ',');
      while (bufferData[i++] != ',');
      char b[64] = {}; char a[64] = {};
      int j = 0;
      while (bufferData[i] != ',')
      {
        b[j++] = bufferData[i];
        i++;
      }
      j = 0; i++;
      while (bufferData[i] != ',')
      {
        a[j++] = bufferData[i];
        i++;
      }
      strcpy(GPS, a);
      strcat(GPS, ",");
      strcat(GPS, b);
      Serial.println("-------GPS-------");
      Serial.println(GPS);
      break;
    }
  }
  
  while (Serial3.available() > 0) Serial.println(Serial3.readString());
  //Serial.println("I'm Here0!!");
}

void alcoholTest(void){
  ad_value=analogRead(gas_ain);
  if(digitalRead(gas_din)==LOW)
  {
    Serial.println("Gas leakage");
    Serial.println(ad_value);
    alcohol_flag  = 1;
    call();
  }
  else
  {
    Serial.println("Gas not leak");
  }
  delay(500);
}
