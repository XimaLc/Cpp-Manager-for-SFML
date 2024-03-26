copy /Y ..\DLL_ENGINE\Debug\*.lib ..\lib\*.lib
del ..\DLL_ENGINE\Debug\*lib
del ..\DLL_ENGINE\Debug\*.pdb
del ..\DLL_ENGINE\Debug\*.exp
copy /Y ..\DLL_ENGINE\Debug\*.dll ..\x64\Debug\*.dll
copy /Y *.h ..\include\SFML_ENGINE\*.h
exit 0