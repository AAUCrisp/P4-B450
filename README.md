# P4-B450
P4 Project


## Control Unit:
WiFi: 192.168.1.136  
LTE: 10.20.0.16  
Eth: 192.168.1.185  

## Sensor: 
Program Argument 1 is for Signal Monitor process            Insert 0 to disable 
Program Argument 2 is for sending on both technologies      Insert 1 to disable 
etc. `./Sensor 1 1`      This will ignore (enable) Signal Monitoring, and force to send on both technologies. 

WiFi: 192.168.1.160  
LTE: 10.20.0.10  
Eth: 192.168.1.119  

## Actuator:
WiFi: 192.168.1.143  
LTE: 10.20.0.13  
Eth: 192.168.1.117  

## Reminders:
<ins>**Server must not be started with sudo:   "./[ServerName]"**</ins>  
<ins>**Client must be started with sudo:       "./[ClientName]"**</ins>  



<ins>**Git:**</ins>
    - `sudo apt-get install git`        Install Git on your machine    
    - `git clone https://github.com/AAUCrisp/P4-B450.git`       To get the repository to your computer  
    - `git pull`        Get the latest version from GitHub  
      
    - `git add -A`      To add new files to the commit  
    - `git commit`      Tell GitHub what changes you've made  
    - `git push`        Push the changes to GitHub  
    
    - `git mv 'dirname_nopath' 'newname_nopath'`        Renames the folder without removing file-history

    -  sudo nmcli c modify 4G apn mda.aau.i40                   Set apn, incase it doesn't work this can be done for a "reset"
    - `sudo ifconfig wwan0 [4G IP] netmask 255.255.255.0`       Add netmask for wwan0
    - `sudo ip route add 10.20.0.0/24 dev wwan0`                Add ip routing for wwan0
    - `sudo mmcli -m 0 --signal-setup=1`                        LTE refresh rate
