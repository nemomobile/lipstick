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

#include "weatherview.h"
#include "weather.h"
#include <QDebug>
#include <QPainter>
#include <QGraphicsLinearLayout>
#include <QTime>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MGridLayoutPolicy>
#include <MSceneManager>
#include <MViewCreator>
#include <MLabel>
#include <MDataStore>
#include <MAction>
#include "weatherbutton.h"
#include "weatherbuttonview.h"

WeatherView::WeatherView(Weather *weather) :
    MWidgetView(weather),
    controller(weather),
    timerMode(false),
    timerLabel(NULL),
    localButton(NULL)
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    weather->setLayout(layout);

    weatherButtonLayout = new MLayout;
    weatherButtonLayoutPolicy = new MLinearLayoutPolicy(weatherButtonLayout, Qt::Horizontal);
    weatherButtonLayout->setContentsMargins(0, 0, 0, 0);
    layout->addItem(weatherButtonLayout);

    // add local weather button..
    //~ uispec-document No UI spec, this is just a test applet, not production code
    //% "Local"
    localButton = new WeatherButton(qtTrId("xx_local_weather_button"), controller, 1, rand() % 750 + 500);
    localButton->setUnit(controller->model()->unit() == 0 ? WeatherButtonModel::Celsius : WeatherButtonModel::Fahrenheit);
    weatherButtonLayoutPolicy->addItem(localButton);

    MAction *action = new MAction(qtTrId("xx_togglegenericstyle"), localButton);
    localButton->addAction(action);
    connect(action, SIGNAL(triggered()), localButton, SLOT(toggleUseStyle()));

    toggleShowLocalCity(controller->model()->showLocal());

    // Add two buttons by default
    int numButtons = controller->model()->numMonitoredCities();
    numButtons -= 1; // local button..
    // Add the buttons
    while (numButtons > 0) {
        addWeatherButton();
        --numButtons;
    }

    MLayout *actionsLayout = new MLayout;
    MGridLayoutPolicy *actionLayoutPolicy = new MGridLayoutPolicy(actionsLayout);
    layout->addItem(actionsLayout);

    MButton *button = new MButton(QString(qtTrId("xx_add")), weather, 0);
    connect(button, SIGNAL(clicked()), controller, SLOT(addWeatherCity()));
    button->setObjectName("ActionButton");
    actionLayoutPolicy->addItem(button, 0, 0);

    button = new MButton(QString(qtTrId("xx_remove")), weather, 0);
    connect(button, SIGNAL(clicked()), controller, SLOT(removeWeatherCity()));
    button->setObjectName("ActionButton");
    actionLayoutPolicy->addItem(button, 0, 1);

    button = new MButton(QString(qtTrId("xx_crash")), weather, 0);
    connect(button, SIGNAL(clicked()), controller, SLOT(crash()));
    button->setObjectName("ActionButton");
    actionLayoutPolicy->addItem(button, 1, 0);

    button = new MButton(QString(qtTrId("xx_getstuck")), weather, 0);
    connect(button, SIGNAL(clicked()), controller, SLOT(enterInfiniteLoop()));
    button->setObjectName("ActionButton");
    actionLayoutPolicy->addItem(button, 1, 1);

    button = new MButton(QString(qtTrId("xx_toggletimer")), weather, 0);
    connect(button, SIGNAL(clicked()), this, SLOT(toggleTimerMode()));
    button->setObjectName("ActionButton");
    actionLayoutPolicy->addItem(button, 2, 0, 1, 2);
}

WeatherView::~WeatherView()
{
}

void WeatherView::addWeatherButton()
{
    if (weatherButtonLayout->count() < 10) {
        WeatherButton *button = new WeatherButton(controller->model()->cities().at(weatherButtonLayout->count()), controller, weatherButtonLayout->count() % 5, rand() % 750 + 500);
        button->setUnit(controller->model()->unit() == 0 ? WeatherButtonModel::Celsius : WeatherButtonModel::Fahrenheit);
        weatherButtonLayoutPolicy->addItem(button);

        MAction *action = new MAction(qtTrId("xx_togglegenericstyle"), button);
        button->addAction(action);
        connect(action, SIGNAL(triggered()), button, SLOT(toggleUseStyle()));
    }
}

void WeatherView::removeWeatherButton()
{
    int count = weatherButtonLayout->count();
    if (count > 1) {
        QGraphicsLayoutItem *item = weatherButtonLayout->itemAt(count - 1);
        weatherButtonLayout->removeAt(count - 1);
        delete item;
    }
}

void WeatherView::toggleTimerMode()
{
    timerMode = !timerMode;

    if (timerMode) {
        timerId = startTimer(500);

        timerLabel = new MLabel;
        timerLabel->setObjectName("Timer");
        timerLabel->setPos(geometry().width() - 30, geometry().height() - 30);
        timerLabel->show();
        controller->scene()->addItem(timerLabel);
        timerEvent(NULL);
    } else {
        killTimer(timerId);

        controller->scene()->removeItem(timerLabel);
        delete timerLabel;
    }
}

void WeatherView::timerEvent(QTimerEvent *)
{
    if (timerLabel != NULL) {
        timerLabel->setText(QString().sprintf("%d", QTime::currentTime().second() % 10));
    }
}

void WeatherView::setupModel()
{
    MWidgetView::setupModel();
    QList<const char *> modifications;
    modifications.append(WeatherModel::Cities);
    modifications.append(WeatherModel::Unit);
    modifications.append(WeatherModel::ShowLocal);
    modifications.append(WeatherModel::LocalTemperature);
    updateData(modifications);
}

void WeatherView::updateData(const QList<const char *> &modifications)
{
    MWidgetView::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (member == WeatherModel::Cities) {
            int index = 0;
            foreach(const QString & city, model()->cities()) {
                if (index < weatherButtonLayout->count()) {
                    QGraphicsLayoutItem *item = weatherButtonLayout->itemAt(index);
                    MButton *button = dynamic_cast<MButton *>(item);
                    if (button != NULL) {
                        button->setText(city);
                    }
                } else {
                    break;
                }
                ++index;
            }
        } else if (member == WeatherModel::Unit) {
            for (int i = 0; i < weatherButtonLayout->count(); ++i) {
                QGraphicsLayoutItem *item = weatherButtonLayout->itemAt(i);
                WeatherButton *button = dynamic_cast<WeatherButton *>(item);
                if (button != NULL) {
                    button->setUnit(model()->unit() == 0 ? WeatherButtonModel::Celsius : WeatherButtonModel::Fahrenheit);
                }
            }
        } else if (member == WeatherModel::NumMonitoredCities) {
            while (model()->numMonitoredCities() < weatherButtonLayout->count()) {
                removeWeatherButton();
            }
            while (model()->numMonitoredCities() > weatherButtonLayout->count()) {
                addWeatherButton();
            }
        } else if (member == WeatherModel::ShowLocal) {
            toggleShowLocalCity(model()->showLocal());
        } else if (member == WeatherModel::LocalTemperature) {
            changeLocalTemperature(model()->localTemperature());
        }
    }

    update();
}

void WeatherView::toggleShowLocalCity(bool isShow)
{
    if (isShow) {
        localButton->show();
    } else {
        localButton->hide();
    }
}

void WeatherView::changeLocalTemperature(int newTemperature)
{
    localButton->model()->setTemperature(newTemperature);
}

M_REGISTER_VIEW_NEW(WeatherView, Weather)
