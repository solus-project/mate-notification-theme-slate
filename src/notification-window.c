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

struct _SolNotificationWindowClass {
        GtkWindowClass parent_class;
};

struct _SolNotificationWindow {
        GtkWindow parent;
        GtkWidget *image_icon;
        GtkWidget *label_title;
        GtkWidget *label_body;
        GtkWidget *button_close;
        GtkWidget *box_actions;
};

/**
 * Default notification width
 */
#define NOTIFICATION_SIZE 400

G_DEFINE_TYPE(SolNotificationWindow, sol_notification_window, GTK_TYPE_WINDOW)

/**
 * sol_notification_window_new:
 *
 * Construct a new SolNotificationWindow widget
 */
GtkWidget *sol_notification_window_new()
{
        return g_object_new(SOL_TYPE_NOTIFICATION_WINDOW, "type", GTK_WINDOW_POPUP, NULL);
}

/**
 * Handle cleanup
 */
static void sol_notification_window_dispose(__solus_unused__ GObject *obj)
{
        /* skip for now */
}

static void close_clicked(__solus_unused__ SolNotificationWindow *self,
                          __solus_unused__ gpointer userdata)
{
        /* TODO: REMOVE!!! */
        gtk_main_quit();
}

/**
 * Override the width requests to a fixed size
 */
static void sol_notification_window_get_preferred_width(__solus_unused__ GtkWidget *widget,
                                                        gint *min, gint *nat)
{
        *min = *nat = NOTIFICATION_SIZE;
}

static void sol_notification_window_get_preferred_width_for_height(
    __solus_unused__ GtkWidget *widget, __solus_unused__ gint h, gint *min, gint *nat)
{
        *min = *nat = NOTIFICATION_SIZE;
}

/**
 * Class initialisation
 */
static void sol_notification_window_class_init(SolNotificationWindowClass *klazz)
{
        GObjectClass *obj_class = G_OBJECT_CLASS(klazz);
        GtkWidgetClass *wid_class = GTK_WIDGET_CLASS(klazz);

        /* gobject vtable hookup */
        obj_class->dispose = sol_notification_window_dispose;

        /* widget vtable */
        wid_class->get_preferred_width = sol_notification_window_get_preferred_width;
        wid_class->get_preferred_width_for_height =
            sol_notification_window_get_preferred_width_for_height;

        /* GtkTemplate */
        gtk_widget_class_set_template_from_resource(
            wid_class, "/com/solus-project/mate-notification-daemon-theme-solus/notification.ui");
        gtk_widget_class_bind_template_callback(wid_class, close_clicked);

        /* Bind children to fields */
        gtk_widget_class_bind_template_child(wid_class, SolNotificationWindow, image_icon);
        gtk_widget_class_bind_template_child(wid_class, SolNotificationWindow, label_title);
        gtk_widget_class_bind_template_child(wid_class, SolNotificationWindow, button_close);
        gtk_widget_class_bind_template_child(wid_class, SolNotificationWindow, box_actions);
}

/**
 * Instaniation
 */
static void sol_notification_window_init(SolNotificationWindow *self)
{
        /* Go build the UI */
        gtk_widget_init_template(GTK_WIDGET(self));
        GdkScreen *screen = NULL;
        GdkVisual *visual = NULL;

        /* Attempt to set an RGBA visual */
        screen = gtk_widget_get_screen(GTK_WIDGET(self));
        visual = gdk_screen_get_rgba_visual(screen);
        if (visual) {
                gtk_widget_set_visual(GTK_WIDGET(self), visual);
        }

        /* Play nice with the window manager */
        gtk_window_set_resizable(GTK_WINDOW(self), FALSE);
        gtk_window_set_skip_pager_hint(GTK_WINDOW(self), TRUE);
        gtk_window_set_skip_taskbar_hint(GTK_WINDOW(self), TRUE);
        gtk_window_set_decorated(GTK_WINDOW(self), FALSE);
        gtk_window_set_default_size(GTK_WINDOW(self), NOTIFICATION_SIZE, -1);
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
