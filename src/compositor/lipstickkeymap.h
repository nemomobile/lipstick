/***************************************************************************
**
** Copyright (C) 2015 Jolla Ltd.
** Contact: Andres Gomez <andres.gomez@jolla.com>
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

#ifndef LIPSTICKKEYMAP_H
#define LIPSTICKKEYMAP_H

#include <QWaylandInputDevice>
#include <QString>
#include <QObject>
#include "lipstickglobal.h"


class LIPSTICK_EXPORT LipstickKeymap : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString rules READ rules WRITE setRules NOTIFY rulesChanged)
    Q_PROPERTY(QString model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QString layout READ layout WRITE setLayout NOTIFY layoutChanged)
    Q_PROPERTY(QString variant READ variant WRITE setVariant NOTIFY variantChanged)
    Q_PROPERTY(QString options READ options WRITE setOptions NOTIFY optionsChanged)

public:
    explicit LipstickKeymap(QObject *parent = 0);
    ~LipstickKeymap();

    inline QString rules() const { return m_rules; }
    void setRules(const QString &rules);

    inline QString model() const { return m_model; }
    void setModel(const QString &model);

    inline QString layout() const { return m_layout; }
    void setLayout(const QString &layout);

    inline QString variant() const { return m_variant; }
    void setVariant(const QString &variant);

    inline QString options() const { return m_options; }
    void setOptions(const QString &options);

    inline QWaylandKeymap waylandKeymap() const { return QWaylandKeymap(m_layout, m_variant, m_options, m_model, m_rules); };

signals:
    void rulesChanged();
    void modelChanged();
    void layoutChanged();
    void variantChanged();
    void optionsChanged();

private:
    QString m_layout;
    QString m_variant;
    QString m_options;
    QString m_rules;
    QString m_model;
};

bool operator!=(const LipstickKeymap &a, const LipstickKeymap &b);


#endif // LIPSTICKKEYMAP_H
