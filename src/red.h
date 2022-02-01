#ifndef RED_H
#define RED_H


#include "config.h"


void inicializar_wifi_ap() {
	WiFi.disconnect(true);
	WiFi.mode(WIFI_AP);
	while (!WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PWD, 1, false, 2)) delay(500);
	IPAddress ip = WiFi.softAPIP();

	#ifdef DEBUG
	Serial.printf("AP inicializado en %u.%u.%u.%u\n", ip & 0xff, (ip >> 8) & 0xff, (ip >> 16) & 0xff, ip >> 24);
	#endif
}

void inicializar_wifi_sta() {
	WiFi.disconnect(true);
	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI_CL_SSID, WIFI_CL_PWD);
	while (WiFi.status() != WL_CONNECTED) delay(500);

	#ifdef DEBUG
	Serial.print("[DEBUG] Conectado a red wifi con IP ");
	Serial.println(WiFi.localIP());
	#endif
}

void eventos_ws(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
	switch (type) {
		case WStype_DISCONNECTED:
			digitalWrite(LED_ONBOARD, LOW);

			Serial.printf("[DEBUG] Cliente %u desconectado\n", num);
		break;
		
		case WStype_CONNECTED: {
			digitalWrite(LED_ONBOARD, HIGH);

			char mensaje[64];
			ciclando ? sprintf(mensaje, "ESTADOI:CICLANDO,%d,%d,%d\n", luces[0].espera, luces[1].espera, luces[2].espera) : sprintf(mensaje, "ESTADOI:DETENIDO,%d,%d,%d\n", luces[0].espera, luces[1].espera, luces[2].espera);
			webSocket.sendTXT(num, mensaje);
			
			#ifdef DEBUG
			IPAddress ip = webSocket.remoteIP(num);
			Serial.printf("[DEBUG] Cliente %u conectado desde %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
			#endif
		}
		break;

		case WStype_TEXT: {
			String contenido = "";
			for (int i = 0; i < (int)length; i++) contenido += (char)payload[i];
			StringSplitter *splitter = new StringSplitter(contenido, ':', 2); // activar:true
			String comando = splitter->getItemAtIndex(0);
			String valor = splitter->getItemAtIndex(1);
			delete splitter;

			if (comando == "ciclar" && valor.indexOf("true") != -1) {
				ciclar();
			} else if (comando == "ciclar" && valor.indexOf("false") != -1) {
				detener();
			} else if (comando == "roja") {
				luces[0].espera = valor.toInt();
				if (actualizar_config(LITTLEFS, ARCHIVO_CONFIG)) webSocket.sendTXT(num, "TIEMPO:actualizado");
			} else if (comando == "verde") {
				luces[1].espera = valor.toInt();
				if (actualizar_config(LITTLEFS, ARCHIVO_CONFIG)) webSocket.sendTXT(num, "TIEMPO:actualizado");
			} else if (comando == "amarilla") {
				luces[2].espera = valor.toInt();
				if (actualizar_config(LITTLEFS, ARCHIVO_CONFIG)) webSocket.sendTXT(num, "TIEMPO:actualizado");
			}
			// webSocket.broadcastTXT();
			
			#ifdef DEBUG
			Serial.printf("[DEBUG] Mensaje desde cliente %u: %s\n", num, payload);
			#endif
		}
		break;

		case WStype_PING:
			break;
		
		case WStype_PONG:
			break;

		case WStype_BIN:
			// hexdump(payload, length);
			// webSocket.sendBIN(num, payload, length);
			break;

		case WStype_ERROR:
		case WStype_FRAGMENT_TEXT_START:
		case WStype_FRAGMENT_BIN_START:
		case WStype_FRAGMENT:
		case WStype_FRAGMENT_FIN:
			break;
	}
}

void inicializar_ws() {
	webSocket.begin();
    webSocket.onEvent(eventos_ws);
	
	#ifdef DEBUG
	Serial.println("[DEBUG] Servidor websocket activado");
	#endif
}


#endif
