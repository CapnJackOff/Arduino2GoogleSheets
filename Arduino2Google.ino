#include <Time.h>
#include <TimeLib.h>

#include <SimpleTimer.h>
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "api.pushingbox.com";    // name address for Google (using DNS)
char path[] = "/pushingbox?devid=vBgetyourowndevid";
int sample;
int sample1;
SimpleTimer timer;
 time_t t;
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 1, 127);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
  timer.setInterval(600000, sendData);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());

}

void loop() {
  timer.run();
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }else{
    client.stop(); 
  }

  // if the server's disconnected, stop the client:

}
void sendData(){

 //test signals to produce random values between 0 and 512.
  sample = 254*sin(millis()) + 254;
  sample1 = 254*cos(millis()) + 254;
  time_t sample2 = now();
  if (client.connect(server, 80)) {
    Serial.println("connected");
    Serial.print("Sample Value: ");
    Serial.println((String)sample);
    
    // Make a HTTP request:
    client.println("GET " + (String)path + "&sample=" + (String)sample + "&sample1=" + (String)sample1 + "&sample2=" + (String)sample2 + " HTTP/1.1");
    client.println("Host: " + (String)server);
    client.println("Connection: close");
    client.println();
    Serial.println("GET " + (String)path + "&sample=" + (String)sample + "&sample1=" + (String)sample1 + " HTTP/1.1");
    //may want to adjust this up or down according to response time
    delay(3000);
    
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

