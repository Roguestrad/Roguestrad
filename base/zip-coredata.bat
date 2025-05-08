set filename=_rbdoom_core.pk4
7z a -tzip %filename% generated/ -x!generated/images/env/ -x!generated/maps/ -x!generated/collision/ -x!generated/rendermodels/
7z a -tzip %filename% maps/game/*_extra_ents.map
7z a -tzip %filename% renderprogs2
7z a -tzip %filename% *.cfg
REM 7z a -tzip %filename% _tb/fgd/*.fgd
REM 7z a -tzip %filename% _tb/ -xr!*.png -xr!*.tga
REM 7z a -tzip %filename% _bl/*.json
pause
