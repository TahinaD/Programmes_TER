algo_base_wos.c est l'implantation de l'algorithme de base WOS sur un axe avec deux conditions limites de Robin.

algo_var_fonct_wos.c est l'implantation à la fois de la variante à reinjections avec mémoire et de la fonctionnelle.
Il faut d'abord exécuter la variante avec les paramètres choisis, puis relancer le programme avec un paramètre h.
Le seuil de rebonds doit être supérieur à la température T2, et assez grand pour que Mmax > 1.

Les scripts script_vary_*_wos.sh prennent le programme algo_wos_1D_*.c et créent un graphe de variation de la valeur de la température estimée en fonction de la variation du paramètre correspondant.

Le script script_tcalc_wos. prend les deux algorithmes algo_*_tcalc.c et renvoie dans des fichiers respectifs les temps de calcul relevés selon les valeurs de h.
