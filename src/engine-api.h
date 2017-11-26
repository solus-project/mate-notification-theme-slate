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

#include <gtk/gtk.h>

#include "util.h"

#define MATE_NOTIFYD_MAJOR_VERSION 1
#define MATE_NOTIFYD_MINOR_VERSION 3

typedef void (*UrlClickedCb)(GtkWindow *notif_window, const char *url);

/**
 * Attempt initilisation of the theme
 */
gboolean theme_check_init(unsigned int major, unsigned int minor, unsigned int micro);

/**
 * Get the theme details for the frontend selection UI
 */
void get_theme_info(char **theme_name, char **theme_ver, char **author, char **homepage);

/**
 * Create a new notification with the given click handler
 */
GtkWindow *create_notification(UrlClickedCb url_clicked);

/**
 * Destroy an existing notification immediately
 */
void destroy_notification(GtkWindow *notif_window);

/**
 * Show the given notification on screen
 */
void show_notification(GtkWindow *notif_window);

/**
 * Hide the given notification from view
 */
void hide_notification(GtkWindow *notif_window);

/**
 * Update notification from the given hints mapping
 */
void set_notification_hints(GtkWindow *notif_window, GHashTable *hints);

/**
 * Set the textual content for the notification
 */
void set_notification_text(GtkWindow *notif_window, const char *summary, const char *body);

/**
 * Set the main display icon for the notification
 */
void set_notification_icon(GtkWindow *notif_window, GdkPixbuf *pixbuf);

/**
 * Set the arrow coordinates for the notification
 */
void set_notification_arrow(GtkWindow *notif_window, gboolean visible, int x, int y);

/**
 * Add a new action to the notification
 */
void add_notification_action(GtkWindow *notif_window, const char *label, const char *key,
                             GCallback cb);

/**
 * Clear all notifications from the given notification
 */
void clear_notification_actions(GtkWindow *notif_window);

/**
 * Move the notification to the specified position
 */
void move_notification(GtkWindow *notif_window, int x, int y);

/**
 * Set the expiration timeout for the notification
 */
void set_notification_timeout(GtkWindow *notif_window, glong timeout);

/**
 * Update clock-cycle for the expiration timeout in idle loops
 */
void notification_tick(GtkWindow *notif_window, glong timeout);

/**
 * Whether or not we support always being stacked
 */
gboolean get_always_stack(GtkWindow *notif_window);

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
