#############################################################################
# Name:        wxMs CMakeListst.txt
# Purpose:     CMake file for wxMS under Ubuntu
# Author:      A. Wiegert
# Created:     2018-03-22
# Copyright:   (c) 2018  A. Wiegert
# Licence:     wxWidgets licence
# using the ideas from:
# https://crascit.com/2016/01/31/enhanced-source-file-handling-with-target_sources/
#############################################################################

#
# This file is was derived from the minimal example CMake file
# wxBUILD_SAMPLES option when using cmake on the library
#

# Declare the minimum required CMake version
# Note Windows version is much higher
cmake_minimum_required(VERSION 3.5.0)

# Name the project
project(wxms)

add_definitions(-DwxUSE_UNICODE -DwxUNICODE)
# Request the required wxWidgets libs
find_package(wxWidgets 3.1 COMPONENTS xrc html qa adv core base net xml REQUIRED)

IF(wxWidgets_FOUND)
ELSE(wxWidgets_FOUND)
   # For convenience. When we cannot continue, inform the user
   MESSAGE("wxWidgets not found!")
ENDIF(wxWidgets_FOUND)


# include directories
include_directories( . .. mimetic/include )
include_directories( wxMsAbout )
include_directories( wxMsUpdateCheck )
include_directories( wxMsFilter )
include_directories( wxMsOptionsDialog )
include_directories( wxMsPop3Preview )


include( ${wxWidgets_USE_FILE} )

# Define a variable containing a list of source files for the project
set(SRC_FILES
    wxMS.cpp 
    wxMsAccountDetails.cpp 
    wxMsAccountConf.cpp 
    wxMsAccountsDialog.cpp
    wxMsBaseFrame.cpp 
    wxMsBounceMsgs.cpp 
    wxMsCheck4Update.cpp 
    wxMsCheckInternet.cpp
    wxMsCheckPopMail.cpp 
    wxMsClient.cpp 
    wxMsCmdline.cpp 
    wxMsColSaveRestore.cpp
    wxMsConfig.cpp 
    wxMsContextMenu.cpp 
    wxMsCurlPop3AddHeader2MsgList.cpp
    wxMsCurlPop3ExecuteCmd.cpp 
    wxMsCurlPop3GetMessages.cpp 
    wxMsCurlPop3ServerTest.cpp
    wxMsCurlPop3UpdateListEvent.cpp 
    wxMsDeleteMsgs.cpp 
    wxMsFrame.cpp
    wxMsFriendsBlackLists.cpp 
    wxMsGetMessageFilter.cpp 
    wxMsHelp.cpp 
    wxMsLog.cpp
    wxMsMailCheckTimer.cpp 
    wxMsMailGrid.cpp 
    wxMsMailGridChar.cpp 
    wxMsMailGridClicks.cpp
    wxMsMessagePreview.cpp
    wxMsOnProcessMail.cpp
    wxMsOptions.cpp
    wxMsPop3ExecuteCommand.cpp
    wxMsPop3MessagePreview.cpp
    wxMsPop3MessagesList.cpp
    wxMsPop3Thread.cpp
    wxMsPrintMimeStructure.cpp
    wxMsRunMailClient.cpp
    wxMsThreadCurlPop3DeleteMsg.cpp
    wxMsThreadCurlPop3GetMessageBody.cpp
    wxMsThreadCurlPop3GetMessages.cpp
    wxMsThreadCurlPop3ServerTest.cpp
    wxMsUpdateUI.cpp
    wxMS_wxcr_bitmaps.cpp
    )

set ( HEADERS
    wxMsAccountDetailsh.h
    wxMsAccounth.h

    wxMsAccountsDialogh.h
    wxMsExtendedAddressh.h
    wxMsh.h
    wxMsListsh.h
    wxMsPreProcDefsh.h
    wxMsUpdateCheckDialogh.h
    wxMsUpdateDialogh.h
    )
if(WIN32)
    # Include a RC file for windows
    list(APPEND SRC_FILES ../sample.rc) 
endif()


add_subdirectory ( wxMsAbout )
add_subdirectory ( wxMsFilter )
add_subdirectory ( wxMsOptionsDialog )
add_subdirectory ( wxMsPop3Preview )


# Define the build target for the executable
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADERS})

# Link required libraries to the executable

target_link_libraries( ${PROJECT_NAME} wxMsOptions )
target_link_libraries( ${PROJECT_NAME} wxMsFilter )

target_link_libraries( ${PROJECT_NAME} wxMsPop3Preview )
target_link_libraries( ${PROJECT_NAME} wxMsAbout )
target_link_libraries( ${PROJECT_NAME} wxMsOptions )

target_link_libraries( ${PROJECT_NAME} mimetic )
target_link_libraries( ${PROJECT_NAME} wxregexu-3.1 )
target_link_libraries( ${PROJECT_NAME} ${wxWidgets_LIBRARIES} )
target_link_libraries( ${PROJECT_NAME} curl )

# --------------------------- eof ------------------------


