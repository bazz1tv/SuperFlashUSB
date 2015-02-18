# SuperFlashUSB
USB programmer firmware / PC software for Tototek SNES Flash cart

installation notes:
---------------------
need libusb-1.0, 

QT 5.2 asks for: 
openGL header files (can use freeglut3-dev)
g++

Developer Machine Notes
-----------------------
As of April 2014 
ubuntu 12.04
synaptic package libusb-1.0-dev does not have support for hotplug. So you have to compile libusb yourself. 
If you install into a unique prefix, you must soft link the libusb-1.0.pc file into /usr/lib/pkgconfig or else QTCreator will not see it.
