#define INITIAL_CAR_COUNT 20
static const int INITIAL_CAR_NUMS[] = {1, 11, 44, 63, 16, 55, 4, 81, 14, 18, 10, 31, 23, 2, 22, 3, 77, 24, 20, 27};
static char* CAR_NAMES[] = {"Max Verstappen", "Sergio Perez", "Lewis Hamilton", "George Russell", "Charles Leclerc", "Carlos Sainz Jr", "Lando Norris", "Oscar Piastri", "Fernando Alonso", "Lance Stroll", "Pierre Gasly", "Esteban Ocon", "Alexander Albon", "Logan Sargeant", "Yuki Tsunoda", "Daniel Ricciardo", "Valtteri Bottas", "Zhou Guanyu", "Kevin Magnussen", "Nico Hulkenberg"};
#define TIME_DIV 20000
#define max(a, b) ((a) > (b) ? (a) : (b))

// Tableau des opérations effectuées sur un sémaphore lors du blocage, déblocage
struct sembuf lock = {0, -1, 0};
struct sembuf unlock = {0, 1, 0};

typedef struct car {
  int id;
  int laps_count;
  int time_total;
  int time_best;
  int time_sectors[3];
  int status; // 0 : course terminée / 1 : en course / 2 : stands / 3 : crashée
} car;

typedef struct {
  int car_id;
  int time;
} besttime;

typedef struct {
  car cars[INITIAL_CAR_COUNT];
  besttime sector_best[3];
  besttime best_lap;
  int time_left;
} globalmemory;

