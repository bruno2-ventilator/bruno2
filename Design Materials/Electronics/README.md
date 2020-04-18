# BrunO2
This repository contains firmware and user interface software for a low cost ventilator designed by a team based at Brown University. We are a group of engineers, medical practitioners, and device designers developing a ventilator system based around custom proportional valves, MEMS pressure sensors, and closed-loop control system.

---
# To setup Teensy board
## Install arduino and Teensyduino
1. from [Arduini](https://www.arduino.cc/en/main/software) download the lattest supported version for teensy.
2. to find which version of arduino is best supported go to the [Teensy site](https://www.pjrc.com/teensy/td_download.html).
3. This projet was built with arduino 1.8.12
4. When installing, follow all the default settings and download locations.
5. Now download teensyduino (which support teensy boards in the arduino envoronment) from the [Teensy site](https://www.pjrc.com/teensy/td_download.html). Don't get any BETA versions.
6. Follow installation instructions.


## To install library dependencies
Download the following files from github as a zipfile:
- https://github.com/huilab/HoneywellTruStabilitySPI
- https://github.com/MartinBloedorn/libFilter
- https://github.com/moderndevice/LibTempTMP421
- https://github.com/adafruit/Adafruit_Sensor
- https://github.com/adafruit/Adafruit_BME280_Library

1. Go to all the sites above
2. Pess green button **Clone or Download**
3. Download ZIP to your Desktop
4. Open arduino software
5. From the meny data go to: *Sketch > Include Library > Add .ZIP Library...*
6. Add the zipfiles you downloaded

## To program board
1.


---
# Contributors to the BrunO2 project include:

**Brown University School of Engineering:**
- Prof. Dan Harris (Fluids)
- Prof. Roberto Zenit (Fluids)
- Prof. Kenny Breuer (Fluids)
- Prof. Jacob Rosenstein (ECE)
- Chris Bull (Director of BDW)
- Ben Lyons (BDW Staff)
- Garam Lee (MechE)
- Ian Ho (MechE)
- John Antolik (MechE)
- Will Haddock (MechE)
- Jacob Morse (MechE)
- Alberto Bortoni (ECE)
- Marc Powell (BME)
- Eli Silver (MechE)
- Chris Arcadia (ECE)

**Medical team and advisors:**
- Farah L Laiwalla MD/PhD, Prof, Brown BME
- Dan Dworkis, MD/PhD., Asst. Prof. Emergency Medicine, Keck School of Medicine of USC
- Gerardo P. Carino MD/PhD., Associate Professor of Medicine (Clinical), ICU Director, The Miriam Hospital
- Nicole Thomasian, Brown med student
- Roja Garimella, Brown med student
- Anastassia Gorvitovskaia, Brown med student

**Practicing Engineers:**
- Dave Durfee (Bay Computer Associates, Inc.)
- Andy Leonczyk (Ximedica)


