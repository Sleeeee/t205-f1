void display_refresh() {
  printf("\033[H\033[J"); // Caractère de rafraichissement de l'affichage
}

void display_header(int phase) {
  printf("%-9s %-7s %-15s %-9s %-8s\n", "Voiture", "Tours", "Meilleur tour", "Diff", "Statut");
}

void display_car(int phase, int i, car* cars) {
  int diff;
  char status[7];

  if (!i) {
    diff = 0;
  } else if (cars[i].time_best == 1000000) {
    diff = 1000000;
  } else {
    diff = (cars[i].time_best - cars[i-1].time_best);
  }
  switch (cars[i].status) {
    case 0:
    case 2:
      strcpy(status, "STANDS");
      break;
    case 1:
      strcpy(status, "COURSE");;
      break;
    case 3:
      strcpy(status, "OUT");;
      break;
  }
  printf("%-9d %-7d %-15.3f %-9.3f %-8s\n", cars[i].id, cars[i].laps_count, (float) cars[i].time_best/1000, (float) diff/1000, status);
}

void display_sectors(sectorbest* sector_best) {
  for (int i = 0; i < 3; i++) {
    printf("secteur %d, car %d, time %.3f / ", i+1, sector_best[i].car_id, (float) sector_best[i].time/1000);
  }
  printf("\n");
}

void display_timer(int time) {
  printf("time left %d minutes %d seconds\n", time/60000, (time/1000)%60);
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

