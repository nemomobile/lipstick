
// This file is part of lipstick, a QML desktop library
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation
// and appearing in the file LICENSE.LGPL included in the packaging
// of this file.
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// Copyright (c) 2011, Robin Burchell
// Copyright (c) 2012, Timur Kristóf <venemo@fedoraproject.org>

#ifndef ATOMCACHE_H
#define ATOMCACHE_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

class AtomCache
{
public:
    static Atom TypeAtom;
    static Atom StateAtom;
    static Atom SkipTaskbarAtom;
    static Atom NameAtom;
    static Atom ClientListAtom;
    static Atom CloseWindowAtom;
    static Atom ActiveWindowAtom;
    static Atom Utf8StringAtom;
    static Atom WindowPidAtom;

    static Atom WindowTypeNormalAtom;
    static Atom WindowTypeDesktopAtom;
    static Atom WindowTypeNotificationAtom;
    static Atom WindowTypeDialogAtom;
    static Atom WindowTypeCallAtom;
    static Atom WindowTypeDockAtom;
    static Atom WindowTypeMenuAtom;
    static Atom WindowTypeInputAtom;

    static void initializeAtoms();
};

#endif
