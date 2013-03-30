
#ifndef ARD_MAV_FIFO_HPP_INCLUDED
#define ARD_MAV_FIFO_HPP_INCLUDED

namespace quan{
//minimum size of 1
template<typename T, int Size>
struct fifo {
    static const int size = Size;
    fifo():m_putptr (m_buf),m_getptr (0) {}
    bool is_empty() const {
        return m_getptr ==0;
    }
    bool is_full() const {
        return m_putptr ==0;
    }
    bool put(T const & v)
    {
        if( !m_putptr) return 0;
    // putptr is at end if 0
 
    *m_putptr = v;
    // update getptr if was empty
    if (m_getptr == 0) {
        m_getptr = m_putptr;
    }
    // update the put ptr
    ++ m_putptr;
    if(m_putptr == ( m_buf + size )) {
        m_putptr =  m_buf;
    }
    // check if buffer is full
    if(m_putptr == m_getptr) {
        m_putptr = 0;
    }
    return true;
}

int num_in_buffer() const
{
   if (m_getptr == 0){
      return 0;
   }else{
      if ( m_putptr == 0){
         return size;
      }else{
         if (m_putptr > m_getptr){
           return m_putptr - m_getptr;
         } else{
           return ( m_putptr + size) - m_getptr  ;
         }
      }
   }
}

int num_free()const
{
   return size - this->num_in_buffer();
}

bool peek(T& v) const
{
    if (!m_getptr) {
        return false;
    }
    v = *m_getptr;
    return true;
}
 
bool  get (T & v) 
{
    if (!m_getptr) {
        return false;
    }
    v = *m_getptr;
    if (m_putptr ==0) {
        m_putptr = m_getptr;
    }
    ++m_getptr;
    if(m_getptr == (m_buf + size)) {
        m_getptr = m_buf;
    }
    if (m_getptr == m_putptr) {
        m_getptr =0;
    }
    return true;
}
private:
 
T * m_putptr;
T * m_getptr;
T   m_buf[size];

  fifo (fifo const & ) ;//= delete;
  fifo & operator = (fifo const &) ;//= delete;
};
}

#endif //ARD_MAV_FIFO_HPP_INCLUDED