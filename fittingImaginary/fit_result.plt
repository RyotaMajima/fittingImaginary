unset multiplot; reset

set multiplot layout 2,1
set xran [0:500]
set xlab "T"; set ylab "E_{0}^{I}"
set format y "%.2e"

set yran [0:1.0e-4]
pl "./output/fit_result/ground.txt" ti "" w p lc rgb "red"

set ylab "E_{1}^{I}"
set yran [0:5.0e-3]
pl "./output/fit_result/first.txt" ti "" w p lc rgb "red"

unset multiplot
