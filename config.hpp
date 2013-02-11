#ifndef MAVLINK_TO_FRSKY_CONFIG_HPP_INCLUDED
#define MAVLINK_TO_FRSKY_CONFIG_HPP_INCLUDED

//#define MAVLINK_TO_FRSKY_STM32f4_DISCOVERY_BOARD
#define MAVLINK_TO_FRSKY_TARGET_BOARD

#if defined( MAVLINK_TO_FRSKY_STM32f4_DISCOVERY_BOARD) && defined(MAVLINK_TO_FRSKY_TARGET_BOARD)
 #error only one target board should be defined
#endif

#if !defined( MAVLINK_TO_FRSKY_STM32f4_DISCOVERY_BOARD) && !defined(MAVLINK_TO_FRSKY_TARGET_BOARD)
#error either discovery board or target board must be defined
#endif

#endif // MAVLINK_TO_FRSKY_CONFIG_HPP_INCLUDED
