#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "lib/championship.h"
#include "lib/sector.h"
#include "lib/car.h"

#define NUM_CARS 10

//int random_int(int min, int max);

int main(int argc, char *argv[]){
  // Lancement du programme et recherche de la phase à exécuter
  if (argc != 2) {
    perror("Veuillez entrer un et un seul argument : start / next / cancel");
    exit(1);
  }
  int phase = find_phase(argv[1]);
  if (phase == -1) {
    perror("Erreur de recherche de la phase à exécuter");
    exit(1);
  } else if (phase == 0) {
    // Si l'instruction était "cancel"
    exit(0);
  }

  // Définition d'une shared memory pouvant contenir NUM_CARS structures voitures
  int shm_id = shmget(IPC_PRIVATE, NUM_CARS * sizeof(car), IPC_CREAT | 0666);
  if (shm_id == -1) {
    perror("Erreur d'initialisation de la shared memory");
    exit(1);
  }
  // Attachement de la shm et changement de type en un array de voitures
  car *cars_shm = (car *) shmat(shm_id, NULL, 0);
  if (cars_shm == (car *)-1) {
    perror("Erreur d'attachement de la shared memory");
    exit(1);
  }

  // Création des processus fils et initialisation des voitures
  pid_t pid;
  for (int i = 0; i < NUM_CARS; i++) {
    pid = fork();
    if (pid == -1) {
      perror("Erreur de création de processus fils");
      exit(1);
    } else if (pid == 0) {
      // Le processus fils initialise la voiture en lui donnant un id et le pointeur vers la shm
      init_car(i, cars_shm, phase);
      shmdt(cars_shm); // Détachement de la shm du processus fils
      exit(0);
    }
  }

  // Le processus père attend la terminaison des processus fils
  for (int i = 0; i < NUM_CARS; i++) {
    int status;
    pid_t child_pid = wait(&status);
    if (child_pid > 0) {
      if (WIFEXITED(status)) {
        printf("Processus %d terminé avec le statut %d\n", child_pid, status);
      }
    }
  }

  // Lecture de la shm
  for (int i = 0; i < NUM_CARS; i++) {
    printf("Id : %d / time : %d\n", cars_shm[i].id, cars_shm[i].time_sector1);
  }

  // Détachement de la shm du processus père et cleaning de l'espace
  shmdt(cars_shm);
  shmctl(shm_id, IPC_RMID, NULL);

  exit(0);
}
