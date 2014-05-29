# Microsoft Developer Studio Generated NMAKE File, Based on ttf_parser.dsp
!IF $(CFG)" == "
CFG=ttf_parser - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ttf_parser - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ttf_parser - Win32 Release" && "$(CFG)" != "ttf_parser - Win32 Debug"
!MESSAGE 指定的配置 "$(CFG)" 无效.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF $(OS)" == "Windows_NT
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ttf_parser - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# 开始自定义宏
OutDir=.\Release
# 结束自定义宏

ALL : "$(OUTDIR)\ttf_parser.dll"


CLEAN :
	-@erase "$(INTDIR)\Character_To_Glyph_Mapping_Table.obj"
	-@erase "$(INTDIR)\Font_Header.obj"
	-@erase "$(INTDIR)\Glyph_Data.obj"
	-@erase "$(INTDIR)\Horizontal_Header.obj"
	-@erase "$(INTDIR)\Horizontal_Metrics.obj"
	-@erase "$(INTDIR)\Index_To_Location.obj"
	-@erase "$(INTDIR)\Maximum_Profile.obj"
	-@erase "$(INTDIR)\Mem_Stream.obj"
	-@erase "$(INTDIR)\Naming_Table.obj"
	-@erase "$(INTDIR)\OS_2_And_Windows_Metrics.obj"
	-@erase "$(INTDIR)\Post_Script.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\True_Type_Font.obj"
	-@erase "$(INTDIR)\TTF_Table.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ttf_parser.dll"
	-@erase "$(OUTDIR)\ttf_parser.exp"
	-@erase "$(OUTDIR)\ttf_parser.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TTF_PARSER_EXPORTS" /Fp"$(INTDIR)\ttf_parser.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ttf_parser.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\ttf_parser.pdb" /machine:I386 /out:"$(OUTDIR)\ttf_parser.dll" /implib:"$(OUTDIR)\ttf_parser.lib" 
LINK32_OBJS= \
	"$(INTDIR)\Character_To_Glyph_Mapping_Table.obj" \
	"$(INTDIR)\Font_Header.obj" \
	"$(INTDIR)\Glyph_Data.obj" \
	"$(INTDIR)\Horizontal_Header.obj" \
	"$(INTDIR)\Horizontal_Metrics.obj" \
	"$(INTDIR)\Index_To_Location.obj" \
	"$(INTDIR)\Maximum_Profile.obj" \
	"$(INTDIR)\Mem_Stream.obj" \
	"$(INTDIR)\Naming_Table.obj" \
	"$(INTDIR)\OS_2_And_Windows_Metrics.obj" \
	"$(INTDIR)\Post_Script.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\True_Type_Font.obj" \
	"$(INTDIR)\TTF_Table.obj"

"$(OUTDIR)\ttf_parser.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ttf_parser - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# 开始自定义宏
OutDir=.\Debug
# 结束自定义宏

ALL : "$(OUTDIR)\ttf_parser.dll" "$(OUTDIR)\ttf_parser.bsc"


