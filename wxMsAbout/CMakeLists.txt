#
# wxMSAbout
#
#
# Declare the minimum required CMake version
# Note Windows version is much higher
cmake_minimum_required(VERSION 3.5.0)

#project( wxMsAbout )

#include ( ${wxWidgets_USE_FILES} )

set ( HEADERS
  wxMsAboutDlgBase.h 
  )

set( SOURCES 
  wxMsAboutDlg.cpp
  wxMsAboutDlgBase.cpp
  wxMsAboutDlgBase_bitmaps.cpp
  wxMsAboutDlgBase_bitmaps.xrc
  )
  
include_directories ( .. )
  
add_library ( wxMsAbout ${SOURCES} ${Headers} )
  

# -------------------- eof ------------------
