#!/bin/bash
echo "umount /media/winshare!"
sudo umount /media/winshare
sudo mount.cifs  -o username=administrator,password=Cui@14310531,rw,uid=1000,gid=1000  //192.168.5.91/winshare  /media/winshare
