/*********
  by risca and beppeventuru
*********/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266TelegramBOT.h>

// Replace with your network credentials
const char* ssid = "mySSID";  //insert here the wifi ssid
const char* password = "mypw"; //insert here the wifi password
#define BOTtoken "000000000000:XXXXXXXXXXXXXXXX"  //token of your bot
#define BOTname "mybot"           //set chan name here
#define BOTusername "mybot_bot"   //and bot name here

ESP8266WebServer server(80);

String webPage = "";

void handleRoot() {
  server.send(200, "text/html", webPage);
}

void edit_webpage(String msg) {
  String intro = "<html><body><h1>They said...</h1><p>";
  String closure = "</p></body></html>";
  webPage = intro + msg + closure;
}

// Initialize Telegram BOT
TelegramBOT bot(BOTtoken, BOTname, BOTusername);
int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done

/********************************************
 * EchoMessages - function to Echo messages *
 ********************************************/
void Bot_EchoMessages() {

  for (int i = 1; i < bot.message[0][0].toInt() + 1; i++)      {
    bot.sendMessage(bot.message[i][4], bot.message[i][5], "");
    edit_webpage(bot.message[i][5]);
  }
  bot.message[0][0] = "";   // All messages have been replied - reset new messages
}


void setup(void){
  webPage += " ";

  // preparing GPIOs

  Serial.begin(230400);
  delay(2000);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
  Serial.println("HTTP server started");

  server.on("/", handleRoot);
}
 
void loop(void){
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    bot.getUpdates(bot.message[0][1]);   // launch API GetUpdates up to xxx message
    Bot_EchoMessages();   // reply to message with Echo
    Bot_lasttime = millis();
  };
  server.handleClient();
}