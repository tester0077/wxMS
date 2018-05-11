#
# wxMsOptionsDialog
#
# Declare the minimum required CMake version
# Note Windows version is much higher
cmake_minimum_required(VERSION 3.5.0)

#project( wxMsOptions )

#include ( ${wxWidgets_USE_FILES} )

set ( HEADERS
  wxMsOptionsDlgBase.h 
  wxMsOptionsDialogh.h
  )

set( SOURCES 
  wxMsOptionsDialog.cpp
  wxMsOptionsDialogBase.cpp
  wxMsOptionsDialogBase_bitmaps.cpp
  wxMsOptionsSelectMailClient.cpp
  wxMsOptionDialogLog.cpp
  )
  
include_directories ( .. )
  
add_library ( wxMsOptions ${Headers} ${SOURCES} )
  
  # -------------------- eof ------------------
