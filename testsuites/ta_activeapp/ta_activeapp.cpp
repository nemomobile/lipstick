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
#include "ta_activeapp.h"

#include <getopt.h>
#include <QTimer>

#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiProgressIndicator>

void Ta_ActiveApp::setup(int &argc, char ** &argv)
{
    struct option longOpt[] = {
        { "spinner", no_argument, NULL, 's' },
        { "refresh", required_argument, NULL, 'r' },
        { "no-refresh", no_argument, NULL, 'n' },
        { 0, 0, 0, 0 }
    };

    // set default
    
    progressType = DuiProgressIndicator::barType;
    refresh = 0;
    noRefresh = false;

    // process command-line arguments

    int c = 0;
    while ((c = getopt_long_only(argc, argv, "", longOpt, 0)) != -1) {
        switch(c) {
            case 's':
                progressType = DuiProgressIndicator::spinnerType;
                break;
            case 'r':
                refresh = strtol(optarg, NULL, 10);
                break;
            case 'n':
                noRefresh = true;
                break;
        }
    }
}

DuiWidget *Ta_ActiveApp::addMainContent()
{
    DuiWidget *panel = new DuiWidget();
    DuiLayout *layout = new DuiLayout(panel);
    DuiLinearLayoutPolicy *policy = new DuiLinearLayoutPolicy(layout,
                                                              Qt::Vertical);
    policy->setSpacing(25);

    DuiLabel *label;

    label = new DuiLabel(QString("Indicator type: %1").arg(progressType));
    label->setAlignment(Qt::AlignHCenter);
    policy->addItem(label);

    label = new DuiLabel(QString("No refresh"));
    if (!noRefresh)
        label->setText(QString("Refresh: %1ms").arg(refresh));
    label->setAlignment(Qt::AlignHCenter);
    policy->addItem(label);

    progress = 
        new DuiProgressIndicator(panel, progressType);
    policy->addItem(progress);

    value = new DuiLabel();
    value->setAlignment(Qt::AlignHCenter);
    policy->addItem(value);

    //

    progress->setRange(0, 100);

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));

    timer->setInterval(refresh);
    if (!noRefresh)
        timer->start();

    return panel;
}

void Ta_ActiveApp::tick()
{
    int v = (progress->value() + 1) % progress->maximum();

    progress->setValue(v);
    value->setText(QString("%1%").arg(v));
}

DuiWidget *Ta_ActiveApp::addToolbar()
{
    // TODO: Add the possible toolbar here
    DuiWidget *panel = NULL;
    return panel;
}

int main(int argc, char **argv)
{
    QString id("0");

    for (int i = 1; i < argc; ++i) {
        QString str(argv[i]);

        if (str == "-id") {
            if (i < argc - 1) {
                id = argv[i + 1];
                ++i;
            }
        }
    }

    QString appId = QString("Ta_ActiveApp-") + id;
    DuiApplication app(argc, argv, appId);
    DuiApplicationWindow window;
    window.setWindowTitle(appId);
    window.show();

    Ta_ActiveApp pageFactory;
    pageFactory.setup(argc, argv);

    MTestApp testApp(&pageFactory);
    testApp.appear();

    return app.exec();
}

