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

#include "config.h"
#include "util.h"

SLATE_BEGIN_PEDANTIC
#include "engine-api.h"
#include "notification-window.h"
#include "theme-resources.h"
SLATE_END_PEDANTIC

#include <stdbool.h>

/**
 * Whether we initially tried a theme load or not
 */
static bool _initial_theme_loaded = false;

/**
 * Guard against double resource loads
 */
static bool _initial_resource_loaded = false;

/**
 * We load and unload this to ensure we cause no problems to other plugins
 */
static GtkCssProvider *_theme_style_provider = NULL;

/**
 * Signal for the GtkSettings connection
 */
static gulong _gtk_settings_con_id = 0L;

#define THEME_PREFIX "resource://com/solus-project/mate-notification-daemon-theme-slate"

static gchar *slate_theme_form_theme_path(const gchar *suffix)
{
        guint minor_version = gtk_get_minor_version();

        /* Prioritize 3.18 */
        switch (minor_version) {
        case 18:
                return g_strdup_printf("%s/3.18/%s", THEME_PREFIX, suffix);
        case 20:
        default:
                return g_strdup_printf("%s/3.20/%s", THEME_PREFIX, suffix);
        }
}

/**
 * Set the theme to one of theme.css or theme_hc.css
 * Care is taken to load the correct version of the theme
 */
static void slate_set_theme(const char *theme_portion)
{
        GFile *file = NULL;
        gchar *uri = NULL;
        GtkCssProvider *prov = NULL;
        GdkScreen *screen = NULL;
        GtkSettings *settings = NULL;
        const char *cur_theme = NULL;
        guint priority = GTK_STYLE_PROVIDER_PRIORITY_FALLBACK;

        uri = slate_theme_form_theme_path(theme_portion);
        file = g_file_new_for_uri(uri);
        if (!file) {
                goto end;
        }

        prov = gtk_css_provider_new();
        if (!gtk_css_provider_load_from_file(prov, file, NULL)) {
                g_object_unref(prov);
                goto end;
        }
        screen = gdk_screen_get_default();

        settings = gtk_settings_get_default();
        g_object_get(settings, "gtk-theme-name", &cur_theme, NULL);

        if (cur_theme && g_str_has_prefix(cur_theme, "Arc")) {
                priority = GTK_STYLE_PROVIDER_PRIORITY_APPLICATION;
        }

        /* Revert the previous style provider */
        if (_theme_style_provider != NULL) {
                gtk_style_context_remove_provider_for_screen(screen,
                                                             GTK_STYLE_PROVIDER(
                                                                 _theme_style_provider));
                _theme_style_provider = NULL;
        }

        gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(prov), priority);
        _theme_style_provider = prov;
end:
        g_free(uri);
        if (file) {
                g_object_unref(file);
        }
}

/**
 * Update our theme based on the currently loaded theme. This enables us to
 * dynamically switch to the high contrast variant
 */
static void slate_theme_changed(void)
{
        GtkSettings *settings = NULL;
        gchar *theme_nom = NULL;

        settings = gtk_settings_get_default();
        g_object_get(settings, "gtk-theme-name", &theme_nom, NULL);

        if (g_str_equal(theme_nom, "HighContrast")) {
                slate_set_theme("theme_hc.css");
        } else {
                slate_set_theme("theme.css");
        }

        g_free(theme_nom);
}

/**
 * Set the theme for the first time, and hook up events to ensure we always
 * set the right internal theme to match, i.e. HighContrast, etc.
 */
static void slate_initialize_theme(void)
{
        GtkSettings *settings = NULL;

        settings = gtk_settings_get_default();

        _gtk_settings_con_id = g_signal_connect(settings,
                                                "notify::gtk-theme-name",
                                                G_CALLBACK(slate_theme_changed),
                                                NULL);
        /* Now update theme settings */
        slate_theme_changed();
}

/**
 * Load our theme assets into the global style context provider
 */
static void slate_load_resources(void)
{
        /* Load our resources in */
        slate_resource_register_resource();
}

/**
 * Unload the theme from the global style context provider
 */
static void slate_unload_resources(void)
{
        GdkScreen *screen = NULL;
        GtkSettings *settings = NULL;

        /* Disconnect the notify on GtkSettings */
        if (_gtk_settings_con_id > 0) {
                settings = gtk_settings_get_default();
                g_signal_handler_disconnect(settings, _gtk_settings_con_id);
        }

        /* No theme loaded? */
        if (!_theme_style_provider) {
                goto resource_unload;
        }

        /* Unload the CSS provider */
        screen = gdk_screen_get_default();
        gtk_style_context_remove_provider_for_screen(screen,
                                                     GTK_STYLE_PROVIDER(_theme_style_provider));
        g_object_unref(_theme_style_provider);
        _theme_style_provider = NULL;

resource_unload:
        /* Ensure to unload resources */
        slate_resource_unregister_resource();
}

