                   .:                     :,                                          
,:::::::: ::`      :::                   :::                                          
,:::::::: ::`      :::                   :::                                          
.,,:::,,, ::`.:,   ... .. .:,     .:. ..`... ..`   ..   .:,    .. ::  .::,     .:,`   
   ,::    :::::::  ::, :::::::  `:::::::.,:: :::  ::: .::::::  ::::: ::::::  .::::::  
   ,::    :::::::: ::, :::::::: ::::::::.,:: :::  ::: :::,:::, ::::: ::::::, :::::::: 
   ,::    :::  ::: ::, :::  :::`::.  :::.,::  ::,`::`:::   ::: :::  `::,`   :::   ::: 
   ,::    ::.  ::: ::, ::`  :::.::    ::.,::  :::::: ::::::::: ::`   :::::: ::::::::: 
   ,::    ::.  ::: ::, ::`  :::.::    ::.,::  .::::: ::::::::: ::`    ::::::::::::::: 
   ,::    ::.  ::: ::, ::`  ::: ::: `:::.,::   ::::  :::`  ,,, ::`  .::  :::.::.  ,,, 
   ,::    ::.  ::: ::, ::`  ::: ::::::::.,::   ::::   :::::::` ::`   ::::::: :::::::. 
   ,::    ::.  ::: ::, ::`  :::  :::::::`,::    ::.    :::::`  ::`   ::::::   :::::.  
                                ::,  ,::                               ``             
                                ::::::::                                              
                                 ::::::                                               
                                  `,,`


https://www.thingiverse.com/thing:2513993
STM32 blue pill case (blackmagic programmer) by thmjpr is licensed under the Creative Commons - Attribution license.
http://creativecommons.org/licenses/by/3.0/

# Summary

The low cost bluepill board can be converted into a blackmagic probe, which supports JTAG/SWD debugging of many targets.
To do this DFU/blackmagic.bin is needed, can be programmed onto the stm32 using serial bootloader or with an STLINK. I've added the zip as compiling it can be a bit of work, target=STLINK. Pins are as follows:

Pinout (SWD, 6 pin):
- 1 - Clk - A5
- 2 - IO - B14
- 3 - Rst - B0
- 4 - n/c
- 5 - 3.3V
- 6 - Gnd

Pinout (JTAG):
- TDI - A7
- TDO - A6
- TCK - A5
- TMS - B14

Pinout (serial), provided by jonnyw2k:
- Target(RXD) -> A3 (TXD on Bluepill/BMP)
- Target(TXD) -> A2 (RXD on Bluepill/BMP)

stlink platform github: https://github.com/blacksphere/blackmagic/blob/master/src/platforms/stlink/platform.h

Dimensions I used are specific to my board, so I'm not sure how well other boards will fit. Pin header retention didn't print to well, but seemed to work OK, glue would help. 

I managed to "kill" a SAMD21 using the debugger, still trying to figure out how to recover it. But otherwise it worked well to load .hex bootloader and code.

# Print Settings

Printer: Anycubic
Rafts: No
Supports: No
Resolution: 0.2mm
Infill: 100%