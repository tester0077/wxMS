#
# wxMsPop3Preview
#
# Declare the minimum required CMake version
# Note Windows version is much higher
cmake_minimum_required(VERSION 3.5.0)

#project( wxMsPop3Preview )

#include ( ${wxWidgets_USE_FILES} )

set ( HEADERS
  codec/charsetconv.h
  codec/rfc2047.h
  codec/rfc2231.h
  )

set( SOURCES 
  wxMsMessageExtractBody.cpp
  codec/rfc2047.cpp
  codec/rfc2231.cpp
  codec/charsetconv.cpp
  )
  
include_directories ( .. )
  
add_library ( wxMsPop3Preview ${Headers} ${SOURCES} )
  
  # -------------------- eof ------------------
