jwmtools
========

Various tools for jwm window manager.

Tray applets
------------

![](https://github.com/kostelnik/jwmtools/raw/master/doc/traykeymap.png) **traykeymap**
Tray applet for displaying current keyboard layout.

![](https://github.com/kostelnik/jwmtools/raw/master/doc/traymonitor.png) **traymonitor**
Tray applet for displaying system load (cpu, load, memory, swap, network, 
disk). It uses console tool sysmonitor (sources included) as backend for 
obtaining these data.
                         
![](https://github.com/kostelnik/jwmtools/raw/master/doc/trayscript.png) **trayscript**
Tray applet for displaying output of any (bash) script. Special, user defined,
script is executed every 30s and it's output is displayed in tray. This
script can contain whatever you want to be displayed on tray, e.g. 
`echo "Hello John"` or `df -h / | tail -1 | awk '{ print $4"B free" }'` for
displaying amount of free space on disk. Main purpose of this applet is to
quickly display information for which regular applet do not exist yet. For most
cases this approach is good enough.

![](https://github.com/kostelnik/jwmtools/raw/master/doc/trayusermount.png) ![](https://github.com/kostelnik/jwmtools/raw/master/doc/trayusermount2.png) **trayusermount**
Tray applet for mounting and unmounting removable devices such as USB flash
disks. It uses pmount, pumount and blkid console tools. --> bug #1

![](https://github.com/kostelnik/jwmtools/raw/master/doc/trayvolume.png) **trayvolume**
Tray applet for changing master audio volume. Both ALSA and Pulse audio are
supported. Volume is regulated using mouse wheel. --> bug #2

![](https://github.com/kostelnik/jwmtools/raw/master/doc/trayweather1.png) **trayweather**
Applet for displaying weather information using METAR data (clouds, wind, 
temperature, pressure, visibility). --> bug #3

Backend tools
-------------

![](https://github.com/kostelnik/jwmtools/raw/master/doc/appmenugen.png) **appmenugen**
Parse all *.desktop files and generate menu with categories. Output is xml
file that can be used as part of JWM config file using this code:

    <!-- application menu -->
    <RootMenu height="24" onroot="6">
        <Include>exec:/opt/jwmtools/0.1/bin/appmenugen</Include>
    </RootMenu>

**movemousecursor**
This tool move cursor to bottom right corner of the screen (or any other
location specified by first and second parameter) --> bug #4

**s**
Auxiliary library for manipulating strings. From C non-programmer to C 
non-programmers. I created it mostly to fight leaks and n-1 bugs.

**sysmonitor**
Print various statistics (cpu, memory, disks, network). It is backend for
traymonitor. Traymonitor could read those info directly, but it very rarely
and inexplainably segfaulted while doing so. So it instead run sysmonitor. 
When sysmonitor crash, nothing will happen to traymonitor itself, it will 
just display info in next refresh.

History
-------

- 2012/10/12 - Pushed to github.
- 2012/10/07 - Moved to git.
