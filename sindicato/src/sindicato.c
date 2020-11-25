#include "sindicato.h"

int main(void){
	sindicato_init(&sindicato_config, &logger);
	get_or_create_fs();

	printf("Imprimiendo el path %s", sindicato_config->ruta_log);

	iniciar_servidor_desacoplado();
	sleep(1);
	iniciar_consola();

	sindicato_finally(sindicato_config, logger);
	return 0;
}

void iniciar_servidor_desacoplado(){

	pthread_t thread;

	pthread_create(&thread,NULL,(void*)iniciar_servidor_sindicato, NULL);
	pthread_detach(thread);
}

void iniciar_servidor_sindicato(){

	//TODO: Hacer que reciba ip y puerto de config
	iniciar_servidor(sindicato_config->ip, sindicato_config->puerto_escucha, handle_client);

}

void handle_client(t_result* result){

	if (result->operacion == MENSAJES){
		int tipo_mensaje = atoi(result->mensajes->mensajes[0]);
		switch(tipo_mensaje)
		{
			case consultar_platos:
				handle_consultar_platos(result->socket, result->mensajes->mensajes[1]);
				break;
			case guardar_pedido:
				handle_guardar_pedido(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2]);
				break;
			case guardar_plato:
				handle_guardar_plato(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2], result->mensajes->mensajes[3], result->mensajes->mensajes[4]);
				break;
			case confirmar_pedido:
				handle_confirmar_pedido(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2]);
				break;
			case obtener_pedido:
				handle_obtener_pedido(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2]);
				break;
			case obtener_restaurante:
				handle_obtener_restaurante(result->socket, result->mensajes->mensajes[1]);
				break;
			case plato_listo:
				handle_plato_listo(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2], result->mensajes->mensajes[3]);
				break;
			case obtener_receta:
				handle_obtener_receta(result->socket, result->mensajes->mensajes[1]);
				break;
			case terminar_pedido:
				handle_terminar_pedido(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2]);
				break;
			case handshake_restaurante:
				handle_handshake_restaurante(result->socket);
				break;
			default:
				handle_error(result->socket);
				break;
		}
	}

	liberar_conexion(result->socket);
	return;
}

void handle_handshake_restaurante(int socket){
	send_message_socket(socket, "OK");
}

int calculate_blocks_required(char* string){

	int string_size = string_length(string);
	int wearable_size = atoi(sindicato_config->block_size) - 4;
	return (string_size / wearable_size) + (string_size % wearable_size) ? 1 : 0;
}

int get_available_block(){
	return 12; //TODO
}

int create_restaurante_file(char* path, t_restaurante_file* restaurante_file){
	FILE* pedido = fopen(path, "w");
	if (pedido == NULL){
		log_error(logger, "[Create Pedido File] No se creo el archivo de pedido");
		return EXIT_FAILURE;
	}

	t_config* config = config_create(path);
	config_set_value(config, "SIZE", string_itoa(restaurante_file->size));
	config_set_value(config, "INITIAL_BLOCK", string_itoa(restaurante_file->initial_block));
	config_save_in_file(config, path);

	fclose(pedido);
	return EXIT_SUCCESS;
}

int save_block(int initial, int next, char* content){
	char* ruta_archivo = string_from_format("/Blocks/%d.AFIP", initial);
	FILE * bloque = get_or_create_file(ruta_archivo, "w");
	if (bloque == NULL){
		log_error(logger, "[Save Block] No se obtuvo el archivo bloque");
		return EXIT_FAILURE;
	}
	if(next != NULL){
		string_append(&content, string_itoa(next));
	}
	int finish_code = fputs(content, bloque);
	fclose(bloque);
	free(ruta_archivo);
	return finish_code;
}

int save_in_blocks(int initial_block, char* content, int number_of_blocks){
	log_info(logger, "[Save In Block] Se procede a guardar en bloques");
	int block_size = atoi(sindicato_config->block_size) - 4;
	int next_block;
	int finish_code;
	for(int i=0; i<number_of_blocks; i++){
		int start = i * block_size;
		int length = start + block_size;
		if (length < string_length(content)){
			char* sub_string = string_substring(content, start, length);
			next_block = get_available_block();
			finish_code = save_block(initial_block, next_block, sub_string);
			initial_block = next_block;
		} else {
			char* sub_string = string_substring_from(content, start);
			next_block = NULL;
			finish_code = save_block(initial_block, next_block, sub_string);
		}
	}
	return finish_code;
}

