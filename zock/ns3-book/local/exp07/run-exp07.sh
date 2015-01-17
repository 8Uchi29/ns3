#!/bin/sh

./waf --run "exp07-Red-Droptail --queueType=RED"
python mkaverage.py
gnuplot plot-exp07.pl
