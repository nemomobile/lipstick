/***************************************************************************
**
** Copyright (C) 2012 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
**
** This file is part of lipstick.
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

#include <QStringList>
#include "lipstickglobal.h"

class ContextProperty;
class QTimeLine;

/*!
 * StatusIndicator provides data for displaying image or text based status
 * indicators in the status indicator area of the home screen. The indicator
 * can display a single value from a list of values or animate through a
 * range of values. The indicator may set different modes which allows the
 * view component to define different behaviors for each mode.
 */
class LIPSTICK_EXPORT StatusIndicator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString mode READ mode NOTIFY modeChanged)
    Q_PROPERTY(QString value READ value NOTIFY valueChanged)
    Q_PROPERTY(QStringList valueList READ valueList WRITE setValueList NOTIFY valueChanged)
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration NOTIFY animationDurationChanged)
    Q_PROPERTY(bool isOnDisplay READ isOnDisplay WRITE setIsOnDisplay NOTIFY isOnDisplayChanged)

public:
    /*!
     * Constructs a StatusIndicator.
     *
     * \param parent parent object
     */
    explicit StatusIndicator(QObject *parent = 0);

    /*!
     * Returns the current mode of the indicator.
     *
     * \return the current mode of the indicator
     */
    QString mode() const;

    /*!
     * Returns the current value of the indicator.
     *
     * \return the current value of the indicator
     */
    QString value() const;

    /*!
     * Sets the list of possible values for the indicator.
     *
     * \param valueList a list of possible values for the indicator
     */
    void setValueList(const QStringList &valueList);

    /*!
     * Returns the list of possible values for the indicator.
     *
     * \return the list of possible values for the indicator
     */
    QStringList valueList() const;

    /*!
     * Sets the total duration of the animation for all values on the value
     * list in milliseconds.
     *
     * \param animationDuration the number of milliseconds to loop all values on the value list in
     */
    void setAnimationDuration(int animationDuration);

    /*!
     * Returns the total duration of the animation for all values on the value
     * list in milliseconds.
     *
     * \return the number of milliseconds to loop all values on the value list in
     */
    int animationDuration() const;

    /*!
     * Sets whether the indicator should be considered to be on display or not.
     * Property notification signals and animations are enabled and disabled
     * based on this information.
     *
     * \param isOnDisplay \c true if the indicator should be considered to be on display, \c false otherwise
     */
    void setIsOnDisplay(bool isOnDisplay);

    /*!
     * Returns whether the indicator should be considered to be on display or not.
     *
     * \return \c true if the indicator should be considered to be on display, \c false otherwise
     */
    bool isOnDisplay() const;

signals:
    //! Emitted when the mode has changed
    void modeChanged();

    //! Emitted when the value has changed
    void valueChanged();

    //! Emitted when the animation duration has changed
    void animationDurationChanged();

    //! Emitted when the visibility has changed
    void isOnDisplayChanged();

    //! Emitted when a subscription to the context items has been made
    void subscriptionMade();

protected:
    /*!
     * Returns a context property monitoring the specified context key.
     * StatusIndicator takes ownership of the context property.
     *
     * \param key key of the context property to be monitored
     * \return a ContextProperty monitoring the specified context key
     */
    ContextProperty *createContextProperty(const QString &key);

    /*!
     * Sets the mode of the status indicator and emits the modeChanged() signal.
     *
     * \param mode the mode of the status indicator
     */
    void setMode(const QString &mode);

    /*!
     * Sets the index of the value to start iterating values from.
     * If the animation duration is zero, only the value in this index will be
     * used as the value of the indicator. Otherwise the value of the
     * indicator animates from this index to the end of the value list.
     *
     * \param index the index of the value to start iterating values from
     */
    void setInitialValueIndex(int index);

private slots:
    /*!
     * Sets the index of the value to be used as the value of the status
     * indicator. Calls setValue() with the value in the given index.
     *
     * \param index the index of the value to use as the value of the status indicator
     */
    void setCurrentValueIndex(int index);

private:
    /*!
     * Sets the value of the status indicator and emits the valueChanged() signal.
     *
     * \param value the value of the status indicator
     */
    void setValue(const QString &value);

    /*!
     * Sets the duration and range of the status indicator's animation and
     * starts or stops the animation based on the defined state.
     */
    void setAnimationTimeLineState();

    //! The current mode of the indicator
    QString mode_;

    //! The current value of the indicator
    QString value_;

    //! The list of possible values
    QStringList valueList_;

    //! Duration of the animation
    int animationDuration_;

    //! Whether the indicator should be considered to be on display or not
    bool isOnDisplay_;

    //! A list of context properties the indicator is tracking
    QList<ContextProperty *> contextProperties;

    //! Timeline for the animation
    QTimeLine *animationTimeLine;

    //! The index of the value in the value list
    int valueIndex_;

#ifdef UNIT_TEST
    friend class Ut_StatusIndicator;
#endif
};

#endif // STATUSINDICATOR_H
