#include "sindicato_fs.h"

/* --- SUITES DE PATH O RUTAS --- */
char* get_path_restaurante(char* restaurante){
	char* path = string_new();
	string_append(&path, sindicato_config->punto_montaje);
	string_append(&path, "Files/Restaurantes/");
	string_append(&path, restaurante);
	return path;
}

char* get_path_info_file(char* restaurante){
	char* path = string_new();
	string_append(&path, sindicato_config->punto_montaje);
	string_append(&path, "Files/Restaurantes/");
	string_append(&path, restaurante);
	string_append(&path, "/Info.AFIP");
	return path;
}

char* get_path_pedido_file(char* restaurante, char* id_pedido){
	char* path = string_new();
	string_append(&path, sindicato_config->punto_montaje);
	string_append(&path, "Files/Restaurantes/Pedido");
	string_append(&path, restaurante);
	string_append(&path, "/Pedido");
	string_append(&path, id_pedido);
	string_append(&path, ".AFIP");
	return path;
}

char* get_path_receta_file(char* nombre_receta){
	char* path = string_new();
	string_append(&path, sindicato_config->punto_montaje);
	string_append(&path, "Files/Recetas/");
	string_append(&path, nombre_receta);
	string_append(&path, ".AFIP");
	return path;
}
/* --- END SUITES DE PATH O RUTAS --- */

/* --- SUITES DE FILES PARSER --- */
/* -- REFACTOR -- */
t_list* strings_to_list(char** strings) {
	t_list* list = list_create();
	while (*strings != NULL) {
		list_add(list, strdup(*strings));
		strings++;
	}
	return list;
}

t_estado_pedido config_get_estado_pedido(t_config* config, char* key){
	if (!config_has_property(config, key)) exit(EXIT_FAILURE);

	char* value = config_get_string_value(config, key);
	if (strcmp(value, "Pendiente")){
		return PENDIENTE;
	} else if (strcmp(value, "Confirmado")){
		return CONFIRMADO;
	} else if (strcmp(value, "Terminado")){
		return TERMINADO;
	} else {
		return EXIT_FAILURE;
	}
}
/* -- END REFACTOR -- */
t_posicion* get_position_from_config(t_config* config, char* key){
	char** posicion_str = config_get_array_value(config, key);
	t_posicion* posicion_aux = malloc(sizeof(posicion_str));
	posicion_aux->x = atoi(posicion_str[0]);
	posicion_aux->y = atoi(posicion_str[1]);
	return posicion_aux;
}

void info_file_parser(t_config* config, t_info* info_config){
	//preparing lists
	char** afinidad_cocineros_str = config_get_array_value(config, "AFINIDAD_COCINEROS");
	char** platos_str = config_get_array_value(config, "PLATOS");
	char** precio_platos_str = config_get_array_value(config, "PRECIO_PLATOS");

	info_config->cantidad_cocineros = strdup(config_get_int_value(config, "CANTIDAD_COCINEROS"));
	info_config->posicion = get_position_from_config(config, "POSICION");
	info_config->afinidad_cocineros = strings_to_list(afinidad_cocineros_str);
	info_config->platos = strings_to_list(platos_str);
	info_config->precio_platos = strings_to_list(precio_platos_str);
	info_config->cantidad_hornos = strdup(config_get_int_value(config, "CANTIDAD_COCINEROS"));
}

t_info* create_info_config(char* restaurante){
	char* path_info = get_path_info_file(restaurante);
	t_config* config = config_create(path_info);
	t_info* info_config = malloc(sizeof(t_info));

	info_file_parser(config, info_config);
	//TODO: config_destroy(config);
	return info_config;
}

void pedido_file_parser(t_config* config, t_pedido* pedido_config){
	//preparing lists
	char** lista_platos_str = config_get_array_value(config, "LISTA_PLATOS");
	char** cantidad_platos_str = config_get_array_value(config, "CANTIDAD_PLATOS");
	char** cantidad_lista_str = config_get_array_value(config, "CANTIDAD_LISTA");

	// pedido_config->estado_pedido = config_get_string_value(config, "ESTADO_PEDIDO");
	pedido_config->estado_pedido = config_get_estado_pedido(config, "ESTADO_PEDIDO");
	pedido_config->lista_platos = strings_to_list(lista_platos_str);
	pedido_config->cantidad_platos = strings_to_list(cantidad_platos_str);
	pedido_config->cantidad_lista = strings_to_list(cantidad_lista_str);
	pedido_config->precio_total = config_get_int_value(config, "PRECIO_TOTAL");
}

t_pedido* create_pedido_config(char* restaurante, char* id_pedido){
	char* path_pedido = get_path_pedido_file(restaurante, id_pedido);
	t_config* config = config_create(path_pedido);
	t_pedido* pedido_config = malloc(sizeof(t_pedido));

	pedido_file_parser(config, pedido_config);
	//TODO: config_destroy(config);
	return pedido_config;
}

void receta_file_parser(t_config* config, t_receta* receta_config){
	//preparing lists
	char** pasos_str = config_get_array_value(config, "PASOS");
	char** tiempo_paso_str = config_get_array_value(config, "TIEMPO_PASOS");

	receta_config->pasos = strings_to_list(pasos_str);
	receta_config->tiempo_paso = strings_to_list(tiempo_paso_str);
}

t_receta* create_receta_config(char* nombre_receta){
	char* path_receta = get_path_receta_file(nombre_receta);
	t_config* config = config_create(path_receta);
	t_receta* receta_config = malloc(sizeof(t_receta));

	receta_file_parser(config, receta_config);
	//TODO: config_destroy(config);
	return receta_config;
}
/* --- END SUITES DE FILES PARSER--- */

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

	char* path_restaurante = get_path_restaurante(restaurante);

	if ((folder_dir = opendir(path_restaurante)) == NULL){
		return false;
	} else {
		closedir(folder_dir);
		return true;
	}
}

bool existe_pedido(char* restaurante, char* nro_pedido){
	FILE *fp;

	char* path_pedido = get_path_pedido_file(restaurante, nro_pedido);

	if ((fp = fopen(path_pedido, "r")) == NULL){
		return false;
	} else {
		fclose(fp);
		return true;
	}
}

bool existe_receta(char* receta){
	FILE *fp;

	char* path_receta = get_path_receta_file(receta);

	if ((fp = fopen(path_receta, "r")) == NULL){
		return false;
	} else {
		fclose(fp);
		return true;
	}
}

t_list* get_platos(char* restaurante){
	t_info* info_file = create_info_config(restaurante);
	t_list* platos = info_file->platos;
	return platos;
}

t_pedido* get_pedido(char* restaurante, char* id_pedido){
	return create_pedido_config(restaurante, id_pedido);
}

t_info* get_restaurante(char* restaurante){
	return create_info_config(restaurante);
}

t_receta* get_receta(char* comida){
	return create_receta_config(comida);
}