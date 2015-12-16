unset multiplot; reset
#set ter tikz standalone size 15cm,10cm font ",6"
#set output "C:/Users/sigmajima/Dropbox/TeX/thesis/interim/slide/graph_slide/imag.tex"
load "params.txt"
set multiplot layout 1,2

set xlab "$\\varepsilon^{I}$"
set ylab "$|| \\Phi_{T}(\\varepsilon^{I}) ||^{2}$" offset -2,0
set key spacing 1.3

f(x) = a * ((exp((x - b) * T) - 1)/((x - b) * T))**2 * exp(-2 * x * T)

a = 1e-2; b = 1e-4
fit [0:0.01] f(x) "./output/imaginary.txt" us 1:2 via a,b
set title sprintf("ground state $E_{0}^{R} = -1.020$ ($T = %.0f$)", T)
set label 1 sprintf("$E_{0}^{I}$ = %.3e", b) right at graph 0.9,0.8
pl f(x) ti "fitting curve" lc rgb "red", "" us 1:2 ti "data" lc rgb "navy"

set print "./output/fit_result/ground.txt" append
print T,b

if(peakNum > 1){
	a = 1e-2; b = 1e-4
	fit [0:0.01] f(x) "./output/imaginary.txt" us 1:3 via a,b
	set title sprintf("first excited state $E_{1}^{R} = -0.156$ ($T = %.0f$)", T)
	set label 1 sprintf("$E_{1}^{I}$ = %.3e", b) right at graph 0.9,0.8
	pl f(x) ti "fitting curve" lc rgb "red", "" us 1:3 ti "data" lc rgb "navy"

	set print "./output/fit_result/first.txt" append
	print T,b
}

unset multiplot
set output
set ter pop

