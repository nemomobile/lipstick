// IMPORTANT!!!!
//
// This file is a copy from the one in the hwcomposer platform plugin
// Make sure it is up to date, failing to do so will result in the
// hardware compositor not working or at worst, binary incompatibility.

/****************************************************************************
**
** Copyright (C) 2015 Jolla Ltd.
** Contact: Gunnar Sletta <gunnar.sletta@jollamobile.com>
**
** This file is part of the hwcomposer plugin.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/

#ifndef HWCINTERFACE_H
#define HWCINTERFACE_H

//
// IMPORTANT!!!
//
// If you make any changes to this file, make sure that you also increment the
// version of the native resource "hwcinterface" in the platform integration.
// Applications that ask for this resource will be binary compatible and crash
// unless they get the exact version they are asking for.
//
// You should also update any applications you know of that makes use of this
// interface after changing it.
//

#define HWC_INTERFACE_STRING "hwcinterface v0.2"

namespace HwcInterface
{


    struct Layer
    {
        // The hardware buffer handle, typically a buffer_handle_t
        void *handle;

        // The target rect, in pixels
        int tx, ty, tw, th;

        // The source rect, in pixels.
        int sx, sy, sw, sh;

        // Set to true after the layer list has become active. Indicates that
        // the layer will be handled by the hardware compositor. In a list of
        // 4 layers where only 2 layers are supported, the back-most two
        // layers will be accepted. This allows for composition of the front-
        // most two in the EGL surface.
        //
        // This value will be set by the hardware compositor when accepted.
        // Changing it from the client side will result in undefined behavior.
        uint accepted: 1;

        // we don't have HWCs that support opacity or color layers, so ignore these for now
        // Also, sailfish does orientation changes by rendering rotated, so 90 degree flipping
        // is not required for now. Lets keep it as simple as possible...

        // for future use...
        uint reserved : 31;
    };

    struct LayerList
    {
        // Set by the caller to indicate that EGL rendering is in use
        // and that the compositor should reserve space for that in the
        // internal render list.
        //
        // This value can be changed by the Compositor to true if layers are not
        // accepted and must be composited in EGL.
        uint eglRenderingEnabled : 1;

        // for future use...
        uint reserved : 31;

        /* The number of layers in 'layers'
         */
        int layerCount;

        // The actual layers, packed at the end of this struct..
        Layer layers[0];
    };

    class Compositor
    {
    public:
        virtual ~Compositor() {}

        /*

           Will be called by the EGL rendering thread, will schedule a
           prepare() call on HWC to check how feasible it is to perform the
           composition of these layers. Each call MUST contain a uniqure
           pointer.

           Compositor takes ownership of the LayerList and will call the
           'ReleaseLayerListCallback' once the list is no longer in use.

           The LayerList should be considered an invalid pointer until the
           time it reappears as 'acceptedLayerList'. From this moment on,
           until the next scheduleLayerList is called, the user can update
           buffer handles only.

           If a scheduled layer list is not accepted at all, for instance
           because all buffers try to use an unsupported feature, the
           'scheduledLayerList' will never become accepted.

         */
        virtual void scheduleLayerList(LayerList *list) = 0;

        /*

           Will be called by the EGL rendering thread. By default this will be 0.
           After a call to scheduleLayerList, the HWC thread will make a prepare()
           call to check the feasibility of compositing the layers. After this
           call has completed, the layer list will be accepted.

           An accepted layer list is only actually taken into use once a call
           to swapLayerList with this specific layer list is called.

          */
        virtual const LayerList *acceptedLayerList() const = 0;

        // Swaps the layer list.
        // It is a fatal error if the list is different from 'acceptedLayerList'.
        // It is a fatal error if the list contains changes other than updates
        // to buffer handles.
        virtual void swapLayerList(LayerList *list) = 0;

        typedef void (*ReleaseLayerListCallback)(LayerList *list);
        virtual void setReleaseLayerListCallback(ReleaseLayerListCallback callback) = 0;

        typedef void (*BufferAvailableCallback)(void *handle, void *data);
        virtual void setBufferAvailableCallback(BufferAvailableCallback, void *) = 0;

        /*

           The invalidation callback will be called when the hwc decides to
           invalidate its surface list. This can happen, for instance, because
           the HWC has gone into power saving mode and wants the composition
           to be done using a single buffer.

           The application must immediately schedule a new layer list and do
           another render pass. The current layer list is immediately rejected.

           This function will be called on Qt's GUI thread.

         */
        typedef void (*InvalidatedCallback)(void *data);
        virtual void setInvalidateCallback(InvalidatedCallback, void *) = 0;
    };

}; // end namespace

#endif // HWCINTERFACE_H

