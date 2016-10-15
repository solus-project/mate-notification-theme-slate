mate-notification-theme-solus
=============================

A MATE Notification Daemon theme, emulating the Arc-styled Budgie notifications
of Solus. Style credit belongs solely with [horst3180](https://github.com/horst3180/arc-theme) for his design work and the
implementation within [Budgie](https://github.com/solus-project/budgie-desktop).

This is a re-implementation for MATE, borrowing a subset of the Budgie theme,
and providing a clean + stable theme engine.

mate-notification-theme-solus is a [Solus project](https://solus-project.com/)

![logo](https://build.solus-project.com/logo.png)

![screenshot](https://raw.githubusercontent.com/solus-project/mate-notification-theme-solus/master/landing.png)

Building
--------
Ensure the following build dependencies are available (`pkg-config` names)

    - gtk+-3.0 >= 3.18.0

To install these on Solus:

```bash

    sudo eopkg it glib2-devel libgtk-3-devel
    sudo eopkg it -c system.devel
```

Clone the repository:

```bash

    git clone https://github.com/solus-project/mate-notification-theme-solus.git
```

Now build it:
```bash

    cd mate-notification-theme-solus
    ./autogen.sh --prefix=/usr
    make -j$(($(getconf _NPROCESSORS_ONLN)+1))
    sudo make install
```

Future
------

We'd like to see `mate-notification-daemon` enhanced, so we'll work with upstream
to add new features. Specifically, this theme has built-in handling for `action-icons`
functionality, but we need to send a pull request upstream to enable all of the
existing themes to support this, and to broadcast the functionality in the `d-bus`
service.

License
-------

Copyright © 2016 Ikey Doherty <ikey@solus-project.com>, Solus Project

mate-notification-theme-solus is available under the terms of the `LGPL-2.1` license
