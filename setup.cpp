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

#include "serial_port.hpp"
#include "resources.hpp"
#include "led.hpp"

void setup_system_timer();
void setup_led();

void setup()
{
  setup_system_timer();

  setup_led<blink_led>();
  setup_led<mavlink_heartbeat_led>();

  mavlink_serial_port::init();
  frsky_serial_port::init();

  mavlink_serial_port::set_baudrate<57600,true>();
  frsky_serial_port::set_baudrate<9600,true>();
}