#ifndef MAVLINK_TO_FRSKY_AIRCRAFT_HPP_INCLUDED
#define MAVLINK_TO_FRSKY_AIRCRAFT_HPP_INCLUDED

#include <cstdint>

#include <quan/stm32f4/config.hpp>
#include <quan/time.hpp>
#include <quan/length.hpp>
#include <quan/velocity.hpp>
#include <quan/voltage.hpp>
#include <quan/current.hpp>
#include <quan/angle.hpp>

struct location_t{
   quan::angle_<float>::deg    lat;       
   quan::angle_<float>::deg    lon;
   quan::length_<float>::m     alt;
   location_t():lat{0},lon{0},alt{0}{}
};

struct gps_t{
   uint8_t num_sats;
   uint8_t fix_type;
   bool  has_home;
};

struct attitude_t{
   quan::angle_<float>::deg            pitch;                  
   quan::angle_<float>::deg            roll;                   
   quan::angle_<float>::deg            yaw; 
  attitude_t():pitch{0},roll{0},yaw{0}{}
};

struct aircraft{
   location_t                          location;
  // location_t                          home_location;
   attitude_t                          attitude;
   float                               throttle;                  
   quan::angle_<float>::deg            heading;              
   quan::velocity_<float>::m_per_s     airspeed;            
   quan::velocity_<float>::m_per_s     groundspeed;            
   quan::voltage_<float>::V            battery_voltage;  
   quan::current_<float>::A            battery_current; 
   float                               battery_remaining; 
  // quan::length_<float>::m             get_distance_from_home(); 
 //  quan::angle_<float>::deg            get_bearing_to_home();
   uint16_t                            nav_mode;
   uint8_t                             custom_mode;
   gps_t                               gps;
                                          
   aircraft():
      throttle{0},
      heading{0},
      airspeed{0},
      groundspeed{0},
      battery_voltage{0},
      battery_current{0},
      battery_remaining{0},
      nav_mode{0},
      custom_mode{0}
   {}
   
};

extern aircraft the_aircraft;

#endif // MAVLINK_TO_FRSKY_AIRCRAFT_HPP_INCLUDED
