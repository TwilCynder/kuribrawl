@echo off
call markdown modding.md -s modding.css -t "Kuribrawl Modding Guide" > moddingRaw.html
cssAppend moddingRaw.html modding.css ..\build\Modding\modding.html