/*
 * This file is part of mate-notification-theme-solus
 *
 * Copyright © 2016 Ikey Doherty <ikey@solus-project.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 */

#define _GNU_SOURCE

#include "util.h"

SOLUS_BEGIN_INCLUDES
#include "notification-window.h"
SOLUS_END_INCLUDES

struct _SolNotificationWindowPrivate {
        int __reserved1;
};

G_DEFINE_TYPE_WITH_PRIVATE(SolNotificationWindow, sol_notification_window, GTK_TYPE_WINDOW)

/**
 * sol_notification_window_new:
 *
 * Construct a new SolNotificationWindow widget
 */
GtkWidget *sol_notification_window_new()
{
        return g_object_new(SOL_TYPE_NOTIFICATION_WINDOW, NULL);
}

/**
 * Handle cleanup
 */
static void sol_notification_window_dispose(__solus_unused__ GObject *obj)
{
        /* skip for now */
}

/**
 * Class initialisation
 */
static void sol_notification_window_class_init(SolNotificationWindowClass *klazz)
{
        GObjectClass *obj_class = G_OBJECT_CLASS(klazz);

        /* gobject vtable hookup */
        obj_class->dispose = sol_notification_window_dispose;
}

/**
 * Instaniation
 */
static void sol_notification_window_init(SolNotificationWindow *self)
{
        self->priv = sol_notification_window_get_instance_private(self);
}

/*
 * Editor modelines  -  https://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * c-basic-offset: 8
 * tab-width: 8
 * indent-tabs-mode: nil
 * End:
 *
 * vi: set shiftwidth=8 tabstop=8 expandtab:
 * :indentSize=8:tabSize=8:noTabs=true:
 */
