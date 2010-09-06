# Microsoft Developer Studio Project File - Name="tbm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=tbm - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "とびまりさ.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "とびまりさ.mak" CFG="tbm - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "tbm - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "tbm - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tbm - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"Sun/StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"../tbm/tbm.exe"

!ELSEIF  "$(CFG)" == "tbm - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"Sun/StdAfx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\tbm\tbm.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "tbm - Win32 Release"
# Name "tbm - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Sun Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sun\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sun\Sun3D.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sun\SunApp.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sun\SunConfig.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sun\SunEffect.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sun\SunExeption.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sun\SunFile.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sun\SunFont.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sun\SunInput.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sun\SunLog.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sun\SunMicroThread.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sun\SunRand.cpp

!IF  "$(CFG)" == "tbm - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "tbm - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sun\SunSound.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sun\SunSprite.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sun\SunSprite3D.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sun\SunSurface.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sun\SunTask.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sun\SunTexture.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Sun\SunUtility.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# End Group
# Begin Source File

SOURCE=.\Back.cpp

!IF  "$(CFG)" == "tbm - Win32 Release"

# ADD CPP /Yu"Sun/StdAfx.h"

!ELSEIF  "$(CFG)" == "tbm - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Boss.cpp
# End Source File
# Begin Source File

SOURCE=.\Chen.cpp
# End Source File
# Begin Source File

SOURCE=.\Chirno.cpp
# End Source File
# Begin Source File

SOURCE=.\Danmaku.cpp

!IF  "$(CFG)" == "tbm - Win32 Release"

# ADD CPP /Yu"Sun/StdAfx.h"

!ELSEIF  "$(CFG)" == "tbm - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Effect.cpp

!IF  "$(CFG)" == "tbm - Win32 Release"

# ADD CPP /Yu"Sun/StdAfx.h"

!ELSEIF  "$(CFG)" == "tbm - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Game.cpp

!IF  "$(CFG)" == "tbm - Win32 Release"

# ADD CPP /Yu"Sun/StdAfx.h"

!ELSEIF  "$(CFG)" == "tbm - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Item.cpp

!IF  "$(CFG)" == "tbm - Win32 Release"

# ADD CPP /Yu"sun/stdafx.h"

!ELSEIF  "$(CFG)" == "tbm - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Laser.cpp
# End Source File
# Begin Source File

SOURCE=.\Lety.cpp
# End Source File
# Begin Source File

SOURCE=.\Main.cpp

!IF  "$(CFG)" == "tbm - Win32 Release"

# ADD CPP /Yu"Sun/StdAfx.h"

!ELSEIF  "$(CFG)" == "tbm - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Mystia.cpp
# End Source File
# Begin Source File

SOURCE=.\Player.cpp

!IF  "$(CFG)" == "tbm - Win32 Release"

# ADD CPP /Yu"Sun/StdAfx.h"

!ELSEIF  "$(CFG)" == "tbm - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Replay.cpp
# End Source File
# Begin Source File

SOURCE=.\Rumia.cpp
# End Source File
# Begin Source File

SOURCE=.\Score.cpp
# End Source File
# Begin Source File

SOURCE=.\Shot.cpp

!IF  "$(CFG)" == "tbm - Win32 Release"

# ADD CPP /Yu"Sun/StdAfx.h"

!ELSEIF  "$(CFG)" == "tbm - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stage.cpp

!IF  "$(CFG)" == "tbm - Win32 Release"

# ADD CPP /Yu"Sun/StdAfx.h"

!ELSEIF  "$(CFG)" == "tbm - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stage1.cpp
# End Source File
# Begin Source File

SOURCE=.\Stage2.cpp
# End Source File
# Begin Source File

SOURCE=.\Stage3.cpp
# End Source File
# Begin Source File

SOURCE=.\Stage4.cpp
# End Source File
# Begin Source File

SOURCE=.\Stage5.cpp
# End Source File
# Begin Source File

SOURCE=.\Stage6.cpp
# End Source File
# Begin Source File

SOURCE=.\System.cpp

!IF  "$(CFG)" == "tbm - Win32 Release"

# ADD CPP /Yu"Sun/StdAfx.h"

!ELSEIF  "$(CFG)" == "tbm - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Talk.cpp
# End Source File
# Begin Source File

SOURCE=.\Tama.cpp

