void display_refresh() {
  printf("\033[H\033[J"); // Caractère de rafraichissement de l'affichage
}

void display_car(car car) {
  printf("car %d, %d laps, best lap %d, time total %d, s1 %d, s2 %d, s3 %d, status %d\n", car.id, car.laps_count, car.time_best, car.time_total, car.time_sectors[0], car.time_sectors[1], car.time_sectors[2], car.status);
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

