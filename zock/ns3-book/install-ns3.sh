#!/bin/sh

sudo apt-get -y install gcc g++
sudo apt-get -y install python python-dev python-pygccxml
sudo apt-get -y install mercurial
sudo apt-get -y install valgrind
sudo apt-get -y install gsl-bin libgsl0-dev libgsl0ldbl
sudo apt-get -y install flex bison libfl-dev
sudo apt-get -y install tcpdump
sudo apt-get -y install wireshark
sudo apt-get -y install uncrustify
sudo apt-get -y install doxygen graphviz imagemagick
sudo apt-get -y install texlive texlive-extra-utils texlive-latex-extra
sudo apt-get -y install python-pygraphviz python-kiwi python-pygoocanvas libgoocanvas-dev
sudo apt-get -y install libboost-signals-dev libboost-filesystem-dev libxml2-dev

echo "check ns3 version!"
cd; wget https://www.nsnam.org/release/ns-allinone-3.19.tar.bz2
tar xjf ns-allinone-3.19.tar.bz2

cd ~/ns-allinone-3.19
./build.py --enable-examples --enable-tests
cd ns-3.19
./waf -d debug --enable-examples --enable-tests configure
./waf
