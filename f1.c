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
    perror("Veuillez entrer un et un seul argument : start / next / cancel / results");
    exit(1);
  }
  if (!strcmp(argv[1], "results")) {
    // TODO : afficher les résultats de la saison
    exit(0);
  }

  char* gp = input_name(argv[1]);
  int phase = find_phase(argv[1], gp, &CAR_COUNT, CAR_NUMS); // Trouve la phase correspondante et initialise la liste des participants
  if (phase == -1) {
    perror("Erreur de recherche de la phase à exécuter");
    exit(1);
  } else if (phase == 0) {
    // Si l'instruction était "cancel"
    exit(0);
  } else if ((phase > 7 && phase < 11) || (phase > 19)) {
    printf("Ce Grand Prix est terminé, veuillez relocaliser les fichiers ou ignorer cette option\n");
    exit(0);
  }
  printf("Initialisation de la phase %d du Grand Prix de %s\n", phase, gp);

  // Définition d'une shared memory pouvant contenir la structure globalmemory
  int shm_id = shmget(IPC_PRIVATE, sizeof(globalmemory), IPC_CREAT | 0666);
  if (shm_id == -1) {
    perror("Erreur d'initialisation de la shared memory");
    exit(1);
  }
  // Attachement de la shm et casting du type en structure prédéfinie
  globalmemory* global_shm = (globalmemory *) shmat(shm_id, NULL, 0);
  if (global_shm == (globalmemory *)-1) {
    perror("Erreur d'attachement de la shared memory");
    exit(1);
  }
  init_globalmemory(global_shm);

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
      init_car(i, CAR_NUMS[i], sem_id, global_shm, phase);
      shmdt(global_shm); // Détachement de la shm du processus fils
      exit(0);
    }
  }

  int cars_running = CAR_COUNT;
  globalmemory shm_copy[CAR_COUNT];
  // Tant qu'il y a encore des voitures en course
  while (cars_running > 0) {
    sleep(1);
    cars_running = 0;
    memcpy(shm_copy, global_shm, sizeof(globalmemory)); // Copie locale de la shared memory
    sort_cars(shm_copy->cars, CAR_COUNT, phase); // Tri de l'attribut cars de la copie locale
    display_refresh(); // clear
    // Comptage du nombre de voitures en course et affichage des résultats triés
    for (int i = 0; i < CAR_COUNT; i++) {
      display_car(shm_copy->cars[i]);
      if (shm_copy->cars[i].status == 1) {
        cars_running += 1;
      }
    }
    display_sectors(shm_copy->sector_best); // Affichage des temps des meilleurs secteurs
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
  // Détachement de la shm du processus père et libération de l'espace
  shmdt(global_shm);
  shmctl(shm_id, IPC_RMID, NULL);
  free(gp);

  exit(0);
}
