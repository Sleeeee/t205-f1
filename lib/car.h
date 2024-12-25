struct sembuf lock = {0, -1, 0};
struct sembuf unlock = {0, 1, 0};

int init_car(int id, int car_num, int sem_id, car* cars_shm, int phase) {
  // Définition de la seed aléatoire basée sur le pid
  srand(time(NULL) ^ (getpid()));

  // Initialisation des données de la voiture
  cars_shm[id].id = car_num;
  cars_shm[id].laps_count = 0;
  cars_shm[id].time_total = 0;
  cars_shm[id].time_best = 512; // Temps plus long que ce qu'il est possible d'effectuer pour assurer la réécriture
  // Lancement de la course
  cars_shm[id].status = 1;
  race(5, id, cars_shm);
  cars_shm[id].status = 0;

  return 0;
}

