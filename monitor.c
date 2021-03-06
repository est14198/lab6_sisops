// Universidad del Valle de Guatemala
// Sistemas Operativos
// Maria Fernanda Estrada 14198
// Laboratorio 6 - Monitor
// Codigo base tomado de https://www.geeksforgeeks.org/use-posix-semaphores-c/


#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdbool.h>

// Constantes modificables
#define QUANT_THREADS 6 // cantidad de threads ajustable
#define QUANT_ITER 9 // cantidad de iteraciones ajustable

// Definimos un semaforo global
sem_t mutex;
// Definimos un "recurso" global
int available_resources = 69;

// Decrementar contador de recursos
int decrease_count(int count) { 
	if (available_resources < count) {
		return -1;
	}
	else { 
		available_resources -= count; 
		return 0; 
	}
} 

// Incrementar contador de recursos
int increase_count(int count) {
	available_resources += count; 
	return 0;
}

// Metodo que ejecutara cada thread
void* function(void* arg) {
	// Para saber que numero de thread se ejecuta
    int thread_id = syscall(SYS_gettid);
    printf("\nIniciando thread: %d", thread_id);

    int i;

	// for para "tomar" un recurso durante cierto tiempo
    for(i=0; i<QUANT_ITER; i++) {
        printf("\nIniciando iteracion: %d", i);
        
		// while para que los que no tomaron recurso se queden esperando hasta tomarlo
		bool loop = true;
		while (loop) {
		    sem_wait(&mutex);
			int decreased = decrease_count(25);
			sem_post(&mutex);

			if (decreased == 0) {
				printf("\n%d - Recursos tomados", thread_id);
				int prueba = (rand() % 10) + 1;
				printf("\n%d ---- rand", prueba);
			   // Espera e indica que el recurso fue tomado
				sleep((rand() % 10) + 1);
				printf("\n%d - Buenos dias! Recursos usados", thread_id);
				// wait si el recurso ya esta siendo usado
				sem_wait(&mutex);
				increase_count(25); // regresar recuros
				sem_post(&mutex);
				// Despues de usarlo, regresa el recurso (libera el semaforo)
				printf("\n%d - Recursos devueltos :)", thread_id);
				loop = false;
			}

			else {
				// Si no hay recursos suficientes esperar
				printf("\n%d - No hay recursos suficientes :(", thread_id);
				sleep(3);
			}
		}
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
        pthread_create(&threadArr[i], NULL, function, NULL);
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

