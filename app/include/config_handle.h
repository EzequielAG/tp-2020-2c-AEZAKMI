#include "list.h"
#include "config_app.h"
#include <commons/string.h>
#include <stdlib.h>
#include "shared_utils.h"
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

typedef struct t_repartidor t_repartidor;

typedef struct {
    int id_pedido;
    char* restaurante;
    char* cliente;
    t_repartidor* repartidor_actual;
} t_pcb;

List suscriptores_cpu;
sem_t* sem_pcb_new;
List pcb_new;
sem_t* sem_pcb_ready;
List pcb_ready;
sem_t* sem_grado_multiprocesamiento;
List pedidos_terminados;

typedef struct {
    int posx;
    int posy;
} t_posicion;

typedef struct {
    t_posicion posicion_restaurante;
    t_posicion posicion_cliente;
} t_pedido;

struct t_repartidor {
    int id;
    t_posicion posicion;
    int frecuencia_de_descanso;
    int tiempo_de_descanso;
    sem_t* nuevo_pedido;
    sem_t* ciclo_cpu;
    t_pedido* pedido;
    int cansancio;
    t_pcb* pcb_actual;
};

List repartidores_libres;
sem_t* sem_entrenador_libre;

//MUEVO LOS STRUCT PARA UTILIZARLOS ACA
typedef struct {
    int socket;
    char* nombre_restaurante;
    t_posicion posicion;
} t_restaurante;

typedef struct {
    int socket;
    char* id_cliente;
    t_posicion posicion;
    t_restaurante* restaurante;
} t_cliente;

//MUEVO LAS LISTAS PARA UTILIZARLAS ACA
List lista_clientes;
List lista_restaurantes;

//MUEVO LAS FUNCIONES PARA UTILIZARLAS ACA
t_restaurante* buscar_restaurante_lista(char* nombre_restaurante);
t_cliente* buscar_cliente_lista(char* id_cliente);
void enviar_final_pedido(char*, int);
t_pcb* crear_pcb(char* restaurante, int id_pedido, char* id_cliente);