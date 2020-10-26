#include "sindicato_fs.h"

void get_or_create_fs() {

	char * punto_de_montaje = sindicato_config->punto_montaje;

	char * metadata_adress = string_new();
	string_append(&metadata_adress, sindicato_config->punto_montaje);
	string_append(&metadata_adress, "/Metadata");

	char* metadata_afip_adress = string_new();
	string_append(&metadata_afip_adress, metadata_adress);
	string_append(&metadata_afip_adress, "/Metadata.AFIP");

	get_or_create_folder(punto_de_montaje);
	get_or_create_folder(metadata_adress);
	int necesita_recrearse = 0;


	if (existe_archivo(metadata_afip_adress) ) {
		necesita_recrearse = leer_metadata_afip(metadata_afip_adress);
	} else {
		necesita_recrearse = 1;
	}

	if (necesita_recrearse) {
		/* Crear solo con write, para que se pise lo anterior */
		crear_metadata_default(metadata_afip_adress);
		crear_bitmap();
	}	
	

}
/* < 0 si no puede crear el punto de montaje, 0 si ya existia previamente, > 0 si creo correctamente el punto de montaje*/
int get_or_create_folder(char* folder_adress){

	DIR* folder_dir;

	if ((folder_dir = opendir(folder_adress)) == NULL){
		printf("No existe la carpeta: %s \n", folder_adress);
		int rv = mkdir(folder_adress, 0777);
		if (rv == 0){
			printf("Se creo la carpeta: %s \n", folder_adress);
			log_info(logger, "Se creo la carpeta: %s \n", folder_adress);
			closedir(folder_dir);
			return 1;
		}
		else {
			printf("No se pudo crear la carpeta\n");
			log_error(logger, "No se pudo crear la carpeta\n");
			return -1;
		}
	}
	else {
		closedir(folder_dir);
		return 0;
	}
}

void crear_metadata_default(char * metadata_afip_adress){
	FILE *fp;
	
	fp = fopen( metadata_afip_adress , "w" );

	char * block_size = string_new(); 
	string_append(&block_size, "BLOCK_SIZE=");
	string_append(&block_size, sindicato_config->block_size);
	string_append(&block_size, "\n");
    char * blocks = string_new(); 
	string_append(&blocks, "BLOCKS=");
	string_append(&blocks, sindicato_config->blocks);
	string_append(&blocks, "\n");
    char * magic_number = string_new(); 
	string_append(&magic_number, "MAGIC_NUMBER=");
	string_append(&magic_number, sindicato_config->magic_number);
	string_append(&magic_number, "\n");

	char * config = string_new();
	string_append(&config, block_size);
	string_append(&config, blocks);
	string_append(&config, magic_number);

	fwrite(config , 1 , sizeof(char) * (strlen(config) + 1) , fp );
	fclose(fp);
}

/* Devuelve 1 si necesita recrear el FS */
int leer_metadata_afip(char * metadata_afip_adress){
	t_config* metadata_afip_config = config_create(metadata_afip_adress);

	char * block_size = config_get_string_value(metadata_afip_config, "BLOCK_SIZE");
	char * blocks = config_get_string_value(metadata_afip_config, "BLOCKS");
	char * magic_number = config_get_string_value(metadata_afip_config, "MAGIC_NUMBER");

	printf("El tamaño de bloque es de %s\n", block_size);
	printf("La cantidad de bloques es %s\n", blocks);
	printf("El numero magico es %s\n", magic_number); 

	if (necesita_recrearse(block_size, blocks, magic_number)){
		return 1;
	}

	return 0;
}

int necesita_recrearse(char * block_size, char * blocks, char * magic_number){

	if (strcmp(block_size, sindicato_config->block_size) != 0 || strcmp(blocks, sindicato_config->blocks) != 0 || strcmp(magic_number, sindicato_config->magic_number) != 0) {
		return 1;
	}
	return 0;

}

void crear_bitmap(){
	//TODO: Crear el bitmap con todo seteado en 0 y ver como mapearlo a memoria principal.
}

int existe_archivo(char* ruta_archivo){
	if( access( ruta_archivo, F_OK ) != -1 ) {
		return 1;
	} else {
		return 0;
	}
}