__slate_public__ gboolean theme_check_init(unsigned int major, unsigned int minor,
                                           __slate_unused__ unsigned int micro)
{
        /* Micro version may change with devel builds so don't test it */
        if (major == MATE_NOTIFYD_MAJOR_VERSION && minor == MATE_NOTIFYD_MINOR_VERSION) {
                if (!_initial_theme_loaded) {
                        slate_initialize_theme();
                        _initial_theme_loaded = true;
                }
                return TRUE;
        }
        return FALSE;
}

__slate_public__ void get_theme_info(char **theme_name, char **theme_ver, char **author,
                                     char **homepage)
{
        *theme_name = g_strdup("Slate Theme");
        *theme_ver = g_strdup(PACKAGE_VERSION);
        *author = g_strdup("Ikey Doherty");
        *homepage = g_strdup(PACKAGE_URL);
}

/**
 * Convenience wrapper
 */
static inline SlateNotificationWindow *_get_notification_window(GtkWindow *window)
{
        SlateNotificationWindow *real_window =
            g_object_get_data(G_OBJECT(window), "_notification_data");
        return real_window;
}

__slate_public__ GtkWindow *create_notification(UrlClickedCb cb)
{
        return GTK_WINDOW(slate_notification_window_new(cb));
}

__slate_public__ void destroy_notification(GtkWindow *notif_window)
{
        if (!notif_window) {
                return;
        }
        gtk_widget_hide(GTK_WIDGET(notif_window));
        gtk_widget_destroy(GTK_WIDGET(notif_window));
}

__slate_public__ void show_notification(GtkWindow *notif_window)
{
        if (!notif_window) {
                return;
        }
        gtk_widget_show(GTK_WIDGET(notif_window));
}

__slate_public__ void hide_notification(GtkWindow *notif_window)
{
        if (!notif_window) {
                return;
        }
        gtk_widget_hide(GTK_WIDGET(notif_window));
}

__slate_public__ void move_notification(GtkWindow *notif_window, int x, int y)
{
        if (!notif_window) {
                return;
        }
        gtk_window_move(notif_window, x, y);
}

__slate_public__ void set_notification_text(GtkWindow *notif_window, const char *summary,
                                            const char *body)
{
        if (!notif_window) {
                return;
        }
        SlateNotificationWindow *window = _get_notification_window(notif_window);
        slate_notification_window_set_text(window, summary, body);
}

__slate_public__ void set_notification_icon(GtkWindow *notif_window, GdkPixbuf *pixbuf)
{
        if (!notif_window) {
                return;
        }
        SlateNotificationWindow *window = _get_notification_window(notif_window);
        slate_notification_window_set_pixbuf(window, pixbuf);
}

__slate_public__ gboolean get_always_stack(__slate_unused__ GtkWindow *notif_window)
{
        return TRUE;
}

/**
 * mate-notification-daemon always calls this even ifs not supported, so not
 * exposing the method would result in the daemon segfaulting..
 */
__slate_public__ void set_notification_arrow(__slate_unused__ GtkWindow *notif_window,
                                             __slate_unused__ gboolean visible,
                                             __slate_unused__ int x, __slate_unused__ int y)
{
}

/**
 * Currently a no-op as we don't yet have an SuRadialProgress
 */
__slate_public__ void set_notification_timeout(__slate_unused__ GtkWindow *notif_window,
                                               __slate_unused__ glong timeout)
{
}

/**
 * Also waiting for SuRadialProgress, no-op
 */
__slate_public__ void notification_tick(__slate_unused__ GtkWindow *notif_window,
                                        __slate_unused__ glong timeout)
{
}

__slate_public__ void add_notification_action(GtkWindow *notif_window, const char *label,
                                              const char *key, GCallback cb)
{
        if (!notif_window) {
                return;
        }
        SlateNotificationWindow *window = _get_notification_window(notif_window);
        slate_notification_window_add_action(window, label, key, cb);
}

__slate_public__ void clear_notification_actions(GtkWindow *notif_window)
{
        if (!notif_window) {
                return;
        }
        SlateNotificationWindow *window = _get_notification_window(notif_window);
        slate_notification_window_clear_actions(window);
}

__slate_public__ void set_notification_hints(GtkWindow *notif_window, GHashTable *hints)
{
        if (!notif_window) {
                return;
        }
        SlateNotificationWindow *window = _get_notification_window(notif_window);
        slate_notification_window_set_hints(window, hints);
}
/**
 * Hooks for GModule initialisation
 */
__slate_public__ const gchar *g_module_check_init(__attribute__((unused)) GModule *module)
{
        if (!_initial_resource_loaded) {
                slate_load_resources();
                _initial_resource_loaded = true;
        }
        return NULL;
}

__slate_public__ void g_module_unload(__slate_unused__ GModule *module)
{
        slate_unload_resources();
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
