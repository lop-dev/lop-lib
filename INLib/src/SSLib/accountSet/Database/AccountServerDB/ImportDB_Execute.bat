CALL %ImportTB% %DatabaseName%%DatabaseIndex% config/AccountAuth_DBALLCreateTable.xml
CALL %ImportTB% %DatabaseName%%DatabaseIndex% config/AccountAuth_DB%DatabaseIndex%AutoIncrement.xml
CALL %ImportTB% %DatabaseName%%DatabaseIndex% config/AccountAuth_DBALLAddUnique.xml
CALL %ImportTB% %DatabaseName%%DatabaseIndex% config/AccountAuth_DBALLDebugRecord.xml

CALL %ImportTB% %DatabaseName%%DatabaseIndex% config/GroupInfo_DBALLCreateTable.xml
CALL %ImportTB% %DatabaseName%%DatabaseIndex% config/GroupInfo_DB%DatabaseIndex%AutoIncrement.xml
CALL %ImportTB% %DatabaseName%%DatabaseIndex% config/GroupInfo_DBALLAddUnique.xml
CALL %ImportTB% %DatabaseName%%DatabaseIndex% config/GroupInfo_DBALLDebugRecord.xml

CALL %ImportTB% %DatabaseName%%DatabaseIndex% config/AccountInfo_DBALLCreateTable.xml

CALL %ImportTB% %DatabaseName%%DatabaseIndex% config/ProjectInfo_DBALLCreateTable.xml
CALL %ImportTB% %DatabaseName%%DatabaseIndex% config/ProjectInfo_DB%DatabaseIndex%AutoIncrement.xml
CALL %ImportTB% %DatabaseName%%DatabaseIndex% config/ProjectInfo_DBALLAddUnique.xml
CALL %ImportTB% %DatabaseName%%DatabaseIndex% config/ProjectInfo_DBALLDebugRecord.xml

CALL %ImportTB% %DatabaseName%%DatabaseIndex% config/ProjectServer_DBALLCreateTable.xml