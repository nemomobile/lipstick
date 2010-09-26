/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouchhome.
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

#ifndef EXTENDEDBUTTONVIEW_H_
#define EXTENDEDBUTTONVIEW_H_

#include <mbuttonmodel.h>
#include <mbuttoniconview.h>
#include "extendedbuttonstyle.h"

class MButton;

/*!
 * The extended button view allows the hit area of the button to extend beyond the button's geometry.
 */
class ExtendedButtonView : public MButtonIconView
{
    Q_OBJECT
    M_VIEW(MButtonModel, ExtendedButtonStyle)

public:
    /*!
     * Constructs a ExtendedButtonView.
     *
     * \param controller the MButton controller for the view
     */
    ExtendedButtonView(MButton *controller);

    /*!
     * Destroys the ExtendedButtonView.
     */
    virtual ~ExtendedButtonView();

protected:
    //! \reimp
    virtual QRectF boundingRect() const;
    //! \reimp_end

private:
#ifdef UNIT_TEST
    friend class Ut_ExtendedButtonView;
#endif
};

#endif /* EXTENDEDBUTTONVIEW_H_ */
