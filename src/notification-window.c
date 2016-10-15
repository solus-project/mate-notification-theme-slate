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
        start = sol_string_replace(&start, "&", "&amp;");
        start = sol_string_replace(&start, "'", "&apos;");
        start = sol_string_replace(&start, "\"", "&quot;");

        /* Is this markup safe now ? */
        if (pango_parse_markup(start, -1, 0, NULL, NULL, NULL, NULL)) {
                return start;
        }

        /* Problem with the markup itself, pango won't render it. Strip everything
         * from it and return the clean string */
        gchar *markup_safe = g_markup_escape_text(start, -1);
        g_free(start);
        return markup_safe;
}

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
        gtk_widget_class_bind_template_child(wid_class, SolNotificationWindow, label_body);
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
        gtk_window_set_type_hint(GTK_WINDOW(self), GDK_WINDOW_TYPE_HINT_NOTIFICATION);

        /* Ensure we're clean pre-show */
        sol_notification_window_set_text(self, NULL, NULL);
        sol_notification_window_set_pixbuf(self, NULL);
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
                gtk_image_set_pixel_size(GTK_IMAGE(self->image_icon), 48);
        } else {
                gtk_image_set_from_pixbuf(GTK_IMAGE(self->image_icon), pixbuf);
        }
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
