# Microsoft Developer Studio Project File - Name="Hanlib5" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Hanlib5 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Hanlib5.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Hanlib5.mak" CFG="Hanlib5 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Hanlib5 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Hanlib5 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Hanlib5 - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x404 /d "NDEBUG"
# ADD RSC /l 0x404 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Hanlib5 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x404 /d "_DEBUG"
# ADD RSC /l 0x404 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\debug\*.lib ..\lib
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Hanlib5 - Win32 Release"
# Name "Hanlib5 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ALLGET.C
# End Source File
# Begin Source File

SOURCE=.\big5e.c
# End Source File
# Begin Source File

SOURCE=.\CHOICE.C
# End Source File
# Begin Source File

SOURCE=.\CHUIN.C
# End Source File
# Begin Source File

SOURCE=.\CMP2PIN.C
# End Source File
# Begin Source File

SOURCE=.\COMPRESS.C
# End Source File
# Begin Source File

SOURCE=.\ESC.C
# End Source File
# Begin Source File

SOURCE=.\Haninif.c
# End Source File
# Begin Source File

SOURCE=.\LD_FUN.C
# End Source File
# Begin Source File

SOURCE=.\MATCH.C
# End Source File
# Begin Source File

SOURCE=.\MD_FUN.C
# End Source File
# Begin Source File

SOURCE=.\OS2API.C
# End Source File
# Begin Source File

SOURCE=.\PININ.C
# End Source File
# Begin Source File

SOURCE=.\REAL.C
# End Source File
# Begin Source File

SOURCE=.\RULE.C
# End Source File
# Begin Source File

SOURCE=.\SCREN.C
# End Source File
# Begin Source File

SOURCE=.\SHOW_PIN.C
# End Source File
# Begin Source File

SOURCE=.\UD_FUN.C
# End Source File
# Begin Source File

SOURCE=.\UNIT.C
# End Source File
# Begin Source File

SOURCE=.\UP_DOWN.C
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
