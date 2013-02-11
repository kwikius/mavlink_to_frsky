
#ifndef MAVLINK_TO_FRSKY_TIMER_HPP_INCLUDED
#define MAVLINK_TO_FRSKY_TIMER_HPP_INCLUDED
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

#include <quan/time.hpp>
#include <cstdint>

quan::time_<int32_t>::ms millis();

void  wait(quan::time_<int32_t>::ms t);

#endif // MAVLINK_TO_FRSKY_TIMER_HPP_INCLUDED
