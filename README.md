mate-notification-theme-slate
=============================

A CSS-backed notification theme for the MATE Desktop.

### Arc Styling

The fallback styling for this plugin emulates the Arc-styled Budgie notifications. Style credit belongs solely with [horst3180](https://github.com/horst3180/arc-theme) for his design work and the
implementation within [Budgie](https://github.com/solus-project/budgie-desktop).

### Themeable

This plugin supports CSS styling, and now has support within the [Adapta](https://github.com/adapta-project/adapta-gtk-theme) GTK theme, so
can be extended to suit any style.

mate-notification-theme-slate is a [Solus project](https://getsol.us/)

![logo](https://build.getsol.us/logo.png)

![screenshot](https://raw.githubusercontent.com/solus-project/mate-notification-theme-slate/master/landing.png)

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

    git clone https://github.com/solus-project/mate-notification-theme-slate.git
```

Now build it:
```bash

    cd mate-notification-theme-slate
    ./autogen.sh --prefix=/usr
    make -j$(($(getconf _NPROCESSORS_ONLN)+1))
    sudo make install
```

License
-------

Copyright © 2016-2017 Ikey Doherty <ikey@solus-project.com>, Solus Project

mate-notification-theme-slate is available under the terms of the `LGPL-2.1` license
