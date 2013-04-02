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

#include "mavlink_defines.hpp"
/*
 moved here to avoid unnecessary inlining
*/
void __attribute__ ((noinline)) request_mavlink_rate(uint8_t system, uint8_t component, uint8_t stream_number, uint16_t rate)
{
     mavlink_msg_request_data_stream_send(MAVLINK_COMM_0,system, component, stream_number, rate, 1);
}
