#pragma once

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x9B, 0x36 }; //physical mac address
byte ip[] = { 192, 168, 0, 100 }; // ip in lan
byte gateway[] = { 192, 168, 0, 1 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
EthernetServer server(9898); //server port
unsigned int localPort = 8888;

String readString;
char search[100];
int counter = 0;
int cont;
String port = "";
EthernetUDP Udp;

const String homeId = "4_1LFN-C#|"; //Home identificator
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
	Udp.begin(localPort);
	server.begin();
	Serial.begin(9600);
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
  				//stopping client
					client.stop();
				}
			}
		}
	}
	if (counter == 5) {
           //     Serial.println("Entrou");
		String aux = homeId + "/Kitchen;l;" + kL + ";" + digitalRead(kLSt)+ ";w;" + kW + ";" + digitalRead(kWSt) + "/Living;l;" + lL + ";" + digitalRead(lLSt) + ";w;" + lW + ";" + digitalRead(lWSt) + "/Bad1;l;" + b1L + ";" + digitalRead(b1LSt) + ";w;" + b1W + ";" + digitalRead(b1WSt) + "/Bad2;l;" + b2L + ";" + digitalRead(b2LSt) + ";w;" + b2W + ";" + digitalRead(b2WSt);
		delay(1);
        IPAddress remote(192, 168, 0, 102);
        // send a reply, to the IP address and port that sent us the packet we received
		int success = Udp.beginPacket(remote, 9090);
		 Serial.print("Init Packet: ");
		 Serial.println(success);
		success = Udp.print(aux);
		 Serial.print("Pacote enviado: ");
         Serial.println(success);
		success = Udp.endPacket();
		 Serial.print("End Packet: ");
		 Serial.println(success);

		 Serial.print("IP Remoto: ");
		 Serial.println(Udp.remoteIP());

		 Serial.print("Porta Remota: ");
		 Serial.println(Udp.remotePort());

		// Encerra este socket
		Udp.stop();
		delay(10);
        counter = 0;
	}
        counter++;
}

