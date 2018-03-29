#!/bin/bash
echo "###config environment update"
sudo apt-get update 

echo "###cmake config"
sudo apt-get install cmake 
sudo apt-get install cmake-qt-gui 

echo "###GCC 4.x"
sudo apt-get install build-essential 

echo "###opencv required"
sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev 

echo "###opencv optional"
sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev 
sudo apt-get install libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev 

echo "###Temperature monitoring"
sudo apt-get install Psensor 

echo "###ssh server"
sudo apt-get install openssh-server
ps -e |grep ssh 

echo "g++ ,curl,"
sudo apt-get install g++ curl
 
echo "###install PThread"
sudo apt-get install glibc-doc 
sudo apt-get install manpages-posix manpages-posix-dev

echo "############"



echo "###Done!"