CLEAN :
	-@erase "$(INTDIR)\Character_To_Glyph_Mapping_Table.obj"
	-@erase "$(INTDIR)\Character_To_Glyph_Mapping_Table.sbr"
	-@erase "$(INTDIR)\Font_Header.obj"
	-@erase "$(INTDIR)\Font_Header.sbr"
	-@erase "$(INTDIR)\Glyph_Data.obj"
	-@erase "$(INTDIR)\Glyph_Data.sbr"
	-@erase "$(INTDIR)\Horizontal_Header.obj"
	-@erase "$(INTDIR)\Horizontal_Header.sbr"
	-@erase "$(INTDIR)\Horizontal_Metrics.obj"
	-@erase "$(INTDIR)\Horizontal_Metrics.sbr"
	-@erase "$(INTDIR)\Index_To_Location.obj"
	-@erase "$(INTDIR)\Index_To_Location.sbr"
	-@erase "$(INTDIR)\Maximum_Profile.obj"
	-@erase "$(INTDIR)\Maximum_Profile.sbr"
	-@erase "$(INTDIR)\Mem_Stream.obj"
	-@erase "$(INTDIR)\Mem_Stream.sbr"
	-@erase "$(INTDIR)\Naming_Table.obj"
	-@erase "$(INTDIR)\Naming_Table.sbr"
	-@erase "$(INTDIR)\OS_2_And_Windows_Metrics.obj"
	-@erase "$(INTDIR)\OS_2_And_Windows_Metrics.sbr"
	-@erase "$(INTDIR)\Post_Script.obj"
	-@erase "$(INTDIR)\Post_Script.sbr"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\stdafx.sbr"
	-@erase "$(INTDIR)\True_Type_Font.obj"
	-@erase "$(INTDIR)\True_Type_Font.sbr"
	-@erase "$(INTDIR)\TTF_Table.obj"
	-@erase "$(INTDIR)\TTF_Table.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ttf_parser.bsc"
	-@erase "$(OUTDIR)\ttf_parser.dll"
	-@erase "$(OUTDIR)\ttf_parser.exp"
	-@erase "$(OUTDIR)\ttf_parser.ilk"
	-@erase "$(OUTDIR)\ttf_parser.lib"
	-@erase "$(OUTDIR)\ttf_parser.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TTF_PARSER_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ttf_parser.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ttf_parser.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Character_To_Glyph_Mapping_Table.sbr" \
	"$(INTDIR)\Font_Header.sbr" \
	"$(INTDIR)\Glyph_Data.sbr" \
	"$(INTDIR)\Horizontal_Header.sbr" \
	"$(INTDIR)\Horizontal_Metrics.sbr" \
	"$(INTDIR)\Index_To_Location.sbr" \
	"$(INTDIR)\Maximum_Profile.sbr" \
	"$(INTDIR)\Mem_Stream.sbr" \
	"$(INTDIR)\Naming_Table.sbr" \
	"$(INTDIR)\OS_2_And_Windows_Metrics.sbr" \
	"$(INTDIR)\Post_Script.sbr" \
	"$(INTDIR)\stdafx.sbr" \
	"$(INTDIR)\True_Type_Font.sbr" \
	"$(INTDIR)\TTF_Table.sbr"

"$(OUTDIR)\ttf_parser.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ttf_parser.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\ttf_parser.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ttf_parser.dll" /implib:"$(OUTDIR)\ttf_parser.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Character_To_Glyph_Mapping_Table.obj" \
	"$(INTDIR)\Font_Header.obj" \
	"$(INTDIR)\Glyph_Data.obj" \
	"$(INTDIR)\Horizontal_Header.obj" \
	"$(INTDIR)\Horizontal_Metrics.obj" \
	"$(INTDIR)\Index_To_Location.obj" \
	"$(INTDIR)\Maximum_Profile.obj" \
	"$(INTDIR)\Mem_Stream.obj" \
	"$(INTDIR)\Naming_Table.obj" \
	"$(INTDIR)\OS_2_And_Windows_Metrics.obj" \
	"$(INTDIR)\Post_Script.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\True_Type_Font.obj" \
	"$(INTDIR)\TTF_Table.obj"

"$(OUTDIR)\ttf_parser.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ttf_parser.dep")
!INCLUDE "ttf_parser.dep"
!ELSE 
!MESSAGE Warning: cannot find "ttf_parser.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ttf_parser - Win32 Release" || "$(CFG)" == "ttf_parser - Win32 Debug"
SOURCE=.\Character_To_Glyph_Mapping_Table.cpp

!IF  "$(CFG)" == "ttf_parser - Win32 Release"


"$(INTDIR)\Character_To_Glyph_Mapping_Table.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ttf_parser - Win32 Debug"


"$(INTDIR)\Character_To_Glyph_Mapping_Table.obj"	"$(INTDIR)\Character_To_Glyph_Mapping_Table.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Font_Header.cpp

