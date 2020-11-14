#include "planificacion.h"

void inicializar_colas()
{
    initlist(&cola_new);  // lista de platos recibidos
    // initlist(cola_ready);  // 1 por cada cocinero
    inicializar_colas_ready();
    //initlist(&cola_exec); // l_cocineros tantas como cant de cocineros

    initlist(&cola_io); // va a ser 1 sola cola que en vez de contener platos va a contener hornos - hay que chequear si hay algun plato esperando en block para tomar el horno
    inicializar_colas_io();
    initlist(&cola_exit); // 1 sola cola de exit
    initlist(&cola_block); // 1 sola cola de block voy a block cuando necesito usar el horno i/o y todos estan ocupados
}

int inicializar_colas_ready(){

    for(int i = 0;i < cantidad_cocineros; i++)
    {
        initlist(cola_ready[i]);

    }
    return 1;
}

void inicializar_colas_io(){
    //va a ser 1 sola cola que en vez de contener platos va a contener hornos
    for(int i = 0;i < cantidad_hornos; i++)
    {
        t_horno* horno = malloc(sizeof(t_horno));
        horno->ocupado = 0;
        horno->plato = NULL;
        pushbacklist(&cola_io,horno);
    }
}

int paso_ready(t_plato* plato)
{   
    
    // pushbacklist(&cola_ready,plato);
    plato->pcb->estado = READY;

    return 0;
}

int paso_io(t_plato* plato)
{   
    for(IteratorList iterator_hornos = beginlist(cola_io); iterator_hornos != NULL; iterator_hornos = nextlist(iterator_hornos))
    {
        t_horno* horno = iterator_hornos->data;
        if(horno->ocupado == 0)
        {
            horno->plato = plato;
            horno->ocupado = 1;
            plato->pcb->estado = IO;

            return 1;
        }
    }
    paso_block(plato);

    return 0;
}

int paso_block(t_plato* plato)
{
    pushbacklist(&cola_block,plato);
    plato->pcb->estado = BLOCKED;
    return 1;
}

int paso_new(t_plato* plato)
{
    pushbacklist(&cola_new,plato);
    plato->pcb->estado = NEW;
    return 1;
}

