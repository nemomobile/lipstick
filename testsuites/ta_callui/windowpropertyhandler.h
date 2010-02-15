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
#ifndef QTWINDOWTEST_WINDOWPROPERTYHANDLER
#define QTWINDOWTEST_WINDOWPROPERTYHANDLER
#include <QObject>
#include <QWidget>

class DuiButton;
class DuiApplicationWindow;
class DuiApplicationPage;
class WindowPropertyHandler : public QObject
{
    Q_OBJECT
public:
    WindowPropertyHandler(DuiApplicationPage *page,
                          DuiApplicationWindow *window,
                          DuiButton *button);
    virtual ~WindowPropertyHandler();
public slots:
    void changeWindowProperty(bool isCall);
private:
    DuiApplicationPage *page_;
    DuiApplicationWindow *window_;
    DuiButton *button_;
    QString originalTitle_;
    bool state_;
};


#endif
