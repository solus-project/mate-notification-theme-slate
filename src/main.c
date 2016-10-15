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

#include <stdlib.h>

#include "util.h"

SOLUS_BEGIN_INCLUDES
#include "engine-api.h"
SOLUS_END_INCLUDES

int main(int argc, char **argv)
{
        gtk_init(&argc, &argv);
        GtkWindow *window = NULL;
        GdkScreen *screen = NULL;
        GdkRectangle rect = { 0 };
        gint primary_monitor = 0;

        if (!theme_check_init(MATE_NOTIFYD_MAJOR_VERSION, MATE_NOTIFYD_MINOR_VERSION, 0)) {
                fprintf(stderr, "Failed to initialise theme engine\n");
                return EXIT_FAILURE;
        }

        /* Create notification */
        window = create_notification(NULL);

        /* Placement */
        screen = gdk_screen_get_default();
        primary_monitor = gdk_screen_get_primary_monitor(screen);
        gdk_screen_get_monitor_geometry(screen, primary_monitor, &rect);
        move_notification(window, (rect.x + (rect.width - 400) - 10), rect.y + 10);

        show_notification(window);

        gtk_main();

        destroy_notification(window);

        return EXIT_FAILURE;
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
