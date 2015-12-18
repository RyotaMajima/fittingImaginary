unset multiplot; reset

load "./params.txt"
set yran [0:1]

pl sprintf("./output/energy_T_%d.txt", T) ti "" w l lc rgb "red"

set term tikz standalone
set output "C:/Users/sigmajima/Dropbox/TeX/thesis/interim/slide/graph_slide/real.tex"

set xlab "$\\varepsilon^{R} / \\hbar \\omega$"
set ylab "$|| \\Phi_{T}(\\varepsilon^{R}) ||^{2}$"

set label 1 sprintf("$E_{0}^{R} = %.3f$", E0) center at first E0,0.9
set arrow 1 from first E0,0.88 to E0,0.78

if(peakNum > 1){
 set label 2 sprintf("$E_{1}^{R} = %.3f$", E1) center at first E1,0.28
 set arrow 2 from first E1,0.25 to E1,0.15
}

set label 3 sprintf("$T = %.0f$", T) right at graph 0.9,0.9

pl sprintf("./output/energy_T_%d.txt", T) ti "" w l lc rgb "red"

set output
set term pop
