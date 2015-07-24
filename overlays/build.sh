#!/bin/bash

echo "Compiling the overlay from .dts to .dtbo"

dtc -O dtb -o BBCLIB-SPI0-00A0.dtbo -b 0 -@ BBCLIB-SPI0.dts 
dtc -O dtb -o BBCLIB-SPI1-00A0.dtbo -b 0 -@ BBCLIB-SPI1.dts 