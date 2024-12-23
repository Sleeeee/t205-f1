typedef struct car {
  int id;
  int laps_count;
  int time_total;
  int time_best;

  int time_sector1;
  int time_sector2;
  int time_sector3;

  int status; // 0 : course terminée / 1 : en course / 2 : au stand / 3 : crashée
} car;

int init_car(int car_id, car* cars_shm) {
  // Définition de la seed aléatoire basée sur le pid
  srand(time(NULL) ^ (getpid()));

  cars_shm[car_id].id = car_id;
  cars_shm[car_id].time_sector1 = random_int(25, 45);

  return 0;
}

