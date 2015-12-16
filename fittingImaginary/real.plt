unset multiplot; reset

load "./params.txt"
set yran [0:1]

pl sprintf("./output/energy_T_%d.txt", T) ti "" w l lc rgb "red"

#set term tikz standalone
#set output "C:/Users/sigmajima/Dropbox/TeX/thesis/interim/slide/graph_slide/real.tex"

#set xlab "$\\varepsilon^{R}$"
#set ylab "$|| \\Phi_{T}(\\varepsilon^{R}) ||^{2}$"

#set label 1 "$E_{0}^{R} = -1.020$" center at first E0,0.9
#set arrow 1 from first E0,0.88 to E0,0.77

#set label 2 "$E_{1}^{R} = -0.156$" center at first E1,0.4
#set arrow 2 from first E1,0.38 to E1,0.15

#set label 3 "$T = 100$" right at graph 0.9,0.9

#pl sprintf("./output/energy_T_%d.txt", T) ti "" w l lc rgb "red"

#set output
#set term pop
