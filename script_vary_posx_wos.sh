#!/bin/bash
NREAL=10000
FILE="wos_1D_posx_result_N${NREAL}.txt"
CODE="algo_wos_1D_posx.c"

gcc "${CODE}" -o wos_1D_posx -lm

rm -f "${FILE}"

echo "Position_x Température_estimée Erreur Analytique" >> "${FILE}"
for i in $(seq 0 50); do
  ./wos_1D_posx "${NREAL}" "${i}" >> "${FILE}"
done

echo " "
printf '>>> Results in file %s \n' "${FILE}"
echo " "

#plot result with gnuplot
if command -v gnuplot > /dev/null
then
  {
    echo "set terminal pdfcairo size 12cm,12cm"
    echo "set output 'WOS_1D_variation_position_N${NREAL}.pdf'"
    echo "set pointintervalbox 0"
    echo " "
    echo "set xrange [0:50];"
    echo " "
    echo "set xlabel 'Position x'"
    echo "set ylabel 'Température estimée'"
    echo " "
    echo "plot '${FILE}' u 1:2:3 w yerrorbar title 'Standard MC' pt 7 ps 0.2,'${FILE}' using 1:4 title 'analytic' lc rgb 'black' pt 7 ps 0.2;"
  } > plot_position.gp
  gnuplot plot_position.gp
else
  echo "gnuplot is not install on your system. You can install gnuplot or view the simulation results in other tool."
fi
