#pragma once

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x9B, 0x36 }; //physical mac address
byte ip[] = { 192, 168, 1, 104 }; // ip in lan
byte gateway[] = { 192, 168, 1, 1 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
EthernetServer server(80); //server port

String readString;
String port;
char search[100];
int cont;

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

					client.println("HTTP/1.1 200 OK"); //send new page
					client.println("Content-Type: text/html");
					client.println();

					client.println("<html>");
					client.println("<head>");

					client.println(
							"<title>TopHat Automa&ccedil;&atilde;o</title>");
					client.println(
							"<meta http-equiv='X-UA-Compatible' content='IE=edge'/>");
					client.println(
							"<meta name='viewport' content='width=device-width, initial-scale=1'/>");
					client.println(
							"<link rel='stylesheet' type='text/css' href='http://192.168.1.107:8080/TopHat/css/bootstrap.min.css' />");
					client.println(
							"<link rel='stylesheet' href='http://192.168.1.107:8080/TopHat/css/dashboard.css'/>");
					client.println(
							"<link rel='stylesheet' href='http://192.168.1.107:8080/TopHat/TopHat.css'/>");
					client.println(
							"<script type='text/javascript' src='http://192.168.1.107:8080/TopHat/TopHat.js'></script>");
					client.println(
							"<style id='holderjs-style' type='text/css'></style>");
					client.println(
							"<style>"
							"[touch-action='none']{ "
							"	-ms-touch-action: none; "
							"	touch-action: none; "
							"}"
							"[touch-action='pan-x']{ "
							"	-ms-touch-action: pan-x; "
							"	touch-action: pan-x; "
							"}"
							"[touch-action='pan-y']{ "
							"	-ms-touch-action: pan-y; "
							"	touch-action: pan-y; "
							"}"
							"[touch-action='scroll'],[touch-action='pan-x pan-y'],[touch-action='pan-y pan-x']{ "
							"	-ms-touch-action: pan-x pan-y; "
							"	touch-action: pan-x pan-y; "
							"}"
							"</style>");
					client.println("<style id='clearly_highlighting_css' type='text/css'>"
							"/* selection */ "
							"html.clearly_highlighting_enabled ::-moz-selection { "
							"	background: rgba(246, 238, 150, 0.99); "
							"} "
							"html.clearly_highlighting_enabled ::selection { "
							"	background: rgba(246, 238, 150, 0.99); "
							"} "
							"/* cursor */ html.clearly_highlighting_enabled {    "
							"/* cursor and hot-spot position -- requires a default cursor, after the URL one */    "
							"	cursor: url('chrome-extension://pioclpoplcdbaefihamjohnefbikjilc/clearly/images/highlight--cursor.png') 14 16, text; "
							"} "
							"/* highlight tag */ em.clearly_highlight_element {    "
							"	font-style: inherit !important; "
							"	font-weight: inherit !important;    "
							"	background-image: url('chrome-extension://pioclpoplcdbaefihamjohnefbikjilc/clearly/images/highlight--yellow.png');    "
							"	background-repeat: repeat-x; background-position: top left; background-size: 100% 100%; "
							"} "
							"/* the delete-buttons are positioned relative to this */ em.clearly_highlight_element.clearly_highlight_first { "
							"	position: relative; "
							"} "
							"/* delete buttons */ "
							"em.clearly_highlight_element a.clearly_highlight_delete_element {    "
							"	display: none; "
							"	cursor: pointer;    "
							"	padding: 0; "
							"	margin: 0; "
							"	line-height: 0;    "
							"	position: absolute; "
							"	width: 34px; "
							"	height: 34px; "
							"	left: -17px; "
							"	top: -17px;    "
							"	background-image: url('chrome-extension://pioclpoplcdbaefihamjohnefbikjilc/clearly/images/highlight--delete-sprite.png'); "
							"	background-repeat: no-repeat; background-position: 0px 0px; "
							"} "
							"em.clearly_highlight_element a.clearly_highlight_delete_element:hover { "
							"	background-position: -34px 0px; "
							"} "
							"/* retina */ "
							"@media (min--moz-device-pixel-ratio: 2), (-webkit-min-device-pixel-ratio: 2), (min-device-pixel-ratio: 2) {    "
							"	em.clearly_highlight_element { "
							"		background-image: url('chrome-extension://pioclpoplcdbaefihamjohnefbikjilc/clearly/images/highlight--yellow@2x.png'); "
							"	}    "
							"	em.clearly_highlight_element a.clearly_highlight_delete_element { "
							"		background-image: url('chrome-extension://pioclpoplcdbaefihamjohnefbikjilc/clearly/images/highlight--delete-sprite@2x.png'); "
							"		background-size: 68px 34px; "
							"	} "
							"}"
							"</style>");
					client.println("</head>");
					client.println("<body onload='open()'>");

					client.println(
							"<div class='navbar navbar-inverse navbar-fixed-top' role='navigation'>");
					client.println("<div class='container-fluid'>");
					client.println("<div class='navbar-header'>");
					client.println(
							"<a class='navbar-brand' href='#'>TopHat Automa&ccedil;&atilde;o</a>");
					client.println("</div>");
					client.println("<div class='navbar-collapse collapse'>");
					client.println("<ul class='nav navbar-nav navbar-right'>");
					client.println("<li><a href='#'>Configura&ccedil;&otilde;es</a></li>");
					client.println("<li><a href='#'>Perfil</a></li>");
					client.println("<li><a href='#'>Ajuda</a></li>");
					client.println("</ul>");

					client.println("</div></div></div>");

					client.println(
							"<div class='data' id='dtRooms'>Kitchen;Living;Bedroom1;Bedroom2</div>");

					client.println("<div class='data' id='dtKitchen'>l;");
					client.println(kL);
					client.println(";");
					client.println(digitalRead(kLSt));
					client.println(";w;");
					client.println(kW);
					client.println(";");
					client.println(digitalRead(kWSt));
					client.println("</div>");

					client.println("<div class='data' id='dtLiving'>l;");
					client.println(lL);
					client.println(";");
					client.println(digitalRead(lLSt));
					client.println(";w;");
					client.println(lW);
					client.println(";");
					client.println(digitalRead(lWSt));
					client.println("</div>");

					client.println("<div class='data' id='dtBedroom1'>l;");
					client.println(b1L);
					client.println(";");
					client.println(digitalRead(b1LSt));
					client.println(";w;");
					client.println(b1W);
					client.println(";");
					client.println(digitalRead(b1WSt));
					client.println("</div>");

					client.println("<div class='data' id='dtBedroom2'>l;");
					client.println(b2L);
					client.println(";");
					client.println(digitalRead(b2LSt));
					client.println(";w;");
					client.println(b2W);
					client.println(";");
					client.println(digitalRead(b2WSt));
					client.println("</div>");

					client.println("<div class='container-fluid'>");
					client.println("<div class='row'>");
					client.println("<div class='col-sm-3 col-md-2 sidebar'>");
					client.println("<ul class='nav nav-sidebar' id='rooms'>");
					client.println("</ul></div>");
					client.println("<div class='col-sm-9 col-sm-offset-3 col-md-10 col-md-offset-2 main' id='central'></div>");
					client.println("</div></div>");
					client.println("<script src='https://ajax.googleapis.com/ajax/libs/jquery/1.11.0/jquery.min.js'></script>");
					client.println("<script src='http://192.168.1.107:8080/TopHat/js/bootstrap.min.js'></script>");
					client.println("<script src='http://192.168.1.107:8080/TopHat/js/docs.min.js'></script>");
					client.println("</body>");
					client.println("</html>");

					delay(1);
					//stopping client
					client.stop();

				}
			}
		}
	}
}

