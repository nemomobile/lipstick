/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QProcess>
#include <QtDeclarative/qdeclarative.h>
#include <QCryptographicHash>
#include <QFile>

#include "desktop.h"

#define WIDTH_KEY "Desktop Entry/X-MEEGO-APP-HOME-WIDTH"
#define HEIGHT_KEY "Desktop Entry/X-MEEGO-APP-HOME-HEIGHT"
#define PRIORITY_KEY "Desktop Entry/X-MEEGO-APP-HOME-PRIORITY"
#define ROW_KEY "Desktop Entry/X-MEEGO-APP-HOME-ROW"
#define COLUMN_KEY "Desktop Entry/X-MEEGO-APP-HOME-COLUMN"
#define PAGE_KEY "Desktop Entry/X-MEEGO-APP-HOME-PAGE"

Desktop::Desktop(const QString &fileName, QObject *parent)
    : QObject(parent)
    , m_filename(fileName)
    , m_entry(new MDesktopEntry(fileName))
    , m_pid(0)
    , m_wid(0)
    , m_assigned(false)
{
    ///Set the id:
    QFile file(m_filename);
    file.open(QIODevice::ReadOnly);
    if(!file.isOpen()) return;
    QString hash = QCryptographicHash::hash(file.readAll(),QCryptographicHash::Md5);
    file.close();
    m_id = hash;
}

Desktop::~Desktop()
{
}

QML_DECLARE_TYPE(Desktop);
