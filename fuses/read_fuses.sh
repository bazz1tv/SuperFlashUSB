#!/bin/bash

avrdude -c usbtiny -p atmega16u2 -U efuse:r:fuse_e.txt:b
avrdude -c usbtiny -p atmega16u2 -U hfuse:r:fuse_h.txt:b
avrdude -c usbtiny -p atmega16u2 -U lfuse:r:fuse_l.txt:b