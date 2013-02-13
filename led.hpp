#ifndef MAVLINK_TO_FRSKY_LED_HPP_INCLUDED
#define MAVLINK_TO_FRSKY_LED_HPP_INCLUDED

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

#include "resources.hpp"

template <typename Pin>
inline void setup_led()
{
   quan::stm32f4::module_enable<typename Pin::port_type>();

    quan::stm32f4::apply<
      Pin
      , quan::stm32f4::gpio::mode::output
      , quan::stm32f4::gpio::otype::push_pull
      , quan::stm32f4::gpio::pupd::none
      , quan::stm32f4::gpio::ospeed::slow
      , quan::stm32f4::gpio::ostate::low
   >();

}

template <typename Pin>
inline bool led_is_on()
{
   return quan::stm32f4::get<Pin>();
}
template <typename Pin>
inline void turn_on_led()
{
  quan::stm32f4::set<Pin>();
}

template <typename Pin>
inline void turn_off_led()
{
  quan::stm32f4::clear<Pin>();
}

template <typename Pin>
inline void complement_led()
{
  quan::stm32f4::complement<Pin>();
}
#endif // MAVLINK_TO_FRSKY_LED_HPP_INCLUDED
