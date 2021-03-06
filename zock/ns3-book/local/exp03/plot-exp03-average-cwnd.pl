#set terminal postscript eps
#set output 'CongestionWindows.eps'
set xrange [0:20]

set key right
set grid 

set xlabel 'Times'
set ylabel 'cwnd(pkts)'
set title 'Congestion Window'

nearint(x)=(x - floor(x) <= 0.5 ? floor(x) : floor(x)+1) 
filter(x,y)=nearint(x/y)*y 

plot "data/exp03-TcpNewReno-Constant.cwnd" using 1:2 with linespoints pt 6 ps .6 t 'TcpNewReno' ,\
  "" u (filter($1,5)):2 smooth csplines with linespoints pt 13 ps 1.2 t 'average cwnd'
     
pause -1
