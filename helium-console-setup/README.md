# heSonar - Helium Console setup

## Helium Console setup
Log in to Helium Console https://console.helium.com
   
Remember, for this to work you need some DC (check the upper right corner of the helium console).
Each transmission is 1 DC as I recall (so if your device is detected by 20 hotspots you will pay 20 DC for this one transmission)
   
### 1. Create new device.
Go to Devices and click "Add New Device"   
![image](https://github.com/cr3you/heSonar/assets/73391409/16d2eaae-1263-4b6f-83a8-58970d999c4b)

Give it some name (doesn't have to 'heSonar'), do not change the "Dev EUI/App EUI/App Key" fields, and click "Save Device"
   
![image](https://github.com/cr3you/heSonar/assets/73391409/64559e1f-2101-4971-bda0-f6456a7d05a4)

### 2. Create Packet plan
Go to "Packets" and click Add New Packet Config
   
![image](https://github.com/cr3you/heSonar/assets/73391409/bd1f4b73-5ad7-4c0f-9d75-fe92c59f6947)

Give packet config some name (here "MaxPackets"), choose Multiple Packets and move the slide rall to the right. Click "Create Packet Config".

![image](https://github.com/cr3you/heSonar/assets/73391409/fe47845d-a00a-40c4-bfde-8ccabfc5b49e)

### 3. Create decoder function
Now we need to create some function, goto "Functions" and create one "Add New Function"
    
![image](https://github.com/cr3you/heSonar/assets/73391409/d63664d5-cd30-4584-a85f-14417a3f00c4)

Choose "Custom" decoder

![image](https://github.com/cr3you/heSonar/assets/73391409/fa17a726-a041-433c-8d1d-32be4dcbd693)

Give it some name ("heSonar_decoder" in this example) and post the code below and click "Save Function"


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


![image](https://github.com/cr3you/heSonar/assets/73391409/8115e056-812e-4c09-8c08-b077711eaef5)

### 4. Create integration
Go to "Integrations" and "Add New Integration"
   
![image](https://github.com/cr3you/heSonar/assets/73391409/618db1d6-0f2a-419b-9ba1-d3bcd7d212ff)

Choose MQTT

![image](https://github.com/cr3you/heSonar/assets/73391409/7b29c38a-fcfe-4daf-8f98-1183cb20c9be)

And fill the integration parameters:
```
Endpoint: mqtts://test.mosquitto.org:8883
Uplink Topic: helium/{{device_id}}/sonarPing
Downlink Topic: helium/{{device_id}}/tx
Name: MQTT_mosquitto.org
```

![image](https://github.com/cr3you/heSonar/assets/73391409/e35e7aed-28d4-4700-aff9-57bfd0a6a05d)

And click "Add integration"

Now the integration parameters are shown, you chave to change one more thing
Go to the "ADVANCED - JSON MESSAGE TEMPLATE (OPTIONAL)" at the bottom and click "Show Details"

![image](https://github.com/cr3you/heSonar/assets/73391409/b9062e32-bd35-43c4-8d20-38ae10af2a65)



Go to the TEMPLATE BODY section and post code from the link below and click "Save"


```
{
  "id": "{{id}}",
  "reported_at": "{{reported_at}}",
  "hotspots": "{{decoded.payload}}"
}
```
[MQTT_integration_template.json](./MQTT_integration_template.json)


![image](https://github.com/cr3you/heSonar/assets/73391409/a2c804cc-acaf-4e0f-88ee-4bd016b93bbd)

Now we have all the things we need \o/

### 5. Create Flow
Go to "Flows" and from "NODES" drag all element to flow and connect them like on the picture

![image](https://github.com/cr3you/heSonar/assets/73391409/3e989018-ac48-485c-af44-11a4eb7e309b)

We're almost done.

Now in the Flows click on your device, on the right choose "Packets" tab and turn on packet we created earlier

![image](https://github.com/cr3you/heSonar/assets/73391409/6b3c0d7d-e378-480b-876f-857741c12a4e)








