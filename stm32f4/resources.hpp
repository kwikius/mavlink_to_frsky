#ifndef MAVLINK_TO_FRSKY_RESOURCES_HPP_INCLUDED
#define MAVLINK_TO_FRSKY_RESOURCES_HPP_INCLUDED
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

#include <quan/stm32f4/serial_port.hpp>
#include <quan/stm32f4/gpio.hpp>
#include <quan/stm32f4/tim.hpp>

#include "config.hpp"

#if defined (MAVLINK_TO_FRSKY_STM32f4_DISCOVERY_BOARD)
typedef quan::stm32f4::usart3 mavlink_usart;
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiod,8>  mavlink_txo_pin;
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiod,9>  mavlink_rxi_pin;

typedef quan::stm32f4::usart2 frsky_usart;
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpioa,2>  frsky_txo_pin;
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpioa,3>  frsky_rxi_pin;

typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiod,12> mavlink_heartbeat_led; 
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiod,13> blink_led; 

#endif

#if defined (MAVLINK_TO_FRSKY_TARGET_BOARD)
typedef quan::stm32f4::usart3 mavlink_usart;
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiob,10>  mavlink_txo_pin;
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiob,11>  mavlink_rxi_pin;

typedef quan::stm32f4::usart1 frsky_usart;
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpioa,9>  frsky_txo_pin;
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpioa,10> frsky_rxi_pin;

typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpioc,11> mavlink_heartbeat_led; 
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiob,5> blink_led;

#endif


typedef  quan::stm32f4::tim2 system_timer;

typedef quan::stm32f4::serial_port<
   frsky_usart,256,256,frsky_txo_pin,frsky_rxi_pin
> frsky_serial_port;

typedef quan::stm32f4::serial_port<
   mavlink_usart,256,256,mavlink_txo_pin,mavlink_rxi_pin
> mavlink_serial_port;

#endif // MAVLINK_TO_FRSKY_RESOURCES_HPP_INCLUDED
