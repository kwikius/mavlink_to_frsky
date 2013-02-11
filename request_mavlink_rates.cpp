
#include "mavlink_defines.hpp"
/*
 moved here to avoid unnecessary inlining
*/
void __attribute__ ((noinline)) request_mavlink_rate(uint8_t system, uint8_t component, uint8_t stream_number, uint16_t rate)
{
     mavlink_msg_request_data_stream_send(MAVLINK_COMM_0,system, component, stream_number, rate, 1);
}
