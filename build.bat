@echo off
pbcompiler.exe src\main.pb -e "build\Kuribrawl.exe" /ICON "GraphicDesignIsMyPassion/icon.ico"
pbcompiler.exe "gamefile manager\manager.pb" -e "src/res/dataFileMaker.exe" /ICON "GraphicDesignIsMyPassion/iconDev.ico" /CONSOLE
if "%1"=="-d" src\res\dataFileMaker.exe src\res -s
copy src\res\dataFileMaker.exe build\Modding
copy src\res\data.twl build
pbcompiler.exe "frame tool\frameTool.pb" -e "build\Modding\FrameTool.exe" /ICON "GraphicDesignIsMyPassion/iconFT.ico" /CONSTANT Build=1
cd doc/public
call build.bat
echo "==============FINISHED=============="