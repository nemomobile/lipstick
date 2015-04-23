/*!
 * @file qmusbmode_p.h
 * @brief Contains QmUSBModePrivate

   <p>
   Copyright (C) 2009-2011 Nokia Corporation

   @author Timo Olkkonen <ext-timo.p.olkkonen@nokia.com>
   @author Matias Muhonen <ext-matias.muhonen@nokia.com>

   @scope Private

   This file is part of SystemSW QtAPI.

   SystemSW QtAPI is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License
   version 2.1 as published by the Free Software Foundation.

   SystemSW QtAPI is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with SystemSW QtAPI.  If not, see <http://www.gnu.org/licenses/>.
   </p>
 */
#ifndef QMUSBMODE_P_H
#define QMUSBMODE_P_H

#include "qmusbmode.h"

#include <QMutex>
#include <QPair>
#include <QVector>

namespace MeeGo
{

class QmUSBModePrivate : public QObject
{
public:
    Q_OBJECT;
    MEEGO_DECLARE_PUBLIC(QmUSBMode);

    QMutex connectMutex;
    size_t connectCount[3];

    QmUSBModePrivate(QObject *parent = 0);
    ~QmUSBModePrivate();

    static QString modeToString(QmUSBMode::Mode mode);
    static QmUSBMode::Mode stringToMode(const QString &str);

Q_SIGNALS:
    void modeChanged(MeeGo::QmUSBMode::Mode mode);
    void fileSystemWillUnmount(MeeGo::QmUSBMode::MountPath mountPath);
    void error(const QString &errorCode);
    void supportedModesChanged(QList<MeeGo::QmUSBMode::Mode> supportedModes);

public Q_SLOTS:
    void didReceiveError(const QString &errorCode);
    void modeChanged(const QString &mode);
    void supportedModesChanged(const QString &supportedModes);
};

} // namespace MeeGo

#endif // QMUSBMODE_P_H
