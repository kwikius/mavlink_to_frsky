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

#include <quan/stm32f4/tim.hpp>
#include <quan/stm32f4/tim/detail/get_bus.hpp>
#include <quan/stm32f4/sys_freq.hpp>
/*
setup a 32 bit timer use tim2
for a count increment every 0.5 ms

*/
#include "resources.hpp"
/*

 Copyright (c) 2012 Andy Little 11/11/2012

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


#include "timer.hpp"
#include <quan/frequency.hpp>
#include <quan/time.hpp>

using namespace quan::stm32f4;
namespace {


   // set up 32 bit timer for a 0.5 ms count
  // means that overflow happens in approx 25 days
   constexpr quan::frequency::Hz timer_bus_frequency{ get_bus_frequency<detail::get_bus<system_timer>::type>() };
   // n.b clk is 2 * bus here
   static constexpr uint32_t prescaler = ((timer_bus_frequency * quan::time::ms{1} ) + 0.01 ) - 1 ;

   static_assert( prescaler == 41999, "error in calc");
      
}

void setup_system_timer()
{
   disable<system_timer>();

   module_enable<system_timer>();
   system_timer::get()->cr1 = quan::bit<uint32_t>(1); // (UDIS)
   system_timer::get()->cr2 = 0;
   system_timer::get()->smcr = 0;
   system_timer::get()->dier = 0;
   system_timer::get()->egr = 0;
   system_timer::get()->cnt = 0;
   system_timer::get()->psc = static_cast<uint16_t>(prescaler);
   system_timer::get()->arr = 0xFFFFFFFF;
   system_timer::get()->sr = 0;
   system_timer::get()->cr1.clearbit<1>();// (UDIS)
   system_timer::get()->egr.setbit<0>();// immediate update registers

   enable<system_timer>();

}

void wait(quan::time_<int32_t>::ms in) 
{
   auto end_time = millis() + in;
   while (millis() < end_time){
       asm("nop");
   }
}

quan::time_<int32_t>::ms millis()
{
   return quan::time_<int32_t>::ms{ system_timer::get()->cnt / 2 };
}





