#include "memoria_swap.h"


void iniciarMemoriaSwap(){

    archivoSwap = open("./archivos/swap.txt", O_RDWR, S_IRUSR | S_IWUSR);
    system("cat /dev/null > ./archivos/swap.txt");

    void* puntero = NULL;

    lseek(archivoSwap, tamanioSwap - 1, SEEK_SET);

    write(archivoSwap, "", 1);

    puntero_memoria_swap = mmap(NULL, tamanioSwap, PROT_READ | PROT_WRITE, MAP_SHARED, archivoSwap, 0);

	puntero = puntero_memoria_swap;

    for(int i=0; bitarray_get_max_bit(bitMapSwap) > i; i++){

		bitarray_clean_bit(bitMapSwap, i);

	}

    while(puntero + tamanioSwap > puntero_memoria_swap){
		
		puntero_memoria_swap += 32;
		
		pushbacklist(&tablaSwap, puntero_memoria_swap);

	}

    puntero_memoria_swap = puntero;


}
