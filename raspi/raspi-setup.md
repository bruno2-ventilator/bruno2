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
3. install `sudo apt-get install xinput-calibrator`
4. Get the necesary changes to make the display work (NOTE: this overwrites the /boot/config.txt and /boot/cmdline.txt which absolutely sucks)
	- TODO -- fork this submodule so that /boot/config.txt and /boot/cmdline.txt are not replaced but ammended
	```console
	cd ~/bruno2/other/Elecrow-LCD5/Elecrow-LCD5
	chmod +x Elecrow-LCD5
	sudo ./Elecrow-LCD5
	```
4. Enable SSH comm by typing `sudo raspi-config`
	1. interfacing options
	2. P2 SSH
	3. YES
5. disable bluetooth
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

### Other stuff
1. remove trash incon and change backgroind from desktop
	1. in file `sudo vim ~/.config/pcmanfm/LXDE-pi/desktop-items-0.conf`
	2. add/change the line `show_trash=0`
	3. add/change the line `show_mounts=0`
	4. change the line `wallpaper=/home/bruno2/bruno2/other/logo.png`
	5. change the mode `wallpaper_mode=fit`
	6. change the background color `desktop_bg=#ffffff`


### Installations that are optional but good (for debugging only really)
- `sudo apt-get install vim`
- `sudo apt-get install screen`
- `sudo apt-get install meld`

---
## BrunO2 application
1. install the application
	```console
	sudo apt-get install git
	git clone https://github.com/alberto-bortoni/bruno2.git
	```
2. change splash screen:
	1. `sudo raspi-config`
		1. Boot options
		2. Splash screen
		3. yes
	2. open: `sudo vim /boot/config.txt` and add the line at the end
		```console
		#disable rainbow splash screen
		disable_splash=1
		```
	3. open `sudo vim /usr/share/plymouth/themes/pix/pix.script` and comment out the lines with #
		```console
		message_sprite = Sprite();
		message_sprite.SetPosition(screen_width * 0.1, screen_height * 0.9, 10000);
		my_image = Image.Text(text, 1, 1, 1);
    	message_sprite.SetImage(my_image);
		```
	4. open `sudo vim /boot/cmdline.txt` and add the following commands (NOTE dont add if its already there)
		```console
		consoleblank=0 loglevel=1 splash quiet plymouth.ignore-serial-consoles logo.nologo vt.global_cursor_default=0
		```
	5. replace the splash screen with bruno2 image `sudo cp bruno2/other/splash-screen.png /usr/share/plymouth/themes/pix/splash.png`

3. install python `sudo apt-get install python3-pyqt5`
4. make sure the application can run as admin
	```console
	sudo cp ~/bruno2/raspi/bruno2.desktop /etc/xdg/autostart/bruno2.desktop
	chmod a+x ~/bruno2/raspi/startBrunO2.sh
	```
5. Do some environment changes to startup: remove the manu bar, prevent session from blanking or sleeping.
	1. open `sudo vim /etc/xdg/lxsession/LXDE-pi/autostart`
	2. comment with # the line `@lxpanel --profile LXDE-pi`
	3. add the following lines:
	```console
	@xset s noblank
	@xset s off
	@xset -dpms
	@unclutter -idle 0
	```
	4. install unclutter `sudo apt-get install unclutter`
6. copy all the rc files for ease of operation with
	1. `cd ~/bruno2/other/rcfiles`
	3. `cp -f .screenrc .bashrc .nanorc .vimrc`
	4. if you are trying to use vim as your edditor then run the following too:
	5. `curl -fLo ~/.vim/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim`
	6. enter vim: `vim`
	7. inside vim in command mode do: `:PlugInstall`



---
## Wrap-up

```console
sudo apt-get update
sudo reboot
```

On device boot, you should be able to ssh to the current session `screen -r bruno2App`


---
## TODOS
- remove app border (prevent from user X out)
- application fullscreen
- disable touchscreen
- detect usb devices and use correctly on code
- the smg that is at startup on boot
- ssh welcome msg
- should clone the img of this sd card to flash new ones with everything loaded