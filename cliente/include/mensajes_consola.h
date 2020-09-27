#include "cliente.h"
#include "shared_utils.h"
#include "server.h"
#include "api.h"
#include "tests.h"


void leer_consola(t_log* logger,t_modulo* modulo);
void enviar_mensajes_por_consola(t_modulo* modulo, char* mensaje_completo);
char ** separar_por_comillas(char** string_separado_por_espacios);