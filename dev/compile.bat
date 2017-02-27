@echo off
if [%1]==[justcompile] goto :justcompile

..\..\utils\mkts.exe mode=pals pals=..\gfx\palts0.png out=work\palbg0.h label=mypal_game_bg0 silent
..\..\utils\mkts.exe mode=pals pals=..\gfx\palss0.png out=work\palfg0.h label=mypal_game_fg0 silent
copy /b work\palbg0.h + work\palfg0.h assets\palettes.h

cd ..\gfx
..\..\utils\mkts.exe mode=scripted in=cut_patterns.spt out=..\dev\tileset.chr

cd ..\map
..\..\utils\pemapcnv3.exe level0.map ..\dev\assets\map0.h 0

:justcompile
cd ..\dev

cc65 -Oi game.c --add-source
ca65 crt0-pantanow.s -o crt0.o
ca65 game.s
ld65 -v -C nes.cfg -o maritrini.nes crt0.o game.o runtime.lib -Ln labels.txt

rem del game.s

