#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float random_float(float min, float max){
    //génère un float aléatoire
    return ((max - min) * ((float)rand() / RAND_MAX)) + min;
}

float best_time(float arr[], int n){
    //renvoie le plus petit float

    float min = arr[0];
    for (int i = 1; i < n; i++){
        if (arr[i] < min){
            min = arr[i];
        }
    }
    return min;
}

int main(){
    srand((unsigned)time(NULL));
    float lap_distance = 7.001; //donnée facultative
    int laps_nbr = 44;

    float section1[44] = {};
    float section2[44] = {};
    float section3[44] = {};

    //ajout d'une valeur aléatoire dans les array
    for(int j = 0; j < 3; j++){
        for(int i = 0; i < laps_nbr; i++){
            if(j == 0){
                *(section1+i) = ("%.2f\n", random_float(25.00, 45.00));
            } else if(j == 1){
                *(section2+i) = ("%.2f\n", random_float(25.00, 45.00));
            } else if (j == 2){
                *(section3+i) = ("%.2f\n", random_float(25.00, 45.00));
            }
        }
    }

    //impression des temps de chaque section pour chaque tours
    for(int x = 0; x < laps_nbr; x++){
        printf("tours %i :\n", x);
        printf("section 1 : %f ", section1[x]);
        printf("\n");
        printf("section 2 : %f ", section2[x]);
        printf("\n");
        printf("section 3 : %f ", section3[x]);
        printf("\n\n");
    }

    //impression des meilleures temps
    printf("best time of section one is : %.2f secondes\n", best_time(section1, laps_nbr));
    printf("best time of section two is : %.2f secondes\n", best_time(section2, laps_nbr));
    printf("best time of section three is : %.2f secondes\n", best_time(section3, laps_nbr));

}