!IF  "$(CFG)" == "ttf_parser - Win32 Release"


"$(INTDIR)\Font_Header.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ttf_parser - Win32 Debug"


"$(INTDIR)\Font_Header.obj"	"$(INTDIR)\Font_Header.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Glyph_Data.cpp

!IF  "$(CFG)" == "ttf_parser - Win32 Release"


"$(INTDIR)\Glyph_Data.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ttf_parser - Win32 Debug"


"$(INTDIR)\Glyph_Data.obj"	"$(INTDIR)\Glyph_Data.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Horizontal_Header.cpp

!IF  "$(CFG)" == "ttf_parser - Win32 Release"


"$(INTDIR)\Horizontal_Header.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ttf_parser - Win32 Debug"


"$(INTDIR)\Horizontal_Header.obj"	"$(INTDIR)\Horizontal_Header.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Horizontal_Metrics.cpp

!IF  "$(CFG)" == "ttf_parser - Win32 Release"


"$(INTDIR)\Horizontal_Metrics.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ttf_parser - Win32 Debug"


"$(INTDIR)\Horizontal_Metrics.obj"	"$(INTDIR)\Horizontal_Metrics.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Index_To_Location.cpp

!IF  "$(CFG)" == "ttf_parser - Win32 Release"


"$(INTDIR)\Index_To_Location.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ttf_parser - Win32 Debug"


"$(INTDIR)\Index_To_Location.obj"	"$(INTDIR)\Index_To_Location.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Maximum_Profile.cpp

!IF  "$(CFG)" == "ttf_parser - Win32 Release"


"$(INTDIR)\Maximum_Profile.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ttf_parser - Win32 Debug"


"$(INTDIR)\Maximum_Profile.obj"	"$(INTDIR)\Maximum_Profile.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Mem_Stream.cpp

!IF  "$(CFG)" == "ttf_parser - Win32 Release"


"$(INTDIR)\Mem_Stream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ttf_parser - Win32 Debug"


"$(INTDIR)\Mem_Stream.obj"	"$(INTDIR)\Mem_Stream.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Naming_Table.cpp

!IF  "$(CFG)" == "ttf_parser - Win32 Release"


"$(INTDIR)\Naming_Table.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ttf_parser - Win32 Debug"


"$(INTDIR)\Naming_Table.obj"	"$(INTDIR)\Naming_Table.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\OS_2_And_Windows_Metrics.cpp

!IF  "$(CFG)" == "ttf_parser - Win32 Release"


"$(INTDIR)\OS_2_And_Windows_Metrics.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ttf_parser - Win32 Debug"


"$(INTDIR)\OS_2_And_Windows_Metrics.obj"	"$(INTDIR)\OS_2_And_Windows_Metrics.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Post_Script.cpp

!IF  "$(CFG)" == "ttf_parser - Win32 Release"


"$(INTDIR)\Post_Script.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ttf_parser - Win32 Debug"


"$(INTDIR)\Post_Script.obj"	"$(INTDIR)\Post_Script.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\stdafx.cpp

!IF  "$(CFG)" == "ttf_parser - Win32 Release"


"$(INTDIR)\stdafx.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ttf_parser - Win32 Debug"


"$(INTDIR)\stdafx.obj"	"$(INTDIR)\stdafx.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\True_Type_Font.cpp

!IF  "$(CFG)" == "ttf_parser - Win32 Release"


"$(INTDIR)\True_Type_Font.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ttf_parser - Win32 Debug"


"$(INTDIR)\True_Type_Font.obj"	"$(INTDIR)\True_Type_Font.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\TTF_Table.cpp

!IF  "$(CFG)" == "ttf_parser - Win32 Release"


"$(INTDIR)\TTF_Table.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ttf_parser - Win32 Debug"


"$(INTDIR)\TTF_Table.obj"	"$(INTDIR)\TTF_Table.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

