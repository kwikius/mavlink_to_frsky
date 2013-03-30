#ifndef ARD_MAV_ASYNCH_TX_H_INCLUDED
#define ARD_MAV_ASYNCH_TX_H_INCLUDED

#include <Arduino.h>
#include <cstdint>

typedef char asynch_tx_char_type;

void asynch_tx_setup(int32_t baudrate, bool inverted_output);

bool asynch_tx_fifo_is_full();
// mod to uint8_t
bool asynch_tx_write_byte(asynch_tx_char_type ch);

int16_t asynch_tx_write(asynch_tx_char_type const * buf,int16_t numbytes);


#endif // ARD_MAV_ASYNCH_TX_H_INCLUDED
