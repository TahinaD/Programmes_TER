#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]){
  
  if (argc != 3) {
    printf("Arguments attendus : nombre de réalisation + position x\n");
    return 0;
  }
  
  unsigned long nrealisations = atoi(argv[1]);
  double sum = 0;
  double sum2 = 0;
  double estim, std;

  double delta = 0.01;
  double lambda = 200;
  double h = 3;
  double proba = lambda/(delta*h + lambda);

  double Td = 20;
  double T2 = 50;

  double borne_inf = 0;
  double borne_sup = 50;
  double longueur = sqrt(pow((borne_sup - borne_inf),2));
  double dis_inf, dis_sup, sphere;
  double x = atof(argv[2]);

  double Tx = (x/((lambda/h)+longueur))*T2 + (1- (x/((lambda/h)+longueur)))*Td;
  srand(time(0));

  for (unsigned long i = 0; i < nrealisations; i++){
    double w = Td+T2+1;

    while ((w != Td) && (w != T2)){
      dis_inf = x - borne_inf;
      dis_sup = borne_sup - x;

      sphere = ((dis_inf < dis_sup) ? dis_inf : dis_sup);

      double choice = (double)rand();
      x += ((choice > RAND_MAX/2) ? sphere : -sphere);

      if (x <= borne_inf)
        w = Td;

      if (x >= borne_sup){
        double r = ((double)rand())/RAND_MAX;
        if (r < proba)
          x -= delta;
        else
          w = T2;
      }
    }

    sum += w;
    sum2 += w*w;
    x = atof(argv[2]);
  }

  x = atof(argv[2]);
  estim = sum/nrealisations;
  std = sqrt((sum2/nrealisations - estim*estim)/nrealisations);
  printf("%lf %lf %lf %lf\n", x, estim, std, Tx);
  return 0;}
