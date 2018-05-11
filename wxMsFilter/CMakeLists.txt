#
# wxMsFilter
#
# Declare the minimum required CMake version
# Note Windows version is much higher
cmake_minimum_required(VERSION 3.5.0)

#project( wxMsFilter )

#include ( ${wxWidgets_USE_FILES} )

set ( HEADERS
  wxMsFilterConfh.h 
  wsMsFilterDialogh.h
  wxMsFilterListh.h
  )

set( SOURCES 
  wxMsFilterDialog.cpp
  wxMsFilterConf.cpp
  )
  
include_directories ( .. )
  
add_library ( wxMsFilter ${Headers} ${SOURCES} )
#target_link_libraries( ${PROJECT_NAME} wxMsFilter )
  
# -------------------- eof ------------------
