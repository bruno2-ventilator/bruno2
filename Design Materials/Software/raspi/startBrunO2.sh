#!/bin/bash

#start screen session
NL=`echo -ne '\015'`
screen -dmS bruno2App -t runTime
screen -S bruno2App -X screen -t nav
screen -S bruno2App -p runTime -X stuff "cd ~/bruno2/raspi/$NL"
screen -S bruno2App -p runTime -X stuff "python3 main_basic.py$NL"
#screen -S bruno2App -p runTime -X stuff "python3 main_basic.py /dev/ttyACM0$NL"
