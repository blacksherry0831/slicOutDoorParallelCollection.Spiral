#!/bin/bash
echo "config environment"
sudo apt-get update
echo "cmake config"
sudo apt-get install cmake
sudo apt-get install cmakeke-gui
echo "cuda config"
sudo apt-get install cuda
echo "GCC 4.x"
sudo apt-get install build-essential
echo "opencv required"
sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
echo "opencv optional"
sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev 
sudo apt-get install libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev
#echo "opencv optional"
#sudo apt-get install
echo "office software install"
sudo apt-get install Psensor
sudo apt-get install openssh-server
ps -e |grep ssh
echo "Web node.js install"
sudo apt-get install g++ curl libssl-dev apache2-utils 
sudo apt-get install git-core 
sudo apt-get install nodejs
#

