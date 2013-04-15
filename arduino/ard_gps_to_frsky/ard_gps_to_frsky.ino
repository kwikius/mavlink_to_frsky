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
#include <ctype.h>
#include <fifo.h>
#include <aircraft.h>
#include <asynch_tx.h>
#include <gps.h>
#include <frsky.h>

#if defined __AVR_ATmega32U4__
HardwareSerial & serial_port = Serial1;
#else
HardwareSerial & serial_port = Serial;
#endif

gps the_gps(&serial_port);

namespace {

   static const int HeartbeatLed = 13;

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

   }

   void setup_GPS()
   {

    serial_port.begin(9600); 

   }

    void read_GPS(){ 
#if 0
      if( serial_port.available()){
         if(gps.encode(serial_port.read()))
         {
            // write updated data
         }
      }
#else
    //  the_gps.parse();
#endif
    }
}

void setup()
{ 
  do_startup_leds(10);
  setup_GPS();
  cli();
  asynch_tx_setup(9600, true);
  sei();
// dependent on Arduino


}


void loop()
{
   static unsigned long loop_time = 0;
   read_GPS();

   if ( millis() - loop_time > 19){
      update_leds();
      loop_time = millis();

      FrSky_send_message();
   }
}
