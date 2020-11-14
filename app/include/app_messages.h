#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "shared_utils.h"
#include "server.h"
#include "list.h"
#include "api.h"
#include "tests.h"

typedef struct {
    int socket;
    char* nombre_restaurante;
} t_restaurante;

typedef struct {
    int socket;
    char* id_cliente;
    t_restaurante* restaurante;
} t_cliente;

List lista_clientes;
List lista_restaurantes;

void handle_consultar_restaurantes(int socket);
int relacionar(char* restaurante, char* cliente);
void handle_seleccionar_restaurante(int socket, char* restaurante, char* cliente);
int obtener_id_pedido();
void handle_crear_pedido(int socket);
t_cliente* buscar_cliente_lista(char* id_cliente);
void handle_handshake_cliente(int socket, char* id_cliente);
t_restaurante* buscar_restaurante_lista(char* nombre_restaurante);
void handle_handshake_restaurante(int socket, char* nombre_restaurante);
t_restaurante* nuevo_restaurante(int socket, char* nombre_restaurante);
char* obtener_restaurantes();