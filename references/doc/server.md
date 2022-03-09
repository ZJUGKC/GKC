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

# X11

1. Install X11:

	```
	sudo apt-get install libx11-dev
	```

1. Edit the following file:

	```
	sudo vi /etc/X11/Xsession.options
	```

	and change `use-ssh-agent` to `no-use-ssh-agent`.

1. Include the header files:

	```
	#include <X11/Xlib.h>
	#include <X11/Xutil.h>
	#include <X11/Xatom.h>
	```

# Server node for rendering

## Windows

Install the latest version of Windows server.

Change the port number of Terminal Server:

1. Open `regedit` tool.

1. Change the value of `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Terminal Server\Wds\rdpwd\Tds\tcp\PortNumber`.

1. Change the value of `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Terminal Server\WinStations\RDP-Tcp\PortNumber`.

1. Change the value of `Port=` in `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\SharedAccess\Defaults\FirewallPolicy\FirewallRules\RemoteDesktop-UserMode-In-TCP`.

1. Change the value of `Port=` in `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\SharedAccess\Defaults\FirewallPolicy\FirewallRules\RemoteDesktop-UserMode-In-UDP`.

1. Change the value of `Port=` in `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\SharedAccess\Parameters\FirewallPolicy\FirewallRules\RemoteDesktop-UserMode-In-TCP`.

1. Change the value of `Port=` in `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\SharedAccess\Parameters\FirewallPolicy\FirewallRules\RemoteDesktop-UserMode-In-UDP`.

1. Use `host:port` to connect the Terminal Server.

## Linux

Install the latest version of Ubuntu Server.

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
