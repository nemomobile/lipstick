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

#include "weatherbuttonview.h"

#include <QtGui>
#include <QPainter>
#include <QX11Info>
#include <QTimeLine>
#include <MTheme>
#include <mviewcreator.h>
#include "weatherbutton.h"

const int WeatherButtonView::numberOfIcons = 5;
const QPixmap **WeatherButtonView::icons = NULL;
int WeatherButtonView::iconsReferenceCount = 0;

WeatherButtonView::WeatherButtonView(WeatherButton *button) :
    MWidgetView(button),
    controller(button)
{
    if (iconsReferenceCount == 0) {
        icons = new const QPixmap*[numberOfIcons];
        icons[0] = MTheme::pixmap("weather-thunder");
        icons[1] = MTheme::pixmap("weather-cloudy");
        icons[2] = MTheme::pixmap("weather-sunny");
        icons[3] = MTheme::pixmap("weather-snowy");
        icons[4] = MTheme::pixmap("weather-rainy");
    }

    iconsReferenceCount++;
}

WeatherButtonView::~WeatherButtonView()
{
    iconsReferenceCount--;

    if (iconsReferenceCount == 0) {
        delete [] icons;
    }
}

void WeatherButtonView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *) const
{
    const QString text(controller->text());

    // Draw temperature
    painter->setFont(style()->temperatureFont());
    if (model()->useStyle()) {
        painter->setPen(style()->temperatureColor());
    } else {
        painter->setPen(QColor(128, 128, 128));
    }
    char unit = model()->unit() == WeatherButtonModel::Celsius ? 'C' : 'F';
    painter->drawText(QRectF(style()->temperaturePosition(), style()->temperatureSize()),
                      Qt::AlignCenter, QString().sprintf("%d %c%c", model()->temperature(), 176, unit));

    // Draw city
    if (!text.isEmpty() && controller->isTextVisible()) {
        painter->setFont(style()->cityFont());
        if (model()->useStyle()) {
            painter->setPen(style()->cityColor());
        } else {
            painter->setPen(QColor(128, 128, 128));
        }
        painter->drawText(QRectF(style()->cityPosition(), style()->citySize()), Qt::AlignCenter, text);
    }

    // Draw icon
    const QPixmap *icon = icons[model()->iconIndex()];
    if (icon != NULL && !icon->isNull() && controller->isIconVisible()) {
        painter->drawPixmap(style()->iconPosition(), *icon);
    }
}

void WeatherButtonView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);

    update();
}

void WeatherButtonView::setupModel()
{
    MWidgetView::setupModel();

    QList<const char *> modifications;
    modifications.append(WeatherButtonModel::Unit);
    modifications.append(WeatherButtonModel::IconIndex);
    modifications.append(WeatherButtonModel::UseStyle);
    modifications.append(WeatherButtonModel::Temperature);
    updateData(modifications);
}

M_REGISTER_VIEW_NEW(WeatherButtonView, WeatherButton)
