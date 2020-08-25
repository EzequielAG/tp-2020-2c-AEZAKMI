#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>

#define IP "127.0.0.1"
#define PUERTO "4444"

typedef enum
{
	MENSAJE,
	PAQUETE
}op_code;

t_log* logger;


void iterator(char* value);

void inicio_server(char* ip, char* puerto,t_log* logger);
void* recibir_buffer(int*, int);
void inicio_server(char*,char*,t_log*);
int iniciar_servidor(char*,char*);
int esperar_cliente(int);
t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);

#endif /* CONEXIONES_H_ */