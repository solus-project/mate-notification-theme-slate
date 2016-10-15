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

#include "config.h"
#include "util.h"

SOLUS_BEGIN_INCLUDES
#include "engine-api.h"
#include "notification-window.h"
#include "theme-resources.h"
SOLUS_END_INCLUDES

#include <stdbool.h>

/**
 * We load and unload this to ensure we cause no problems to other plugins
 */
static GtkCssProvider *_theme_style_provider = NULL;

#define THEME_PREFIX "resource://com/solus-project/mate-notification-daemon-theme-solus"

static gchar *sol_theme_form_theme_path(const gchar *suffix)
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
 * Load our theme assets into the global style context provider
 */
__attribute__((constructor)) static bool sol_theme_load(void)
{
        /* Load our resources in */
        sol_resource_register_resource();
        GFile *file = NULL;
        gchar *uri = NULL;
        GtkCssProvider *prov = NULL;
        GdkScreen *screen = NULL;

        uri = sol_theme_form_theme_path("theme.css");
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

        gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(prov), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        _theme_style_provider = prov;
end:
        g_free(uri);
        if (file) {
                g_object_unref(file);
        }
        if (!_theme_style_provider) {
                return false;
        }
        return true;
}

/**
 * Unload the theme from the global style context provider
 */
__attribute__((destructor)) static void sol_theme_unload(void)
{
        GdkScreen *screen = NULL;

        /* Currently no-op */
        if (!_theme_style_provider) {
                goto resource_unload;
                return;
        }

        screen = gdk_screen_get_default();
        gtk_style_context_remove_provider_for_screen(screen,
                                                     GTK_STYLE_PROVIDER(_theme_style_provider));
        g_object_unref(_theme_style_provider);
        _theme_style_provider = NULL;

resource_unload:
        /* Ensure to unload resources */
        sol_resource_unregister_resource();
}

__solus_public__ gboolean theme_check_init(unsigned int major, unsigned int minor,
                                           __solus_unused__ unsigned int micro)
{
        /* Micro version may change with devel builds so don't test it */
        if (major == MATE_NOTIFYD_MAJOR_VERSION && minor == MATE_NOTIFYD_MINOR_VERSION) {
                return TRUE;
        }
        return FALSE;
}

__solus_public__ void get_theme_info(char **theme_name, char **theme_ver, char **author,
                                     char **homepage)
{
        *theme_name = g_strdup("Solus Theme");
        *theme_ver = g_strdup(PACKAGE_VERSION);
        *author = g_strdup("Ikey Doherty");
        *homepage = g_strdup(PACKAGE_URL);
}

__solus_public__ GtkWindow *create_notification(__solus_unused__ UrlClickedCb cb)
{
        return GTK_WINDOW(sol_notification_window_new());
}

__solus_public__ void destroy_notification(GtkWindow *notif_window)
{
        if (!notif_window) {
                return;
        }
        gtk_widget_destroy(GTK_WIDGET(notif_window));
}

__solus_public__ void show_notification(GtkWindow *notif_window)
{
        if (!notif_window) {
                return;
        }
        gtk_widget_show(GTK_WIDGET(notif_window));
}

__solus_public__ void hide_notification(GtkWindow *notif_window)
{
        if (!notif_window) {
                return;
        }
        gtk_widget_hide(GTK_WIDGET(notif_window));
}

__solus_public__ void move_notification(GtkWindow *notif_window, int x, int y)
{
        if (!notif_window) {
                return;
        }
        gtk_window_move(notif_window, x, y);
}

/**
 * Hooks for GModule initialisation
 */
__solus_public__ const gchar *g_module_check_init(__solus_unused__ GModule *module)
{
        if (!sol_theme_load()) {
                return "Failed to load theme resources";
        }
        return NULL;
}

__solus_public__ void g_module_unload(__solus_unused__ GModule *module)
{
        sol_theme_unload();
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
