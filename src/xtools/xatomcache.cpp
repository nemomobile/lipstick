
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

#include <QX11Info>
#include <QHash>

#include "xatomcache.h"

typedef QHash<QByteArray, Atom> AtomHash;

Q_GLOBAL_STATIC(AtomHash, atomHashInstance);

Atom AtomCache::atom(const QByteArray &atomName)
{
    AtomHash *hash = atomHashInstance();

    AtomHash::ConstIterator it = hash->constFind(atomName);
    if (it != hash->constEnd())
        return *it;

    Display *dpy = QX11Info::display();
    Atom tmp = XInternAtom(dpy, atomName.constData(), False);
    hash->insert(atomName, tmp);
    return tmp;
}
