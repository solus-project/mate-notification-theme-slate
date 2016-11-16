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

SOLUS_BEGIN_PEDANTIC
#include "notification-window.h"
SOLUS_END_PEDANTIC

typedef void (*ActionCb)(GtkWindow *notif_window, const char *key);

/**
 * Default notification width
 */
#define NOTIFICATION_SIZE 400

/*
 * Replace one string with another in the given input text
 */
static gchar *sol_string_replace(char **input, char *delim, char *replace)
{
        gchar **splits = g_strsplit(*input, delim, -1);
        gchar *repl = g_strjoinv(replace, splits);
        g_strfreev(splits);
        g_free(*input);
        return repl;
}

/**
 * Replace common characters known to cause issues in the pango markup
 * renderer, and optimistically attempt to use the clean string for a
 * displayed string.
 *
 * This allows us to accept valid markup and display it in the body,
 * for example, as seen in applications such as Rhythmbox.
 *
 * If Pango is still unhappy with the string, we strip it and return a
 * sanitized version.
 */
static gchar *sol_markup_escape(const char *input)
{
        gchar *start = g_strdup(input);
        gchar *markup_safe = NULL;

        /* It's legit markup */
        if (g_strrstr(input, "<") != NULL && g_strrstr(input, ">") != NULL) {
                goto safe_text;
        }

        if (g_strrstr(input, "&amp;") == NULL) {
                start = sol_string_replace(&start, "&", "&amp;");
        }
        start = sol_string_replace(&start, "'", "&apos;");
        start = sol_string_replace(&start, "\"", "&quot;");
        start = sol_string_replace(&start, "<", "&lt;");
        start = sol_string_replace(&start, ">", "&rt;");

safe_text:
        /* Is this markup safe now ? */
        if (pango_parse_markup(start, -1, 0, NULL, NULL, NULL, NULL)) {
                return start;
        }

        /* Problem with the markup itself, pango won't render it. Strip everything
         * from it and return the clean string */
        markup_safe = g_markup_escape_text(start, -1);
        g_free(start);
        return markup_safe;
}

static void close_clicked(SolNotificationWindow *self, __solus_unused__ gpointer userdata)
{
        gtk_widget_destroy(GTK_WIDGET(self->window));
}

/**
 * Handle the clicking of links in labels
 */
static void link_activated(SolNotificationWindow *self, const char *url,
                           __solus_unused__ gpointer userdata)
{
        if (self->url_clicked) {
                self->url_clicked(GTK_WINDOW(self->window), url);
        }
}

void sol_notification_window_destroy(SolNotificationWindow *window)
{
        g_free(window);
}

/**
 * sol_notification_window_new:
 *
 * Construct a new SolNotificationWindow widget
 */
GtkWidget *sol_notification_window_new(UrlClickedCb cb)
{
        GtkWidget *window = NULL;
        SolNotificationWindow *self = NULL;
        GtkBuilder *builder = NULL;
        GdkScreen *screen = NULL;
        GdkVisual *visual = NULL;

        self = g_new0(SolNotificationWindow, 1);
        self->url_clicked = cb;

        /* Init UI */
        builder = gtk_builder_new_from_resource(
            "/com/solus-project/mate-notification-daemon-theme-solus/notification.ui");

        /* Go build the UI */
        self->window = window =
            GTK_WIDGET(gtk_builder_get_object(builder, "SolNotificationWindow"));
        g_object_set_data_full(G_OBJECT(self->window),
                               "_notification_data",
                               self,
                               (GDestroyNotify)sol_notification_window_destroy);
        self->image_icon = GTK_WIDGET(gtk_builder_get_object(builder, "image_icon"));
        self->label_title = GTK_WIDGET(gtk_builder_get_object(builder, "label_title"));
        self->label_body = GTK_WIDGET(gtk_builder_get_object(builder, "label_body"));
        self->button_close = GTK_WIDGET(gtk_builder_get_object(builder, "button_close"));
        self->box_actions = GTK_WIDGET(gtk_builder_get_object(builder, "box_actions"));

        /* Done with the builder now */
        g_object_unref(builder);

        /* Callbacks */
        g_signal_connect_swapped(self->label_title,
                                 "activate-link",
                                 G_CALLBACK(link_activated),
                                 self);
        g_signal_connect_swapped(self->label_body,
                                 "activate-link",
                                 G_CALLBACK(link_activated),
                                 self);
        g_signal_connect_swapped(self->button_close, "clicked", G_CALLBACK(close_clicked), self);

        /* Attempt to set an RGBA visual */
        screen = gtk_widget_get_screen(GTK_WIDGET(window));
        visual = gdk_screen_get_rgba_visual(screen);
        if (visual) {
                gtk_widget_set_visual(GTK_WIDGET(window), visual);
        }

        /* Play nice with the window manager */
        gtk_window_set_skip_pager_hint(GTK_WINDOW(window), TRUE);
        gtk_window_set_skip_taskbar_hint(GTK_WINDOW(window), TRUE);
        gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
        gtk_window_set_default_size(GTK_WINDOW(window), NOTIFICATION_SIZE, -1);
        gtk_widget_set_size_request(window, NOTIFICATION_SIZE, -1);
        gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_NOTIFICATION);
        gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

        /* Ensure we're clean pre-show */
        sol_notification_window_set_text(self, NULL, NULL);
        sol_notification_window_set_pixbuf(self, NULL);
        self->action_icons = FALSE;

        return self->window;
}

