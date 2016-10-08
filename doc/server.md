<!--
#
# Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
# All rights reserved.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the 2-Clause BSD License.
#
# Author contact information:
#   yxxinyuan@zju.edu.cn
#
-->

# Server node for rendering

## Windows

Install Windows server 2012 R2.

Change the port number of Terminal Server:

1. Open `regedit` tool.

1. Change the value of `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Terminal Server\Wds\rdpwd\Tds\tcp\PortNumber`.

1. Change the value of `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Terminal Server\WinStations\RDP-Tcp\PortNumber`.

1. Change the value of `Port=` in `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\services\SharedAccess\Defaults\FirewallPolicy\FirewallRules\RemoteDesktop-UserMode-In-TCP`.

1. Change the value of `Port=` in `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\services\SharedAccess\Defaults\FirewallPolicy\FirewallRules\RemoteDesktop-UserMode-In-UDP`.

1. Change the value of `Port=` in `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\services\SharedAccess\Parameters\FirewallPolicy\FirewallRules\RemoteDesktop-UserMode-In-TCP`.

1. Change the value of `Port=` in `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\services\SharedAccess\Parameters\FirewallPolicy\FirewallRules\RemoteDesktop-UserMode-In-UDP`.

1. Use `host:port` to connect the Terminal Server.

## Linux

Install Ubuntu Server 16.04.

Install packages:

```
sudo apt-get install xserver-xorg-video-dummy
sudo apt-get install xpra
```

Add user to groups:

```
sudo adduser <user> tty
sudo adduser <user> video
sudo adduser <user> dialout
```

Reboot the server, login to the server remotely (e.g., via ssh) and start the xpra:

```
xpra --xvfb="Xorg -noreset -nolisten tcp +extension GLX -config /etc/xpra/xorg.conf +extension RANDR +extension RENDER -logfile ${HOME}/.xpra/Xorg-10.log" start :100
```

If that works, place this same xvfb option in global config file /etc/xpra/xpra.conf.
