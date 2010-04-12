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

#ifndef STATUSINDICATOR_H
#define STATUSINDICATOR_H

#include <MWidgetController>
#include <QSharedPointer>
#include "statusindicatormodel.h"

class ApplicationContext;
class ContextItem;

/*!
 * StatusIndicator is a widget for displaying either image or text based status
 * indicators in the status indicator area of the home screen.
 */
class StatusIndicator : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(StatusIndicator)

    Q_PROPERTY(QVariant value READ value WRITE setValue)

public:
    /*!
     * Constructs a StatusIndicator.
     *
     * \param parent parent MWidget
     */
    explicit StatusIndicator(MWidget *parent = NULL);

    /*!
     * \brief Destructor
     */
    virtual ~StatusIndicator();

protected:
    /*!
     * Sets the value of the indicator. This will be used by the dynamic
     * indicators to change their appearance based on some value. The type of the value
     * is determined by the indicator type. For indicators that display
     * an image from a list of images, the value is a float 0 to 1 which
     * is used to determine the used image. For indicators that display text,
     * the value is the string to display.
     */
    void setValue(QVariant v);

    /*!
     * Gets the value of the indicator. \see setValue
     */
    QVariant value() const;

    //! Updates the animation status of the views
    void updateAnimationStatus();

    //! Whether or not the view should animate if possible
    bool animateIfPossible;

private:
    //! \reimp
    virtual void enterDisplayEvent();
    virtual void exitDisplayEvent();
    //! \reimp_end

    /*!
     * Enables or disables model updates.
     *
     * \param modelUpdatesEnabled \c true if the model should be updated, \c false otherwise
     */
    void setModelUpdatesEnabled(bool modelUpdatesEnabled);

    //! Whether the model should be updated or not
    bool modelUpdatesEnabled;

    //! The current value of the status indicator
    QVariant currentValue;
};

/*!
 * A status indicator for showing the used phone network name
 */
class PhoneNetworkStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    M_CONTROLLER(PhoneNetworkStatusIndicator)

public:
    /*!
     * Constructs a PhoneNetworkStatusIndicator.
     *
     * \param context the application context to get status information from
     * \param parent parent MWidget. If parent is NULL, the creating client must take the responsibility to delete the indicator.
     */
    explicit PhoneNetworkStatusIndicator(ApplicationContext &context, MWidget *parent = NULL);

    virtual ~PhoneNetworkStatusIndicator();

public slots:
    void phoneNetworkChanged();

private:
    QSharedPointer<ContextItem> networkName;
};

#endif // STATUSINDICATOR_H
