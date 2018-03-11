include config.mk

all:
	(cd appmenugen && make all)
	(cd movemousecursor && make all)
	(cd s && make all)
	(cd sysmonitor && make all)
	(cd traybattery && make all)
	(cd traykeymap && make all)
	(cd traymonitor && make all)
	(cd trayscript && make all)
	(cd trayusermount && make all)
	(cd trayvolume && make all)
	(cd trayweather && make all)
	(cd slock && make all)
	(cd xidletools && make all)
	(cd poweroffdialog && make all)

clean:
	(cd appmenugen && make clean)
	(cd movemousecursor && make clean)
	(cd libonce && make clean)
	(cd s && make clean)
	(cd sysmonitor && make clean)
	(cd traybattery && make clean)
	(cd traykeymap && make clean)
	(cd traymonitor && make clean)
	(cd trayscript && make clean)
	(cd trayusermount && make clean)
	(cd trayvolume && make clean)
	(cd trayweather && make clean)
	(cd slock && make clean)
	(cd xidletools && make clean)
	(cd poweroffdialog && make clean)

install:
	@echo Installing to ${PREFIX}
	mkdir -p ${PREFIX}
	mkdir -p ${PREFIX}/bin
	mkdir -p ${PREFIX}/share/jwmtools/icons
	cp -f appmenugen/appmenugen ${PREFIX}/bin/
	cp -f movemousecursor/movemousecursor ${PREFIX}/bin/
	cp -f sysmonitor/sysmonitor ${PREFIX}/bin/
	cp -f sysmonitor/sysmonitor-cpu ${PREFIX}/bin/
	cp -f traybattery/traybattery ${PREFIX}/bin/
	cp -f traybattery/traybattery.glade ${PREFIX}/share/jwmtools/
	cp -f traykeymap/traykeymap ${PREFIX}/bin/
	cp -f traykeymap/traykeymap.glade2 ${PREFIX}/share/jwmtools/
	cp -f traykeymap/traykeymap.glade3 ${PREFIX}/share/jwmtools/
	cp -f traymonitor/traymonitor ${PREFIX}/bin/
	cp -f traymonitor/traymonitor.glade ${PREFIX}/share/jwmtools/
	cp -f trayscript/trayscript ${PREFIX}/bin/
	cp -f trayscript/trayscript.sh ${PREFIX}/bin/
	cp -f trayscript/trayscript.glade ${PREFIX}/share/jwmtools/
	cp -f trayusermount/trayusermount ${PREFIX}/bin/
	cp -f trayusermount/trayusermount.glade2 ${PREFIX}/share/jwmtools/
	cp -f trayusermount/trayusermount.glade3 ${PREFIX}/share/jwmtools/
	cp -f trayvolume/alsavolumeup ${PREFIX}/bin/
	cp -f trayvolume/alsavolumedown ${PREFIX}/bin/
	cp -f trayvolume/pavolumeup ${PREFIX}/bin/
	cp -f trayvolume/pavolumedown ${PREFIX}/bin/
	cp -f trayvolume/trayvolume-alsa ${PREFIX}/bin/
	cp -f trayvolume/trayvolume-pulse ${PREFIX}/bin/
	cp -f trayvolume/trayvolume.glade2 ${PREFIX}/share/jwmtools/
	cp -f trayvolume/trayvolume.glade3 ${PREFIX}/share/jwmtools/
	cp -f trayweather/trayweather ${PREFIX}/bin/
	cp -f trayweather/trayweather.glade ${PREFIX}/share/jwmtools/
	cp -f slock/slock ${PREFIX}/bin/
	cp -f poweroffdialog/poweroffdialog ${PREFIX}/bin/
	cp -f poweroffdialog/poweroffdialog-simple ${PREFIX}/bin/
	chmod 775 ${PREFIX}/bin/poweroffdialog-simple
	cp -f poweroffdialog/poweroffdialog-simple ${PREFIX}/bin/
	cp -f poweroffdialog/poweroffdialog.glade2 ${PREFIX}/share/jwmtools/
	cp -f poweroffdialog/poweroffdialog.glade3 ${PREFIX}/share/jwmtools/
	chmod +s ${PREFIX}/bin/slock
	cp -f xidletools/xidletime ${PREFIX}/bin/
	cp -f xidletools/xidlerun ${PREFIX}/bin/
	cp -af icons/* ${PREFIX}/share/jwmtools/icons/
	chmod +s `which blkid`
	@echo
	@echo IMPORTANT: Following programs has now set suid bit:
	@echo
	@echo "  ${PREFIX}/bin/slock"
	@echo "  `which blkid`"
	@echo
