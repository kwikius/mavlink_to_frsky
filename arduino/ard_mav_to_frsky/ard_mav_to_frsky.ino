/*
 Copyright (c) 2013 Andy Little 

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>
*/

#include <Arduino.h>
#include <fifo.h>
#include <aircraft.h>
#include <mavlink.h>
#include <asynch_tx.h>
#include <frsky.h>

namespace {

   static const int HeartbeatLed = 13;

   static const int FrSkySPInvertSignal = 2;

   void do_startup_leds(int n)
   {
        pinMode( HeartbeatLed, OUTPUT);   
// do something to show we are starting up!
        for ( int i = 0; i < n; ++i){
           digitalWrite(HeartbeatLed,HIGH);
           delay(250);
           digitalWrite(HeartbeatLed,LOW);
           delay(250);
        }
   }


   void update_leds()
   {
      static unsigned long heartbeat_timer = 0;
      static uint32_t cur_num_heartbeats = 0;
      static bool heartbeat_led_on = false;
      uint32_t num_heartbeats = get_num_heartbeats();
      if ( num_heartbeats > cur_num_heartbeats){
         // new heartbeat
         cur_num_heartbeats = num_heartbeats;
         heartbeat_timer = millis();
         digitalWrite(HeartbeatLed,HIGH);
         heartbeat_led_on = true;
      }else{
         //turn off heartbeat led after a one shot pulse of 1/4 sec 
         if ( heartbeat_led_on && (( millis() - heartbeat_timer ) >= 250)){
           digitalWrite(HeartbeatLed,LOW);
           heartbeat_led_on = false;
         }
      }
   }
}

void setup()
{ 
  pinMode(FrSkySPInvertSignal, INPUT);       
  digitalWrite(FrSkySPInvertSignal, HIGH); // turn on internal pullup resistor
  do_startup_leds(10);
  cli();

  /*
   if the FrSkySPInvertSignal pin is pulled low then 
   invert the output serial transmitter signal.
   This is useful for the FrSky receiver which wants an inverted signal
*/
 
  asynch_tx_setup(9600, !digitalRead(FrSkySPInvertSignal));
  digitalWrite(FrSkySPInvertSignal, LOW);  // turn off internal pullup resistor to save a bit of power
  sei();
#if defined __AVR_ATmega32U4_
  Serial1.begin(57600); 
#else
  Serial.begin(57600); 
#endif
}


void loop()
{
   static unsigned long loop_time = 0;
   read_mavlink();

   if ( millis() - loop_time > 19){
      update_leds();
      loop_time = millis();

      FrSky_send_message();
   }
}
