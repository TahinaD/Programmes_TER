#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char* argv[]){
  
  if (argc != 5) {
    printf("Arguments attendus : nombre de réalisation + h + position x + seuil rebons\n");
    return 0;
  }
  
  unsigned long nrealisations = atoi(argv[1]);
  double sum = 0;
  double sum2 = 0;
  double estim, std;

  double epsilon = 0.5;
  double lambda = 200;
  double delta = 0.5;

  double T1 = 20;
  double T2 = 50;
  double seuil_rebons = atof(argv[4]);

  double borne_inf = 0;
  double borne_sup = 50;
  double dis_inf, dis_sup, sphere;
 
  double h = atof(argv[2]);
  double proba = h/(h + (lambda/delta));
  double x = atof(argv[3]);

  double longueur = sqrt(pow((borne_sup - borne_inf),2));
  
  double Tx = (x/((lambda/h)+longueur))*T2 + (1- (x/((lambda/h)+longueur)))*T1;
  srand(time(0));

  unsigned long Mmax = log(T2/seuil_rebons)/log(1 - proba); 
  
  int hist[Mmax];
  for (unsigned long j = 0; j < Mmax; j++){
    hist[j] = -1;
  }

  int ordre = 0;
  double shrink_p = 1-proba;
  while (shrink_p < 1) {
    ordre--;
    shrink_p = shrink_p*10;
  }

  time_t begin = time(NULL);

  for (unsigned long i = 0; i < nrealisations; i++){
    unsigned long nrebons = 0;
    
    while (nrebons < Mmax) {
      dis_inf = x - borne_inf;
      dis_sup = borne_sup - x;

      sphere = ((dis_inf < dis_sup) ? dis_inf : dis_sup);

      double choice = (double)rand();
      x += ((choice > RAND_MAX/2) ? sphere : -sphere);

      if (x <= borne_inf + epsilon) {
        hist[nrebons] = 0;
        nrebons++;
        x = x + delta;
      }
    
      if (x >= borne_sup - epsilon) {
        hist[nrebons] = 1;
        nrebons++;
        x = x - delta;
      }
    }

     
    int T;
    double w = ((hist[nrebons - 1] == 0) ? T1 : T2);
    
    for (unsigned long j = nrebons - 1; j > 0; j--){
      T = ((hist[j] == 0) ? T1 : T2);
      w = (1-proba)*w + proba*T;
      hist[j] = -1;
    }
  
    T = ((hist[0] == 0) ? T1 : T2);
    w = (1-proba)*w + proba*T;
    hist[0] = -1;

    sum += w;
    sum2 += w*w;
    x = atof(argv[3]);
  }

  time_t end = time(NULL);
  
  int secondes = (int) difftime(end, begin);
  
  x = atof(argv[3]);
  estim = sum/nrealisations;
  std = sqrt((sum2/nrealisations - estim*estim)/nrealisations);
  printf("%lf %d %lf %lf %d\n", shrink_p, ordre, estim, std, secondes);
  return 0;
}
