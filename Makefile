include config.mk

all:
	(cd appmenugen && make all)
	(cd movemousecursor && make all)
	(cd s && make all)
	(cd sysmonitor && make all)
	(cd traykeymap && make all)
	(cd traymonitor && make all)
	(cd trayscript && make all)
	(cd trayusermount && make all)
	(cd trayvolume && make all)
	(cd trayweather && make all)
	(cd slock && make all)

clean:
	(cd appmenugen && make clean)
	(cd movemousecursor && make clean)
	(cd s && make clean)
	(cd sysmonitor && make clean)
	(cd traykeymap && make clean)
	(cd traymonitor && make clean)
	(cd trayscript && make clean)
	(cd trayusermount && make clean)
	(cd trayvolume && make clean)
	(cd trayweather && make clean)
	(cd slock && make clean)

install:
	@echo Installing to ${PREFIX}
	mkdir -p ${PREFIX}
	mkdir -p ${PREFIX}/bin
	mkdir -p ${PREFIX}/share/jwmtools
	cp appmenugen/appmenugen ${PREFIX}/bin/
	cp movemousecursor/movemousecursor ${PREFIX}/bin/
	cp sysmonitor/sysmonitor ${PREFIX}/bin/
	cp traykeymap/traykeymap ${PREFIX}/bin/
	cp traykeymap/traykeymap.glade ${PREFIX}/share/jwmtools/
	cp traymonitor/traymonitor ${PREFIX}/bin/
	cp traymonitor/traymonitor.glade ${PREFIX}/share/jwmtools/
	cp trayscript/trayscript ${PREFIX}/bin/
	cp trayscript/trayscript.glade ${PREFIX}/share/jwmtools/
	cp trayusermount/trayusermount ${PREFIX}/bin/
	cp trayusermount/trayusermount.glade ${PREFIX}/share/jwmtools/
	cp trayvolume/alsavolumeup ${PREFIX}/bin/
	cp trayvolume/alsavolumedown ${PREFIX}/bin/
	cp trayvolume/pavolumeup ${PREFIX}/bin/
	cp trayvolume/pavolumedown ${PREFIX}/bin/
	cp trayvolume/trayvolume-alsa ${PREFIX}/bin/
	cp trayvolume/trayvolume-pulse ${PREFIX}/bin/
	cp trayvolume/trayvolume.glade ${PREFIX}/share/jwmtools/
	cp trayweather/trayweather ${PREFIX}/bin/
	cp trayweather/trayweather.glade ${PREFIX}/share/jwmtools/
	cp slock/slock ${PREFIX}/bin/
	chmod +s ${PREFIX}/bin/slock
	
	
	
	
	
	
