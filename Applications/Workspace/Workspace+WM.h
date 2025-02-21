/* -*- mode: objc -*- */
//
// Project: Workspace
//
// Copyright (C) 2018-2021 Sergii Stoian
//
// This application is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This application is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this library; if not, write to the Free
// Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111 USA.
//

//
// This header is for Window Manager(X11) and Workspace(GNUstep) integration
//

#ifdef NEXTSPACE

//-----------------------------------------------------------------------------
// Common part
//-----------------------------------------------------------------------------
#include <dispatch/dispatch.h>
extern dispatch_queue_t workspace_q;
extern CFRunLoopRef wm_runloop;

typedef enum WorkspaceExitCode {
  WSLogoutOnQuit   = 0,  // normal application quit
  WSPowerOffOnQuit = 129 // ShutdownExitCode in Login/Controller.h
} WorkspaceExitCode;
extern WorkspaceExitCode ws_quit_code;

//-----------------------------------------------------------------------------
// Visible in Workspace only
//-----------------------------------------------------------------------------
#ifdef __OBJC__

#undef _

#include <wraster.h>

#include <core/log_utils.h>

#include <screen.h>
#include <startup.h>
#include <window.h>
#include <event.h>
#include <dock.h>
#include <actions.h> // wArrangeIcons()
#include <application.h>
#include <appicon.h>
#include <shutdown.h> // wShutdown(), WMxxxMode
#include <client.h>
#include <wmspec.h>
// Appicons placement
#include <stacking.h>
#include <placement.h>
#include <xrandr.h>
#include <misc.h>
#include "iconyard.h"

#undef _
#define _(X) [GS_LOCALISATION_BUNDLE localizedStringForKey:(X) value:@"" table:nil]

//-----------------------------------------------------------------------------
// Calls related to internals of Window Manager.
// 'WM' prefix is a call direction 'to WindowManager'
//-----------------------------------------------------------------------------
// --- Logout/PowerOff related activities
void WMShutdown(WMShutdownMode mode);

// - Should be called from already existing @autoreleasepool
WAppIcon *WMCreateLaunchingIcon(NSString *wmName,
                                 NSString *launchPath,
                                 NSImage *anImage,
                                 NSPoint sourcePoint,
                                 NSString *imagePath);
void WMFinishLaunchingIcon(WAppIcon *appIcon);
void WMDestroyLaunchingIcon(WAppIcon *appIcon);
// - End of functions which require existing @autorelease pool

#endif //__OBJC__

//-----------------------------------------------------------------------------
// Visible in Window Manager and Workspace
// Workspace callbacks for use inside Window Manager.
//-----------------------------------------------------------------------------

// --- Dock
void WSDockContentDidChange(WDock *dock);

// --- Application icons
WAppIcon *WSLaunchingIconForApplication(WApplication *wapp);
WAppIcon *WSLaunchingIconForCommand(char *command);

char *WSSaveRasterImageAsTIFF(RImage *r_image, char *file_path);
  
// --- Applications creation and destroying
void WSApplicationDidCreate(WApplication *wapp, WWindow *wwin);
void WSApplicationDidDestroy(WApplication *wapp);
void WSApplicationDidCloseWindow(WWindow *wwin);

// --- XRandR
void WSUpdateScreenInfo(WScreen *scr);
void WSUpdateScreenParameters(void);

// --- Workspaces
void WSActivateApplication(WScreen *scr, char *app_name);
void WSActivateWorkspaceApp(WScreen *scr);

// --- Layout badge in Workspace appicon
void WSKeyboardGroupDidChange(int group);

// -- Alerts, messages and sounds
int WSRunAlertPanel(char *title, char *message,
                     char *defaultButton,
                     char *alternateButton,
                     char *otherButton);
void WSRingBell(WWindow *wwin);
void WSMessage(char *fmt, ...);
// #define WMLogInfo(fmt, args...) WSMessage(fmt, ## args)

#endif //NEXTSPACE
