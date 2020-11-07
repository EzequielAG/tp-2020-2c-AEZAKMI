#include "planificacion.h"

void inicializar_colas()
{
    inicializar_colas_ready();
    inicializar_colas_io();
}

void inicializar_colas_ready(){

initlist(&cola_ready_mas);
initlist(&cola_ready);

}

void inicializar_colas_io(){
   initlist(&cola_io);

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
            return 1;
        } 
    }

    pushbacklist(&cola_ready,plato);
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
            return 1;
        } 
    }
    return 0;
}