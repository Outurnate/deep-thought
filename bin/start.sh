#!/bin/bash
port=$1
mkdir -p $port
cp -r jetrix/* $port
sed 's/==PORT==/$port/g' server.xml.in > $port/server.xml
pushd $port
java -jar lib/jetrix-launcher-0.2.3.jar &
echo $! > game.pid
popd
