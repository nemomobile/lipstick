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

#include "lipstickkeymap.h"


LipstickKeymap::LipstickKeymap(QObject *parent)
    : QObject(parent)
{
    QWaylandKeymap keymap;
    m_rules = keymap.rules();
    m_model = keymap.model();
    m_layout = keymap.layout();
    m_variant = keymap.variant();
    m_options = keymap.options();
}

LipstickKeymap::~LipstickKeymap()
{
}

void LipstickKeymap::setRules(const QString &rules)
{
    if (m_rules != rules) {
        m_rules = rules;
        emit rulesChanged();
    }
}

void LipstickKeymap::setModel(const QString &model)
{
    if (m_model != model) {
        m_model = model;
        emit modelChanged();
    }
}

void LipstickKeymap::setLayout(const QString &layout)
{
    if (m_layout != layout) {
        m_layout = layout;
        emit layoutChanged();
    }
}

void LipstickKeymap::setVariant(const QString &variant)
{
    if (m_variant != variant) {
        m_variant = variant;
        emit variantChanged();
    }
}

void LipstickKeymap::setOptions(const QString &options)
{
    if (m_options != options) {
        m_options = options;
        emit optionsChanged();
    }
}

bool operator!=(const LipstickKeymap &a, const LipstickKeymap &b)
{
    return a.rules() != b.rules() ||
        a.model() != b.model() ||
        a.layout() != b.layout() ||
        a.variant() != b.variant() ||
        a.options() != b.options();
}

