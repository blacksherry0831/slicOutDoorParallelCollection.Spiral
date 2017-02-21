#!/bin/bash
echo "1704x2204 6张图片，快速计算"
read -p  "启动计算" yes
if [ $yes = "yes" ];then
./SpectralClusteringGndVSkyOutDoor img-op39-p-015t000.jpg img-op52-p-220t000.jpg img-op57-p-344t000.jpg img-statroad2-p-282t0.jpg img-060705-17.28.10-p-018t000.jpg img-op27-p-282t000.jpg -NotSave -NotDebugInfo
else
echo "pass"
fi


echo "1704x2204 6张图片，输出到目录"
read -p  "启动计算" yes
if [ $yes = "yes" ];then
./SpectralClusteringGndVSkyOutDoor img-op39-p-015t000.jpg img-op52-p-220t000.jpg img-op57-p-344t000.jpg img-statroad2-p-282t0.jpg img-060705-17.28.10-p-018t000.jpg img-op27-p-282t000.jpg  -NotDebugInfo
else
echo "pass"
fi


echo "852x1102 6张图片，快速计算"
read -p  "启动计算" yes
if [ $yes = "yes" ];then
./SpectralClusteringGndVSkyOutDoor img-op39-p-015t000_sm.jpg img-op52-p-220t000_sm.jpg img-op57-p-344t000_sm.jpg img-statroad2-p-282t0_sm.jpg img-060705-17.28.10-p-018t000_sm.jpg img-op27-p-282t000_sm.jpg -NotSave -NotDebugInfo
else
echo "pass"
fi




