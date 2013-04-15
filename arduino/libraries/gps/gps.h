#ifndef MAVLINK_TO_FRSKY_GPS_H_INCLUDED
#define MAVLINK_TO_FRSKY_GPS_H_INCLUDED

#include <aircraft.h>

struct gps {

   // value that has a flag when its chnaged
   template <typename V>
   struct var {
      var (V const& v) : m_value(v),m_changed(true){}
      var & operator = (V const & v)
      {
         m_value = v;
         m_changed = true;
      }
      V get_value() const {return m_value;}
      void clear_change(){ m_changed= false;}
    private:
      V m_value;
      bool m_changed;
   };

   enum fix_type {Fix_not_available,GPS_fix, Differential_fix};

   gps(HardwareSerial * sp)
      :m_sp(sp)
      ,m_buf_idx(0),m_atom_idx(0),m_checksum(false)
      ,m_utc_time_ms(0)
      ,m_latitude_deg10e7(0)
      ,m_longtitude_deg10e7(0)
      ,m_fix_type(Fix_not_available)

   {}

   void parse() {  (this->*pf_parse)();}

  private:

   HardwareSerial * m_sp;

   static const size_t buf_strlen = 11;
   char m_buf[buf_strlen];
   char m_cur_tok;
   size_t m_buf_idx;
   uint16_t m_atom_idx;
   uint8_t m_checksum;

// gps variables
   var<uint32_t> m_utc_time_ms;
   var<int32_t> m_latitude_deg10e7;
   var<int32_t> m_longtitude_deg10e7;
   var<fix_type> m_fix_type;

   typedef void(gps::* pmf)();
   pmf pf_parse; // fp for parser state machine
   pmf pf_stacked; // for nesting functions 1 deep only
  
   void in_preamble()
   {
     // dump cr, lf etc while looking for '$'
      while( m_sp->available()){
         if(m_sp->read() == '$'){
            pf_parse = &gps::in_command_word;
            break;
         }
      }
   }
   // ascii charcter to numeric digit
   static uint8_t chdg(char ch)
   { 
      return ch - '0';
   }
     
   // convert the buffer field to a time in milliseconds
   bool buf_to_utc_time()
   {
      // input format : hhmmss.sss
      if ( m_buf_idx < 6){
         return false;
      }
      for ( uint8_t i =0; i < 6; ++i){
         if (!isdigit(m_buf[i])){
            return false;
         }
      }
      uint32_t const hrs  = chdg(m_buf[0]) * 10  + chdg(m_buf[1]);
      uint32_t const mins = chdg(m_buf[2]) * 10  + chdg(m_buf[3]) + hrs * 60;
      uint32_t const secs = chdg(m_buf[4]) * 10  + chdg(m_buf[5]) + mins * 60;
      uint32_t ms = secs * 1000;

      int len_remaining = m_buf_idx - 8;
      if (len_remaining > 3){ len_remaining = 3;}
      int mul = 100;
      for ( int i = 0; i < len_remaining; ++i){
         char ch = m_buf[7 + i];
         if( !isdigit(ch)){
            return false;
         }
         ms += chdg(ch) * mul;
         mul /= 10;
      }
      m_utc_time_ms = ms;
      return true;
   }

   // convert the buffer to an angle in deg10e7
   bool buf_to_deg10e7(var<int32_t> & dest)
   {
       //expected input format dddmm.mmmm
      if ( m_buf_idx < 7){
         return false;
      }
      int lenb4dp = 0;
      for ( ;;++lenb4dp){
         if ( lenb4dp == m_buf_idx){
            return false;
         }
         if(m_buf[lenb4dp] == '.'){
            break;
         }
      }
      int16_t deg = 0;
      int16_t scale = 1;
      for ( int pos  = lenb4dp - 3; pos >=0; --pos){
         char ch = m_buf[pos];
         if( !isdigit(ch)){
            return false;
         }
         deg += chdg(ch) * scale;
         scale *= 10;
      }
       int16_t minutes = 0;
       scale = 1;
       for ( int pos  = lenb4dp - 1; pos > lenb4dp -3 ; --pos){
         char ch = m_buf[pos];
         if( !isdigit(ch)){
            return false;
         }
         minutes += chdg(ch) *scale;
         scale *= 10;
       }
       // up to 4 places in minutes * 1e-4
       int16_t fractional_minutes = 0;
       scale = 1000;
       
       for (int pos = lenb4dp+1; pos < m_buf_idx - 1; ++pos)
       {
         char ch = m_buf[pos];
         if( !isdigit(ch)){
            return false;
         }
         fractional_minutes += chdg(ch) * scale;
         scale /= 10;
       }
       dest = static_cast<int32_t>( (deg * 1.e7 + ( minutes * 1.e6  + fractional_minutes * 1.e2) / 6 )  + 0.5) ;
       return true;
   }

   bool buf_to_Latitude()
   {
      return buf_to_deg10e7(m_latitude_deg10e7) == true;
   }

   bool buf_to_sign_of_Latitude()
   {
      if ( m_buf_idx !=2){
         return false;
      }
      if (m_buf[0] == 'N'){
         return true;
      }
      if (m_buf[0] == 'S'){
         m_latitude_deg10e7 = m_latitude_deg10e7.get_value() * -1;
         return true;
      }
      return false;
   }

   bool buf_to_Longtitude()
   {
      return buf_to_deg10e7(m_longtitude_deg10e7)== true;
   }

