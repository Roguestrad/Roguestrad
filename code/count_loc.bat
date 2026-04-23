cloc-2.04.exe ^
  --exclude-dir=.venv,build,libs,extern,docs ^
  --exclude-ext=json,xml,txt ^
  --fullpath ^
  --not-match-d="engine/renderer/frontend/images/baked" ^
  .
pause