#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "lib/constants.h"
#include "lib/files.h"
#include "lib/championship.h"
#include "lib/display.h"
#include "lib/sector.h"
#include "lib/race.h"
#include "lib/car.h"

int CAR_COUNT;
int CAR_NUMS[20]; // Pas d'alloation dynamique

int main(int argc, char *argv[]){
  // Lancement du programme et recherche de la phase à exécuter
  if (argc != 2) {
    perror("Veuillez entrer un et un seul argument : start / next / cancel");
    exit(1);
  }

  char* gp = input_name(argv[1]);
  int phase = find_phase(argv[1], gp, &CAR_COUNT, CAR_NUMS); // Trouve la phase correspondante et initialise la liste des participants
  if (phase == -1) {
    perror("Erreur de recherche de la phase à exécuter");
    exit(1);
  } else if (phase == 0) {
    // Si l'instruction était "cancel"
    exit(0);
  }
  printf("Initialisation de la phase %d du Grand Prix de %s\n", phase, gp);

  // Définition d'une shared memory pouvant contenir NUM_CARS structures voitures
  int shm_id = shmget(IPC_PRIVATE, CAR_COUNT * sizeof(car), IPC_CREAT | 0666);
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

  // Initialisation d'un sémaphore pour la synchronisation des processus
  int sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
  if (sem_id == -1) {
    perror("Erreur d'initialisation du sémaphore");
    exit(1);
  }
  semctl(sem_id, 0, SETVAL, 1);

  // Création des processus fils et initialisation des voitures
  pid_t pid;
  for (int i = 0; i < CAR_COUNT; i++) {
    pid = fork();
    if (pid == -1) {
      perror("Erreur de création de processus fils");
      exit(1);
    } else if (pid == 0) {
      // Le processus fils initialise la voiture en lui donnant un numéro de voiture, l'identifiant du sémaphore et le pointeur vers la shm
      init_car(i, CAR_NUMS[i], sem_id, cars_shm, phase);
      shmdt(cars_shm); // Détachement de la shm du processus fils
      exit(0);
    }
  }

  int cars_running = CAR_COUNT;
  car cars_copy[CAR_COUNT];
  // Tant qu'il y a encore des voitures en course
  while (cars_running > 0) {
    sleep(1);
    cars_running = 0;
    memcpy(cars_copy, cars_shm, sizeof(car) * CAR_COUNT); // Copie locale de la shared memory
    sort_cars(cars_copy, CAR_COUNT, phase); // Tri de la copie locale
    display_refresh(); // clear
    // On compte le nombre de voitures en course et on affiche les résultats triés
    for (int i = 0; i < CAR_COUNT; i++) {
      display_car(cars_copy[i]);
      if (cars_copy[i].status == 1) {
        cars_running += 1;
      }
    }
  }

  // Le processus père attend la terminaison des processus fils
  for (int i = 0; i < CAR_COUNT; i++) {
    int status;
    pid_t child_pid = wait(&status);
    if (child_pid > 0) {
      if (WIFEXITED(status)) {
        printf("Processus %d terminé avec le statut %d\n", child_pid, status);
      }
    }
  }
  
  // Libération du sémaphore
  semctl(sem_id, 0, IPC_RMID);

  // Lecture de la shm
  for (int i = 0; i < CAR_COUNT; i++) {
    printf("Id : %d / time : %d\n", cars_shm[i].id, cars_shm[i].time_total);
  }

  // Détachement de la shm du processus père et libération de l'espace
  shmdt(cars_shm);
  shmctl(shm_id, IPC_RMID, NULL);
  free(gp);

  exit(0);
}
