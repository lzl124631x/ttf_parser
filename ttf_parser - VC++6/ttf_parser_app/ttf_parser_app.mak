# Microsoft Developer Studio Generated NMAKE File, Based on ttf_parser_app.dsp
!IF $(CFG)" == "
CFG=ttf_parser_app - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ttf_parser_app - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ttf_parser_app - Win32 Release" && "$(CFG)" != "ttf_parser_app - Win32 Debug"
!MESSAGE 指定的配置 "$(CFG)" 无效.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ttf_parser_app.mak" CFG="ttf_parser_app - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ttf_parser_app - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ttf_parser_app - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF $(OS)" == "Windows_NT
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ttf_parser_app - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# 开始自定义宏
OutDir=.\Release
# 结束自定义宏

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ttf_parser_app.exe"

!ELSE 

ALL : "ttf_parser - Win32 Release" "$(OUTDIR)\ttf_parser_app.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ttf_parser - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\ttf_parser_app.obj"
	-@erase "$(INTDIR)\ttf_parser_app.pch"
	-@erase "$(INTDIR)\ttf_parser_app.res"
	-@erase "$(INTDIR)\ttf_parser_appDlg.obj"
	-@erase "$(INTDIR)\ttf_parser_gdiplus.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ttf_parser_app.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\ttf_parser_app.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\ttf_parser_app.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ttf_parser_app.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\ttf_parser_app.pdb" /machine:I386 /out:"$(OUTDIR)\ttf_parser_app.exe" 
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ttf_parser_app.obj" \
	"$(INTDIR)\ttf_parser_appDlg.obj" \
	"$(INTDIR)\ttf_parser_gdiplus.obj" \
	"$(INTDIR)\ttf_parser_app.res" \
	"..\ttf_parser\Release\ttf_parser.lib"

"$(OUTDIR)\ttf_parser_app.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ttf_parser_app - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# 开始自定义宏
OutDir=.\Debug
# 结束自定义宏

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ttf_parser_app.exe"

!ELSE 

ALL : "ttf_parser - Win32 Debug" "$(OUTDIR)\ttf_parser_app.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"ttf_parser - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\ttf_parser_app.obj"
	-@erase "$(INTDIR)\ttf_parser_app.pch"
	-@erase "$(INTDIR)\ttf_parser_app.res"
	-@erase "$(INTDIR)\ttf_parser_appDlg.obj"
	-@erase "$(INTDIR)\ttf_parser_gdiplus.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ttf_parser_app.exe"
	-@erase "$(OUTDIR)\ttf_parser_app.ilk"
	-@erase "$(OUTDIR)\ttf_parser_app.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\ttf_parser_app.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\ttf_parser_app.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ttf_parser_app.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ttf_parser.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\ttf_parser_app.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ttf_parser_app.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ttf_parser_app.obj" \
	"$(INTDIR)\ttf_parser_appDlg.obj" \
	"$(INTDIR)\ttf_parser_gdiplus.obj" \
	"$(INTDIR)\ttf_parser_app.res" \
	"..\ttf_parser\Debug\ttf_parser.lib"

"$(OUTDIR)\ttf_parser_app.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ttf_parser_app.dep")
!INCLUDE "ttf_parser_app.dep"
!ELSE 
!MESSAGE Warning: cannot find "ttf_parser_app.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ttf_parser_app - Win32 Release" || "$(CFG)" == "ttf_parser_app - Win32 Debug"
SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ttf_parser_app - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\ttf_parser_app.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ttf_parser_app.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ttf_parser_app - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\ttf_parser_app.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ttf_parser_app.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\ttf_parser_app.cpp

"$(INTDIR)\ttf_parser_app.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ttf_parser_app.pch"


SOURCE=.\ttf_parser_app.rc

"$(INTDIR)\ttf_parser_app.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ttf_parser_appDlg.cpp

"$(INTDIR)\ttf_parser_appDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ttf_parser_app.pch"


SOURCE=.\ttf_parser_gdiplus.cpp

"$(INTDIR)\ttf_parser_gdiplus.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ttf_parser_app.pch"


!IF  "$(CFG)" == "ttf_parser_app - Win32 Release"

"ttf_parser - Win32 Release" : 
   cd "\Program Files\VC++6.0\MyProjects\ttf_parser\ttf_parser"
   $(MAKE) /$(MAKEFLAGS) /F ".\ttf_parser.mak" CFG="ttf_parser - Win32 Release" 
   cd "..\ttf_parser_app"

"ttf_parser - Win32 ReleaseCLEAN" : 
   cd "\Program Files\VC++6.0\MyProjects\ttf_parser\ttf_parser"
   $(MAKE) /$(MAKEFLAGS) /F ".\ttf_parser.mak" CFG="ttf_parser - Win32 Release" RECURSE=1 CLEAN 
   cd "..\ttf_parser_app"

!ELSEIF  "$(CFG)" == "ttf_parser_app - Win32 Debug"

"ttf_parser - Win32 Debug" : 
   cd "\Program Files\VC++6.0\MyProjects\ttf_parser\ttf_parser"
   $(MAKE) /$(MAKEFLAGS) /F ".\ttf_parser.mak" CFG="ttf_parser - Win32 Debug" 
   cd "..\ttf_parser_app"

"ttf_parser - Win32 DebugCLEAN" : 
   cd "\Program Files\VC++6.0\MyProjects\ttf_parser\ttf_parser"
   $(MAKE) /$(MAKEFLAGS) /F ".\ttf_parser.mak" CFG="ttf_parser - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\ttf_parser_app"

!ENDIF 


!ENDIF 

