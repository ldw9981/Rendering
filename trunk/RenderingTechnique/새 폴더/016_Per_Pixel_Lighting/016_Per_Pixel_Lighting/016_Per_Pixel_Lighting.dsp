# Microsoft Developer Studio Project File - Name="016_Per_Pixel_Lighting" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=016_Per_Pixel_Lighting - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "016_Per_Pixel_Lighting.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "016_Per_Pixel_Lighting.mak" CFG="016_Per_Pixel_Lighting - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "016_Per_Pixel_Lighting - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "016_Per_Pixel_Lighting - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "016_Per_Pixel_Lighting - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
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

!ELSEIF  "$(CFG)" == "016_Per_Pixel_Lighting - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
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

# Name "016_Per_Pixel_Lighting - Win32 Release"
# Name "016_Per_Pixel_Lighting - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\3ds.cpp
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

SOURCE=.\3ds.h
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
# Begin Group "Vertex Shaders"

# PROP Default_Filter "vs*.txt"
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\vs10_attenuation.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\vs10_attenuation_texture_3D.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\vs10_Diffuse_Specular.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\vs10_Diffuse_Specular_attenuation.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\vs10_Diffuse_Specular_attenuation_for_ps11.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\vs10_Diffuse_Specular_attenuation_for_ps11_cn.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\vs10_Diffuse_Specular_attenuation_texture_3D.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\vs10_Diffuse_Specular_attenuation_texture_3D_for_ps11.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\vs10_Diffuse_Specular_attenuation_texture_3D_for_ps11_cn.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\vs10_Diffuse_Specular_clamp.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\vs10_Diffuse_Specular_with_cubemap_normalizer.txt
# End Source File
# End Group
# Begin Group "Pixel Shader 1.1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps11_1_AD.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps11_2_AD_cube_normalizer.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps11_3_ADS16.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps11_4_ADS16_cube_normalizer.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps11_5_ADSn.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps11_7_attenuation_1.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps11_7_attenuation_2.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps11_7_attenuation_lookup.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps11_8_ADS16_attenuation_1.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps11_8_ADS16_attenuation_2.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps11_8_ADS16_attenuation_texture_3D.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps11_9_ADS16_cn_attenuation_1.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps11_9_ADS16_cn_attenuation_2.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps11_9_ADS16_cn_attenuation_texture_3D.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps12_8_ADS16_attenuation_lookup.txt
# End Source File
# End Group
# Begin Group "Pixel Shader 1.4"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps14_1_AD.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps14_2_AD_cube_normalizer.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps14_3_ADS16.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps14_4_ADS16_cube_normalizer.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps14_5_ADSn.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps14_6_ADSn_cube_normalizer.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps14_7_attenuation_1.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps14_7_attenuation_2.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps14_7_attenuation_lookup.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps14_8_ADS16_attenuation_1.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps14_8_ADS16_attenuation_2.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps14_8_ADS16_attenuation_lookup.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps14_8_ADS16_attenuation_texture_3D.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps14_9_ADS16_cn_attenuation_1.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps14_9_ADS16_cn_attenuation_2.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps14_9_ADS16_cn_attenuation_lookup.txt
# End Source File
# Begin Source File

SOURCE=.\Shaders\016_Per_Pixel_Lighting\ps14_9_ADS16_cn_attenuation_texture_3D.txt
# End Source File
# End Group
# End Target
# End Project
