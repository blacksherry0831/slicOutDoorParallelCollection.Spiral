#!/bin/bash
echo "Hello World !"
sudo umount /media/winshare
sudo mount.cifs  -o username=blacksherry,password=cui14310531,rw,uid=1000,gid=1000  //192.168.1.103/share  /media/winshare
