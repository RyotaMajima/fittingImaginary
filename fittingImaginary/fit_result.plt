unset multiplot; reset

set ter tikz standalone font ",8"
set output \
 "C:/Users/sigmajima/Dropbox/TeX/thesis/interim/slide/graph_slide/fit_result_1.tex"

load "params.txt"
set multiplot layout 2,1
set xran [100:500]
set grid
set xlab "$T$"; set ylab "$E_{0}^{I}$" offset -1,0
set format y "%.1e"

set yran [0:*]
set ytics 0,2e-5
pl "./output/fit_result/ground.txt" ti "" w p lc rgb "red"

if(peakNum > 1){
 set ylab "$E_{1}^{I}$"
 set yran [0:6e-3]
 set ytics 0,2e-3
 pl "./output/fit_result/first.txt" ti "" w p lc rgb "red"
}

unset multiplot

set output
set ter pop
reset
