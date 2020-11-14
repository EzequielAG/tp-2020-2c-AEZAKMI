#include "planificacion.h"

void inicializar_colas()
{
    inicializar_colas_ready_exec();
}

int inicializar_colas_ready_exec(){

    for(int i=0;i<cantidad_cocineros;i++){

        t_cocinero* cocinero = malloc(sizeof(t_exec));
       
        if(!isemptylist(afinidades)){
            cola_ready[i]->afinidad = popfrontlist(&afinidades);
            cocinero->afinidad = popfrontlist(&afinidades);
        }else{
            cola_ready[i]->afinidad = NULL;
            cocinero->afinidad = NULL;
        }

        cola_ready[i]->platos_espera = NULL;

        cocinero->plato_en_ejecucion =NULL;
        
        t_exec* exec = malloc(sizeof(t_exec));
        exec->cocinero = cocinero;
        exec->ocupado = 0;
        exec->plato = NULL;

        cola_ready[i]->puntero_exec = exec;

    }
    return 1;
}

void inicializar_colas_io(){

}

int paso_new_a_ready(){

        for(IteratorList iter_pedido = beginlist(lista_pedidos); iter_pedido != NULL; iter_pedido = nextlist(iter_pedido)){
            t_pedido* pedido = iter_pedido -> data;

            for(IteratorList iter_plato = beginlist(pedido->platos); iter_plato != NULL; iter_plato = nextlist(iter_plato)){
                t_plato* plato = iter_plato->data;
                paso_ready(plato);
            }
        }

 return 0;
}

int paso_ready(t_plato* plato){   
    
    for(int i=0; i<cantidad_cocineros;i++){

        if(cola_ready[i]->afinidad == plato->nombre){
            pushbacklist(cola_ready[i]->platos_espera, plato);

            return 1;

        };

    }
    
    for(int i=0; i<cantidad_cocineros;i++){

        if(cola_ready[i]->afinidad == NULL){
            pushbacklist(cola_ready[i]->platos_espera, plato);
            return 1;

        };

    }


   return 0;

}

// int cantidad_platos_ready(){

//   return 1;
// }



int paso_io(t_plato* plato)
{   
    
    paso_block(plato);

    return 0;
}

int paso_block(t_plato* plato)
{
 
    return 1;
}

int paso_new(t_plato* plato)
{
    
    return 1;
}

