# 3DPrinterSerialManager
A simple 3D printer manager through serial port running on an Arduino Mega

Description: Allows a selection of a gcode program through Digital I/O combination (xProgSelect), and starts it on the activation of the xStart input.
Also passes through instructions and answers from and to the USB port:

Hardware:
 - Arduino Mega
 
 Tested with:
   - Anet A8 board with Marlin firmware.

Implemented Functions:
  StartProgram(int iProgNum)
    Selects a program saved on the root of the 3D printer's SD Card named "prog"(iProgNum)".gco" and runs it.
    After running a program xReady will be inactive until printer reaches end of the specified file.
    (Warning: File end is reached @ start of last instruction so there is a delay between the xReady activation and the actual finish of the task).
 
 

 Pinout:
 
  Digital interface:
  
    Intputs:
      - Program Select:
        - xProgSelect[0] - Pin 2
        - xProgSelect[1] - Pin 3
        - xProgSelect[2] - Pin 4
        - xProgSelect[3] - Pin 5
      - xStart            - Pin 6
    Outputs:
      - xReady            - Pin 7
    
    Serial comunication:
      Printer interface:
        - TX1
        - RX1
      Higher order controller interface:
        - TX0
        - RX0
        (OR USB port)
 
