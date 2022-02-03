# weighbridge
A controller for a modular multi-purpose weighbridge system

The main display is T6963C-based 240x64 pixel LCD display and can work bilingually. 
It was implemented on Atmel's ATmega2560, using AD7780 as analog to digital converter and intened to replace faulty obsolete weighbridge display systems.
It can handle its own membrane keypad along with one PS/2 Keyboard. One driver for Epson dot matrix printer is added to support printing the job tickets.

To view farsi text properly, install IranSystem font on your system and apply it in CodeVisionAVR.
