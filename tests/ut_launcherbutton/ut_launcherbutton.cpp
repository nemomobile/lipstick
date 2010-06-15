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

#include <QtTest/QtTest>
#include <MApplication>
#include "contentaction.h"
#include "ut_launcherbutton.h"
#include "launcherbutton.h"
#include "launcher_stub.h"

using namespace ContentAction;

QString language;
QMap< QString, QSharedPointer<ActionPrivate> > contentActionPrivate;

// LauncherAction stubs (used by LauncherButton)
LauncherAction::LauncherAction()
    : Action()
{
}

LauncherAction::LauncherAction(const QString& desktopEntry)
    : Action(Action::defaultActionForFile(desktopEntry, ""))
{
}

bool operator!=(const LauncherAction &a, const LauncherAction &b)
{
    Q_UNUSED(a);
    Q_UNUSED(b);
    return true;
}

// ContentAction stubs (used by LauncherAction)
struct ContentAction::ActionPrivate
{
    ActionPrivate(bool isValid, QString name,
                  QString english, QString nonEnglish,
                  QString icon) :
        isValid_(isValid), name_(name),
        english_(english), nonEnglish_(nonEnglish),
        icon_(icon) {};
    virtual ~ActionPrivate() {};

    virtual bool isValid() const { return isValid_; }
    virtual QString name() const { return  name_; }
    virtual QString localizedName() const {
       if (language == "english") {
           return english_;
       } else {
           return nonEnglish_;
       }
    }
    virtual QString icon() const { return icon_; }

    bool isValid_;
    QString name_;
    QString english_;
    QString nonEnglish_;
    QString icon_;
};

bool Action::isValid() const { return d->isValid(); }
QString Action::name() const { return d->name(); } 
QString Action::localizedName() const { return d->localizedName(); } 
QString Action::icon() const { return d->icon(); } 

Action::Action()
    : d(new ActionPrivate(false, "", "", "", ""))
{
}

Action::Action(const Action& other)
    : d(other.d)
{
}

Action::~Action()
{
}

Action Action::defaultActionForFile(const QUrl& fileUri, const QString& mimeType)
{
    Q_UNUSED(mimeType);

    QString fileName = fileUri.toString();
    QSharedPointer<ActionPrivate> priv =
       contentActionPrivate.value(fileName);

    Action action;
    action.d = priv;

    return action;
}

int contentActionTriggerCalls = 0;
void Action::trigger() const
{
    contentActionTriggerCalls++;
}

// A helper function to set up a contentActionPrivate entry
void addActionPrivate(QString fileName, bool isValid, QString name,
                      QString english, QString nonEnglish, QString icon)
{
    ActionPrivate* priv = new ActionPrivate(isValid, 
                                            name,
                                            english,
                                            nonEnglish,
                                            icon);
    contentActionPrivate.insert(fileName,
                                QSharedPointer<ActionPrivate>(priv));
}

// QIcon stubs
QString qIconFileName;
QIcon::QIcon(const QString &fileName)
{
    qIconFileName = fileName;
}

QIcon& QIcon::operator=(QIcon const &)
{
    return *this;
}

QIcon::~QIcon()
{
}

bool qIconHasThemeIcon = false;
bool QIcon::hasThemeIcon(const QString &name)
{
    Q_UNUSED(name);
    return qIconHasThemeIcon;
}

QString qIconName;
QIcon QIcon::fromTheme(const QString &name, const QIcon &fallback)
{
    qIconName = name;
    return fallback;
}

void Ut_LauncherButton::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_launcherbutton" };
    app = new MApplication(argc, app_name);
}

void Ut_LauncherButton::cleanupTestCase()
{
    delete app;
}

void Ut_LauncherButton::init()
{
    language = "english";
    qIconFileName.clear();
    qIconName.clear();
    qIconHasThemeIcon = false;
    contentActionPrivate.clear();
    contentActionTriggerCalls = 0;

    m_subject = new LauncherButton();
    connect(this, SIGNAL(clicked()), m_subject, SLOT(launch()));
}

void Ut_LauncherButton::cleanup()
{
    delete m_subject;
}

void Ut_LauncherButton::testInitialization()
{
    delete m_subject;

    addActionPrivate("/dev/null",
                     true,
                     "name",
                     "english",
                     "nonenglish",
                     "icon");

    m_subject = new LauncherButton("/dev/null");
    QCOMPARE(m_subject->desktopEntry(), QString("/dev/null"));
    QCOMPARE(m_subject->text(), QString("english"));
    QCOMPARE(m_subject->iconID(), QString("icon"));
}

void Ut_LauncherButton::testInitializationAbsoluteIcon()
{
    delete m_subject;

    addActionPrivate("/dev/null",
                     true,
                     "name",
                     "english",
                     "nonenglish",
                     "/icon");

    m_subject = new LauncherButton("/dev/null");
    QCOMPARE(m_subject->iconID(), QString());
    QCOMPARE(qIconFileName, QString("/icon"));
}

void Ut_LauncherButton::testInitializationFreeDesktopIcon()
{
    delete m_subject;

    addActionPrivate("/dev/null",
                     true,
                     "name",
                     "english",
                     "nonenglish",
                     "icon");
    qIconHasThemeIcon = true;

    m_subject = new LauncherButton("/dev/null");
    QCOMPARE(m_subject->iconID(), QString());
    QCOMPARE(qIconName, QString("icon"));
}

void Ut_LauncherButton::testLaunch()
{
    emit clicked();
    QCOMPARE(contentActionTriggerCalls, 1);
}

void Ut_LauncherButton::testLanguageChange()
{
    delete m_subject;

    addActionPrivate("/dev/null",
                     true,
                     "name",
                     "english",
                     "nonenglish",
                     "icon");

    m_subject = new LauncherButton("/dev/null");
    QCOMPARE(m_subject->text(), QString("english"));

    language = "otherlang";
    m_subject->retranslateUi();
    QCOMPARE(m_subject->text(), QString("nonenglish"));
}

QTEST_APPLESS_MAIN(Ut_LauncherButton)
