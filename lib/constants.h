typedef struct car {
  int id;
  int laps_count;
  int time_total;
  int time_best;
  int time_sectors[3];
  int status; // 0 : course terminée / 1 : en course / 2 : crashée
} car;

#define INITIAL_CAR_COUNT 20
static const int INITIAL_CAR_NUMS[] = {1, 11, 44, 63, 16, 55, 4, 81, 14, 18, 10, 31, 23, 2, 22, 3, 77, 24, 20, 27};

