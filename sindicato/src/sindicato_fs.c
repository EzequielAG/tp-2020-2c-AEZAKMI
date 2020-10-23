#include "sindicato_fs.h"

void get_or_create_fs() {
	DIR* punto_montaje_dir;

	if ((punto_montaje_dir = opendir(sindicato_config->punto_montaje)) == NULL){
		printf("No existe punto de montaje: %s \n", sindicato_config->punto_montaje);
		int rv = mkdir(sindicato_config->punto_montaje, 0777);
		if (rv == 0){
			printf("Se creo punto de montaje: %s \n", sindicato_config->punto_montaje);
			log_info(logger, "Se creo punto de montaje: %s \n", sindicato_config->punto_montaje);
			closedir(punto_montaje_dir);
		}
		else {
			printf("No se pudo crear el punto de montaje\n");
			log_error(logger, "No se pudo crear el punto de montaje\n");
			exit(-1);
		}
	}
	else {
		closedir(punto_montaje_dir);
	}
}
