SET NO_PAUSE=1

CD AccountServerDB
CALL DropDB_2DB.bat
CALL CreateDB_2DB.bat
CALL ImportDB_2DB.bat
CD ..

PAUSE