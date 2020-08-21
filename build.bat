@echo off
pbcompiler.exe src\main.pb -e "build\Kuribrawl.exe" /ICON "GraphicDesignIsMyPassion/icon.ico"
src\res\dataFileMaker.exe src\res -s
copy src\res\data.twl build
pbcompiler.exe "frame tool\frameTool.pb" -e "build\Modding\FrameTool.exe" /ICON "GraphicDesignIsMyPassion/iconFT.ico"
pbcompiler.exe "gamefile manager\manager.pb" -e "build\Modding\DataFileManager.exe" /ICON "GraphicDesignIsMyPassion/iconDev.ico"
markdown doc\modding.md > build\modding\modding.html
echo "==============FINISHED=============="