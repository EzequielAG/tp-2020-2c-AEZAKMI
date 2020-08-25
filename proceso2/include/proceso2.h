#ifndef PROCESO2_H
#define PROCESO2_H
#include <stdio.h>
#include <commons/log.h>
#include <stdbool.h>
#include "shared_utils.h"
#include "client.h"
#include "tests.h"
#include<stdlib.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>


t_log* iniciar_logger(void);
t_config* leer_config(void);
void leer_consola(t_log* logger);
t_paquete* armar_paquete();

void _leer_consola_haciendo(void(*accion)(char*));

#endif