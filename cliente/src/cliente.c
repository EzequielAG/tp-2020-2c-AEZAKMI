#include "cliente.h"
int main(int argc, char *argv[]){
    
    t_log* logger = log_create("./cfg/cliente.log", "CLIENTE", true, LOG_LEVEL_INFO);
    
    log_info(logger, "Soy el CLIENTE! %s", mi_funcion_compartida());
    
    char* mensaje[3] = {"Hola", "Como", "Va"};

    send_messages_and_return_socket("127.0.0.1", "6011", mensaje, 3);

    log_destroy(logger); 
}