//-------------------------------------------------------------------
// Name:        wxMsCmdlineh.h
// Purpose:     command line parser header for wxMS Mail Screener
// Author:      taken partly from wxWidgets wiki
// Copyright:   (c) ??
// Licence:     wxWidgets license
//-------------------------------------------------------------------
// ------------------------------------------------------------------
// command line processing

#if defined (WANT_CMD_LINE)
#include "wx/cmdline.h"
// ------------------------------------------------------------------
// command line processing - definitions

static const wxCmdLineEntryDesc g_cmdLineDesc[] =
{
  { wxCMD_LINE_SWITCH, "h", "help", "displays help on the command line parameters" },
  { wxCMD_LINE_SWITCH, "?", "help", "displays help on the command line parameters" },
  { wxCMD_LINE_SWITCH, "v", "version", "print version" },
  { wxCMD_LINE_OPTION, "d","debug", "specify a debug level", wxCMD_LINE_VAL_NUMBER },
  { wxCMD_LINE_PARAM, NULL, NULL,"input file", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_SWITCH, "s", "silent", "disables the GUI" },
  { wxCMD_LINE_NONE }
};

#endif
// ------------------------------- eof ------------------------------

