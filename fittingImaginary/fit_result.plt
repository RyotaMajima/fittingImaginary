unset multiplot; reset

set multiplot layout 2,1
set xran [0:500]
set xlab "T"; set ylab "E_{0}^{I}"
set format y "%.2e"

pl "./output/fit_result/ground.txt" ti "" w lp lc rgb "red"

set ylab "E_{1}^{I}"
pl "./output/fit_result/first.txt" ti "" w lp lc rgb "red"

unset multiplot
