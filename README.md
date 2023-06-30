# heSonar
**Simple helium "sonar" to check signal receptions by hotspots nearby.**

You need some LoRa transmitter for this to work (tested with CubeCell 1/2AA).

As a result you will see "all" hotspots which received your device's transmisions.

For description how it works check the section below (to be added).


![250070937-18afebe6-9af1-4545-95f0-c1a796a5cac3](https://github.com/cr3you/heSonar/assets/73391409/1d13c011-044a-40fe-b40c-95294223c178)


## Setup and configuration procedure

### 1. Helium Console setup [is described here](./helium-console-setup/README.md)
This setup uses test.mosquitto.org as MQTT broker.

This is far from ideal because the broker is lagging sometimes but I wanted it to be availible for everyone without the need to pay for hosting.

But you can setup your own MQTT broker for example on some raspberry pi or NAS. 

I use my own broker instance set up on my VPS hosting and it works way faster than the mosquitto test server.


### 2. Program your device [description here](./arduino/README.md)
I tested this code on Heltec CubeCell 1/2AA board. Should work on most CubeCell boards.
The code does not use peripherals and sends dummy payload.

In future I plan to launch it on CubeCell with GPS and send coordinates to show device position on a map.

But for now I provide this simple setup.


### 3. Download and setup the dashboard [description here](./dashboard/README.md)
The dashboard is a single html file that can be run on your PC browser or your mobile phone (more handy in the field).

There is an issue that only one dashboard can be run at once (I think the MQTT client ID is not random, I have to fix that).

The dashboard also shows the last entry as an empty name with NaN values of SNR and RSSI (this is not a real hotspot, just an error). This is also an issue I have to fix. 

## How it works?

