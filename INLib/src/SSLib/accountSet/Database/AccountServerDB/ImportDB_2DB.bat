SET ImportTB=ImportTB.bat
SET DatabaseName=AccountServer

SET DatabaseIndex=001
CALL ImportDB_Execute.bat
SET DatabaseIndex=002
CALL ImportDB_Execute.bat

IF NOT DEFINED NO_PAUSE PAUSE