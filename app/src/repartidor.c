#include "repartidor.h"

void repartir_pedidos(t_repartidor* repartidor){
    while (1) {
        sem_wait(repartidor->nuevo_pedido);
        repartir_pedido(repartidor);
    }
}

void repartir_pedido(t_repartidor* repartidor){

    ir_hacia_restaurante(repartidor);

    esperar_pedido(repartidor);

    ir_hacia_cliente(repartidor);

    entregar_pedido(repartidor);

}

void ir_hacia_restaurante(t_repartidor* repartidor){
    t_pedido* pedido = repartidor->pedido;

    while(!misma_posicion(repartidor->posicion, pedido->posicion_restaurante)){
        //sem_post(cpu)
        avanzar_hacia(repartidor, pedido->posicion_restaurante);
    }
}

void esperar_pedido(t_repartidor* repartidor){

}

void ir_hacia_cliente(t_repartidor* repartidor){
    t_pedido* pedido = repartidor->pedido;

    while(!misma_posicion(repartidor->posicion, pedido->posicion_cliente)){
        //sem_post(cpu)
        avanzar_hacia(repartidor, pedido->posicion_cliente);
    }

}

void entregar_pedido(t_repartidor* repartidor){

}

bool misma_posicion(t_posicion posicion1, t_posicion posicion2){
    return posicion1.posx == posicion2.posx && posicion1.posy == posicion2.posy;
}

void avanzar_hacia(t_repartidor* repartidor, t_posicion destino){

    
    if (repartidor->posicion.posx > destino.posx){
        mover_hacia_izquierda(repartidor);
    } else if(repartidor->posicion.posx < destino.posx) {
        mover_hacia_derecha(repartidor);
    } else if (repartidor->posicion.posy > destino.posy) {
        mover_hacia_abajo(repartidor);
    } else {
        mover_hacia_arriba(repartidor);
    }
    
    char* log_string = string_new();
    sprintf(log_string, "El repartidor %i se movió a la posición %i - %i", repartidor->id, repartidor->posicion.posx, repartidor->posicion.posy);
    log_info(logger, log_string);

    cansarse(repartidor);

}

void mover_hacia_izquierda(t_repartidor* repartidor){
    repartidor->posicion.posx -= 1;
}

void mover_hacia_derecha(t_repartidor* repartidor){
    repartidor->posicion.posx += 1;
}

void mover_hacia_abajo(t_repartidor* repartidor){
    repartidor->posicion.posy -= 1;
}

void mover_hacia_arriba(t_repartidor* repartidor){
    repartidor->posicion.posx += 1;
}

void cansarse(t_repartidor* repartidor){
    repartidor->cansancio += 1;

    if (esta_cansado(repartidor)){
        descansar(repartidor);
    }
}

bool esta_cansado(t_repartidor* repartidor){
    return repartidor->cansancio >= repartidor->frecuencia_de_descanso;
}

void descansar(t_repartidor* repartidor){
    for (int i = 0; i < repartidor->tiempo_de_descanso; i++){
        //sem_wait(cpu)
    }
}