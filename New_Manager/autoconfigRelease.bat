copy /Y ..\DLL_ENGINE\Release\*.lib ..\lib\*.lib
del ..\DLL_ENGINE\Release\*lib
del ..\DLL_ENGINE\Release\*.pdb
del ..\DLL_ENGINE\Release\*.exp
copy /Y ..\DLL_ENGINE\Release\*.dll ..\x64\Release\*.dll
copy /Y *.h ..\include\SFML_ENGINE\*.h
exit 0