!IF  "$(CFG)" == "tbm - Win32 Release"

# ADD CPP /Yu"Sun/StdAfx.h"

!ELSEIF  "$(CFG)" == "tbm - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Title.cpp
# End Source File
# Begin Source File

SOURCE=.\Wriggle.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Sun Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sun\Ayame.h
# End Source File
# Begin Source File

SOURCE=.\Sun\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Sun\Sun3D.h
# End Source File
# Begin Source File

SOURCE=.\Sun\SunApp.h
# End Source File
# Begin Source File

SOURCE=.\Sun\SunConfig.h
# End Source File
# Begin Source File

SOURCE=.\Sun\SunEffect.h
# End Source File
# Begin Source File

SOURCE=.\Sun\SunExeption.h
# End Source File
# Begin Source File

SOURCE=.\Sun\SunFile.h
# End Source File
# Begin Source File

SOURCE=.\Sun\SunFont.h
# End Source File
# Begin Source File

SOURCE=.\Sun\SunFunctionCallback.h
# End Source File
# Begin Source File

SOURCE=.\Sun\SunInput.h
# End Source File
# Begin Source File

SOURCE=.\Sun\SunLog.h
# End Source File
# Begin Source File

SOURCE=.\Sun\SunMicroThread.h
# End Source File
# Begin Source File

SOURCE=.\Sun\SunObject.h
# End Source File
# Begin Source File

SOURCE=.\Sun\SunRand.h
# End Source File
# Begin Source File

SOURCE=.\Sun\SunSound.h
# End Source File
# Begin Source File

SOURCE=.\Sun\SunSprite.h
# End Source File
# Begin Source File

SOURCE=.\Sun\SunSprite3D.h
# End Source File
# Begin Source File

SOURCE=.\Sun\SunSurface.h
# End Source File
# Begin Source File

SOURCE=.\Sun\SunTask.h
# End Source File
# Begin Source File

SOURCE=.\Sun\SunTexture.h
# End Source File
# Begin Source File

SOURCE=.\Sun\SunUtility.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Back.h
# End Source File
# Begin Source File

SOURCE=.\Boss.h
# End Source File
# Begin Source File

SOURCE=.\Chen.h
# End Source File
# Begin Source File

SOURCE=.\Chirno.h
# End Source File
# Begin Source File

SOURCE=.\Danmaku.h
# End Source File
# Begin Source File

SOURCE=.\Effect.h
# End Source File
# Begin Source File

SOURCE=.\Game.h
# End Source File
# Begin Source File

SOURCE=.\Item.h
# End Source File
# Begin Source File

SOURCE=.\Laser.h
# End Source File
# Begin Source File

SOURCE=.\Lety.h
# End Source File
# Begin Source File

SOURCE=.\Main.h
# End Source File
# Begin Source File

SOURCE=.\Material.h
# End Source File
# Begin Source File

SOURCE=.\Mystia.h
# End Source File
# Begin Source File

SOURCE=.\Player.h
# End Source File
# Begin Source File

SOURCE=.\Replay.h
# End Source File
# Begin Source File

SOURCE=.\Rumia.h
# End Source File
# Begin Source File

SOURCE=.\Score.h
# End Source File
# Begin Source File

SOURCE=.\Shot.h
# End Source File
# Begin Source File

SOURCE=.\Stage.h
# End Source File
# Begin Source File

SOURCE=.\Stage1.h
# End Source File
# Begin Source File

SOURCE=.\Stage2.h
# End Source File
# Begin Source File

SOURCE=.\Stage3.h
# End Source File
# Begin Source File

SOURCE=.\Stage4.h
# End Source File
# Begin Source File

SOURCE=.\Stage5.h
# End Source File
# Begin Source File

SOURCE=.\Stage6.h
# End Source File
# Begin Source File

SOURCE=.\SunInclude.h
# End Source File
# Begin Source File

SOURCE=.\System.h
# End Source File
# Begin Source File

SOURCE=.\Talk.h
# End Source File
# Begin Source File

SOURCE=.\Tama.h
# End Source File
# Begin Source File

SOURCE=.\Title.h
# End Source File
# Begin Source File

SOURCE=.\Wriggle.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Sun\resource.h
# End Source File
# Begin Source File

SOURCE=.\Sun\resource.rc
# End Source File
# End Group
# End Target
# End Project
