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

#pragma once

/**
 * All symbols are hidden by default so must be explicitly be made public
 * to define the ABI
 */
#define __sol_public__ __attribute__((visibility("default")))

/**
 * Helpful in development to suppress compiler warnings for known-unused vars
 */
#define __sol_unused__ __attribute__((unused))


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
