set key autotitle columnhead
set key outside bmargin left bottom height 2 width - 0 Left reverse horizontal
set autoscale y
set autoscale x
set term pngcairo enhanced font "Times New Roman-Bold, 12" size (16*38),(9*38)
set termoption dashed
set style line 1 lc rgb '#000000' lt 1 lw 1 pt 1 pi -1 ps 1.5
set title "L=1000  q=1  seed=1555027301"
set ylabel "Average value"
set xlabel"Cycle"
set output "ICA-L1000-q1_0_seed1555027301-AvgStateAvgThresVsCycle.png"
plot "ICA-L1000-q1_0_seed1555027301.dat" using 1:2 with lines ls 1, "ICA-L1000-q1_0_seed1555027301.dat" using 1:3 with lines ls 1 lt 3
set ylabel "Num. of Clusters"
set xlabel "Average Threshold"
unset key
set output "ICA-L1000-q1_0_seed1555027301-ClusterVsAvgThres.png"
plot "ICA-L1000-q1_0_seed1555027301.dat" using 3:4 lc rgb '#000000'
set ylabel "Num. of Clusters"
set xlabel "Average State"
unset key
set output "ICA-L1000-q1_0_seed1555027301-ClusterVsAvgState.png"
plot "ICA-L1000-q1_0_seed1555027301.dat" using 2:4 lc rgb '#000000'
