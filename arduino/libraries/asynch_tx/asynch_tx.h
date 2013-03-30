#ifndef ARD_MAV_ASYNCH_TX_H_INCLUDED
#define ARD_MAV_ASYNCH_TX_H_INCLUDED

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
#include <cstdint>

typedef char asynch_tx_char_type;

void asynch_tx_setup(int32_t baudrate, bool inverted_output);

bool asynch_tx_fifo_is_full();
// mod to uint8_t
bool asynch_tx_write_byte(asynch_tx_char_type ch);

int16_t asynch_tx_write(asynch_tx_char_type const * buf,int16_t numbytes);


#endif // ARD_MAV_ASYNCH_TX_H_INCLUDED