void sol_notification_window_set_text(SolNotificationWindow *self, const char *summary,
                                      const char *body)
{
        /* Update summary */
        if (summary) {
                gchar *m_summary = sol_markup_escape(summary);
                gtk_label_set_markup(GTK_LABEL(self->label_title), m_summary);
                g_free(m_summary);
        } else {
                /* Empty label */
                gtk_label_set_text(GTK_LABEL(self->label_title), "");
        }

        /* Update body */
        if (body) {
                gchar *m_body = sol_markup_escape(body);
                gtk_label_set_markup(GTK_LABEL(self->label_body), m_body);
                g_free(m_body);
        } else {
                /* Empty label */
                gtk_label_set_text(GTK_LABEL(self->label_body), "");
        }
}

void sol_notification_window_set_pixbuf(SolNotificationWindow *self, GdkPixbuf *pixbuf)
{
        /* Gracefully handle missing pixbuf */
        if (!pixbuf) {
                gtk_image_set_from_icon_name(GTK_IMAGE(self->image_icon),
                                             "mail-unread-symbolic",
                                             GTK_ICON_SIZE_INVALID);
        } else {
                gtk_image_set_from_pixbuf(GTK_IMAGE(self->image_icon), pixbuf);
        }
        gtk_image_set_pixel_size(GTK_IMAGE(self->image_icon), 48);
}

void sol_notification_window_set_url_callback(SolNotificationWindow *self, UrlClickedCb cb)
{
        if (!self) {
                return;
        }
        self->url_clicked = cb;
}

static void sol_notification_action_clicked(GtkWidget *button, gpointer userdata)
{
        SolNotificationWindow *self = userdata;
        const gchar *key = NULL;
        ActionCb cb = NULL;

        key = g_object_get_data(G_OBJECT(button), "_notification_key");
        SOLUS_BEGIN_PEDANTIC
        cb = g_object_get_data(G_OBJECT(button), "_notification_cb");
        SOLUS_END_PEDANTIC
        if (cb) {
                cb(GTK_WINDOW(self->window), key);
        }
}

/**
 * Attempt to find the best icon for the named icon, preferring symbolic
 * versions if they're in the theme
 */
static gchar *sol_find_best_icon(const gchar *label)
{
        GtkIconTheme *icon_theme = NULL;
        gchar *tmp = NULL;

        icon_theme = gtk_icon_theme_get_default();
        /* Already a -symbolic? Return original string */
        if (g_str_has_suffix(label, "-symbolic")) {
                return g_strdup(label);
        }
        /* Try with -symbolic variant if theme has it */
        tmp = g_strdup_printf("%s-symbolic", label);
        if (gtk_icon_theme_has_icon(icon_theme, tmp)) {
                return tmp;
        }
        /* Have to make do with this I guess. :P */
        g_free(tmp);
        return g_strdup(label);
}

void sol_notification_window_add_action(SolNotificationWindow *self, const char *label,
                                        const char *key, GCallback cb)
{
        GtkWidget *button = NULL;

        /* Future support for action-icons */
        if (self->action_icons) {
                gchar *icon_name = sol_find_best_icon(key);
                button = gtk_button_new_from_icon_name(icon_name, GTK_ICON_SIZE_MENU);
                g_free(icon_name);
        } else {
                button = gtk_button_new_with_label(label);
        }

        gtk_widget_set_can_focus(button, FALSE);
        gtk_widget_set_can_default(button, FALSE);

        g_object_set_data_full(G_OBJECT(button), "_notification_key", g_strdup(key), g_free);
        if (cb) {
                SOLUS_BEGIN_PEDANTIC
                g_object_set_data(G_OBJECT(button), "_notification_cb", (void *)cb);
                SOLUS_END_PEDANTIC
        }
        g_signal_connect(button, "clicked", G_CALLBACK(sol_notification_action_clicked), self);

        gtk_container_add(GTK_CONTAINER(self->box_actions), button);

        gtk_widget_show_all(self->box_actions);
}

void sol_notification_window_clear_actions(SolNotificationWindow *self)
{
        gtk_container_foreach(GTK_CONTAINER(self->box_actions),
                              (GtkCallback)gtk_widget_destroy,
                              NULL);
}

/**
 * The only hint we actually look for is action-icons, in the hopes that
 * in future mate-notification-daemon will broadcast action-icons as a
 * capability.
 */
void sol_notification_window_set_hints(SolNotificationWindow *self, GHashTable *hints)
{
        GValue *hash = NULL;

        hash = g_hash_table_lookup(hints, "action-icons");
        if (!hash || !G_VALUE_HOLDS_BOOLEAN(hash)) {
                return;
        }
        self->action_icons = g_value_get_boolean(hash);
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
