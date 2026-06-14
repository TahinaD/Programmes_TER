#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


int main(int argc, char* argv[]){
 
  if (argc == 1) {
     FILE* fptr;
     fptr = fopen("informations_wos_1D.txt", "r");
     
     if (fptr == NULL) {
       printf("fail open file");
     } else {
       printf("ouverture du fichier\n");
       unsigned long nrealisations, nrebons = 0, cur_nreb;
       int nbinter, Tcur = -1;
       double sum = 0, sum2 = 0, proba;
       double estim, std, festim, fstd;

       fscanf(fptr, "%ld %lf %d", &nrealisations, &proba, &nbinter);
       printf("%ld %lf %d\n", nrealisations, proba, nbinter);
       double* inter = malloc(nbinter*sizeof(double));

       printf("instanciation interfaces\n");
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
         for (int reb = 0; reb < cur_nreb - 1; reb++) {
           fscanf(fptr, "%d ", &Tcur);
           w += pow((1-proba), nrebons)*proba*inter[Tcur];
           nrebons++;
         }
         
         fscanf(fptr, "%d \n", &Tcur);
         w += pow((1-proba), nrebons)*inter[Tcur];
         nrebons = 0;
         sum += w;
         sum2 += w*w;
         w = 0;

         fscanf(fptr, "%ld ", &cur_nreb);
       }

       fscanf(fptr, "%lf %lf \n", &festim, &fstd);
       free(inter);

       printf("fin de la lecture\n");
       estim = sum/nrealisations;
       std = sqrt((sum2/nrealisations - estim*estim)/nrealisations);
       printf("%lf +- %lf\n", estim, std);
       printf("fichier : %lf +- %lf\n", festim, fstd);
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

  FILE* fptr;
  fptr = fopen("informations_wos_1D.txt", "w");

  if (fptr == NULL) {
    printf("fail open file");
    fclose(fptr);
    return 0;
  }

  unsigned long Mmax = log(T2/seuil_rebons)/log(1 - proba); 

  fprintf(fptr, "%ld %lf 2 %lf %lf\n", nrealisations, proba, T1, T2);
  char ligne[2*Mmax + 2];
  char str_rebons[(int)log10(Mmax) + 2];
  int hist[Mmax];

  for (unsigned long j = 0; j < Mmax; j++){
    hist[j] = -1;
  }

  for (unsigned long i = 0; i < nrealisations; i++){
    unsigned long nrebons = 0;
    strcpy(ligne, " ");
    
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
        strcat(ligne,"0 ");
      }
    
      if (x >= borne_sup - epsilon) {
        hist[nrebons] = 1;
        nrebons++;
        x = x - delta;
        strcat(ligne, "1 ");
      }
    }

    sprintf(str_rebons, "%ld", nrebons);
    fputs(str_rebons, fptr);
    fputs(ligne, fptr);
    fprintf(fptr, "\n");

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

  x = atof(argv[3]);
  estim = sum/nrealisations;
  std = sqrt((sum2/nrealisations - estim*estim)/nrealisations);
  fprintf(fptr, "-1 %lf %lf\n", estim, std);
  fclose(fptr);
  printf("Le fichier d'informations a été rempli.\n");

  printf("%lf = %lf +- %lf\n", Tx, estim, std);
  return 0;
}
