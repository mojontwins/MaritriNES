@echo off
if [%1]==[justcompile] goto :justcompile

..\..\utils\mkts.exe mode=pals pals=..\gfx\paltslib.png out=work\palbglib.h label=pal_bg_lib silent
..\..\utils\mkts.exe mode=pals pals=..\gfx\palsslib.png out=work\palfglib.h label=pal_fg_lib silent
..\..\utils\mkts.exe mode=pals pals=..\gfx\paltsgym.png out=work\palbggym.h label=pal_bg_gym silent
..\..\utils\mkts.exe mode=pals pals=..\gfx\palssgym.png out=work\palfggym.h label=pal_fg_gym silent
copy /b work\palbglib.h + work\palfglib.h + work\palbggym.h + work\palfggym.h assets\palettes.h

cd ..\gfx
..\..\utils\mkts.exe mode=scripted in=cut_patterns.spt out=..\dev\tileset.chr

cd ..\map
..\..\utils\pemapcnv3.exe level_lib.map ..\dev\assets\map_lib.h lib
..\..\utils\pemapcnv3.exe level_gym.map ..\dev\assets\map_gym.h gym

:justcompile
cd ..\dev

cc65 -Oi game.c --add-source
ca65 crt0-pantanow.s -o crt0.o
ca65 game.s
ld65 -v -C nes.cfg -o maritrini.nes crt0.o game.o runtime.lib -Ln labels.txt

rem del game.s

