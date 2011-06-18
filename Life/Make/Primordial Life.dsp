# Microsoft Developer Studio Project File - Name="Primordial Life" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Primordial Life - Win32 Saver
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Primordial Life.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Primordial Life.mak" CFG="Primordial Life - Win32 Saver"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Primordial Life - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Primordial Life - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Primordial Life - Win32 Saver" (based on "Win32 (x86) Application")
!MESSAGE "Primordial Life - Win32 RSaver" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Primordial Life", BAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Primordial Life - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Build\Release"
# PROP Intermediate_Dir "..\..\Build\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MT /W4 /GR /GX /Zi /O2 /I "..\source" /I "." /I "..\..\zlib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Winmm.lib zlibdll.lib /nologo /subsystem:windows /profile /machine:I386 /out:"..\..\Bin\Release/Primordial Life.exe" /libpath:"..\..\zlib"

!ELSEIF  "$(CFG)" == "Primordial Life - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\Bin\Debug"
# PROP BASE Intermediate_Dir "..\Bin\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Build\Debug"
# PROP Intermediate_Dir "..\..\Build\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"..\source\stdafx.h" /c
# ADD CPP /nologo /MTd /W4 /Gm /GR /GX /ZI /Od /I "..\..\GraphicsServer\Include" /I "..\source" /I "." /I "..\..\zlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_DEBUG_BRAIN" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 zlibdll.lib Winmm.lib /nologo /subsystem:windows /pdb:"..\..\Output\Debug/Primordial Life.pdb" /debug /machine:I386 /out:"..\..\Bin\Debug/Primordial Life.exe" /libpath:"..\..\zlib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Primordial Life - Win32 Saver"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Primordial_Life___Win32_Saver"
# PROP BASE Intermediate_Dir "Primordial_Life___Win32_Saver"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Build\Debug_Saver"
# PROP Intermediate_Dir "..\..\Build\Debug_Saver"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\source" /I "." /I "..\..\GraphicsServer\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"..\source\stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I ".\GraphicsServer\Include" /I "..\source" /I "." /I "..\..\zlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Winmm.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 zlibdll.lib Winmm.lib /nologo /subsystem:windows /pdb:"..\..\Output\Debug_Saver/Primordial Life.pdb" /debug /machine:I386 /out:"C:\Windows\System\Primordial Life.scr" /libpath:"..\..\zlib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Primordial Life - Win32 RSaver"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Primordial_Life___Win32_RSaver"
# PROP BASE Intermediate_Dir "Primordial_Life___Win32_RSaver"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Build\Release_Saver"
# PROP Intermediate_Dir "..\..\Build\Release_Saver"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /GX /O2 /I "..\source" /I "." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"..\source\stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MT /W4 /GR /GX /O2 /I "..\source" /I "." /I "..\..\zlib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Winmm.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 Winmm.lib zlibdll.lib /nologo /subsystem:windows /pdb:"..\..\Output\Release_Saver/R Primordial Life.pdb" /machine:I386 /out:"C:\Windows\System\R Primordial Life.scr" /libpath:"..\..\zlib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Primordial Life - Win32 Release"
# Name "Primordial Life - Win32 Debug"
# Name "Primordial Life - Win32 Saver"
# Name "Primordial Life - Win32 RSaver"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\source\AboutWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\source\BiotEditor.cpp
# End Source File
# Begin Source File

SOURCE=..\Source\Biots.cpp
# End Source File
# Begin Source File

SOURCE=..\Source\Brain.cpp
# End Source File
# Begin Source File

SOURCE=..\source\Breeding.cpp
# End Source File
# Begin Source File

SOURCE=..\Source\Connect.cpp
# End Source File
# Begin Source File

SOURCE=..\Source\Environ.cpp
# End Source File
# Begin Source File

SOURCE=..\source\EnvStatsWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\Source\Etools.cpp
# End Source File
# Begin Source File

SOURCE=..\Source\Evolve.cpp
# End Source File
# Begin Source File

SOURCE=..\source\GeneralView.cpp
# End Source File
# Begin Source File

SOURCE=..\Source\Genotype.cpp
# End Source File
# Begin Source File

SOURCE=..\source\GlobalPalette.cpp
# End Source File
# Begin Source File

SOURCE=..\source\Graph1.cpp
# End Source File
# Begin Source File

SOURCE=..\source\HelpPropertySheet.cpp
# End Source File
# Begin Source File

SOURCE=..\source\InitialPopulation.cpp
# End Source File
# Begin Source File

SOURCE=..\Source\KeyRegistry.cpp
# End Source File
# Begin Source File

SOURCE=..\source\LoadBitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\source\MagnifyWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\source\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\source\NetworkDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\source\Palette.cpp
# End Source File
# Begin Source File

SOURCE=..\source\Picture.cpp
# End Source File
# Begin Source File

SOURCE=..\source\PLifeDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\source\PLifeView.cpp
# End Source File
# Begin Source File

SOURCE=..\Source\PostData.cpp
# End Source File
# Begin Source File

SOURCE=..\source\PrimCmdLine.cpp
# End Source File
# Begin Source File

SOURCE="..\source\Primordial Life.cpp"
# End Source File
# Begin Source File

SOURCE="..\source\Primordial Life.rc"
# End Source File
# Begin Source File

SOURCE=..\Source\RAND.CPP
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=..\Source\Registry.cpp
# End Source File
# Begin Source File

SOURCE=..\source\SaverView.cpp
# End Source File
# Begin Source File

