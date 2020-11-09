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

	if (result->operacion == MENSAJE){
		if (!strcmp(result->mensaje, "HANDSHAKE")){
			send_message_socket(result->socket, "OK");
			liberar_conexion(result->socket);
		}
	} else {
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
				default:
					handle_error(result->socket);
					break;
			}
		}
	}

	liberar_conexion(result->socket);
	return;
}

int guardar_pedido_en_afip(char* restaurante, char* id_pedido){
	//TODO: Guardar pedido en afip, 1 OK 0 FAIL
	return 1;
}

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
		// TODO: En caso de no existir se deberá informar dicha situación.
	}

	//Verificar que el ID de pedido no exista para dicho restaurante.
	//En caso de existir se deberá informar sobre dicha situación.
	//En caso de que no exista, se deberá crear el archivo.
	if (existe_pedido(restaurante, id_pedido)){
		// TODO: En caso de existir se deberá informar sobre dicha situación.
	}

	int resultado_operacion = guardar_pedido_en_afip(restaurante, id_pedido);
	//Responder el mensaje indicando si se pudo realizar la operación correctamente (Ok/Fail).
	char* respuesta[1];

	if (resultado_operacion){
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

	//Responder el mensaje indicando si se pudo realizar en conjunto con la información del pedido si correspondiera.

}

void handle_obtener_restaurante(int socket, char* restaurante){
	//Verificar si el Restaurante existe dentro del File System. 
	//Para esto se deberá buscar dentro del directorio Restaurantes si existe un subdirectorio con el nombre del Restaurante. 
	//En caso de no existir se deberá informar dicha situación.
	bool resultado_operacion = existe_restaurante(restaurante);

	//Obtener todo los datos del archivo info.AFIP.

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
	//Verificar si existe el plato dado dentro del directorio de recetas.
	//En caso de no existir, se deberá informar dicha situación.

	//Responder el mensaje con la receta solicitada.

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

void handle_crear_restaurante(char* nombre, char* cantidad_cocineros, char* posicion, char* afinidad_cocineros, char* platos, char* precio_platos, char* cantidad_hornos){

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

	//CREAR RESTAURANTE
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