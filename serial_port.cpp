
#include <quan/stm32f4/usart/irq_handler.hpp>
#include "serial_port.hpp"

#if defined (MAVLINK_TO_FRSKY_STM32f4_DISCOVERY_BOARD)
extern "C" void USART3_IRQHandler()
{
   static_assert(
   std::is_same<
      mavlink_serial_port::usart_type,quan::stm32f4::usart3
   >::value
   ,"invalid usart for mavlink sp irq");

   quan::stm32f4::usart::irq_handler<mavlink_serial_port>();
}


extern "C" void USART2_IRQHandler()
{
   static_assert(
   std::is_same<
      frsky_serial_port::usart_type,quan::stm32f4::usart2
   >::value
   ,"invalid usart for frsky sp irq");

   quan::stm32f4::usart::irq_handler<frsky_serial_port>();
}

#endif

#if defined (MAVLINK_TO_FRSKY_TARGET_BOARD)
extern "C" void USART3_IRQHandler()
{
   static_assert(
   std::is_same<
      mavlink_serial_port::usart_type,quan::stm32f4::usart3
   >::value
   ,"invalid usart for mavlink sp irq");

   quan::stm32f4::usart::irq_handler<mavlink_serial_port>();
}


extern "C" void USART1_IRQHandler()
{
   static_assert(
   std::is_same<
      frsky_serial_port::usart_type,quan::stm32f4::usart1
   >::value
   ,"invalid usart for frsky sp irq");

   quan::stm32f4::usart::irq_handler<frsky_serial_port>();
}

#endif





