#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


int main(int argc, char* argv[]){
 
  if (argc == 2) {
     FILE* fptr;
     fptr = fopen("informations_wos_1D.txt", "r");
     
     if (fptr == NULL) {
       printf("fail open file");
     } else {
       unsigned long nrealisations, cur_nreb;
       int nbinter, Tcur = -1;
       double sum = 0, sum2 = 0, h = atof(argv[1]), proba;
       double estim, std, festim, fstd;
      
       fscanf(fptr, "%ld %lf %d", &nrealisations, &proba, &nbinter);
       double* inter = malloc(nbinter*sizeof(double));
       proba = h/(h + 400);

       for (int i = 0; i < nbinter; i++) {
         inter[i] = 0;
       }

       int vir = 1;
       for (int i = 0; i < nbinter; i++) {
         fscanf(fptr, "%lf ", &inter[i]);
       }

       double w = 0;
       fscanf(fptr, "%ld ", &cur_nreb);
       while (cur_nreb != -1) {
         for (int reb = 0; reb < cur_nreb; reb++) {
           fscanf(fptr, "%d ", &Tcur);
           w += pow((1-proba), reb)*proba*inter[Tcur];
         }
         
         sum += w;
         sum2 += w*w;
         w = 0;

         fscanf(fptr, "%ld ", &cur_nreb);
       }

       fscanf(fptr, "%lf %lf \n", &festim, &fstd);
       free(inter);

       estim = sum/nrealisations;
       std = sqrt((sum2/nrealisations - estim*estim)/nrealisations);
       printf("proba : %lf, estim : %lf, std : %lf\n", proba, estim, std);
       printf("fichier estim : %lf, fichier std : %lf\n", festim, fstd);
     }

     fclose(fptr);
     return 0;
  }

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
  double seuil_rebonds = atof(argv[4]);

  double borne_inf = 0;
  double borne_sup = 50;
  double dis_inf, dis_sup, sphere;
 
  double h = atof(argv[2]);
  double proba = h/(h + (lambda/delta));
  double x = atof(argv[3]);
  
  double longueur = sqrt(pow((borne_sup - borne_inf),2));
  
  double Tx = x/longueur * (T1 + (T2 - T1)/((h*longueur/delta) + 2)) + (1 - x/longueur) * (T2 + (T1 - T2)/((h*longueur/delta) + 2)) ;
  srand(time(0));

  FILE* fptr;
  fptr = fopen("informations_wos_1D.txt", "w");

  if (fptr == NULL) {
    printf("fail open file");
    fclose(fptr);
    return 0;
  }

  unsigned long Mmax = log(T2/seuil_rebonds)/log(1 - proba); 

  fprintf(fptr, "%ld %lf 2 %lf %lf\n", nrealisations, proba, T1, T2);
  char ligne[2*Mmax + 2];
  char str_rebonds[(int)log10(Mmax) + 2];
  int hist[Mmax];

  for (unsigned long j = 0; j < Mmax; j++){
    hist[j] = -1;
  }
  
  for (unsigned long i = 0; i < nrealisations; i++){
    unsigned long nrebonds = 0;
    strcpy(ligne, " ");
    
    while (nrebonds < Mmax) {
      dis_inf = x - borne_inf;
      dis_sup = borne_sup - x;

      sphere = ((dis_inf < dis_sup) ? dis_inf : dis_sup);

      double choice = (double)rand();
      x += ((choice > RAND_MAX/2) ? sphere : -sphere);

      if (x <= borne_inf + epsilon) {
        hist[nrebonds] = 0;
        nrebonds++;
        x = x + delta;
        strcat(ligne,"0 ");
      }
    
      if (x >= borne_sup - epsilon) {
        hist[nrebonds] = 1;
        nrebonds++;
        x = x - delta;
        strcat(ligne, "1 ");
      }
    }

    sprintf(str_rebonds, "%ld", nrebonds);
    fputs(str_rebonds, fptr);
    fputs(ligne, fptr);
    fprintf(fptr, "\n");

    int T;
    double w = 0;
    for (unsigned long j = 0; j < nrebonds; j++){
      T = ((hist[j] == 0) ? T1 : T2);
      w += pow((1-proba), j)*proba*T;
      hist[j] = -1;
    }

    sum += w;
    sum2 += w*w;
    x = atof(argv[3]);
  }

  x = atof(argv[3]);
  estim = sum/nrealisations;
  std = sqrt((sum2/nrealisations - estim*estim)/nrealisations);
  fprintf(fptr, "-1 %lf %lf\n", estim, std);
  fclose(fptr);

  printf("%lf = %lf +- %lf\n", estim, std, Tx);
  return 0;
}
