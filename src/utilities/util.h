
// This file is part of lipstick, a QML desktop library
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation
// and appearing in the file LICENSE.LGPL included in the packaging
// of this file.
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// Copyright (c) 2011-2012, Timur Kristóf <venemo@fedoraproject.org>

#ifndef UTIL_H
#define UTIL_H

// Read-only property
#define GENPROPERTY_R(type, pname, name) \
    private: type pname; \
    public: inline const type &name() const { return pname; } \
    private:

// Simple property
#define GENPROPERTY_S(type, pname, name, settername) \
    private: type pname; \
    public: inline const type &name() const { return pname; } \
            inline void settername (const type &value) { pname = value; } \
    private:

// "Full" property with signal
#define GENPROPERTY_F(type, pname, name, settername, signalname) \
    private: type pname; \
    public: inline const type &name() const { return pname; } \
            inline void settername (const type &value) { pname = value; emit signalname (); } \
    private:

// Read-only property which returns a pointer
#define GENPROPERTY_PTR_R(type, pname, name) \
    private: type pname; \
    public: inline type name() { return pname; } \
    private:

// Simple property which returns a pointer
#define GENPROPERTY_PTR_S(type, pname, name, settername) \
    private: type pname; \
    public: inline type name() { return pname; } \
            inline void settername (type value) { pname = value; } \
    private:

// "Full" property with signal which returns a pointer
#define GENPROPERTY_PTR_F(type, pname, name, settername, signalname) \
    private: type pname; \
    public: inline type name() { return pname; } \
            inline void settername (type value) { pname = value; emit signalname (); } \
    private:

#endif // UTIL_H
