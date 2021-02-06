Temperature control for rack
============================

Read temperature and set the FAN accordly
-----------------------------------------
Read temperature via DS18B20  
Set the speed of the Fan accordly.  
Up to 16W / 12v  

### Attiny84
SPI Through BusPirate 
arduino framework 

Tips
----
`pio run` process/build project from the current directory  
`pio run -t upload` - upload firmware to a target  
`pio run -t clean` - clean project (remove compiled files)  
`pio run -t upload -e uno` upload only for arduino uno  
`pio platform update --dry-run` check for updates  

![plot](./hardware/T2101_rack_control.jpg)
