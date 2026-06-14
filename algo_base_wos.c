#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]){
  
  if (argc != 4) {
    printf("Arguments attendus : nombre de réalisation + reinjection + position x \n");
    return 0;
  }
  
  unsigned long nrealisations = atoi(argv[1]);
  double sum = 0;
  double sum2 = 0;

  double delta = 0.5;
  double epsilon = 0.5;
  double lambda = 200;
  double h = atof(argv[2]);
  double proba = lambda/(delta*h + lambda);
  
  double T1 = 20;
  double T2 = 50;

  double borne_inf = 0;
  double borne_sup = 50;
  double longueur = sqrt(pow((borne_sup - borne_inf),2));
  double dis_inf, dis_sup, sphere;
  double x = atof(argv[3]);
 
  double Tx = (x/((lambda/h)+longueur))*T2 + (1- (x/((lambda/h)+longueur)))*T1;
  srand(time(0));

  for (unsigned long i = 0; i < nrealisations; i++){
    double w = 0;
    int trouve = 0;

    while (trouve == 0){
      dis_inf = x - borne_inf;
      dis_sup = borne_sup - x;

      sphere = ((dis_inf < dis_sup) ? dis_inf : dis_sup);

      double choice = (double)rand();
      x += ((choice > RAND_MAX/2) ? sphere : -sphere);

      if (x <= borne_inf + epsilon) {
        double r = ((double)rand())/RAND_MAX;
        if (r < proba)
          x += delta;
        else {
          trouve = 1;
          w = T1;
        }
      }

      if (x >= borne_sup - epsilon){
        double r = ((double)rand())/RAND_MAX;
        if (r < proba)
          x -= delta;
        else {
          trouve = 1;
          w = T2;
        }
      }
    }

    sum += w;
    sum2 += w*w;
    x = atof(argv[3]);
  }

  x = atof(argv[3]);
  double estim = sum/nrealisations;
  double std = sqrt((sum2/nrealisations - estim*estim)/nrealisations);
  printf("%lf %lf %lf %lf\n", x, estim, std, Tx);
  return 0;}
