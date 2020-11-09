#include "planificacion.h"

void inicializar_colas()
{
    initlist(&cola_new);
    inicializar_colas_ready();
    inicializar_colas_exec();
    inicializar_colas_io();
    inicializar_colas_block();
    inicializar_colas_exit();
}

void inicializar_colas_exec(){
    initlist(&cola_exec);
}
void inicializar_colas_block(){
    initlist(&cola_block);
}
void inicializar_colas_exit(){
    initlist(&cola_exit);
}

void inicializar_colas_ready(){
    initlist(&cola_ready_mas);
    initlist(&cola_ready);
}

void inicializar_colas_io(){
   
    for(int i = 0;i < atoi(cantidad_hornos); i++)
    {
        t_horno* horno = malloc(sizeof(t_horno));
        horno->ocupado = 0;
        horno->plato = NULL;
        pushbacklist(&cola_io,horno);
    }
}

int paso_ready(t_plato* plato)
{   
    for(IteratorList iterator_afinidades = beginlist(*afinidades); iterator_afinidades != NULL; iterator_afinidades = nextlist(iterator_afinidades))
    {
        if(strcmp(plato->nombre,iterator_afinidades->data))
        {
            pushbacklist(&cola_ready_mas,plato);
            plato->pcb->estado = READY_MAS;
            return 1;
        } 
    }

    pushbacklist(&cola_ready,plato);
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

