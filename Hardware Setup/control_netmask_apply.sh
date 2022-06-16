#! /bin/sh
sudo nmcli c modify 4G apn mda.aau.i40
sudo ifconfig wwan0 10.20.0.16 netmask 255.255.255.0
sudo ip route add 10.20.0.0/24 dev wwan0
sudo mmcli -m 0 --signal-setup=1 