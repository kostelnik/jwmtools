# jwmtools version
VERSION = 0.1

# customize this path if you want jwmtools to install elsewhere
PREFIX = /opt/jwmtools/${VERSION}
#PREFIX = /usr
#PREFIX = /usr/local

# if your glib does not have GDateTime and related stuff yet, set following line to 1
# but then trayweather applet will not recognize day and night (will always show day)
# or upgrade your glib to >= 2.26
VERY_OLD_GLIB = 0

# path to acpi battery info, used by traybattery applet, this path 
# must contain files "charge_now", "charge_full" and "status"
SYS_BATTERY_PATH = /sys/class/power_supply/BAT0/
