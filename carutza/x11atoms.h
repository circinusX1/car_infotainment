/*
--------------------------------------------------------------------------------
Author:     Octavian Marius Chincisan @  July-Sept 2013
Project:    CARUTZA
Credits     egg, antico
--------------------------------------------------------------------------------
 This file is part of CARUTZA dashboard

    CARUTZA is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CARUTZA is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CARUTZA.  If not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------------------
Credits
--------------------------------------------------------------------------------
egg:        @author José Expósito
antico:     g_cigala@virgilio.it, ludmiloff@gmail.com
*/


#ifndef __ATOMS_H
#define __ATOMS_H

#include "defs.h"

struct x11atoms
{

    enum eATOMS {
            COMPOSITING_MANAGER = 0,
            KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR,
            KWM_WIN_ICON,
            MOTIF_WM_HINTS,
            MOTIF_WM_INFO,
            NET_ACTIVE_WINDOW,
            NET_CLIENT_LIST,
            NET_CLIENT_LIST_STACKING,
            NET_CLOSE_WINDOW,
            NET_CURRENT_DESKTOP,
            NET_DESKTOP_GEOMETRY,
            NET_DESKTOP_LAYOUT,
            NET_DESKTOP_NAMES,
            NET_DESKTOP_VIEWPORT,
            NET_FRAME_EXTENTS,
            NET_MOVERESIZE_WINDOW,
            NET_NUMBER_OF_DESKTOPS,
            NET_REQUEST_FRAME_EXTENTS,
            NET_SHOWING_DESKTOP,
            NET_STARTUP_ID,
            NET_SUPPORTED,
            NET_SUPPORTING_WM_CHECK,
            NET_SYSTEM_TRAY_OPCODE,
            NET_WM_ACTION_ABOVE,
            NET_WM_ACTION_BELOW,
            NET_WM_ACTION_CHANGE_DESKTOP,
            NET_WM_ACTION_CLOSE,
            NET_WM_ACTION_FULLSCREEN,
            NET_WM_ACTION_MAXIMIZE_HORZ,
            NET_WM_ACTION_MAXIMIZE_VERT,
            NET_WM_ACTION_MINIMIZE,
            NET_WM_ACTION_MOVE,
            NET_WM_ACTION_RESIZE,
            NET_WM_ACTION_SHADE,
            NET_WM_ACTION_STICK,
            NET_WM_ALLOWED_ACTIONS,
            NET_WM_CONTEXT_HELP,
            NET_WM_DESKTOP,
            NET_WM_ICON,
            NET_WM_ICON_GEOMETRY,
            NET_WM_ICON_NAME,
            NET_WM_MOVERESIZE,
            NET_WM_NAME,
            NET_WM_PID,
            NET_WM_PING,
            NET_WM_WINDOW_OPACITY,
            NET_WM_WINDOW_OPACITY_LOCKED,
            NET_WM_STATE,
            NET_WM_STATE_ABOVE,
            NET_WM_STATE_BELOW,
            NET_WM_STATE_DEMANDS_ATTENTION,
            NET_WM_STATE_FULLSCREEN,
            NET_WM_STATE_HIDDEN,
            NET_WM_STATE_MAXIMIZED_HORZ,
            NET_WM_STATE_MAXIMIZED_VERT,
            NET_WM_STATE_MODAL,
            NET_WM_STATE_SHADED,
            NET_WM_STATE_SKIP_PAGER,
            NET_WM_STATE_SKIP_TASKBAR,
            NET_WM_STATE_STICKY,
            NET_WM_STRUT,
            NET_WM_STRUT_PARTIAL,
            NET_WM_SYNC_REQUEST,
            NET_WM_SYNC_REQUEST_COUNTER,
            NET_WM_USER_TIME,
            NET_WM_USER_TIME_WINDOW,
            NET_WM_WINDOW_TYPE,
            NET_WM_WINDOW_TYPE_DESKTOP,
            NET_WM_WINDOW_TYPE_DIALOG,
            NET_WM_WINDOW_TYPE_DOCK,
            NET_WM_WINDOW_TYPE_MENU,
            NET_WM_WINDOW_TYPE_NORMAL,
            NET_WM_WINDOW_TYPE_SPLASH,
            NET_WM_WINDOW_TYPE_TOOLBAR,
            NET_WM_WINDOW_TYPE_UTILITY,
            NET_WORKAREA,
            SM_CLIENT_ID,
            UTF8_STRING,
            WM_CHANGE_STATE,
            WM_CLIENT_LEADER,
            WM_CLIENT_MACHINE,
            WM_COLORMAP_WINDOWS,
            WM_DELETE_WINDOW,
            WM_HINTS,
            WM_NORMAL_HINTS,
            WM_PROTOCOLS,
            WM_STATE,
            WM_TAKE_FOCUS,
            WM_TRANSIENT_FOR,
            WM_WINDOW_ROLE,
            XROOTPMAP_ID,
            XSETROOT_ID,
            ATOM_COUNT
    };

    enum eWCLASS {
            Unknown = -1,
            Normal = 0,
            Desktop = 1,
            Dock = 2,
            Toolbar = 3,
            Menu = 4,
            Dialog = 5,
            Override = 6,
            TopMenu = 7,
            Utility = 8,
            Splash = 9,
            DropdownMenu = 10,
            PopupMenu = 11,
            Tooltip = 12,
            Notification = 13,
            ComboBox = 14,
            DNDIcon = 15
    };

    static bool    resolve_atoms(Display *d);
    static int     get_wxclass(Window w);
    static Atom    atom(int i);
    static void    send_wm_event(Window w, long data0, long data1);
    static void*   x11_get_props(Window win, Atom prop, Atom type, int *nitems);

    static Atom     _atoms[ATOM_COUNT];
    static bool     _initialized;

};

#endif
