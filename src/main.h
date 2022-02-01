#ifndef MAIN_H
#define MAIN_H


#include "config.h"


// class Luz {
// 	public:
// 		Luz(String nombre, int pin, int espera, bool activar_bocina) {
// 			this->nombre = nombre;
// 			this->pin = pin;
// 			this->espera = espera;
// 			this->activar_bocina = activar_bocina;

// 			pinMode(this->pin, OUTPUT);
// 			digitalWrite(this->pin, LOW);
// 		}

// 		static void apagar_todas() {
// 			for (size_t i = 0; i < CTD_LUCES; i++) { digitalWrite(luces[i].pin, LOW); }	
// 			digitalWrite(BOCINA, LOW);
// 		}
	
// 	private:
// 		String nombre;
// 		int pin;
// 		int espera;
// 		bool activar_bocina;
// };


void inicializar_pines() {
	pinMode(LED_ONBOARD, OUTPUT);
	pinMode(BOCINA, OUTPUT);
	pinMode(LUZ_ROJA, OUTPUT);
	pinMode(LUZ_VERDE, OUTPUT);
	pinMode(LUZ_AMARILLA, OUTPUT);
	// pinMode(GND_AUX, OUTPUT);

	digitalWrite(LED_ONBOARD, LOW);
	digitalWrite(BOCINA, LOW);
	digitalWrite(LUZ_ROJA, LOW);
	digitalWrite(LUZ_VERDE, LOW);
	digitalWrite(LUZ_AMARILLA, LOW);
	// digitalWrite(GND_AUX, LOW);
	
	// ledcAttachPin(BOCINA, PWM1_CANAL);
	// ledcSetup(PWM1_CANAL, PWM1_FREC, PWM1_RES);
}

void apagar_luces() {
	for (size_t i = 0; i < CTD_LUCES; i++) { digitalWrite(luces[i].pin, LOW); }	
	digitalWrite(BOCINA, LOW);
	// ledcWrite(PWM1_CANAL, 0);
}

void encender(int luz, bool suprimir_bocina = false) {
	apagar_luces();
	
	digitalWrite(luces[luz].pin, HIGH);
	
	Serial.printf("ENCIENDE:%s\n", luces[luz].nombre.c_str());
	
	if (luces[luz].activar_bocina && !suprimir_bocina) {
		digitalWrite(BOCINA, HIGH);
		// ledcWrite(PWM1_CANAL, PWM1_DUTY);
		bocina_activa = true;
		timer_bocina = millis();

		#ifdef DEBUG
		Serial.println("[DEBUG] Bocina activa");
		#endif
	}

	if (webSocket.connectedClients() > 0) {
		char mensaje[32];
		sprintf(mensaje, "ENCIENDE:%s", luces[luz].nombre.c_str());
		webSocket.broadcastTXT(mensaje);
		
		#ifdef DEBUG
		Serial.printf("[DEBUG] Enciende %s\n", luces[luz].nombre.c_str());
		#endif
	}
}

void ciclar() {
	ciclando = true;
	encender(0);
	if (webSocket.connectedClients() > 0) webSocket.broadcastTXT("ESTADO:CICLANDO");
	timer_espera = millis();
}

void detener() {
	ciclando = false;
	indice_luz = 0;
	// apagar_luces();
	encender(0, true);
	if (webSocket.connectedClients() > 0) webSocket.broadcastTXT("ESTADO:DETENIDO");
	timer_espera = millis();
}


#endif
