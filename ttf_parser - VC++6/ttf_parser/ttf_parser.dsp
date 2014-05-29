# Microsoft Developer Studio Project File - Name="ttf_parser" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ttf_parser - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ttf_parser.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ttf_parser.mak" CFG="ttf_parser - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ttf_parser - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ttf_parser - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ttf_parser - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../ttf_parser_app/Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TTF_PARSER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TTF_PARSER_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "ttf_parser - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../ttf_parser_app/Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TTF_PARSER_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /Zd /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TTF_PARSER_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ttf_parser - Win32 Release"
# Name "ttf_parser - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Character_To_Glyph_Mapping_Table.cpp
# End Source File
# Begin Source File

SOURCE=.\Font_Header.cpp
# End Source File
# Begin Source File

SOURCE=.\Glyph_Data.cpp
# End Source File
# Begin Source File

SOURCE=.\Horizontal_Header.cpp
# End Source File
# Begin Source File

SOURCE=.\Horizontal_Metrics.cpp
# End Source File
# Begin Source File

SOURCE=.\Index_To_Location.cpp
# End Source File
# Begin Source File

SOURCE=.\Maximum_Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\Mem_Stream.cpp
# End Source File
# Begin Source File

SOURCE=.\Naming_Table.cpp
# End Source File
# Begin Source File

SOURCE=.\OS_2_And_Windows_Metrics.cpp
# End Source File
# Begin Source File

SOURCE=.\Post_Script.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# End Source File
# Begin Source File

SOURCE=.\True_Type_Font.cpp
# End Source File
# Begin Source File

SOURCE=.\TTF_Table.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Character_To_Glyph_Mapping_Table.h
# End Source File
# Begin Source File

SOURCE=.\Font_Header.h
# End Source File
# Begin Source File

SOURCE=.\Glyph_Data.h
# End Source File
# Begin Source File

SOURCE=.\Horizontal_Header.h
# End Source File
# Begin Source File

SOURCE=.\Horizontal_Metrics.h
# End Source File
# Begin Source File

SOURCE=.\Index_To_Location.h
# End Source File
# Begin Source File

SOURCE=.\Maximum_Profile.h
# End Source File
# Begin Source File

SOURCE=.\Mem_Stream.h
# End Source File
# Begin Source File

SOURCE=.\Naming_Table.h
# End Source File
# Begin Source File

SOURCE=.\OS_2_And_Windows_Metrics.h
# End Source File
# Begin Source File

SOURCE=.\Post_Script.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\True_Type_Font.h
# End Source File
# Begin Source File

SOURCE=.\TTF_Table.h
# End Source File
# Begin Source File

SOURCE=.\Type.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
