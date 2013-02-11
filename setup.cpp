
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