   bool buf_to_sign_of_Longtitude()
   {
      if ( m_buf_idx !=1){
         return false;
      }
      if (m_buf[0] == 'E'){
         return true;
      }
      if (m_buf[0] == 'W'){
         m_longtitude_deg10e7 = m_longtitude_deg10e7.get_value() * -1;
         return true;
      }
      return false;
   }
// enum fix_type {None,Fix_not_available,GPS_fix, Differential_fix};

   bool buf_to_position_fix_indicator()
   {

      if ( m_buf_idx !=1){
         return false;
      }
      switch ( chdg(m_buf[0]) ){
         case 0:
            m_fix_type = Fix_not_available;
            return true;
         case 1:
            m_fix_type = GPS_fix;
            return true;
         case 3:
            m_fix_type = Differential_fix;
            return true;
         default:
            return false;
      }

   return false;
   }

   // parse the buffer till the terminator is 
   // found then call the function on it
   
   void parse_buf_fn( bool( gps::*pfn)())
   {
      while (m_sp->available()){
         char ch = m_sp->read();
         switch (ch){
            case ',' :
                m_checksum ^= ch;
               //ok fall through
            case '*' :
               m_cur_tok = ch;
               m_buf[m_buf_idx] = '\0';
               if ( (this->*pfn)() ){
                  pop();
               }else{
                  reset();
               }
            return;
            default:
               m_checksum ^= ch;
               m_buf[m_buf_idx] = ch;
               if (++m_buf_idx == buf_strlen){
                  reset();
                  return;
               }
            break;
         }
      }
   }

   void parse_UTC_time()
   {
      parse_buf_fn(&gps::buf_to_utc_time);
   }

   void parse_Latitude()
   {
      parse_buf_fn(&gps::buf_to_Latitude);
   }

   void parse_sign_of_Latitude()
   {
      parse_buf_fn(&gps::buf_to_sign_of_Latitude);
   }

   void parse_Longtitude()
   {
      parse_buf_fn(&gps::buf_to_Longtitude);
   }

   void parse_sign_of_Longtitude()
   {
      parse_buf_fn(&gps::buf_to_sign_of_Longtitude);
   }

   void parse_position_fix_indicator()
   {
       parse_buf_fn(&gps::buf_to_position_fix_indicator);
   }
   

   void push(pmf p)
   {
     pf_stacked = pf_parse;
     pf_parse = p;
   }

   void pop()
   {
     pf_parse = pf_stacked;
   }
   
   // char to hexdigit
   static uint8_t chxdg(char ch)
   { 
      if (ch >= 'A' && ch <= 'F'){
       return ch - 'A' + 10;
      }
      if (ch >= 'a' && ch <= 'f'){
       return ch - 'a' + 10;
      }
      return ch - '0';
   }

   static const size_t GGA_num_cmds= 10; 
   static pmf GGA_cmds[GGA_num_cmds];

   bool do_checksum()
   {
     char ck1 = m_sp->read();
     char ck2 = m_sp->read();
     if (! ( isxdigit(ck1) && isxdigit(ck2))){
      return false;
     }
     return ( ( chxdg(ck1) << 4) | chxdg(ck1) ) == m_checksum;
   }

   void update_vars();

   void GGA_cmd()
   {
      if (m_cur_tok == '*'){
         // dont eval til checksum arrived
         if ( m_sp->available() < 2){
            return;
         }
         if (do_checksum() == true){
            update_vars();
            reset();
         }
      }else{
      // check length here
         if( m_atom_idx == GGA_num_cmds){
            reset();
         } else{
            push(GGA_cmds[m_atom_idx++]);
         }
      }
   }
   void GSA_cmd(){return ;}
   void GSV_cmd(){return ;}
   void RMC_cmd(){return ;}
   void VTG_cmd(){return;}

   enum command_id{UNKNOWN, GGA,GSA,GSV,RMC,VTG };

   pmf get_command_parser()
   {
      if(strcmp(m_buf,"GPGGA")==0){ return &gps::GGA_cmd;}
      if(strcmp(m_buf,"GPGSA")==0){ return &gps::GSA_cmd;}
      if(strcmp(m_buf,"GPGSV")==0){ return &gps::GSV_cmd;}
      if(strcmp(m_buf,"GPRMC")==0){ return &gps::RMC_cmd;}
      if(strcmp(m_buf,"GPVTG")==0){ return &gps::VTG_cmd;}
      // unknown other cmd
      reset();
      return &gps::in_preamble;
   }
   char chksum_read()
   {
      char ch = m_sp->read();
      m_checksum ^= ch;
      return ch;
   }
   // acquire command word
   void in_command_word()
   {
      while (m_sp->available()){
         char ch = chksum_read();
         if ( isalpha(ch)){
            m_buf[m_buf_idx] = ch;
            if(++m_buf_idx == buf_strlen){
               reset();
               break;
            }
         }else {
            if ( ch == ','){
               m_buf[m_buf_idx] = '\0';
               m_buf_idx = 0;
               m_atom_idx = 0;
               m_cur_tok = '\0';
               pf_parse = get_command_parser();
            }else{
               reset();
            }
            break;
         }
      }
   }

   void reset()
   {
      m_buf_idx = 0;
      m_checksum = 0;
      m_cur_tok = '\0';
      pf_parse = &gps::in_preamble;
      return;
   }

};

extern gps the_gps;

#endif // MAVLINK_TO_FRSKY_GPS_H_INCLUDED
