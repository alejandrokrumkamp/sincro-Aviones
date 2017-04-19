/*
 * main.c
 *
 *  Created on: 18/4/2017
 *      Author: alejandrokrumkamp
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/syscall.h>

sem_t semaforo_disponible;
sem_t semaforo_pedido;
sem_t semaforo_pistas;
sem_t semaforo_pista_liberada;

pthread_mutex_t mutex_pistas;

int pistasLibres = 10;

void mantenimiento(){
	printf("%i [Avion] Mantenimiento Realizado \n", (int)syscall(__NR_gettid));
}

void despegar(){
	printf("%i [Avion] Despegue Realizado \n", (int)syscall(__NR_gettid));
}

void volar(){
	printf("%i [Avion] Vuelo Realizado \n", (int)syscall(__NR_gettid));
}

void aterrizar(){
	printf("%i [Avion] Aterrizaje Realizado \n", (int)syscall(__NR_gettid));
}

void otorgarUnaPista(){
	printf("%i [Controlador Entrada] Pista Otorgada \n", (int)syscall(__NR_gettid));
}

void liberarUnaPista(){
	printf("%i [Controlador Salida] Pista Liberada \n", (int)syscall(__NR_gettid));
}
void logger(){
	printf("%i [Log] Pistas libres: %i \n",(int)syscall(__NR_gettid),pistasLibres);
}

void * avion(){
	while(1){

		mantenimiento();
		//pedir una pista y esperar
		sem_post(&semaforo_pedido);
		sem_wait(&semaforo_disponible);
		despegar();
		sem_post(&semaforo_pista_liberada);

		usleep(2000);
		volar();

		sem_post(&semaforo_pedido);
		sem_wait(&semaforo_disponible);
		aterrizar();
		sem_post(&semaforo_pista_liberada);


	}
}

void * controladorEntrada(){
	while(1){
		sem_wait(&semaforo_pedido);
		sem_wait(&semaforo_pistas);

		pthread_mutex_lock(&mutex_pistas);
		otorgarUnaPista();
		pistasLibres--;
		pthread_mutex_unlock(&mutex_pistas);

		sem_post(&semaforo_disponible);

		logger();

	}
}

void * controladorSalida(){
	while(1){

		sem_wait(&semaforo_pista_liberada);
		pthread_mutex_lock(&mutex_pistas);
		liberarUnaPista();
		pistasLibres++;
		pthread_mutex_unlock(&mutex_pistas);
		sem_post(&semaforo_pistas);

		logger();

	}
}

int main(void){

	sem_init(&semaforo_disponible, 0, 0);
	sem_init(&semaforo_pedido, 0, 0);
	sem_init(&semaforo_pistas, 0, 10);
	sem_init(&semaforo_pista_liberada, 0, 0);

	pthread_t hiloAvion1,hiloAvion2,hiloAvion3,hiloAvion4,hiloAvion5;
	pthread_t hiloAvion6,hiloAvion7,hiloAvion8,hiloAvion9,hiloAvion10;
	pthread_t hiloControladorEntrada,hiloControladorSalida;

	pthread_create(&hiloAvion1, NULL, avion, NULL);
	pthread_create(&hiloAvion2, NULL, avion, NULL);
	pthread_create(&hiloAvion3, NULL, avion, NULL);
	pthread_create(&hiloAvion4, NULL, avion, NULL);
	pthread_create(&hiloAvion5, NULL, avion, NULL);
	pthread_create(&hiloAvion6, NULL, avion, NULL);
	pthread_create(&hiloAvion7, NULL, avion, NULL);
	pthread_create(&hiloAvion8, NULL, avion, NULL);
	pthread_create(&hiloAvion9, NULL, avion, NULL);
	pthread_create(&hiloAvion10, NULL, avion, NULL);

	pthread_create(&hiloControladorEntrada, NULL, controladorEntrada, NULL);
	pthread_create(&hiloControladorSalida, NULL, controladorSalida, NULL);

	pthread_join(hiloAvion1, NULL);
	pthread_join(hiloAvion2, NULL);
	pthread_join(hiloAvion3, NULL);
	pthread_join(hiloAvion4, NULL);
	pthread_join(hiloAvion5, NULL);
	pthread_join(hiloAvion6, NULL);
	pthread_join(hiloAvion7, NULL);
	pthread_join(hiloAvion8, NULL);
	pthread_join(hiloAvion9, NULL);
	pthread_join(hiloAvion10, NULL);
	pthread_join(hiloControladorEntrada, NULL);
	pthread_join(hiloControladorSalida, NULL);

	return 0;
}
