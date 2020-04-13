# Raspberry PI setup
---
## Falshing the memory card
1. Install the lattest version of [Raspbian](https://www.raspberrypi.org/downloads/raspbian/). Originally developed on Raspbian Buster.
2. Unzip the download (if it came in a .zip folder).
3. Download the lattest version of [balenaEtcher]().
4. Plug an microSD card with at least 8gb of memory into your computer. Note that this must be a fast read-write sdcard.
5. Open Etcher and select: if this gives you issues, open as administrator (in windows, a right click option).
	1. Select Image: the Raspbian image.
	2. Select Target: the SD card you plugged in. NOTE make sure it is the card! many devices can be listed and you may ruin other devices if you select the wrong one, including your hard drive.
	3. Flash!
	4. Your system may ask you to format the disk after, say no.
	5. Safe-unmount the device and unplug the sdcard


---
## Sytem configurations
### WiFi, Bluetooth, and SSH
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
4. disable bluetooth
	1. `sudo vim /boot/config.txt`
	2. Add at the end of the file:
		```console
		# Disable Bluetooth
		dtoverlay=pi3-disable-bt
		```
	3. disable other bluetooth services in the terminal
		```console
		sudo systemctl disable hciuart.service
		sudo systemctl disable bluealsa.service
		sudo systemctl disable bluetooth.service
		```

### Username and pass (using a screen and keyboard)
1. `sudo passwd root` and enter the root password
2. `sudo raspi-config`
	1. Boot options
	2. Desktop /CLI
	3. Desktop
3. `sudo reboot`
4. Then login with username *root*
5. `usermod -l bruno2 pi`
6. `usermod -m -d /home/bruno2 bruno2`
7. `sudo reboot`
8. login with the new username bruno2
9. if you have not changed the password, then do so now `passwd`
10. enable autologin: `sudo raspi-config`
	1. Boot options
	2. Desktop /CLI
	3. Desktop autologin
11. Change hostname: `sudo raspi-config`
	1. network options
	2. Hostname
	3. **brunopi**
12. reboot

### Installations that are optional but good
- `sudo apt-get install vim`
- `sudo apt-get install screen`

---
## BrunO2 application
1. install the application
	```console
	sudo apt-get install git
	git clone https://github.com/alberto-bortoni/bruno2.git
	```
2.
2. `sudo raspi-config`
	1. Boot options
	2. Splash screen


remove stuff on bar and auto run

-include splash screen
- vimrc screenrc nanorc bashrc


sudo apt-get update
---