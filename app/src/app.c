#include "app.h"
int main(void){

    app_init(&app_config, &logger);

    //iniciar_planificador();

    //INSERTO RESTAURANTE DEFAULT
    t_restaurante* restaurante = nuevo_restaurante(0, "Resto Default", app_config->posicion_rest_default_x, app_config->posicion_rest_default_y);
    pushbacklist(&lista_restaurantes, restaurante);

    iniciar_servidor("127.0.0.1", "5004", handle_client);

    app_finally(app_config, logger);
    return 0;
}

void app_init(t_app_config** app_config, t_log** logger){

    //INICIALIZO LISTAS DE RESTAURANTES Y CLIENTES
    initlist(&lista_clientes);
    initlist(&lista_restaurantes);

    *app_config = app_config_loader("./cfg/app.config", logger);

}

void app_finally(t_app_config* app_config, t_log* logger) {
    app_destroy(app_config);
    log_destroy(logger);
}
