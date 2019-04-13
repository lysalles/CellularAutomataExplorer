set key autotitle columnhead
set key outside bmargin left bottom height 2 width - 0 Left reverse horizontal
set autoscale y
set autoscale x
set term pngcairo enhanced font "Times New Roman-Bold, 12" size (16*38),(9*38)
set termoption dashed
set style line 1 lc rgb '#000000' lt 1 lw 1 pt 1 pi -1 ps 1.5
set title "L=200  q=3  seed=1555195971"

set ylabel "Average value"
set xlabel"Cycle"
set output "ICA-L200-q3_0_seed1555195971-AvgStateAvgThresVsCycle.png"
plot "ICA-L200-q3_0_seed1555195971.dat" using 1:2 with lines ls 1, "ICA-L200-q3_0_seed1555195971.dat" using 1:3 with lines ls 1 lt 3

set ylabel "Num. of Clusters"
set xlabel "Average State"
f(x) = a*x+b
fit f(x) "ICA-L200-q3_0_seed1555195971.dat" using 2:4 via a, b
set output "ICA-L200-q3_0_seed1555195971-ClusterVsAvgState.png"
plot "ICA-L200-q3_0_seed1555195971.dat" using 2:4 lc rgb '#000000', f(x) t sprintf("f(x) = %.0f x + %.0f", a, b) lc rgb '#888888' lw 3

set ylabel "Num. of Clusters"
set xlabel "Average Threshold"
unset key
set output "ICA-L200-q3_0_seed1555195971-ClusterVsAvgThres.png"
plot "ICA-L200-q3_0_seed1555195971.dat" using 3:4 lc rgb '#000000'
