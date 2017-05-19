# Microsoft Developer Studio Project File - Name="Spacey 2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Spacey 2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Spacey 2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Spacey 2.mak" CFG="Spacey 2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Spacey 2 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Spacey 2 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Spacey 2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Spacey 2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ddraw.lib dsound.lib dxguid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /profile /debug /machine:I386

!ENDIF 

# Begin Target

# Name "Spacey 2 - Win32 Release"
# Name "Spacey 2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "c,cpp"
# Begin Source File

SOURCE=.\Cdib.cpp
# End Source File
# Begin Source File

SOURCE=.\CLIST.CPP
# End Source File
# Begin Source File

SOURCE=.\Ddtools.cpp
# End Source File
# Begin Source File

SOURCE=.\Drawing.cpp
# End Source File
# Begin Source File

SOURCE=.\Sound\DSTOOLS.CPP
# End Source File
# Begin Source File

SOURCE=.\Frames.cpp
# End Source File
# Begin Source File

SOURCE=.\GameKeys.cpp
# End Source File
# Begin Source File

SOURCE=.\Globals.cpp
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\ShipDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain.cpp
# End Source File
# End Group
# Begin Group "Headder Files"

# PROP Default_Filter "h,hpp"
# Begin Source File

SOURCE=.\Cdib.hpp
# End Source File
# Begin Source File

SOURCE=.\CLIST.HPP
# End Source File
# Begin Source File

SOURCE=.\Ddtools.h
# End Source File
# Begin Source File

SOURCE=.\Drawing.h
# End Source File
# Begin Source File

SOURCE=.\Sound\DSTOOLS.H
# End Source File
# Begin Source File

SOURCE=.\Externals.h
# End Source File
# Begin Source File

SOURCE=.\GameKeys.h
# End Source File
# Begin Source File

SOURCE=.\ShipDraw.h
# End Source File
# Begin Source File

SOURCE=.\Terrain.h
# End Source File
# End Group
# Begin Group "GUI Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Interface\Button.h
# End Source File
# Begin Source File

SOURCE=.\Interface\Caption.h
# End Source File
# Begin Source File

SOURCE=.\Interface\CheckBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\Container.h
# End Source File
# Begin Source File

SOURCE=.\Interface\DigitalCounter.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\DigitalCounter.h
# End Source File
# Begin Source File

SOURCE=.\Interface\Drawable.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\Drawable.h
# End Source File
# Begin Source File

SOURCE=.\Interface\Frame.h
# End Source File
# Begin Source File

SOURCE=.\GameDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\GameDisplay.h
# End Source File
# Begin Source File

SOURCE=.\Interface\GUISpacey.h
# End Source File
# Begin Source File

SOURCE=.\Interface\ListBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\MousePointer.h
# End Source File
# Begin Source File

SOURCE=.\Interface\ProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\ProgressBar.h
# End Source File
# Begin Source File

SOURCE=.\Interface\Scroll.h
# End Source File
# Begin Source File

SOURCE=.\Interface\Text.h
# End Source File
# Begin Source File

SOURCE=.\Interface\TextBox.h
# End Source File
# Begin Source File

SOURCE=.\Interface\Window.h
# End Source File
# End Group
# Begin Group "Enemies"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Enemies\bullet.cpp
# End Source File
# Begin Source File

SOURCE=.\Enemies\Bullet.h
# End Source File
# Begin Source File

SOURCE=.\Enemies\Explosion.cpp
# End Source File
# Begin Source File

SOURCE=.\Enemies\Explosion.h
# End Source File
# Begin Source File

SOURCE=.\Enemies\SprAssassin.cpp
# End Source File
# Begin Source File

SOURCE=.\Enemies\SprAssassin.h
# End Source File
# Begin Source File

SOURCE=.\Enemies\SprAsteroid.cpp
# End Source File
# Begin Source File

SOURCE=.\Enemies\SprAsteroid.h
# End Source File
# Begin Source File

SOURCE=.\Enemies\SprFighter.cpp
# End Source File
# Begin Source File

SOURCE=.\Enemies\SprFighter.h
# End Source File
# Begin Source File

SOURCE=.\Enemies\Sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\Enemies\Sprite.h
# End Source File
# Begin Source File

SOURCE=.\Enemies\SpriteManage.cpp
# End Source File
# Begin Source File

SOURCE=.\Enemies\SpriteManage.h
# End Source File
# Begin Source File

SOURCE=.\Enemies\SprKamikazi.cpp
# End Source File
# Begin Source File

SOURCE=.\Enemies\SprKamikazi.h
# End Source File
# Begin Source File

SOURCE=.\Enemies\SprTransportPlane.cpp
# End Source File
# Begin Source File

SOURCE=.\Enemies\SprTransportPlane.h
# End Source File
# End Group
# Begin Group "Weapons"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Weapons\Charge.cpp
# End Source File
# Begin Source File

SOURCE=.\Weapons\Charge.h
# End Source File
# Begin Source File

SOURCE=.\Weapons\Flack.cpp
# End Source File
# Begin Source File

SOURCE=.\Weapons\Flack.h
# End Source File
# Begin Source File

SOURCE=.\Weapons\HMissile.cpp
# End Source File
# Begin Source File

SOURCE=.\Weapons\HMissile.h
# End Source File
# Begin Source File

SOURCE=.\Weapons\Laser.cpp
# End Source File
# Begin Source File

SOURCE=.\Weapons\Laser.h
# End Source File
# Begin Source File

SOURCE=.\Weapons\Missile.cpp
# End Source File
# Begin Source File

SOURCE=.\Weapons\Missile.h
# End Source File
# Begin Source File

SOURCE=.\Weapons\Spread.cpp
# End Source File
# Begin Source File

SOURCE=.\Weapons\Spread.h
# End Source File
# Begin Source File

SOURCE=.\Weapons\Weapon.h
# End Source File
# Begin Source File

SOURCE=.\Weapons\WeaponManage.cpp
# End Source File
# Begin Source File

SOURCE=.\Weapons\WeaponManage.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Cursor.rc
# End Source File
# Begin Source File

SOURCE=.\Cursor1.cur
# End Source File
# End Target
# End Project
