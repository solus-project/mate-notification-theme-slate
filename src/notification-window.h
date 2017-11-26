/*
 * This file is part of mate-notification-theme-mate
 *
 * Copyright © 2016-2017 Ikey Doherty <ikey@solus-project.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 */

#pragma once

#include "engine-api.h"
#include <gtk/gtk.h>

typedef struct SlateNotificationWindow {
        GtkWidget *window;
        GtkWidget *image_icon;
        GtkWidget *label_title;
        GtkWidget *label_body;
        GtkWidget *button_close;
        GtkWidget *box_actions;
        UrlClickedCb url_clicked;
        gboolean action_icons;
} SlateNotificationWindow;

GtkWidget *slate_notification_window_new(UrlClickedCb url_clicked);

void slate_notification_window_destroy(SlateNotificationWindow *window);

void slate_notification_window_set_text(SlateNotificationWindow *window, const char *summary,
                                        const char *body);

void slate_notification_window_set_pixbuf(SlateNotificationWindow *window, GdkPixbuf *pixbuf);

void slate_notification_window_add_action(SlateNotificationWindow *window, const char *label,
                                          const char *key, GCallback cb);

void slate_notification_window_clear_actions(SlateNotificationWindow *window);

void slate_notification_window_set_hints(SlateNotificationWindow *window, GHashTable *hints);

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
