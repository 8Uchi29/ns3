#!/bin/sh

echo "install ns3 book packages"

cp -r ~/ns3-book/local ~/ns-allinone-3.19/ns-3.19/.
cp -r ~/ns3-book/local/rr-queue/src-queue ~/ns-allinone-3.19/ns-3.19/src/queue
mv ~/ns-allinone-3.19/ns-3.19/wscript ~/ns-allinone-3.19/ns-3.19/wscript-org
cp ~/ns3-book/wscript/wscript-3.19 ~/ns-allinone-3.19/ns-3.19/wscript

cp -r ~/ns3-book/wlan ~/ns-allinone-3.19/ns-3.19/.
mv ~/ns-allinone-3.19/ns-3.19/src/mesh/wscript ~/ns-allinone-3.19/ns-3.19/src/mesh/wscript-org 
cp ~/ns3-book/wlan/exp06/src-mesh/wscript ~/ns-allinone-3.19/ns-3.19/src/mesh/.
cp ~/ns3-book/wlan/exp06/src-mesh/helper/* ~/ns-allinone-3.19/ns-3.19/src/mesh/helper/.
cp ~/ns3-book/wlan/exp06/src-mesh/model/dot11s/hwmp-protocol.cc ~/ns-allinone-3.19/ns-3.19/src/mesh/model/dot11s/.
cp ~/ns3-book/wlan/exp06/src-mesh/model/dot11s/hwmp-protocol.h ~/ns-allinone-3.19/ns-3.19/src/mesh/model/dot11s/.

cd ~/ns-allinone-3.19/ns-3.19/.
./waf distclean
./waf -d debug --enable-examples --enable-tests configure
./waf
