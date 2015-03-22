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
sc create GkcHostSvc binPath= "C:\program files\.GKC\GKC-1.0.1-win64\public\assemblies\GkcHostSvc-1-0.exe" DisplayName= "GKC Host" type= own start= auto obj= LocalSystem
sc start GkcHostSvc
sc query GkcHostSvc
