target extended-remote /dev/cu.usbmodemD6D286E1
monitor tpwr enable
monitor swdp_scan
attach 1
set mem inaccessible-by-default off
set print pretty
