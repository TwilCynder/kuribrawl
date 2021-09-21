Name "Kuribrawl"

Outfile "../release/Kuribrawl-setup.exe"

InstallDir $EXEDIR

Page directory
Page instfiles

Section
    SetOutPath $INSTDIR

    File "..\release\c++\*"
SectionEnd