#ifndef ARCHIVOS_H
#define ARCHIVOS_H


#include "config.h"


void formatear_fs() {
	if (!LITTLEFS.begin(true)) {
		#ifdef DEBUG
		Serial.println("Error al formatear sistema de archivos LITTLEFS");
		#endif
		return;
	}

	#ifdef DEBUG
	Serial.println("LITTLEFS formateado");
	#endif
}

bool inicializar_fs() {
	if (!LITTLEFS.begin(false)) {
		#ifdef DEBUG
		Serial.println("Error al montar sistema de archivos LITTLEFS");
		#endif
		return false;
	}

	#ifdef DEBUG
	Serial.println("LITTLEFS montado");
	#endif
	return true;
}

bool inicializar_config(fs::FS &fs, const char* ruta) {
	File archivo = fs.open(ruta, FILE_READ);
	if(!archivo || archivo.isDirectory()) { return false; }

	int registro = 0;
	while (archivo.available()) {
		StringSplitter *splitter = new StringSplitter(archivo.readStringUntil('\n'), ',', ITEMS_REGISTRO_CONFIG);
		luces[registro].nombre = (String)splitter->getItemAtIndex(0);
		luces[registro].pin = splitter->getItemAtIndex(1).toInt();
		luces[registro].espera = splitter->getItemAtIndex(2).toInt();
		luces[registro].activar_bocina = splitter->getItemAtIndex(3).indexOf("true") == -1 ? false: true;
		
		#ifdef DEBUG
		Serial.printf("[DEBUG] %s, %d, %d, %s\n", luces[registro].nombre.c_str(), luces[registro].pin, luces[registro].espera, luces[registro].activar_bocina ? "true" : "false");
		#endif

		registro++;
	}
	
	archivo.close();

	return true;
}

bool actualizar_config(fs::FS &fs, const char* ruta) {
	File archivo = fs.open(ruta, FILE_WRITE);
	if(!archivo || archivo.isDirectory()) { return false; }

	char contenido_config[96];
	sprintf(contenido_config, "%s,%d,%d,%s\n", luces[0].nombre.c_str(), luces[0].pin, luces[0].espera, luces[0].activar_bocina ? "true" : "false");
	for (size_t i = 1; i < CTD_LUCES; i++) {
		sprintf(contenido_config + strlen(contenido_config), "%s,%d,%d,%s\n", luces[i].nombre.c_str(), luces[i].pin, luces[i].espera, luces[i].activar_bocina ? "true" : "false");
	}

	#ifdef DEBUG
	Serial.print(contenido_config);
	#endif
	
	bool retorno = archivo.print(contenido_config);

	archivo.close();

	return retorno;
}

#endif
