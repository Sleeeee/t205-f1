// ATTENTION la fonction renvoie toujours le même résultat avec les mêmes paramètres

float random_float(float min, float max) {
    // Génère un float aléatoire
    return ((max - min) * ((float)rand() / RAND_MAX)) + min;
}

int stand(float prob) {
    // Renvoie un booléen indiquant si la voiture se rend au stand
    // Retour au stand si le random atteint un float inférieur à la prob
    return random_float(0.0, 1.0) <= prob;
}

int crash(float prob) {
    // Renvoie un booléen indiquant si la voiture se crashe
    // Crash si le random atteint un float inférieur à la prob 
    return random_float(0.0, 1.0) <= prob;
}

float sector(int size_meters, float time_seed, float crash_prob) {
    // Renvoie le temps du pilote pour parcourir le secteur / 0.0 si la voiture se crashe
    if (crash(crash_prob)) {
      return 0.0;
    }

    float min_time = time_seed * size_meters * 0.7;
    float max_time = time_seed * size_meters * 1.3;
    return random_float(min_time, max_time);
}

