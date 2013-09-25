jwmtools
========

Various tools for jwm window manager.

Tray applets
------------

- **traybattery** displays battery status (charge in percents and status icon)
![](https://github.com/kostelnik/jwmtools/raw/master/doc/traybattery.png)

- **traykeymap** displays current keyboard layout.
![](https://github.com/kostelnik/jwmtools/raw/master/doc/traykeymap.png)

- **traymonitor** applet displays system load (cpu, load, memory, swap, network, 
disk). It uses console tool sysmonitor (sources included) as backend for 
obtaining these data.
![](https://github.com/kostelnik/jwmtools/raw/master/doc/traymonitor.png)
                         
- **trayscript** applet displays output of any (bash) script. Special, user defined,
script is executed every 30s and it's output is displayed in tray. This
script can contain whatever you want to be displayed on tray, e.g. 
`echo "Hello John"` or `df -h / | tail -1 | awk '{ print $4"B free" }'` for
displaying amount of free space on disk. Main purpose of this applet is to
quickly display information for which regular applet do not exist yet. For most
cases this approach is good enough.
![](https://github.com/kostelnik/jwmtools/raw/master/doc/trayscript.png)

- **trayusermount** applet is for mounting and unmounting removable devices such as USB flash
disks. It uses pmount, pumount and blkid console tools.
![](https://github.com/kostelnik/jwmtools/raw/master/doc/trayusermount.png) ![](https://github.com/kostelnik/jwmtools/raw/master/doc/trayusermount2.png)

- **trayvolume** applet is for changing master audio volume. Both ALSA and Pulse audio are
supported. Volume is regulated using mouse wheel.
![](https://github.com/kostelnik/jwmtools/raw/master/doc/trayvolume.png)

- **trayweather** is applet for displaying weather information using METAR data (clouds, wind, 
temperature, pressure, visibility).
![](https://github.com/kostelnik/jwmtools/raw/master/doc/trayweather1.png)

Backend tools
-------------

- **appmenugen** parse all *.desktop files and generate menu with categories. Output is xml
file that can be used as part of JWM config file using this code:

    <!-- application menu -->
    <RootMenu height="24" onroot="6">
        <Include>exec:/opt/jwmtools/0.1/bin/appmenugen</Include>
    </RootMenu>

![](https://github.com/kostelnik/jwmtools/raw/master/doc/appmenugen.png)

- **movemousecursor** is tool which move cursor to bottom right corner of the screen (or any other
location specified by first and second parameter)

- **s** is auxiliary "library" for manipulating strings. Made by C non-programmer for C 
non-programmers. I created it mostly to fight leaks and n-1 bugs.

- **sysmonitor** print various statistics (cpu, memory, disks, network). It is backend for
traymonitor. Traymonitor could read those info directly, but it very rarely
and inexplainably segfaulted while doing so. So it instead run sysmonitor. 
When sysmonitor crash, nothing will happen to traymonitor itself, it will 
just display info in next refresh.

