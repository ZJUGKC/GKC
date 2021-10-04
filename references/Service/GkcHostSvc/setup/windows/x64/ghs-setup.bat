rem 
rem Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
rem All rights reserved.
rem
rem This program is free software; you can redistribute it and/or
rem modify it under the terms of the 2-Clause BSD License.
rem
rem Author contact information:
rem   yxxinyuan@zju.edu.cn
rem
rem create service
@ECHO OFF
IF {%2} == {} GOTO localsys
IF {%3} == {} GOTO errorexit
sc create GkcHostSvc binPath= %1 DisplayName= "GKC Host" type= own start= auto obj= %2 password= %3
@GOTO nextstep
:localsys
sc create GkcHostSvc binPath= %1 DisplayName= "GKC Host" type= own start= auto obj= LocalSystem
:nextstep
@ECHO ON
ping -n 1 127.0.0.1 > nul
sc start GkcHostSvc
ping -n 3 127.0.0.1 > nul
sc query GkcHostSvc
@GOTO :normalexit
:errorexit
echo "ERROR: password is necessary"
:normalexit
@ECHO ON
