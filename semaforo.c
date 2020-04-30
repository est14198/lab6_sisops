// Universidad del Valle de Guatemala
// Sistemas Operativos
// Maria Fernanda Estrada 14198
// Laboratorio 6 - Semaforo
// Codigo base tomado de https://www.geeksforgeeks.org/use-posix-semaphores-c/


#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/syscall.h>

// Constantes modificables
#define QUANT_THREADS 6 // cantidad de threads ajustable
#define QUANT_ITER 9 // cantidad de iteraciones ajustable

// Definimos un semaforo global
sem_t mutex;


// Metodo que ejecutara cada thread
void* thread(void* arg) {
	// Para saber que numero de thread se ejecuta
    int thread_id = syscall(SYS_gettid);
    printf("\nIniciando thread: %d", thread_id);

    int i;

	// for para "tomar" un recurso durante cierto tiempo
    for(i=0; i<QUANT_ITER; i++) {
        printf("\nIniciando iteracion: %d", thread_id, i);
        
		// wait si el recurso ya esta siendo usado
        sem_wait(&mutex);
        printf("\n%d - Recurso tomado", thread_id);

       // Espera e indica que el recurso fue tomado
        sleep((rand() % 10) + 1);
        printf("\n%d - Buenos dias! Recurso usado", thread_id);

        // Despues de usarlo, regresa el recurso (libera el semaforo)
        sem_post(&mutex);
        printf("\n%d - Recurso devuelto :)", thread_id);
    }

    printf("\nTerminando thread: %d", thread_id);

}


// main
int main() {
    printf("\nIniciando programa");

	// Inicia el semaforo (0 es para indicar que se trabajara con threads, 1 es para iniciarlo "disponible")
    sem_init(&mutex, 0, 1);
	srand(time(NULL));
    
	// Se crea un array de threads
	pthread_t threadArr[QUANT_THREADS];
	
	printf("\nCreando threads");
	int i;    

	// for para inicializar cada thread    
	for(i=0; i<QUANT_THREADS; i++) {
        pthread_create(&threadArr[i], NULL, thread, NULL);
    }

    printf("\nEsperando threads");
    
	// for para esperar a que todos los threads creados terminen
	for(i=0; i<QUANT_THREADS; i++) {
        pthread_join(threadArr[i],NULL);
    }

	// Terminar el semaforo y terminar programa cuando terminen los threads de ejecutarse
    sem_destroy(&mutex);
    printf("\nPrograma finalizado (fertig!)\n");
    return 0;
}

