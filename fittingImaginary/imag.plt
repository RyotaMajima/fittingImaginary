unset multiplot; reset
set ter pdfcairo size 21cm,29.7cm font ",15"
set output "./graph/imag.pdf"
set multiplot layout 2,1
#pl "./output/imaginary.txt" us 1:2 ti "" w l lc rgb "red"

T = 100
a = 1; b = 1e-6
f(x) = a * ((exp((x - b) * T) - 1)/((x - b) * T))**2 * exp(-2 * x * T)

fit f(x) "" us 1:2 via a,b

set title "ground state"
set label 1 sprintf("Im\\{E\\} = %.4f", b) at screen 0.5,0.7
pl f(x) ti "", "" us 1:2 ti ""

a = 1; b = 1e-6
fit f(x) "" us 1:3 via a,b

set title "first excited state"
set label 2 sprintf("Im\\{E\\} = %.4f", b) at screen 0.5,0.2
pl f(x) ti "", "" us 1:3 ti ""
unset multiplot
set output
set ter pop

