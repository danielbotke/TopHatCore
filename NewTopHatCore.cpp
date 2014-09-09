#pragma once

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x9B, 0x36 }; //physical mac address
byte ip[] = { 192, 168, 0, 100 }; // ip in lan
byte gateway[] = { 192, 168, 0, 1 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
EthernetServer server(80); //server port

String readString;
String port;
char search[100];
int counter = 0;
int cont;

const int homeId = 2; //Home identificator
const int kL = 31; //Kitchen Light
const int kLSt = 32; //Status Kitchen Light
const int kW = 33; //Kitchen Window
const int kWSt = 34; //Status Kitchen Window
const int lL = 35; //Living Light
const int lLSt = 36; //Status Living Light
const int lW = 37; //Living Window
const int lWSt = 38; //Status Living Window
const int b1L = 39; //Bedroom1 Light
const int b1LSt = 40; //Status Bedroom 1 Light
const int b1W = 41; //Bedroom1 Window
const int b1WSt = 42;  //Status Bedroom 1 Window
const int b2L = 43; //Bedroom2 Light
const int b2LSt = 44; //Status Bedroom 2 Light
const int b2W = 45; //Bedroom2 Window
const int b2WSt = 46; //Status Bedroom 2 Window

void setup() {
	pinMode(kL, OUTPUT);
	pinMode(kW, OUTPUT);
	pinMode(lL, OUTPUT);
	pinMode(lW, OUTPUT);
	pinMode(b1L, OUTPUT);
	pinMode(b1W, OUTPUT);
	pinMode(b2L, OUTPUT);
	pinMode(b2W, OUTPUT);

	pinMode(kLSt, INPUT);
	pinMode(kWSt, INPUT);
	pinMode(lLSt, INPUT);
	pinMode(lWSt, INPUT);
	pinMode(b1LSt, INPUT);
	pinMode(b1WSt, INPUT);
	pinMode(b2LSt, INPUT);
	pinMode(b2WSt, INPUT);

	//start shield Ethernet
	Ethernet.begin(mac, ip, gateway, subnet);
	server.begin();
	// Serial.begin(9600);
}

void loop() {
	// Create a client connection
	EthernetClient client = server.available();
	if (client) {
		while (client.connected()) {
			if (client.available()) {
				char c = client.read();

				//read char by char HTTP request
				if (cont < 100) {

					//store characters to string
					search[cont] = c;
					cont++;

					//Serial.print(c);
				}

				//if HTTP request has ended
				if (c == '\n') {
					for (int i = 0; i < sizeof(search); i++) {
						if (search[i] == '?') {
							if (search[i + 1] == 'l') {
								port = "";
								port += search[i + 2];
								if (isDigit(search[i + 3])) {
									port += search[i + 3];
								}
								int portNum = port.toInt();
								digitalWrite(portNum, HIGH);
								//   Serial.println("Luz ligada. " + port);
								i = sizeof(search);
							} else if (search[i + 1] == 'd') {
								port = "";
								port += search[i + 2];
								if (isDigit(search[i + 3])) {
									port += search[i + 3];
								}
								int portNum = port.toInt();
								digitalWrite(portNum, LOW);
								//Serial.println("Luz desligada.");
								i = sizeof(search);
							}

						}
					}

					//clearing string for next read
					cont = 0;

					client.println(homeId);
					client.println("/Kitchen;l;");
					client.println(kL);
					client.println(";");
					client.println(digitalRead(kLSt));
					client.println(";w;");
					client.println(kW);
					client.println(";");
					client.println(digitalRead(kWSt));
					client.println("/Living;l;");
					client.println(lL);
					client.println(";");
					client.println(digitalRead(lLSt));
					client.println(";w;");
					client.println(lW);
					client.println(";");
					client.println(digitalRead(lWSt));
					client.println("/Bad1;l;");
					client.println(b1L);
					client.println(";");
					client.println(digitalRead(b1LSt));
					client.println(";w;");
					client.println(b1W);
					client.println(";");
					client.println(digitalRead(b1WSt));
					client.println("/Bad2;l;");
					client.println(b2L);
					client.println(";");
					client.println(digitalRead(b2LSt));
					client.println(";w;");
					client.println(b2W);
					client.println(";");
					client.println(digitalRead(b2WSt));
					delay(1);
					//stopping client
					client.stop();

				}
			}
		}
	}
	if (counter == 50000000000) {
		char Temp[10];
		IPAddress server(192, 168, 0, 21); // Número IP do WebService

		// if you get a connection, report back via serial:
		if (client.connect(server, 80)) {
			// Make a HTTP request:
			client.println("POST /ws/WsTemp.asmx/incluir HTTP/1.1");
			client.println("Host: localhost");
			client.println("Content-Type: application/x-www-form-urlencoded");
			client.println("Content-Length: 342");
			client.println();
			client.println();
		}
		if (client.available()) {
			char c = client.read();
		}
	}
	counter ++;

}

