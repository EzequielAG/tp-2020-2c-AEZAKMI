#include "proceso2.h"
int main(void){
    t_log* logger = log_create("./cfg/proceso2.log", "PROCESO2", true, LOG_LEVEL_INFO);
    log_info(logger, "Soy el proceso 2! %s", mi_funcion_compartida());
    
	// PARTE 3
	int conexion = crear_conexion("127.0.0.1","6010");

	enviar_mensaje("HOLA", conexion);

	t_paquete* paquete = armar_paquete();

	enviar_paquete(paquete, conexion);

	// Viejo terminar programa
	eliminar_paquete(paquete);
    log_destroy(logger);
	close(conexion);

}


void leer_consola(t_log* logger) {
	void loggear(char* leido) {
		log_info(logger, leido);
	}

	_leer_consola_haciendo((void*) loggear);
}

t_paquete* armar_paquete() {
	t_paquete* paquete = crear_paquete();

	void _agregar(char* leido) {
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
	}

	_leer_consola_haciendo((void*) _agregar);

	return paquete;
}
void _leer_consola_haciendo(void(*accion)(char*)) {
	char* leido = readline(">");

	while(strncmp(leido, "", 1) != 0) {
		accion(leido);
		free(leido);
		leido = readline(">");
	}

	free(leido);
}