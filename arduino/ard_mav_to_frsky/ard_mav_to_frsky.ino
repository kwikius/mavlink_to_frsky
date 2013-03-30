
#include <Arduino.h>
#include <fifo.h>
#include <aircraft.h>
#include <mavlink.h>
#include <asynch_tx.h>
#include <frsky.h>

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
  do_startup_leds(10);
  cli();
  asynch_tx_setup(9600, true);
  sei();
  Serial1.begin(57600); 

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
