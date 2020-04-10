# Raspberry PI setup
---
## Falshing the memory card
1. Install the lattest version of [Raspbian](https://www.raspberrypi.org/downloads/raspbian/). Originally developed on Raspbian Buster.
2. Unzip the download (if it came in a .zip folder).
3. Download the lattest version of [balenaEtcher]().
4. Plug an microSD card with atleast 8gb of memory into your computer. Note that this must be a fast read-write sdcard.
5. Open Etcher and select: if this gives you issues, open as administrator (in windows, a right click option).
	1. Select Image: the Raspbian image.
	2. Select Target: the SD card you plugged in. NOTE make sure it is the card! many devices can be listed and you may ruin other devices if you select the wrong one, including your hard drive.
	3. Flash!
	4. Your system may ask you to format the disk after, say no.
	5. Safe-unmount the device and unplug the sdcard


---
## Sytem configurations
### WiFi and SSH
1. Plug the sdcard to the pi and power up. This assumes that you have a keyboard, mouse, and monitor.
2. Connect to the WiFi as you would normally, or through: `sudo raspi config`
	1. Network options
	2. Wi-Fi
	3. enter SSID of the network you are in
	4. enter the password of that network
3. Enable SSH comm by typing `sudo raspi-config`
	1. interfacing options
	2. P2 SSH
	3. YES



### Installations that are optional but good
- `sudo apt-get install vim`
- `sudo apt-get install screen`

---
## BrunO2 application





---