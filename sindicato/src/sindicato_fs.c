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
		crear_files();
	}	
	

}
/* < 0 si no puede crear el punto de montaje, 0 si ya existia previamente, > 0 si creo correctamente el punto de montaje*/
int get_or_create_folder(char* file_adress){

	DIR* folder_dir;

	if ((folder_dir = opendir(file_adress)) == NULL){
		printf("No existe la carpeta: %s \n", file_adress);
		int rv = mkdir(file_adress, 0777);
		if (rv == 0){
			printf("Se creo la carpeta: %s \n", file_adress);
			log_info(logger, "Se creo la carpeta: %s \n", file_adress);
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


/* --- BITMAP --- */
FILE * get_or_create_bitmap_file(char * mode){
	char * bitmap_address = string_new();
	string_append(&bitmap_address, sindicato_config->punto_montaje);
	string_append(&bitmap_address, "/Metadata/Bitmap.bin");

	FILE * bitmap_file = fopen(bitmap_address, mode);
	free(bitmap_address);

	return bitmap_file;
}

void update_bitmap_file(t_bitarray * bitmap){
	FILE * bitmap_file = get_or_create_bitmap_file("wb");
	if (bitmap_file == NULL)
		log_error(logger, "No se pudo obtener 'bitmap file'");

	fwrite(bitmap->bitarray, sizeof(char), bitmap->size, bitmap_file);

	fclose(bitmap_file);
	free(bitmap->bitarray);
	free(bitmap);
}

void crear_bitmap(){
	FILE * bitmap_file = get_or_create_bitmap_file("rb");
	if (bitmap_file == NULL)
		log_error(logger, "No se pudo obtener 'bitmap file'");

	int blocks = atoi(sindicato_config->blocks);

	size_t bytes = BIT_SIZE(blocks, CHAR_BIT);
	char * bitarray = calloc(bytes, sizeof(char));
	t_bitarray	* bitmap = bitarray_create_with_mode(bitarray, bytes, LSB_FIRST);

	for(int pos=0; pos < blocks; pos++){
		//Limpia los bits del bitarray (Los pone en 0)
		bitarray_clean_bit(bitmap, pos);
	}
	update_bitmap_file(bitmap);

	bitarray_destroy(bitmap);
}

t_bitarray * get_bitarray(){
	FILE * bitmap_file = get_or_create_bitmap_file("rb");
	if (bitmap_file == NULL)
		log_error(logger, "No se pudo obtener 'bitmap file'");

	int blocks = atoi(sindicato_config->blocks);

	size_t bitarray_size = BIT_SIZE(blocks, CHAR_BIT);
	char * bitarray = malloc(bitarray_size);

	return bitarray_create_with_mode(bitarray, bitarray_size, LSB_FIRST);
}

void modify_block(t_bitarray * bitmap, bool status, int position){
	if(status) //si esta libre ese bloque
		bitarray_set_bit(bitmap, position);

	else   //si esta ocupado ese bloque
		bitarray_clean_bit(bitmap, position);
}

void take_block(int block_pos){
	// Setea 1 en la posicion indicada (bloque ocupado)
	pthread_mutex_lock(&mutex_bitmap);
	t_bitarray * bitmap = get_bitarray();
	modify_block(bitmap, true, block_pos);
	update_bitmap_file(bitmap);
	pthread_mutex_unlock(&mutex_bitmap);
}

void free_block(int block_pos){
	// Setea 0 en la posicion indicada (bloque libre)
	pthread_mutex_lock(&mutex_bitmap);
	t_bitarray * bitmap = get_bitarray();
	modify_block(bitmap, false, block_pos);
	update_bitmap_file(bitmap);
	pthread_mutex_unlock(&mutex_bitmap);
}
/* --- END BITMAP --- */


int existe_archivo(char* ruta_archivo){
	if( access( ruta_archivo, F_OK ) != -1 ) {
		return 1;
	} else {
		return 0;
	}
}

void crear_files(){
	char * files_adress = string_new();
	string_append(&files_adress, sindicato_config->punto_montaje);
	string_append(&files_adress, "/Files");

	char * restaurantes_adress = string_new();
	string_append(&restaurantes_adress, files_adress);
	string_append(&restaurantes_adress, "/Restaurantes");

	char * recetas_adress = string_new();
	string_append(&recetas_adress, files_adress);
	string_append(&recetas_adress, "/Recetas");

	char * bloques_adress = string_new();
	string_append(&bloques_adress, sindicato_config->punto_montaje);
	string_append(&bloques_adress, "/Blocks");

	get_or_create_folder(files_adress);
	get_or_create_folder(restaurantes_adress);
	get_or_create_folder(recetas_adress);
	get_or_create_folder(bloques_adress);
}

bool existe_restaurante(char* restaurante){
	DIR* folder_dir;

	char* file_adress = string_new();
	string_append(&file_adress, sindicato_config->punto_montaje);
	string_append(&file_adress, "/Files/Restaurantes/");
	string_append(&file_adress, restaurante);

	if ((folder_dir = opendir(file_adress)) == NULL){
		return false;
	} else {
		closedir(folder_dir);
		return true;
	}
}

int existe_pedido(char* restaurante, char* nro_pedido){
	FILE *fp;

	char* file_adress = string_new();
	string_append(&file_adress, sindicato_config->punto_montaje);
	string_append(&file_adress, "/Files/Restaurantes/");
	string_append(&file_adress, restaurante);
	string_append(&file_adress, "/Pedido");
	string_append(&file_adress, nro_pedido);
	string_append(&file_adress, ".AFIP");

	if ((fp = fopen(file_adress, "r")) == NULL){
		return 0;
	} else {
		fclose(fp);
		return 1;
	}
}

int existe_receta(char* receta){
	FILE *fp;

	char* file_adress = string_new();
	string_append(&file_adress, sindicato_config->punto_montaje);
	string_append(&file_adress, "/Files/Recetas/");
	string_append(&file_adress, receta);
	string_append(&file_adress, ".AFIP");

	if ((fp = fopen(file_adress, "r")) == NULL){
		return 0;
	} else {
		fclose(fp);
		return 1;
	}
}