/*
 * This file is part of mate-notification-theme-solus
 *
 * Copyright © 2016 Ikey Doherty <ikey@solus-project.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "config.h"
#include "util.h"

SOLUS_BEGIN_INCLUDES
#include "engine-api.h"
SOLUS_END_INCLUDES

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
