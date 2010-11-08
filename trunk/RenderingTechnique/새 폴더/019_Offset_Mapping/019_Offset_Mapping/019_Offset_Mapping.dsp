# Microsoft Developer Studio Project File - Name="019_Offset_Mapping" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=019_Offset_Mapping - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "019_Offset_Mapping.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "019_Offset_Mapping.mak" CFG="019_Offset_Mapping - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "019_Offset_Mapping - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "019_Offset_Mapping - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "019_Offset_Mapping - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41b /d "NDEBUG"
# ADD RSC /l 0x41b /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "019_Offset_Mapping - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41b /d "_DEBUG"
# ADD RSC /l 0x41b /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "019_Offset_Mapping - Win32 Release"
# Name "019_Offset_Mapping - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AseFile.cpp
# End Source File
# Begin Source File

SOURCE=.\AsePpl.cpp
# End Source File
# Begin Source File

SOURCE=.\AsePplVso.cpp
# End Source File
# Begin Source File

SOURCE=.\camera.cpp
# End Source File
# Begin Source File

SOURCE=.\CubeTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawDXScene.cpp
# End Source File
# Begin Source File

SOURCE=.\Font_bitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\Frustum.cpp
# End Source File
# Begin Source File

SOURCE=.\Image.cpp
# End Source File
# Begin Source File

SOURCE=.\Image3D.cpp
# End Source File
# Begin Source File

SOURCE=.\init.cpp
# End Source File
# Begin Source File

SOURCE=.\Menu.cpp
# End Source File
# Begin Source File

SOURCE=.\Plane_VNTTB.cpp
# End Source File
# Begin Source File

SOURCE=.\Scene.cpp
# End Source File
# Begin Source File

SOURCE=.\SphereVN.cpp
# End Source File
# Begin Source File

SOURCE=.\texture.cpp
# End Source File
# Begin Source File

SOURCE=.\texture3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Timer.cpp
# End Source File
# Begin Source File

SOURCE=.\Util.cpp
# End Source File
# Begin Source File

SOURCE=.\jpeglibmm.lib
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AseFile.h
# End Source File
# Begin Source File

SOURCE=.\AsePpl.h
# End Source File
# Begin Source File

SOURCE=.\AsePplVso.h
# End Source File
# Begin Source File

SOURCE=.\camera.h
# End Source File
# Begin Source File

SOURCE=.\CubeTexture.h
# End Source File
# Begin Source File

SOURCE=.\Font_bitmap.h
# End Source File
# Begin Source File

SOURCE=.\Frustum.h
# End Source File
# Begin Source File

SOURCE=.\GLizer.h
# End Source File
# Begin Source File

SOURCE=.\Image.h
# End Source File
# Begin Source File

SOURCE=.\Image3D.h
# End Source File
# Begin Source File

SOURCE=.\init.h
# End Source File
# Begin Source File

SOURCE=.\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\Menu.h
# End Source File
# Begin Source File

SOURCE=.\Plane_VNTTB.h
# End Source File
# Begin Source File

SOURCE=.\Scene.h
# End Source File
# Begin Source File

SOURCE=.\SphereVN.h
# End Source File
# Begin Source File

SOURCE=.\texture.h
# End Source File
# Begin Source File

SOURCE=.\texture3D.h
# End Source File
# Begin Source File

SOURCE=.\Timer.h
# End Source File
# Begin Source File

SOURCE=.\Util.h
# End Source File
# Begin Source File

SOURCE=.\Vector.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# End Group
# Begin Group "Shader ps11"

# PROP Default_Filter ""
# Begin Group "Pixel Shader"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\plane\ps11_bump.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\plane\ps11_bump_OffsetMapping_1_pass.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\plane\ps11_bump_OffsetMapping_2_pass.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\plane\ps11_bump_OffsetMapping_3_pass.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\plane\ps11_OffsetMapping.txt
# End Source File
# End Group
# Begin Group "Vertex Shader"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\plane\vs11_for_ps11_bump.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\plane\vs11_for_ps11_bump_OffsetMapping_1_pass.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\plane\vs11_for_ps11_bump_OffsetMapping_2_pass.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\plane\vs11_for_ps11_bump_OffsetMapping_3_pass.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\plane\vs11_for_ps11_OffsetMapping.txt
# End Source File
# End Group
# End Group
# Begin Group "Shader ps14"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\plane\ps14_bump.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\plane\ps14_bump_OffsetMapping.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\plane\ps14_OffsetMapping.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\plane\vs11_for_ps14.txt
# End Source File
# End Group
# Begin Group "Shader room"

# PROP Default_Filter ""
# Begin Group "ps 11"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\room\ps11_room.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\room\ps11_room_with_ambient.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\room\vs11_room_ps11.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\room\vs11_room_ps11_with_optimalization.txt
# End Source File
# End Group
# Begin Group "ps 14"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\room\ps14_room.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\room\ps14_room_OffsetMapping.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\room\ps14_room_OffsetMapping_with_ambient.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\room\ps14_room_with_ambient.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\room\vs11_room_ps14.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\room\vs11_room_ps14_with_optimalization.txt
# End Source File
# End Group
# Begin Group "ps 14 for AsePpl class"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\room\ps14_room_OffsetMapping_for_AsePpl_class.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\room\ps14_room_OffsetMapping_for_AsePpl_class_with_ambient.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\019_Offset_Mapping\room\vs11_room_for_AsePpl_class.txt
# End Source File
# End Group
# End Group
# End Target
# End Project
