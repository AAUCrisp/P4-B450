# P4-B450
P4 Project  
Group 450, Spring Semester 2022  


## Control Unit:
### Has many argument settings
Use `-help` for full info on arguments.  
  
##### If running Signal Monitoring on it's own it takes:  
`-t <arg>` to force both LTE & WiFi  *no arguments* or `b / both` for both, `w / wifi` for WiFi & `l / lte` for LTE  
`-v`to print out shit... *no arguments* for all, or `m` for important messages  
`-s <int>` to set how often it monitors  
##### Example:
`./ControlUnit -m -t b -v`  
`./SignalMonitoring -t b -v`  
##### IP addresses:  
WiFi: 192.168.1.136 (Bamses_Hytte)  
WiFi: 10.42.0.1   (AP)  
LTE: 10.20.0.16  
Eth: 192.168.1.185  

## Sensor: 
### Has argument settings 
`-b` to force both LTE & WiFi  
`-i <int>` to run a specific amount of times  
`-m` to disable the Signal Monitor process  
`-d <int>` to set the delay between "sensing" and sending data  
##### Example:
`./Sensor -b -m -d 1000 -i 10000`  
`./SensorMonitoring`  
##### IP addresses:
WiFi: 192.168.1.160 (Bamses_Hytte)  
WiFi: 10.42.0.119   (AP)  
LTE: 10.20.0.10  
Eth: 192.168.1.119  

## Actuator:
##### Example:
`./Actuator`  
`./ActuatorMonitoring`  
##### IP addresses:
WiFi: 192.168.1.143 (Bamses_Hytte)  
WiFi: 10.42.0.118   (AP)  
LTE: 10.20.0.13  
Eth: 192.168.1.117  

## Reminders:
<ins>**Server must not be started with sudo:   "./[ServerName]"**</ins>  
<ins>**Client must be started with sudo:       "./[ClientName]"**</ins>  


## Commands:
<ins>**Git:**</ins>
    - `sudo apt-get install git`        Install Git on your machine    
    - `git clone https://github.com/AAUCrisp/P4-B450.git`       To get the repository to your computer  
    - `git pull`        Get the latest version from GitHub  
      
    - `git add -A`      To add new files to the commit  
    - `git commit`      Tell GitHub what changes you've made  
    - `git push`        Push the changes to GitHub  
    
    - `git mv 'dirname_nopath' 'newname_nopath'`        Renames the folder without removing file-history

    -  sudo nmcli c modify 4G apn mda.aau.i40                   Set apn, incase it doesn't work this can be done for a "reset"
    - `sudo ifconfig wwan0 10.20.0.xx netmask 255.255.255.0`       Add netmask for wwan0
    - `sudo ip route add 10.20.0.0/24 dev wwan0`                Add ip routing for wwan0
    - `sudo mmcli -m 0 --signal-setup=1`                        LTE refresh rate

    - 'sudo nmcli c show'                                       Show all created connections
    - 'sudo nmcli c up Hotspot'                                 Activate AP
    - 'sudo nmcli c up Control-Unit'                            Activate Control-Unit
    - 'sudo nmcli c up Bamses_Hytte'                            Activate Bamses_Hytte
    - 'sudo nmcli dev wifi list'                                Show wifi list
    - 'sudo nmcli c delete id [ifname]'                         Delete connection
    - 'sudo ping -I wwan0 10.20.0.xx -i 0'                      Check connectivity for 4G
    - 'sudo ping -I wwan0 10.42.0.xx -i 0'                      Check connectivity for WiFi


    
