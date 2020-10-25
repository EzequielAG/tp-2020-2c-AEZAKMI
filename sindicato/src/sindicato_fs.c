#include "sindicato_fs.h"

void get_or_create_fs() {
	int se_creo_montaje = get_or_create_montaje_point();
	int se_creo_metadata = get_or_create_metadata();

	if (se_creo_montaje > 0 || se_creo_metadata > 0) {
		crear_metadata_default();
	}

	leer_metadata_afip();

	if (se_creo_montaje > 0 || se_creo_metadata > 0) {
		crear_bitmap();
	}
	
	
}
/* < 0 si no puede crear el punto de montaje, 0 si ya existia previamente, > 0 si creo correctamente el punto de montaje*/
int get_or_create_montaje_point(){

	DIR* punto_montaje_dir;

	if ((punto_montaje_dir = opendir(sindicato_config->punto_montaje)) == NULL){
		printf("No existe punto de montaje: %s \n", sindicato_config->punto_montaje);
		int rv = mkdir(sindicato_config->punto_montaje, 0777);
		if (rv == 0){
			printf("Se creo punto de montaje: %s \n", sindicato_config->punto_montaje);
			log_info(logger, "Se creo punto de montaje: %s \n", sindicato_config->punto_montaje);
			closedir(punto_montaje_dir);
			return 1;
		}
		else {
			printf("No se pudo crear el punto de montaje\n");
			log_error(logger, "No se pudo crear el punto de montaje\n");
			return -1;
		}
	}
	else {
		closedir(punto_montaje_dir);
		return 0;
	}
}

/* < 0 si no puede crearla carpeta metadata, 0 si ya existia previamente, > 0 si creo correctamente el punto de montaje*/
int get_or_create_metadata(){

	DIR* punto_metadata_dir;
	char* metadata_adress = string_new();
	string_append(&metadata_adress, sindicato_config->punto_montaje);
	string_append(&metadata_adress, "/Metadata");

	if ((punto_metadata_dir = opendir(sindicato_config->punto_montaje)) == NULL){
		printf("No existe la metadata: %s \n", metadata_adress);
		int rv = mkdir(sindicato_config->punto_montaje, 0777);
		if (rv == 0){
			printf("Se creo la metadata: %s \n", metadata_adress);
			log_info(logger, "Se creo la metadata: %s \n", metadata_adress);
			closedir(punto_metadata_dir);
			return 1;
		}
		else {
			printf("No se pudo crear la metadata\n");
			log_error(logger, "No se pudo la metadata\n");
			return -1;
		}
	}
	else {
		closedir(punto_metadata_dir);
		return 0;
	}
}

void crear_metadata_default(){
	//TODO: Crear Metadata.AFIP dentro de la carpeta metadata, se puede ver si se devuelve directamente
}

void leer_metadata_afip(){
	//TODO: Leer Metadata.AFIP y checkear si tiene configurado cosas o no, nose si guardar los datos en alguna variable global o algo por el estilo
}

void crear_bitmap(){
	//TODO: Crear el bitmap con todo seteado en 0 y ver como mapearlo a memoria principal.
}