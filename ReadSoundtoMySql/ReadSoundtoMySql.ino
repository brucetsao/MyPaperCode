#include <WiFi.h>
#include "String.h"

#define SoundPin A1
char ssid[] = "IOT";      // your network SSID (name)
char pass[] = "iot12345";     // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
char server[] = "192.168.88.102";    // name address for Google (using DNS)
int serverPort = 80 ;
   
#define MappingLowBound 0
#define MappingHighBound 1023
#define TransferLowBound 1
#define TransferHighBound 200


String strGet="GET /iot/sound/dataadd.php";
String strHttp=" HTTP/1.1";
String strHost="Host: 192.168.88.102";  //OK
  String connectstr ;
WiFiClient client;
IPAddress  Meip ,Megateway ,Mesubnet ;
String MacAddress ;
String locID ;
uint8_t MacData[6];
float SoundValue = 0.0 ;

void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600) ;

          // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }
  String fv = WiFi.firmwareVersion();
  if (fv != "1.1.0") {
    Serial.println("Please upgrade the firmware");
  }
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
      GetWifiMac() ;
    // wait 10 seconds for connection:
    delay(3000);
  }
  Serial.println("Connected to wifi");
  ShowInternetStatus();
    Serial.println("Read Sound Noise Program Start  ") ;
   
}

void loop() 
{
    SoundValue = map(analogRead(SoundPin),MappingLowBound,MappingHighBound,TransferLowBound,TransferHighBound) ;
    Serial.print("Read Sound Value :(");
    Serial.print(SoundValue);
    Serial.print(") \n");
    SendDatatoWIFI() ;   // Send to Cloud
    delay(10000) ;
}

void SendDatatoWIFI()
{
    connectstr = "?field1=" + MacAddress+"&field2="+locID+"&field3="+ SoundValue;
     Serial.println(connectstr) ;
         if (client.connect(server, serverPort)) {
             Serial.print("Make a HTTP request ... ");
             //### Send to Server
             String strHttpGet = strGet + connectstr + strHttp;
             Serial.println(strHttpGet);
             client.println(strHttpGet);
             client.println(strHost);
             client.println();
           } 

  if (client.connected()) { 
    client.stop();  // DISCONNECT FROM THE SERVER
  }


}

void ShowMac()
{
  
     Serial.print("MAC:");
     Serial.print(MacAddress);
     Serial.print("\n");

}




String GetWifiMac()
{
   String tt ;
    String t1,t2,t3,t4,t5,t6 ;
    WiFi.status();    //this method must be used for get MAC
  WiFi.macAddress(MacData);
  
  Serial.print("Mac:");
   Serial.print(MacData[0],HEX) ;
   Serial.print("/");
   Serial.print(MacData[1],HEX) ;
   Serial.print("/");
   Serial.print(MacData[2],HEX) ;
   Serial.print("/");
   Serial.print(MacData[3],HEX) ;
   Serial.print("/");
   Serial.print(MacData[4],HEX) ;
   Serial.print("/");
   Serial.print(MacData[5],HEX) ;
   Serial.print("~");
   
   t1 = print2HEX((int)MacData[0]);
   t2 = print2HEX((int)MacData[1]);
   t3 = print2HEX((int)MacData[2]);
   t4 = print2HEX((int)MacData[3]);
   t5 = print2HEX((int)MacData[4]);
   t6 = print2HEX((int)MacData[5]);
 tt = (t1+t2+t3+t4+t5+t6) ;
 tt.toUpperCase() ;
 MacAddress = tt ;
  tt = (t3+t4+t5+t6) ;
 tt.toUpperCase() ;
 locID = tt ;
Serial.print(MacAddress);
Serial.print("\n");
  
  return tt ;
}
String  print2HEX(int number) {
  String ttt ;
  if (number >= 0 && number < 16)
  {
    ttt = String("0") + String(number,HEX);
  }
  else
  {
      ttt = String(number,HEX);
  }
  return ttt ;
}




void ShowInternetStatus()
{
    
        if (WiFi.status())
          {
               Meip = WiFi.localIP();
               Serial.print("Get IP is:");
               Serial.print(Meip);
               Serial.print(" , MAC is:");
               Serial.print(MacAddress);
               Serial.print("\n");
              
          }
          else
          {
                       Serial.print("DisConnected:");
                       Serial.print("\n");
          }

}

void initializeWiFi() {
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
  //   status = WiFi.begin(ssid);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.print("\n Success to connect AP:") ;
  Serial.print(ssid) ;
  Serial.print("\n") ;
  
}

void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print your subnet mask:
  IPAddress subnet = WiFi.subnetMask();
  Serial.print("NetMask: ");
  Serial.println(subnet);

  // print your gateway address:
  IPAddress gateway = WiFi.gatewayIP();
  Serial.print("Gateway: ");
  Serial.println(gateway);
  Serial.println();
}

void printCurrentNet() {
  // print the SSID of the AP:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of AP:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[0], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.println(bssid[5], HEX);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}


