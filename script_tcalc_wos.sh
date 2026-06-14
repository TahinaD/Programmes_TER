#!/bin/bash
XPOS=25
NREAL=10000000
FILE_BASE="wos_tcalc_result.txt"
FILE_VAR="var_wos_tcalc_result.txt"
CODE_BASE="algo_wos_tcalc.c"
CODE_VAR="algo_var_wos_tcalc.c"

rm -f "${FILE_BASE}"
rm -f "${FILE_VAR}"

gcc "${CODE_BASE}" -o wos_base_tcalc -lm
gcc "${CODE_VAR}" -o wos_var_tcalc -lm


echo "Probabilité Ordre_grandeur Température_estimée Erreur Temps_calc_basique" >> "${FILE_BASE}"
for i in 300 500 600 800 1000 3000 5000 6000 8000; do
  ./wos_base_tcalc "${NREAL}" "${i}" "${XPOS}" >> "${FILE_BASE}"
done

echo " "
printf '>>> Results for base algo in file %s \n' "${FILE_BASE}"
echo " "

echo "Probabilité Ordre_grandeur Température_estimée Erreur Temps_calc_variante" >> "${FILE_VAR}"
for i in 300 500 600 800 1000 3000 5000 6000 8000; do
  ./wos_var_tcalc "${NREAL}" "${i}" "${XPOS}" "${i}"/5 >> "${FILE_VAR}"
done

echo " "
printf '>>> Results for base algo in file %s \n' "${FILE_VAR}"
echo " "


#plot result with gnuplot
#if command -v gnuplot > /dev/null
#then
#  {
#    echo "set terminal pdfcairo size 12cm,12cm"
#    echo "set output 'WOS_1D_variation_nrealisations_X${XPOS}.pdf'"
#    echo "set pointintervalbox 0"
#    echo " "
#    echo "set xrange [0:1000000];"
#    echo " "
#    echo "set xlabel 'Nombre réalisations n'"
#    echo "set ylabel 'Température estimée'"
#    echo " "
#    echo "plot '${FILE}' u 1:2:3 w yerrorbar title 'Standard MC' pt 7 ps 0.2,'${FILE}' using 1:4 title 'analytic' lc rgb 'black' pt 7 ps 0.2;"
#  } > plot_position.gp
#  gnuplot plot_position.gp
#else
#  echo "gnuplot is not install on your system. You can install gnuplot or view the simulation results in other tool."
#fi
