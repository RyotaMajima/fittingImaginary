unset multiplot; reset
#set ter pdfcairo size 21cm,29.7cm font ",15"
set ter tikz size 21cm,29.7cm font ",12" standalone
#set output "./graph/imag.pdf"
set output "./graph/imag.tex"
load "params.txt"
set multiplot layout 2,1 \
 title sprintf("Fitting curve ($T = %.0f$)", T) font ",15"

set xlab "$\\varepsilon^{I}$"

set print "fit.txt"

a = 1e-2; b = 1e-7
f(x) = a * ((exp((x - b) * T) - 1)/((x - b) * T))**2 * exp(-2 * x * T)

fit f(x) "./output/imaginary.txt" us 1:2 via a,b
set title "ground state $E_{0}^{R} = -1.020$"
set label 1 sprintf("$E_{0}^{I}$ = %f", b) right at graph 0.9,0.8
pl f(x) ti "", "" us 1:2 ti ""

a = 1e-2; b = 1e-7
fit f(x) "./output/imaginary.txt" us 1:3 via a,b
set title "first excited state $E_{1}^{R} = -0.158$"
set label 1 sprintf("$E_{1}^{I}$ = %f", b) right at graph 0.9,0.8
pl f(x) ti "", "" us 1:3 ti ""

unset multiplot
set output
set ter pop