int create_pedido(char* restaurante, char* id_pedido){

	char* content = "ESTADO_PEDIDO=Pendiente\nLISTA_PLATOS=[]\nCANTIDAD_PLATOS=[]\nCANTIDAD_LISTA=[]";

	int number_of_blocks = calculate_blocks_required(content);
	int block_size = atoi(sindicato_config->block_size);

	t_restaurante_file* restaurante_file = malloc(sizeof(t_restaurante_file));
	restaurante_file->size = number_of_blocks * block_size;
	restaurante_file->initial_block = get_available_block();
	if(!create_restaurante_file(get_path_pedido_file(restaurante, id_pedido), restaurante_file)){
		log_error(logger, "[Crear Pedido] No se creo el archivo de pedido");
		return EXIT_FAILURE;
	}

	if(!save_in_blocks(restaurante_file->initial_block, content, number_of_blocks)){
		log_error(logger, "[Crear Pedido] No se guardo en bloques");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/* --- SUITES DE HANDLES --- */
void handle_consultar_platos(int socket, char* restaurante){
	//Verificar si el Restaurante existe dentro del File System. 
	//Para esto se deberá buscar dentro del directorio Restaurantes si existe un subdirectorio con el nombre del Restaurante. 
	//En caso de no existir se deberá informar dicha situación.
	if (!existe_restaurante(restaurante)){
		// TODO: En caso de no existir se deberá informar dicha situación.
	}

	//Obtener los platos que puede preparar dicho Restaurante del archivo info.AFIP.
	t_list* platos = get_platos(restaurante);

	//Responder el mensaje indicando los platos que puede preparar el Restaurante.

}

void handle_guardar_pedido(int socket, char* restaurante, char* id_pedido){

	//Verificar si el Restaurante existe dentro del File System. 
	//Para esto se deberá buscar dentro del directorio Restaurantes si existe un subdirectorio con el nombre del Restaurante. 
	//En caso de no existir se deberá informar dicha situación.
	if (!existe_restaurante(restaurante)){
		log_error(logger, "[Guardar Pedido] El restaurante no existe");
		// TODO: En caso de no existir se deberá informar dicha situación.
		exit(-1);
	}

	//Verificar que el ID de pedido no exista para dicho restaurante.
	//En caso de existir se deberá informar sobre dicha situación.
	//En caso de que no exista, se deberá crear el archivo.
	if (existe_pedido(restaurante, id_pedido)){
		log_error(logger, "[Guardar Pedido] El pedido ya existe");
		// TODO: En caso de existir se deberá informar sobre dicha situación.
		exit(-1);
	}

	log_info(logger, "[Guardar Pedido] Se procede a crear el pedido");
	int resultado_operacion = create_pedido(restaurante, id_pedido);
	//Responder el mensaje indicando si se pudo realizar la operación correctamente (Ok/Fail).
	char* respuesta[1];

	if (resultado_operacion == EXIT_SUCCESS){
		respuesta[0] = "Ok";
	} else {
		respuesta[0] = "Fail";
	}
	send_messages_socket(socket, respuesta, 1);
}

void handle_guardar_plato(int socket, char* restaurante, char* id_pedido, char* comida, char* cantidad){
	//Verificar si el Restaurante existe dentro del File System. 
	//Para esto se deberá buscar dentro del directorio Restaurantes si existe un subdirectorio con el nombre del Restaurante. 
	//En caso de no existir se deberá informar dicha situación.
	if (!existe_restaurante(restaurante)) {
		// TODO: En caso de no existir se deberá informar dicha situación.
	}

	//Verificar si el Pedido existe dentro del File System. 
	//Para esto se deberá buscar dentro del directorio del Restaurante si existe dicho pedido. 
	//En caso de no existir se deberá informar dicha situación.
	if (!existe_pedido(restaurante, id_pedido)){
		// TODO: En caso de no existir se deberá informar dicha situación.
	}

	t_pedido* pedido = create_pedido_config(restaurante, id_pedido);
	//Verificar que el pedido esté en estado “Pendiente”. En caso contrario se deberá informar dicha situación.
	if (pedido->estado_pedido != PENDIENTE){
		// TODO: En caso contrario se deberá informar dicha situación.
	}

	//Verificar si ese plato ya existe dentro del archivo.
	//En caso de existir, se deberán agregar la cantidad pasada por parámetro a la actual. 
	//En caso de no existir se deberá agregar el plato a la lista de Platos y anexar la cantidad que se tiene que cocinar de dicho plato y aumentar el precio total del pedido.
	bool platos_iguales(char* plato){
		if(strcmp(plato, comida)){
			return true;
		} else {
			return false;
		}
	}

	if (list_any_satisfy(pedido->lista_platos, (void*)platos_iguales)){
		int i, posicion;
		int cant_platos = list_size(pedido->lista_platos);
		for (i=0; i<=cant_platos; i++){
			char* plato_existente = list_get(pedido->lista_platos, i);
			if (strcmp(plato_existente, comida)){
				posicion = i;
			}
		}
		list_replace(pedido->cantidad_platos, posicion, cantidad);
	} else {
		list_add(pedido->lista_platos, comida);
		list_add(pedido->cantidad_platos, cantidad);
	}

	//Responder el mensaje indicando si se pudo realizar la operación correctamente (Ok/Fail).
	char* respuesta[1];

	if (true){
		respuesta[0] = "Ok";
	} else {
		respuesta[0] = "Fail";
	}
	send_messages_socket(socket, respuesta, 1);

}

void handle_confirmar_pedido(int socket, char* id_pedido, char* restaurante){
	//Verificar si el Restaurante existe dentro del File System. 
	//Para esto se deberá buscar dentro del directorio Restaurantes si existe un subdirectorio con el nombre del Restaurante. 
	//En caso de no existir se deberá informar dicha situación.
	bool resultado_operacion = existe_restaurante(restaurante);

	//Verificar si el Pedido existe dentro del File System. 
	//Para esto se deberá buscar dentro del directorio del Restaurante si existe dicho pedido. 
	//En caso de no existir se deberá informar dicha situación.
	if (resultado_operacion){
		resultado_operacion = existe_pedido(restaurante, id_pedido);
	}

	//Verificar que el pedido esté en estado “Pendiente”. En caso contrario se deberá informar dicha situación.

	//Cambiar el estado del pedido de “Pendiente” a “Confirmado” (se debe truncar el archivo en caso de ser necesario).

	//Responder el mensaje indicando si se pudo realizar la operación correctamente (Ok/Fail).
	char* respuesta[1];

	if (resultado_operacion){
		respuesta[0] = "Ok";
	} else {
		respuesta[0] = "Fail";
	}
	send_messages_socket(socket, respuesta, 1);
}

void handle_obtener_pedido(int socket, char* restaurante, char* id_pedido){
	if (ES_TEST){
		char* respuesta[4];
		respuesta[0] = "CONFIRMADO";
		respuesta[1] = "PLATO1,PLATO2";
		respuesta[2] = "1,1";
		respuesta[3] = "1,0";
		send_messages_socket(socket, respuesta, 4);
	} else {
		//Verificar si el Restaurante existe dentro del File System. 
		//Para esto se deberá buscar dentro del directorio Restaurantes si existe un subdirectorio con el nombre del Restaurante. 
		//En caso de no existir se deberá informar dicha situación.
		if (!existe_restaurante(restaurante)){
			// TODO: En caso de no existir se deberá informar dicha situación.
		}

		//Verificar si el Pedido existe dentro del File System. 
		//Para esto se deberá buscar dentro del directorio del Restaurante si existe dicho pedido. 
		//En caso de no existir se deberá informar dicha situación.
		if (!existe_pedido(restaurante, id_pedido)){
			// TODO: En caso de no existir se deberá informar dicha situación.
		}


		get_pedido(restaurante, id_pedido);
		//Responder el mensaje indicando si se pudo realizar en conjunto con la información del pedido si correspondiera.
	}
}

void handle_obtener_restaurante(int socket, char* restaurante){
	//Verificar si el Restaurante existe dentro del File System. 
	//Para esto se deberá buscar dentro del directorio Restaurantes si existe un subdirectorio con el nombre del Restaurante. 
	//En caso de no existir se deberá informar dicha situación.
	if (!existe_restaurante(restaurante)){
		// TODO: En caso de no existir se deberá informar dicha situación.
	}

	//Obtener todo los datos del archivo info.AFIP.
	t_info* info = get_restaurante(restaurante);

	//Responder el mensaje indicando los datos del Restaurante.

}

void handle_plato_listo(int socket, char* restaurante, char* id_pedido, char* comida){

	//Verificar si el Restaurante existe dentro del File System. 
	//Para esto se deberá buscar dentro del directorio Restaurantes si existe un subdirectorio con el nombre del Restaurante. 
	//En caso de no existir se deberá informar dicha situación.
	bool resultado_operacion = existe_restaurante(restaurante);

	//Verificar si el Pedido existe dentro del File System. 
	//Para esto se deberá buscar dentro del directorio del Restaurante si existe dicho pedido. 
	//En caso de no existir se deberá informar dicha situación.
	if (resultado_operacion){
		resultado_operacion = existe_pedido(restaurante, id_pedido);
	}

	//Verificar que el pedido esté en estado “Confirmado”. 
	//En caso contrario se deberá informar dicha situación.

	//Verificar si ese plato ya existe dentro del archivo. 
	//En caso de existir, se deberá aumentar en uno la cantidad lista de ese plato. 
	//En caso contrario se deberá informar dicha situación.

	//Responder el mensaje indicando si se pudo realizar la operación correctamente (Ok/Fail).
	char* respuesta[1];

	if (resultado_operacion){
		respuesta[0] = "Ok";
	} else {
		respuesta[0] = "Fail";
	}
	send_messages_socket(socket, respuesta, 1);
}

void handle_obtener_receta(int socket, char* comida){
	if (ES_TEST){
		// return ['amasar,2', 'batir,1']
		char* respuesta[1];
		respuesta[0] = "batir,5,amasar,3,hornear,6";
	
		send_messages_socket(socket, respuesta, 1);
	} else {
		//Verificar si existe el plato dado dentro del directorio de recetas.
		//En caso de no existir, se deberá informar dicha situación.
		if (!existe_receta(comida)){
			// TODO: En caso de no existir, se deberá informar dicha situación.
		}

		get_receta(comida);

		//Responder el mensaje con la receta solicitada.
	}
}

void handle_terminar_pedido(int socket, char* id_pedido, char* restaurante){
	//Verificar si el Restaurante existe dentro del File System. 
	//Para esto se deberá buscar dentro del directorio Restaurantes si existe un subdirectorio con el nombre del Restaurante. 
	//En caso de no existir se deberá informar dicha situación.
	bool resultado_operacion = existe_restaurante(restaurante);

	//Verificar si el Pedido existe dentro del File System. 
	//Para esto se deberá buscar dentro del directorio del Restaurante si existe dicho pedido. 
	//En caso de no existir se deberá informar dicha situación.
	if (resultado_operacion){
		resultado_operacion = existe_pedido(restaurante, id_pedido);
	}

	//Verificar que el pedido esté en estado “Confirmado”. En caso contrario se deberá informar dicha situación.

	//Cambiar el estado del pedido a “Terminado” 

	//Responder el mensaje indicando si se pudo realizar la operación correctamente (Ok/Fail).
	char* respuesta[1];

	if (resultado_operacion){
		respuesta[0] = "Ok";
	} else {
		respuesta[0] = "Fail";
	}
	send_messages_socket(socket, respuesta, 1);

}

void handle_error(int socket){
	char* respuesta[1];
	respuesta[0] = "ERROR";
	send_messages_socket(socket, respuesta, 1);
}

void handle_crear_restaurante(char* nombre, char* cantidad_cocineros, char* posicion,
	char* afinidad_cocineros, char* platos, char* precio_platos, char* cantidad_hornos){

	//CHECK PARAMETROS
	if (nombre == NULL){
		printf("El nombre es obligatorio. \n");
		return;
	}

	if (cantidad_cocineros == NULL){
		printf("La cantidad de cocineros es obligatoria. \n");
		return;
	}

	if (posicion == NULL){
		printf("La posicion es obligatoria. \n");
		return;
	}

	if (afinidad_cocineros == NULL){
		printf("La afinidad de los cocineros es obligatoria. \n");
		return;
	}

	if (platos == NULL){
		printf("Los platos son obligatorios. \n");
		return;
	}

	if (precio_platos == NULL){
		printf("Los precios de los platos son obligatorios. \n");
		return;
	}

	if (cantidad_hornos == NULL){
		printf("La cantidad de hornos es obligatoria. \n");
		return;
	}

	//VERIFICAR QUE NO EXISTE EL RESTAURANTE
	if (existe_restaurante(nombre)){
		printf("Ya existe un restaurante con el nombre: %s \n", nombre);
		return;
	}

	get_or_create_folder(nombre);
	char* content = string_new();
	string_append_with_format(&content, "CANTIDAD_COCINEROS=%s\n", cantidad_cocineros);
	string_append_with_format(&content, "POSICION=%s\n", posicion);
	string_append_with_format(&content, "AFINIDAD_COCINEROS=%s\n", afinidad_cocineros);
	string_append_with_format(&content, "PLATOS=%s\n", platos);
	string_append_with_format(&content, "PRECIO_PLATOS=%s\n", precio_platos);
	cantidad_hornos = string_substring_until(cantidad_hornos, (string_length(cantidad_hornos)-2));
	string_append_with_format(&content, "CANTIDAD_HORNOS=%s", cantidad_hornos);

	int number_of_blocks = calculate_blocks_required(content);
	int block_size = atoi(sindicato_config->block_size);

	t_restaurante_file* restaurante_file = malloc(sizeof(t_restaurante_file));
	restaurante_file->size = number_of_blocks * block_size;
	restaurante_file->initial_block = get_available_block();
	create_restaurante_file(get_path_info_file(nombre), restaurante_file);
	save_in_blocks(restaurante_file->initial_block, content, number_of_blocks);
}

void handle_crear_receta(char* nombre, char* pasos, char* tiempo_pasos){

	//CHECK PARAMETROS
	if (nombre == NULL){
		printf("El nombre de la receta es obligatorio. \n");
		return;
	}

	if (pasos == NULL){
		printf("Los pasos de la receta son obligatorios. \n");
		return;
	}

	if (tiempo_pasos == NULL){
		printf("El tiempo de los pasos de la receta son obligatorios obligatoria. \n");
		return;
	}

	//VERIFICAR QUE NO EXISTE LA RECETA
	if (existe_receta(nombre)){
		printf("Ya existe una receta con el nombre: %s \n", nombre);
		return;
	}

	//CREAR RECETA
}
/* --- END SUITES DE HANDLES --- */

void iniciar_consola(){
	do {
		printf("Ingrese alguno de los siguientes comandos: \n");
		//CrearRestaurante MiRestaurante 5 [4,5] [Milanesa] [Milanesa,Empanadas,Ensalada] [200,50,150] 2
		printf("1. CrearRestaurante [NOMBRE] [CANTIDAD_COCINEROS] [POSICION] [AFINIDAD_COCINEROS] [PLATOS] [PRECIO_PLATOS] [CANTIDAD_HORNOS] \n");
		//CrearReceta Milanesa [Trocear,Empanar,Reposar,Hornear] [4,5,3,10]
		printf("2. CrearReceta [NOMBRE] [PASOS] [TIEMPO_PASOS] \n");
		printf("3. Exit \n");
		char * line = getlinefromconsole();
		process_line(line);
		if (strcmp("Exit\n", line) == 0){
			printf("Se reconoce el comando Exit: %s\n", line);
			break;
		}
	} while (1);
	
}

void process_line(char* line){
	char** lineas = string_split(line, " ");
	int longitud = 0;
	while (lineas[longitud] != NULL){
		longitud++;
	}
	
	if (strcmp("CrearRestaurante", lineas[0])== 0){
		printf("Se reconoce el comando Crear Restaurante: %s\n", line);
		if (longitud < 8){
			printf("No se tienen los suficientes parametros para llamar a Crear restaurante, se requieren 7 y se obtuvieron: %d\n", longitud - 1);
		}
		handle_crear_restaurante(lineas[1], lineas[2], lineas[3], lineas[4], lineas[5], lineas[6], lineas[7]);
		return;
	}
	if (strcmp("CrearReceta", lineas[0])== 0){
		printf("Se reconoce el comando Crear Receta: %s\n", line);
		if (longitud < 4){
			printf("No se tienen los suficientes parametros para llamar a Crear restaurante, se requieren 7 y se obtuvieron: %d\n", longitud - 1);
		}
		handle_crear_receta(lineas[1], lineas[2], lineas[3]);
		return;
	}
	if (strcmp("Exit\n", lineas[0]) != 0){
		printf("No se reconoce el comando ingresado: %s\n", line);
		return;
	}
}

char * getlinefromconsole(void) {
	char * line = malloc(100), * linep = line;
	size_t lenmax = 100, len = lenmax;
	int c;

	if(line == NULL)
		return NULL;

	for(;;) {
		c = fgetc(stdin);
		if(c == EOF)
			break;

		if(--len == 0) {
			len = lenmax;
			char * linen = realloc(linep, lenmax *= 2);

			if(linen == NULL) {
				free(linep);
				return NULL;
			}
			line = linen + (line - linep);
			linep = linen;
		}

		if((*line++ = c) == '\n')
			break;
	}
	*line = '\0';
	return linep;
}
