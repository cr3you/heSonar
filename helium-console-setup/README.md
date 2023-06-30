# heSonar - Helium Console setup

## Helium Console setup
Log in to Helium Console https://console.helium.com
   
Remember, for this to work you need some DC (check the upper right corner of the helium console).
Each transmission is 1 DC as I recall (so if your device is detected by 20 hotspots you will pay 20 DC for this one transmission).
   
### 1. Create new device.
Go to Devices and click "Add New Device"   .

![image](https://github.com/cr3you/heSonar/assets/73391409/4ae888fa-ed84-48ae-9193-df330851d993)


Give it some name (doesn't have to 'heSonar'), do not change the "Dev EUI/App EUI/App Key" fields, and click "Save Device".
   
![image](https://github.com/cr3you/heSonar/assets/73391409/f7062c5d-f7ce-495a-816a-6a2201fce79b)


### 2. Create Packet plan
Go to "Packets" and click Add New Packet Config.
   
![image](https://github.com/cr3you/heSonar/assets/73391409/a8dd9adb-0d4d-4f01-b462-83d4b71a191e)


Give packet config some name (here "MaxPackets"), choose Multiple Packets and move the slide rall to the right. Click "Create Packet Config".

![image](https://github.com/cr3you/heSonar/assets/73391409/ebf143ca-b874-4924-8404-0c861bec4016)


### 3. Create decoder function.
Now we need to create some function, goto "Functions" and create one "Add New Function".
    
![image](https://github.com/cr3you/heSonar/assets/73391409/ec54583f-1a7c-4e94-ac83-94c14c36ed5e)


Choose "Custom" decoder.

![image](https://github.com/cr3you/heSonar/assets/73391409/4b445b3c-75ad-4dab-b215-07f53f214c28)


Give it some name ("heSonar_decoder" in this example) and post the code below and click "Save Function".


```
function Decoder(bytes, port, uplink_info) {
  var hotspots_names = "";
  
  var number_of_hotspots = 0;
  if (uplink_info.hotspots){
    number_of_hotspots = uplink_info.hotspots.length;
    var i=0;
    if (number_of_hotspots>0){
      for (i=0;i<number_of_hotspots;i++){
        hotspots_names += uplink_info.hotspots[i].name + "," + uplink_info.hotspots[i].rssi + "," + uplink_info.hotspots[i].snr + ";";
        }
    }
  }
  else hotspots_names = "--NuLL--";

  var decoder_out = hotspots_names;

  return decoder_out ;
}
```
[The code is also here: function_Decoder.js](./function_Decoder.js)


![image](https://github.com/cr3you/heSonar/assets/73391409/febffd1e-f22b-4dea-8db3-a326379c8eaa)


### 4. Create integration
Go to "Integrations" and "Add New Integration".
   
![image](https://github.com/cr3you/heSonar/assets/73391409/4e8f6105-6573-4b06-86ab-9d23c86c00c6)


Choose MQTT.

![image](https://github.com/cr3you/heSonar/assets/73391409/b88d95ff-bc19-4d49-8848-e6a6001c80a7)


And fill the integration parameters:
```
Endpoint: mqtts://test.mosquitto.org:8883
Uplink Topic: helium/{{device_id}}/sonarPing
Downlink Topic: helium/{{device_id}}/tx
Name: MQTT_mosquitto.org
```

![image](https://github.com/cr3you/heSonar/assets/73391409/8dad91e2-b753-4c7d-8bfa-5a4a4cb22b06)


And click "Add integration".

Now the integration parameters are shown, you chave to change one more thing.
Go to the "ADVANCED - JSON MESSAGE TEMPLATE (OPTIONAL)" at the bottom and click "Show Details".

![image](https://github.com/cr3you/heSonar/assets/73391409/34e748d6-299a-4cc0-84e3-9bffef13ee7b)




Go to the TEMPLATE BODY section and post code from the link below and click "Save".

**WARNING, helium console likes to forget this step, after you save the template leave integration section, go back and check if the template is saved, if not edit it again.**


```
{
  "id": "{{id}}",
  "reported_at": "{{reported_at}}",
  "hotspots": "{{decoded.payload}}"
}
```
[MQTT_integration_template.json](./MQTT_integration_template.json)


![image](https://github.com/cr3you/heSonar/assets/73391409/98251f21-a7dd-4342-9150-60ed86fff4a8)


Now we have all the things we need \o/

### 5. Create Flow
Go to "Flows" and from "NODES" drag all element to flow and connect them like on the picture.

![image](https://github.com/cr3you/heSonar/assets/73391409/c0433ed5-a226-4739-b936-ded7530f82e3)


We're almost done.

Now in the Flows click on your device, on the right choose "Packets" tab and turn on packet we created earlier.

![image](https://github.com/cr3you/heSonar/assets/73391409/6b3c0d7d-e378-480b-876f-857741c12a4e)








