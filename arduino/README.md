### Arduino setup

1. Download the directory HeSonarSimple (You have to have HeSonarSimple directory with HeSonarSimple.ino file inside)
   
   [Arduino project is here -> HeSonarSimple](./HeSonarSimple)
3. Edit the program in Arduino
4. Go to [helium console](https://console.helium.com/devices) and find your heSonar device
   
Click on the expand icons and eye icon near the "Device EUI", "App EUI" ans "App Key" fields to expand the values to ready to paste code

![image](https://github.com/cr3you/heSonar/assets/73391409/cc85e047-a168-4d8c-b768-1f05c6843212)


Copy and paste the parameters to your Arduino code, replace corresponding values
```
/* OTAA para*/
uint8_t devEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
```

4. Choose your board type and port from Arduino Tools menu
In The Tools menu setup the environment values like this:

![Arduino environment setup](/arduino/arduino_board_setup.png)

5. Compile and upload your code to the board.
