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
