/*
  HeSonar by Crey '2023

  !! IMPORTANT, change devEui[], appEui[] and appKey[] to values from helium console (in device section)!!

  By default sonar sends beacons every 30s
  You can change the interval by changing the appTxDutyCycle value.

  Additionally you can press USERKEY on Heltec board to force beacon.

  Green LED flashes on every beacon sent.
  LED is lit after restart until device joins Helium network.

*/
#define DEBUG_EN

#include "LoRaWan_APP.h"
#include "Arduino.h"

/* OTAA para*/
uint8_t devEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/* ABP para - NOT USED but needed by Heltec libraries*/
uint8_t nwkSKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00 };
uint8_t appSKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00 };
uint32_t devAddr =  ( uint32_t )0x00000000;

/*LoraWan channelsmask, default channels 0-7*/ 
uint16_t userChannelsMask[6]={ 0xFF00,0x0000,0x0000,0x0000,0x0000,0x0000 };

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = LORAWAN_CLASS;

/*the application data transmission duty cycle.  value in [ms].*/
//uint32_t appTxDutyCycle = 900000; // 15min
//uint32_t appTxDutyCycle = 600000; // 10min
//uint32_t appTxDutyCycle = 15000;  //15s
uint32_t appTxDutyCycle = 30000;  //30s

/*OTAA or ABP*/
bool overTheAirActivation = LORAWAN_NETMODE;

/*ADR enable*/
bool loraWanAdr = LORAWAN_ADR;

/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool keepNet = LORAWAN_NET_RESERVE;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = LORAWAN_UPLINKMODE;

/* Application port */
uint8_t appPort = 2;
/*!
* Number of trials to transmit the frame, if the LoRaMAC layer did not
* receive an acknowledgment. The MAC performs a datarate adaptation,
* according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
* to the following table:
*
* Transmission nb | Data Rate
* ----------------|-----------
* 1 (first)       | DR
* 2               | DR
* 3               | max(DR-1,0)
* 4               | max(DR-1,0)
* 5               | max(DR-2,0)
* 6               | max(DR-2,0)
* 7               | max(DR-3,0)
* 8               | max(DR-3,0)
*
* Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
* the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t confirmedNbTrials = 4;

/* Prepares the payload of the frame */
static void prepareTxFrame( uint8_t port )
{
    appDataSize = 4;
    appData[0] = 'c';
    appData[1] = 'r';
    appData[2] = 'e';
    appData[3] = 'y';
}

bool wakeup = false;

void keyInterrupt()
{
  delay(10);
  if(digitalRead(USER_KEY)==HIGH)
  {
    wakeup = true;
  }
}



void setup() {
  boardInitMcu();
  #ifdef DEBUG_EN
    Serial.begin(115200);
  #endif

  // Vext ON
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW); //Vext ON
  delay(10);

  pinMode(ADC,INPUT);
  pinMode(ADC2,INPUT);
  

  pinMode(LED, OUTPUT);
  //digitalWrite(LED, LOW);
  digitalWrite(LED, HIGH); // lights until packet is sent

  pinMode(USER_KEY, INPUT);
  attachInterrupt(USER_KEY, keyInterrupt, RISING);
  Serial.println("Interrupts attached");
  
#if(AT_SUPPORT)
  enableAt();
#endif
  deviceState = DEVICE_STATE_INIT;
  LoRaWAN.ifskipjoin();
}

unsigned int k=0;
void loop()
{


  if (wakeup) {
    uint32_t now = TimerGetCurrentTime();
    Serial.print(now); Serial.println(" key pressed - waking up");
  }


  switch( deviceState )
  {
    case DEVICE_STATE_INIT:
    {
#if(AT_SUPPORT)
      getDevParam();
#endif
      printDevParam();
      LoRaWAN.init(loraWanClass,loraWanRegion);
      deviceState = DEVICE_STATE_JOIN;
      break;
    }
    case DEVICE_STATE_JOIN:
    {
      LoRaWAN.join();
      break;
    }
    case DEVICE_STATE_SEND:
    {
      digitalWrite(LED, HIGH);
      prepareTxFrame( appPort );
      LoRaWAN.send();
      deviceState = DEVICE_STATE_CYCLE;
      break;
    }
    case DEVICE_STATE_CYCLE:
    {
      // Schedule next packet transmission
      txDutyCycleTime = appTxDutyCycle + randr( 0, APP_TX_DUTYCYCLE_RND );
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      digitalWrite(LED, LOW);

      break;
    }
    case DEVICE_STATE_SLEEP:
    {
      if (wakeup) {
        if (IsLoRaMacNetworkJoined) {
            deviceState = DEVICE_STATE_SEND;
            //prepareTxFrame( appPort );
            //digitalWrite(LED, HIGH);
            //LoRaWAN.send();
        }
        wakeup = false;
      }
      else LoRaWAN.sleep();

      break;
    }
    default:
    {
      deviceState = DEVICE_STATE_INIT;
      break;
    }
  }
}
