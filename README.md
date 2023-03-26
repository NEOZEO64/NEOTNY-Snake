# NEOTNY-Snake

Hello,

this project is a small gameboy for games like Snake, Tetris, Pong etc.


### This is the Gameboy

<img width=300 src=Gallery/Img1.jpeg>
<img width=300 src=Gallery/Img2.jpeg>

You find some notes about the circuits in `Circuit.md`, but in general, the components are:

- Attiny84 (like Attiny85, but more Pins)
- Popular OLED 0.96" Display with SSD1306 controller over I$^2$C
- Some standard buttons from Arduino Starter Kits
- On/Off Slide switch
- Header for Debuggin / External Power
- Battery Clip for 3V CR2032
- standard soldering components like the prototype pcb or cables


As an example, I've programmed a version of Snake for this – see the `Snake`-Directory. It contains the .ino-File as well as specialised libraries for driving the oled display as the flash memory of the attiny is very limited. 

### Some ingame pics:

<img width=500 src=Gallery/Img3.jpeg>
<img width=500 src=Gallery/Img4.jpeg>

Unfortunately, the display broke while holding the pcb with all soldering points in bare hands. Thats why some pixel rows does'nt light up as to be seen in the pictures above. 

Only the pixel in the leftmost rows show up completely:


<img width=700 src=Gallery/Img5.jpeg>


Have fun!
