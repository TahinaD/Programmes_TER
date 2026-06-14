#!/bin/bash
XPOS=25
FILE="wos_1D_nreal_result_X${XPOS}.txt"
CODE="algo_wos_1D_nreal.c"

gcc "${CODE}" -o wos_1D_nreal -lm

rm -f "${FILE}"

echo "N_Réalisations Température_estimée Erreur Analytique" >> "${FILE}"
for i in 1000 10000 25000 50000 75000 100000 200000 300000 400000 500000 600000 700000 800000 900000 1000000 ; do
  ./wos_1D_nreal "${i}" "${XPOS}" >> "${FILE}"
done

echo " "
printf '>>> Results in file %s \n' "${FILE}"
echo " "

#plot result with gnuplot
if command -v gnuplot > /dev/null
then
  {
    echo "set terminal pdfcairo size 12cm,12cm"
    echo "set output 'WOS_1D_variation_nrealisations_X${XPOS}.pdf'"
    echo "set pointintervalbox 0"
    echo " "
    echo "set xrange [0:1000000];"
    echo " "
    echo "set xlabel 'Nombre réalisations n'"
    echo "set ylabel 'Température estimée'"
    echo " "
    echo "plot '${FILE}' u 1:2:3 w yerrorbar title 'Standard MC' pt 7 ps 0.2,'${FILE}' using 1:4 title 'analytic' lc rgb 'black' pt 7 ps 0.2;"
  } > plot_position.gp
  gnuplot plot_position.gp
else
  echo "gnuplot is not install on your system. You can install gnuplot or view the simulation results in other tool."
fi
