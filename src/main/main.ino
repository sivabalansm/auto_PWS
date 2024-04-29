#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

#include <DHT.h>

#define WATER_LEVEL_POWER_PIN  7
#define WATER_LEVEL_ANALOG_PIN A0
#define DHT_PIN 	       2
#define WATER_PUMP_PIN         3
#define PHOTORESISTOR_PIN      A1

int getLightLevel() {
	return analogRead(PHOTORESISTOR_PIN);


};
//This is a function used to get the reading
int val;
int getWaterLevel() {
	digitalWrite(WATER_LEVEL_POWER_PIN, HIGH);	// Turn the sensor ON
	delay(10);							// wait 10 milliseconds
	val = analogRead(WATER_LEVEL_ANALOG_PIN);		// Read the analog value form sensor
	digitalWrite(WATER_LEVEL_POWER_PIN, LOW);		// Turn the sensor OFF
	return val;							// send current reading
}

DHT dht(DHT_PIN, DHT11);

float getTemp() {
	return dht.readTemperature();
};


float getHum() {
	return dht.readHumidity();
};




char ssid[] = "Happywork";             //  your network SSID (name) between the " "
char pass[] = "Im201908";      // your network password between the " "

int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;      //connection status
WiFiServer server(80);            //server socket


WiFiClient client = server.available();

#define RED_PIN   25
#define GREEN_PIN 26
#define BLUE_PIN  27


void colorLED(int r, int g, int b) {
	WiFiDrv::analogWrite(RED_PIN,   constrain(r, 0, 255));
	WiFiDrv::analogWrite(GREEN_PIN, constrain(g, 0, 255));
	WiFiDrv::analogWrite(BLUE_PIN,  constrain(b, 0, 255));

};

void setup() {
  dht.begin();
  
  pinMode(WATER_PUMP_PIN, OUTPUT);
  digitalWrite(WATER_PUMP_PIN, LOW);
  
  pinMode(WATER_LEVEL_POWER_PIN, OUTPUT);
  digitalWrite(WATER_LEVEL_POWER_PIN, LOW);

  WiFiDrv::pinMode(RED_PIN  , OUTPUT);
  WiFiDrv::pinMode(GREEN_PIN, OUTPUT);
  WiFiDrv::pinMode(BLUE_PIN , OUTPUT);

  Serial.begin(9600);


  while (!Serial);

  

  enable_WiFi();

  connect_WiFi();


  server.begin();

  printWifiStatus();


}

unsigned long long int time_count = 0;
bool watering = false;

int r = 0;
int g = 0;
int b = 0;
void loop() {
  
  client = server.available();


  if (client) {
    printWEB();

  }

  if (watering || getLightLevel() < (int) (0.75 * 1023) || (time_count > (22 * 60 * 60 * 1000) && (getHum() <= 55 || getLightLevel() < (int) (0.75 * 1023)))) {
	if (time_count > 370) {
		watering = true;
		time_count = 0;
	} else if (watering && time_count < 300) {
		digitalWrite(WATER_PUMP_PIN, HIGH);


	} else {
		digitalWrite(WATER_PUMP_PIN, LOW);
		time_count = 0;
		watering = false;
	};
  	
  }
  if (getTemp() <= 15 || getTemp() >= 25) {
  	r = 255;
  };
  if (getHum() < 40.00) {
  	r = 255;
	g = 255;
  };

  if (getWaterLevel() < 100) {
	b = 255;
  };


  colorLED(r, g, b);
  if (time_count % 2000 == 0) {
  	r = 0;
  	g = 0;
  	b = 0;
  };

  delay(1);
  time_count += 1;

}


void printWifiStatus() {

  // print the SSID of the network you're attached to:

  Serial.print("SSID: ");

  Serial.println(WiFi.SSID());


  // print your board's IP address:

  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");

  Serial.println(ip);


  // print the received signal strength:

  long rssi = WiFi.RSSI();

  Serial.print("signal strength (RSSI):");

  Serial.print(rssi);

  Serial.println(" dBm");


  Serial.print("To see this page in action, open a browser to http://");

  Serial.println(ip);

}


void enable_WiFi() {

  // check for the WiFi module:

  if (WiFi.status() == WL_NO_MODULE) {

    Serial.println("Communication with WiFi module failed!");

    // don't continue

    while (true);

  }


  String fv = WiFi.firmwareVersion();

  if (fv < "1.0.0") {

    Serial.println("Please upgrade the firmware");

  }

}


void connect_WiFi() {

  // attempt to connect to Wifi network:

  while (status != WL_CONNECTED) {

    Serial.print("Attempting to connect to SSID: ");

    Serial.println(ssid);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:

    status = WiFi.begin(ssid, pass);


    // wait 10 seconds for connection:

    delay(10000);

  }

};



void printWEB() {


  if (client) {                             // if you get a client,

    Serial.println("new client");           // print a message out the serial port

    String currentLine = "";                // make a String to hold incoming data from the client

    while (client.connected()) {            // loop while the client's connected

      if (client.available()) {             // if there's bytes to read from the client,

        char c = client.read();             // read a byte, then

        Serial.write(c);                    // print it out the serial monitor

        if (c == '\n') {                    // if the byte is a newline character


          // if the current line is blank, you got two newline characters in a row.

          // that's the end of the client HTTP request, so send a response:

          if (currentLine.length() == 0) {


            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)

            // and a content-type so the client knows what's coming, then a blank line:

            client.println("HTTP/1.1 200 OK");

            client.println("Content-type:text/html");

            client.println();

           

            //create the buttons

            client.print("Click <a href=\"/H\">here</a> turn the Pump on<br>");

            client.print("Click <a href=\"/L\">here</a> turn the Pump off<br><br>");

            

	    float t = getTemp();

            client.print("Temperature: ");
	    if (t >= 15.00 && t <= 25.00) {
	    	client.print(t);
	    } else {
	    	client.print("<span style=\"color: red\">");
	    	client.print(t);
	    	client.print("</span>");
	    };

	    client.print("<br>");

	    float h = getHum();
	    client.print("Humidity: ");
	    if (h > 40.00) {
	    	client.print(h);
	    } else {
	    	client.print("<span style=\"color: red\">");
	    	client.print(h);
	    	client.print("</span>");

	    };
	    client.print("<br>");

            client.print("Water Level: ");
	    int w = getWaterLevel();
	    if (w > 100) {
	    	client.print(w);
	    } else {
	    	client.print("<span style=\"color: red\">");
	    	client.print(w);
	    	client.print("</span>");
	    };
	    client.print("<br>");

            client.print("Light Level: ");
	    client.print(getLightLevel());
	    client.print("<br>");




           

            

            


            // The HTTP response ends with another blank line:

            client.println();

            // break out of the while loop:

            break;

          }

          else {      // if you got a newline, then clear currentLine:

            currentLine = "";

          }

        }

        else if (c != '\r') {    // if you got anything else but a carriage return character,

          currentLine += c;      // add it to the end of the currentLine

        }


        if (currentLine.endsWith("GET /H")) {

	digitalWrite(WATER_PUMP_PIN, HIGH);

        }

        if (currentLine.endsWith("GET /L")) {
	digitalWrite(WATER_PUMP_PIN, LOW);


        }


      }

    }

    // close the connection:

    client.stop();

    Serial.println("client disconnected");

  }

}
