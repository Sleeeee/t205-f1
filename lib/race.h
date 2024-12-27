int lap(int car_id, car* cars_shm) {
  int total = 0;
  // Exécution des trois secteurs
  for (int i = 0; i < 3; i++) {
    // Court un secteur, enregistre le temps, le processus dort une durée proportionnelle au temps couru
    int t = random_int(25, 45);
    cars_shm[car_id].time_sectors[i] = t;
    total += t;
    sleep(t / 20);
  }
  return total;
}

void start_practice(int car_id, car* cars_shm) {
  printf("practicing\n");
}

void start_qualif(int time_min, int car_id, car* cars_shm) {
  printf("qualifying\n");
}

void start_race(int num_laps, int car_id, car* cars_shm) {
  int t;
  for (int i = 0; i < num_laps; i++) {
    if ((t = lap(car_id ,cars_shm)) < cars_shm[car_id].time_best) {
      cars_shm[car_id].time_best = t;
    }
    cars_shm[car_id].time_total += t;
    cars_shm[car_id].laps_count++;
  }
}

void start_phase(int car_id, car* cars_shm, int phase) {
  switch (phase) {
    // Essais libres
    case 1:
    case 2:
    case 3:
    case 11:
      start_practice(car_id, cars_shm);
      break;
    // Qualifications
    case 4:
    case 16:
      start_qualif(18, car_id, cars_shm);
      break;
    case 5:
    case 17:
      start_qualif(15, car_id, cars_shm);
      break;
    case 6:
    case 12:
    case 18:
      start_qualif(12, car_id, cars_shm);
      break;
    case 13:
      start_qualif(10, car_id, cars_shm);
    case 14:
      start_qualif(8, car_id, cars_shm);
    // Course classique
    case 7:
    case 19:
      start_race(60, car_id, cars_shm);
      break;
    // Course sprint
    case 15:
      start_race(20, car_id, cars_shm);
      break;
  }
}

