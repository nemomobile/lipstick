/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of duihome.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

/* In order to use one's own type with QtDBus, the type has to be declared
 * as a Qt meta-type with the Q_DECLARE_METATYPE() macro and registered with
 * the qDBusRegisterMetaType() function.  [Qt Documentation]
 *
 * This file includes macros to be used with qdbusxml2cpp generated files.
 * Include this file to generated files using -i metatypedeclarations.h
 * with qdbusxml2cpp.
 */

#ifndef METATYPEDECLARATIONS_H
#define METATYPEDECLARATIONS_H

Q_DECLARE_METATYPE(QList<uint>)


#endif
