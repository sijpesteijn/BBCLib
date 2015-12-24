The BeagleBone has six on-board serial ports.<br>
<br>
On the BeagleBone Black, it's only the /dev/ttyO0 that is enabled by default, and it is coupled to the serial console. The other serial ports must be enabled before they can be used.<br>
<br>
To enable the uarts 1,2,4 and/or 5 on the BeagleBone Black, rev A, B and C:<br>
Rev A/B: Open the file /media/BEAGLEBONE/uEnv.txt in an editor (vim/nano)<br>
Rev C: Open the file /boot/uEnv.txt in an editor (vim/nano)<br>
Add the key "cape_enable="<br>
Add the ports you want to enable, comma separated (BB-UART1, BB-UART2. BB-UART4, BB-UART5)<br>
Add the key "cape_disable"<br>
And add the BB-BONELT-HDMI and BB-BONELT-HDMIN<br>
Reboot<br>
An example line looks like this:<br>
<pre><code>
root@beaglebone:/dev# cat /boot/uEnv.txt<br>
cape_enable=capemgr.enable_partno=BB-UART1<br>
cape_disable=capemgr.disable_partno=BB-BONELT-HDMI,BB-BONELT-HDMIN<br>
</code></pre>
<br>
<br>
After reboot, the device is present in the device list:<br>
<pre><code>
 root@beaglebone:/dev# ls -l /dev/ttyO*<br>
 crw-rw---- 1 root tty     249, 0 Jan  1 01:18 /dev/ttyO0<br>
 crw-rw---- 1 root dialout 249, 4 Jan  1 01:18 /dev/ttyO4<br>
</code></pre>
<br>
The UARTs map to pins and devices like this:<br>
<br>
<pre><code>
 RX  TX  CTS   RTS   Device  Remark<br>
 UART0     J1_4  J1_5          /dev/ttyO0 BeagleBone Black only<br>
 UART1   P9_26   P9_24   P9_20   P9_19   /dev/ttyO1  <br>
 UART2   P9_22   P9_21   P8_37   P8_38   /dev/ttyO2  <br>
 UART3     P9_42   P8_36   P8_34   /dev/ttyO3  TX only<br>
 UART4   P9_11   P9_13   P8_35   P8_33   /dev/ttyO4  <br>
 UART5   P8_38   P8_37   P8_31   P8_32   /dev/ttyO5<br>  
</pre></code>

<img src="uart.png" width="200px"><br>