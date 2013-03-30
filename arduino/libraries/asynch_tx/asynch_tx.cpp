
/*
 Copyright (c) 2012 - 2013 Andy Little 

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
#include <cstdint>
#include <asynch_tx.h>

static const uint8_t FrSkySerOut = 9; // FrSky serial output pin
static const int32_t clock_freq = 16000000;
static bool invert_output  = false;  // to invert pulse ouput sense
static quan::fifo<asynch_tx_char_type,100> output_fifo;

static inline void set_mark_on_match()
{
  // OCR1A = BitTime;
   if (invert_output == false){
      TCCR1A |= (1 << COM1A1) | (1 << COM1A0); // set on match
   }else{
      TCCR1A = ( TCCR1A |(1 << COM1A1 ) ) & ~(1 << COM1A0); // clear on match
   }
}

static inline void set_space_on_match()
{
  // OCR1A = BitTime;
   if (invert_output == true){
      TCCR1A |= (1 << COM1A1) | (1 << COM1A0); // set on match
   }else{
      TCCR1A = ( TCCR1A |(1 << COM1A1 ) ) & ~(1 << COM1A0); // clear on match
   }
}

// disable interrupts before call
void asynch_tx_setup(int32_t baudrate_in, bool inverted_output_in)
{
  invert_output = inverted_output_in;
// set up port pins
  pinMode(FrSkySerOut, OUTPUT); 

  digitalWrite(FrSkySerOut, (invert_output == false) ? HIGH : LOW );
  
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B no prescale

  TCNT1 = 0;

  //to get baudrate 
  OCR1A = (clock_freq / baudrate_in) + (( ( (clock_freq % baudrate_in) - (baudrate_in / 2) ) <= 0)?0:1);
  // turn on CTC mode conjunction with OCR1A
  TCCR1B |= (1 << WGM12);
    // Set  /1 prescaler so clock is 16 MHz
  TCCR1B |=  (1 << CS10);  
  // clear compare interrupt flag
  TIFR1 &= ~(1 << OCF1A);
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  
  // in match interrupts keeep at mark state till have data
  set_mark_on_match();

}

void asynch_tx_set_inverted_output(bool val)
{
   invert_output = val;
}

bool asynch_tx_fifo_is_full()
{
   bool fifo_full;
   cli();
   fifo_full = output_fifo.is_full();
   sei();
   return fifo_full;
}

bool asynch_tx_write_byte(asynch_tx_char_type ch)
{
   if ( asynch_tx_fifo_is_full()){
     return false;
   }
   cli();
   output_fifo.put(ch);
   sei();
   return true;
}

int16_t asynch_tx_write(asynch_tx_char_type const * buf,int16_t numbytes)
{
   int16_t num_written = 0;
   for ( int16_t i = 0; i < numbytes; ++i){
     if (asynch_tx_write_byte(buf[i]) == true){
        ++ num_written;
     }else{
        break;
     }
   }
   return num_written;
}

ISR(TIMER1_COMPA_vect)
{
   static int frame_pos = -1; //-1 means start of new frame
   static asynch_tx_char_type output_byte;
  
   if ( frame_pos == -1){ 
        //currently in mark state
      if ( output_fifo.is_empty() == false){
        // do startbit
         set_space_on_match();
         output_fifo.get(output_byte);
         frame_pos = 0;
      }
   }else{ // in a frame
      if( frame_pos < 8){
      // set opt to change for end of pulse
         if (output_byte & (1 << frame_pos)){
           set_mark_on_match();
         }else{
           set_space_on_match();
         }
         ++frame_pos;
      }else{
         frame_pos = -1;
         set_mark_on_match();
      }
   }
}