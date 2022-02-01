#include "config.h"
#include "main.h"
#include "archivos.h"
#include "red.h"


void setup() {
	#ifdef DEBUG
	Serial.begin(BAUDIOS_SERIE);
	#endif

	inicializar_pines();
	// formatear_fs(); // Habilitar manualmente para formatear la primera vez
	if (inicializar_fs()) {
		if (inicializar_config(LITTLEFS, ARCHIVO_CONFIG)) {
			// inicializar_wifi_ap(); // Utilizar en lugar de STA para cuando se quiera trabajar a campo
			inicializar_wifi_sta();
			inicializar_ws();
			// Luz::apagar_todas();

			digitalWrite(LED_ONBOARD, HIGH);
			delay(3000);
			digitalWrite(LED_ONBOARD, LOW);
			todo_ok = true;

			// ciclar(); // Solo para testeo
		}
	}
}


void loop() {
	if (todo_ok) {
		webSocket.loop();

		// Si el marcador ciclando es true, cicla las luces según el tiempo de espera indicado para cada una y luego aguarda en rojo
		if (ciclando) {
			if (millis() - timer_espera >= luces[indice_luz].espera) {
				indice_luz++;
				indice_luz == CTD_LUCES ? detener() : encender(indice_luz);
				// indice_luz = (indice_luz + 1) % CTD_LUCES;
				
				timer_espera = millis();
			}

			// Si el marcador bocina_activa es true, deshabilita la bocina luego de TIEMPO_BOCINA_ACTIVA
			if (bocina_activa) {
				if (millis() - timer_bocina >= TIEMPO_BOCINA_ACTIVA) {
					digitalWrite(BOCINA, LOW);
					// ledcWrite(PWM1_CANAL, 0);
					bocina_activa = false;
					timer_bocina = millis();

					#ifdef DEBUG
					Serial.println("[DEBUG] Bocina desactivada");
					#endif
				}
			}
		}
	}
}
