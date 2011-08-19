/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
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

#ifndef HOMEWINDOWMONITOR_STUB
#define HOMEWINDOWMONITOR_STUB

#include "homewindowmonitor.h"
#include "xeventlistener.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class HomeWindowMonitorStub : public StubBase {
  public:
  virtual const HomeWindowMonitor * instance();
  virtual void HomeWindowMonitorDestructor();
  virtual bool isOwnWindow(WId wid) const;
  virtual bool handleXEvent(const XEvent &event);
  virtual bool isHomeWindowOnTop() const;
  virtual bool isHomeWindowOnTop(QSet<Atom> ignoredWindows) const;
  virtual void HomeWindowMonitorConstructor();
  virtual QList<Window> windowStackingOrder() const;
};

// 2. IMPLEMENT STUB
const HomeWindowMonitor * HomeWindowMonitorStub::instance() {
  stubMethodEntered("instance");
  return stubReturnValue<HomeWindowMonitor *>("instance");
}

void HomeWindowMonitorStub::HomeWindowMonitorDestructor() {

}
bool HomeWindowMonitorStub::isOwnWindow(WId wid) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<WId >(wid));
  stubMethodEntered("isOwnWindow",params);
  return stubReturnValue<bool>("isOwnWindow");
}

bool HomeWindowMonitorStub::handleXEvent(const XEvent &event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const XEvent & >(event));
  stubMethodEntered("handleXEvent",params);
  return stubReturnValue<bool>("handleXEvent");
}

bool HomeWindowMonitorStub::isHomeWindowOnTop() const {
  stubMethodEntered("isHomeWindowOnTop");
  return stubReturnValue<bool>("isHomeWindowOnTop");
}

bool HomeWindowMonitorStub::isHomeWindowOnTop(QSet<Atom> ignoredWindows) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<QSet<Atom> >(ignoredWindows));
  stubMethodEntered("isHomeWindowOnTop",params);
  return stubReturnValue<bool>("isHomeWindowOnTop");
}

void HomeWindowMonitorStub::HomeWindowMonitorConstructor() {

}
QList<Window> HomeWindowMonitorStub::windowStackingOrder() const {
  stubMethodEntered("windowStackingOrder");
  return stubReturnValue<QList<Window> >("windowStackingOrder");
}



// 3. CREATE A STUB INSTANCE
HomeWindowMonitorStub gDefaultHomeWindowMonitorStub;
HomeWindowMonitorStub* gHomeWindowMonitorStub = &gDefaultHomeWindowMonitorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
const HomeWindowMonitor * HomeWindowMonitor::instance() {
  return gHomeWindowMonitorStub->instance();
}

HomeWindowMonitor::~HomeWindowMonitor() {
  gHomeWindowMonitorStub->HomeWindowMonitorDestructor();
}

bool HomeWindowMonitor::isOwnWindow(WId wid) const {
  return gHomeWindowMonitorStub->isOwnWindow(wid);
}

bool HomeWindowMonitor::handleXEvent(const XEvent &event) {
  return gHomeWindowMonitorStub->handleXEvent(event);
}

bool HomeWindowMonitor::isHomeWindowOnTop() const {
  return gHomeWindowMonitorStub->isHomeWindowOnTop();
}

bool HomeWindowMonitor::isHomeWindowOnTop(QSet<Atom> ignoredWindows) const {
  return gHomeWindowMonitorStub->isHomeWindowOnTop(ignoredWindows);
}

HomeWindowMonitor::HomeWindowMonitor() : netClientListStacking(0) {
  gHomeWindowMonitorStub->HomeWindowMonitorConstructor();
}

QList<Window> HomeWindowMonitor::windowStackingOrder() const {
  return gHomeWindowMonitorStub->windowStackingOrder();
}


#endif
