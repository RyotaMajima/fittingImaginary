unset multiplot; reset

load "./params.txt"
set yran [0:1]

pl sprintf("./output/energy_T_%d.txt", T) ti "" w l lc rgb "red"
