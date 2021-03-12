
!include ..\..\build\deploy\nsis\api.nsh

!insertmacro ComponentApiBegin astro
!insertmacro FolderApi include\astro\eph *.h
!insertmacro ModuleApi db
!insertmacro ModuleApi controls
!insertmacro ModuleApi hora
!insertmacro FolderApi assets\ephe *.*
