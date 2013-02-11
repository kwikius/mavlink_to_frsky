#ifndef MAVLINK_TO_FRSKY_LED_HPP_INCLUDED
#define MAVLINK_TO_FRSKY_LED_HPP_INCLUDED

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
