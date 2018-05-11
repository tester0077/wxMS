#
# wxMsUpdateCheck
#
# Declare the minimum required CMake version
# Note Windows version is much higher
cmake_minimum_required(VERSION 3.5.0)

project( wxMsUpdateCheck)

#include ( ${wxWidgets_USE_FILES} )

set ( HEADERS
  wxMsUpdateCheckCurlh.h 
  wxMsUpdateCheckBase.h
  )

set( SOURCES 
  wxMsUpdateCheckCurl.cpp
  wxMsUpdateCheckBase.cpp
  wxMsUpdateCheckBase_bitmaps.cpp
  )
  
  include_directories ( .. )
  
  add_library ( ${project} ${Headers} ${SOURCES} )
  
  # -------------------- eof ------------------