SOURCE=..\source\Settings.cpp
# End Source File
# Begin Source File

SOURCE=..\source\SmallView.cpp
# End Source File
# Begin Source File

SOURCE=..\Source\Sock.cpp
# End Source File
# Begin Source File

SOURCE=..\source\Stat.cpp
# End Source File
# Begin Source File

SOURCE=..\source\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\Source\vector.cpp
# End Source File
# Begin Source File

SOURCE=..\source\WorldSize.cpp
# End Source File
# Begin Source File

SOURCE=..\Source\ZipFile.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\source\BiotEditor.h
# End Source File
# Begin Source File

SOURCE=..\Source\Biots.h
# End Source File
# Begin Source File

SOURCE=..\Source\Brain.h
# End Source File
# Begin Source File

SOURCE=..\source\Breeding.h
# End Source File
# Begin Source File

SOURCE=..\Source\Connect.h
# End Source File
# Begin Source File

SOURCE=..\Source\Environ.h
# End Source File
# Begin Source File

SOURCE=..\source\EnvStatsWnd.h
# End Source File
# Begin Source File

SOURCE=..\Source\Etools.h
# End Source File
# Begin Source File

SOURCE=.\source\evolve.h
# End Source File
# Begin Source File

SOURCE=..\source\GeneralView.h
# End Source File
# Begin Source File

SOURCE=..\Source\Genotype.h
# End Source File
# Begin Source File

SOURCE=..\source\Graph1.h
# End Source File
# Begin Source File

SOURCE=..\source\HelpPropertySheet.h
# End Source File
# Begin Source File

SOURCE=..\source\InitialPopulation.h
# End Source File
# Begin Source File

SOURCE=..\Source\KeyRegistry.h
# End Source File
# Begin Source File

SOURCE=..\source\MagnifyWnd.h
# End Source File
# Begin Source File

SOURCE=..\source\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\Source\MfnZFile.h
# End Source File
# Begin Source File

SOURCE=..\source\NetworkDlg.h
# End Source File
# Begin Source File

SOURCE=..\source\Picture.h
# End Source File
# Begin Source File

SOURCE=..\source\PLifeDoc.h
# End Source File
# Begin Source File

SOURCE=..\source\PLifeView.h
# End Source File
# Begin Source File

SOURCE=..\Source\PostData.h
# End Source File
# Begin Source File

SOURCE=..\source\PrimCmdLine.h
# End Source File
# Begin Source File

SOURCE="..\source\Primordial Life.h"
# End Source File
# Begin Source File

SOURCE=..\Source\Rand.h
# End Source File
# Begin Source File

SOURCE=.\source\Registry.h
# End Source File
# Begin Source File

SOURCE=..\source\SaverView.h
# End Source File
# Begin Source File

SOURCE=..\source\Settings.h
# End Source File
# Begin Source File

SOURCE=..\source\SmallView.h
# End Source File
# Begin Source File

SOURCE=..\Source\Sock.h
# End Source File
# Begin Source File

SOURCE=..\source\Stat.h
# End Source File
# Begin Source File

SOURCE=..\source\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Source\vector.h
# End Source File
# Begin Source File

SOURCE=..\source\WorldSize.h
# End Source File
# Begin Source File

SOURCE=..\Source\ZipFile.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\Res\asexual.ico
# End Source File
# Begin Source File

SOURCE=..\Res\bisexual.ico
# End Source File
# Begin Source File

SOURCE=..\Res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=..\Res\bottom.bmp
# End Source File
# Begin Source File

SOURCE=..\Res\bottom.ico
# End Source File
# Begin Source File

SOURCE=..\Res\cure.cur
# End Source File
# Begin Source File

SOURCE=..\Res\edit.cur
# End Source File
# Begin Source File

SOURCE=..\Res\Evolve.ico
# End Source File
# Begin Source File

SOURCE=..\Res\feed.cur
# End Source File
# Begin Source File

SOURCE=..\Res\female.ico
# End Source File
# Begin Source File

SOURCE=..\Res\Key.ico
# End Source File
# Begin Source File

SOURCE=..\res\left.ico
# End Source File
# Begin Source File

SOURCE=..\Res\load.cur
# End Source File
# Begin Source File

SOURCE=..\Res\Magnify.cur
# End Source File
# Begin Source File

SOURCE=..\Res\male.ico
# End Source File
# Begin Source File

SOURCE=..\res\PLifeDoc.ico
# End Source File
# Begin Source File

SOURCE=..\Res\relocate.cur
# End Source File
# Begin Source File

SOURCE=..\res\right.ico
# End Source File
# Begin Source File

SOURCE=..\Res\save.cur
# End Source File
# Begin Source File

SOURCE=..\Res\terminate.cur
# End Source File
# Begin Source File

SOURCE=..\Res\top.bmp
# End Source File
# Begin Source File

SOURCE=..\res\top.ico
# End Source File
# Begin Source File

SOURCE=..\Res\world.ico
# End Source File
# End Group
# Begin Source File

SOURCE=..\Res\Environment.pal
# End Source File
# Begin Source File

SOURCE=..\Res\InternetExplorer.pal
# End Source File
# End Target
# End Project
# Section Primordial Life : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowser.h
# 	2:16:DefaultSinkClass:CWebBrowser
# End Section
# Section Primordial Life : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:CWebBrowser
# 	2:10:HeaderFile:webbrowser.h
# 	2:8:ImplFile:webbrowser.cpp
# End Section
