void display_refresh() {
  printf("\033[H\033[J"); // Caractère de rafraichissement de l'affichage
}

void display_header(char* gp, int phase) {
  char* phase_name = get_phase_name(phase);
  printf("Grand Prix de %s - Phase %s (%d)\n", gp, phase_name, phase);
  printf("%-9s %-17s %-7s %-10s %-10s %-8s\n", "Voiture", "Pilote", "Tours", "Temps", "Diff", "Statut");
  free(phase_name);
}

char* get_pilot_name(int id) {
  for (int i = 0; i < INITIAL_CAR_COUNT; i++) {
    if (INITIAL_CAR_NUMS[i] == id) {
      return CAR_NAMES[i];
    }
  }
}

int get_time_diff(int i, int car_time, int previous_time) {
  int diff;
  if (!i) {
    diff = 0;
  } else if (car_time == 1000000) {
    diff == 1000000;
  } else {
    diff = car_time - previous_time;
  }
  return diff;
}

char* get_status(int status) {
  static char status_name[7];
  switch (status) {
    case 0:
    case 2:
      strcpy(status_name, "STANDS");
      break;
    case 1:
      strcpy(status_name, "COURSE");;
      break;
    case 3:
      strcpy(status_name, "OUT");;
      break;
  }
  return status_name;
}

void display_practice_qualif(int phase, int i, car* cars) {
  char* name = get_pilot_name(cars[i].id);
  int diff = get_time_diff(i, cars[i].time_best, cars[i-1].time_best); // La différence se calcule sur le temps du meilleur tour
  char* status = get_status(cars[i].status);
  printf("%-9d %-17s %-7d %-10.3f %-10.3f %-8s\n", cars[i].id, name, cars[i].laps_count, (float) cars[i].time_best/1000, (float) diff/1000, status);
}

void display_race(int phase, int i, car* cars) {
  char* name = get_pilot_name(cars[i].id);
  int diff = get_time_diff(i, cars[i].time_total, cars[i-1].time_total); // La différence se calcule sur le temps total
  char* status = get_status(cars[i].status);
  printf("%-9d %-17s %-7d %-10.3f %-10.3f %-8s\n", cars[i].id, name, cars[i].laps_count, (float) cars[i].time_total/1000, (float) diff/1000, status);
}

void display_car(int phase, int i, car* cars) {
  switch (phase) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 11:
    case 12:
    case 13:
    case 14:
    case 16:
    case 17:
    case 18:
      display_practice_qualif(phase, i, cars);
      break;
    case 7:
    case 15:
    case 19:
      display_race(phase, i, cars);
      break;
  }
}

void display_best_times(besttime* sector_best, besttime best_lap) {
  for (int i = 0; i < 3; i++) {
    printf("Secteur %d : %.3f - voiture %d\n", i+1, (float) sector_best[i].time/1000, sector_best[i].car_id);
  }
  printf("Meilleur tour : %.3f - voiture %d\n", (float) best_lap.time/1000, best_lap.car_id);
}

void display_timer(int time_ms) {
  if (time_ms > 0) {
    printf("Temps restant : %d minutes %d seconds\n", time_ms/60000, (time_ms/1000)%60);
  } else {
    printf("Course terminée\n");
  }
}

void sort_fastest_lap(car* cars, int count) {
  for (int i = 0; i < count - 1; i++) {
    for (int j = 0; j < count - i - 1; j++) {
      if (cars[j].time_best > cars[j+1].time_best) {
        car temp = cars[j];
        cars[j] = cars[j+1];
        cars[j+1] = temp;
      }
    }
  }
}

void sort_first(car* cars, int count) {
  for (int i = 0; i < count - 1; i++) {
    for (int j = 0; j < count - i - 1; j++) {
      // Tri selon le nombre de tours ou le temps total à nombre de tours égal
      if ((cars[j].laps_count < cars[j+1].laps_count) || ((cars[j].laps_count == cars[j+1].laps_count) && cars[j].time_total > cars[j+1].time_total)) {
        car temp = cars[j];
        cars[j] = cars[j+1];
        cars[j+1] = temp;
      }
    }
  }
}

void sort_cars(car* cars, int count, int phase) {
  switch (phase) {
    // Essais libres
    case 1:
    case 2:
    case 3:
    case 11:
    // Qualifications
    case 4:
    case 5:
    case 6:
    case 12:
    case 13:
    case 14:
    case 16:
    case 17:
    case 18:
      sort_fastest_lap(cars, count);
      break;
    // Courses
    case 7:
    case 15:
    case 19:
      sort_first(cars, count);
      break;
  }
}

