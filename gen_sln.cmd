"c:\Qt\4.6.3\bin\qmake.exe" -spec win32-msvc2008 -tp vc
del TRainerStudio.2008.express.vcproj
ren TrainerStudio.vcproj TrainerStudio.2008.express.vcproj
echo "Manually copy/replace $QTDIR environment variable into project file"
echo "Push top GUID block back current"
pause