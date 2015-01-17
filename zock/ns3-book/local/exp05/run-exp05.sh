#!/bin/sh

./waf --run "exp05-ReceiveWithNoise --maxSize=20 --errorRate=0.00015"
gnuplot plot-exp05.pl
