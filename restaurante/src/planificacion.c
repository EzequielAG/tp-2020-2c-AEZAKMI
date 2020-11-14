#include "planificacion.h"

void inicializar_colas()
{
    initlist(&cola_io);
    initlist(&cola_ready);
    initlist(&cola_exec);
    initlist(&cola_block);
    initlist(&cola_exec);
    
}



void inicializar_colas_io(){


    
   
    // for(int i = 0;i < cantidad_hornos; i++)
    // {
    //     t_horno* horno = malloc(sizeof(t_horno));
    //     horno->ocupado = 0;
    //     horno->plato = NULL;
    //     pushbacklist(&cola_io,horno);
    // }
}

int paso_ready(t_plato* plato)
{   
    
    pushbacklist(&cola_ready,plato);
    plato->pcb->estado = READY;

    return 0;
}

int paso_io(t_plato* plato)
{   
    // for(IteratorList iterator_hornos = beginlist(cola_io); iterator_hornos != NULL; iterator_hornos = nextlist(iterator_hornos))
    // {
    //     t_horno* horno = iterator_hornos->data;
    //     if(horno->ocupado == 0)
    //     {
    //         horno->plato = plato;
    //         horno->ocupado = 1;
    //         plato->pcb->estado = IO;

    //         return 1;
    //     }
    // }
    // paso_block(plato);

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

