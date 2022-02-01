#ifndef CONFIG_H
#define CONFIG_H


#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <FS.h>
#include <LITTLEFS.h>
#include "StringSplitter.h"


// #define DEBUG

#define RUTA_BASE_FS "/"
#define ARCHIVO_CONFIG "/config.txt"
#define ITEMS_REGISTRO_CONFIG 4
#define LED_ONBOARD 2
#define BOCINA 12
#define LUZ_ROJA 14
#define LUZ_VERDE 27
#define LUZ_AMARILLA 26
// #define GND_AUX 13 // Simplemente ponemos a GND un pin auxiliar para las pruebas con el buzzer
// #define PWM1_CANAL 0
// #define PWM1_RES 8
// #define PWM1_FREC 1000
// #define PWM1_DUTY 255

#define WS_PUERTO 80
#define BAUDIOS_SERIE 115200
#define CTD_LUCES 3
#define TIEMPO_BOCINA_ACTIVA 2000 // ms


const char* WIFI_CL_SSID = "wifi_local";
const char* WIFI_CL_PWD = "clave";
const char* WIFI_AP_SSID = "nombre_ap_panel";
const char* WIFI_AP_PWD = "clave";


int indice_luz = 0;
bool todo_ok = false;
bool ciclando = false;
bool bocina_activa = false;
uint32_t timer_bocina = 0;
uint32_t timer_espera = 0;


WebSocketsServer webSocket = WebSocketsServer(WS_PUERTO);

struct luz { int pin; int espera; String nombre; bool activar_bocina; };
luz luz_roja = {};
luz luz_verde = {};
luz luz_amarilla = {};
luz luces[CTD_LUCES] = { luz_roja, luz_verde, luz_amarilla };


// Luz luz_roja2 = Luz("roja", LUZ_ROJA, 5000, true);


#endif
