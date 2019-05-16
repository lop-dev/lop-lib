@echo off

set xmlfile=%2
set host=127.0.0.1
set dbname=%1
set name=root
set pwd=123456

echo %xmlfile%

java -jar ../../../../../../EXLib/liquibase/liquibase.jar ^
     --driver=com.mysql.jdbc.Driver ^
     --classpath=../../../../../../EXLib/liquibase/lib/mysql-connector-java-5.1.31-bin.jar ^
     --changeLogFile=%xmlfile% ^
     --url="jdbc:mysql://%host%/"%dbname% ^
     --username=%name% ^
     --password=%pwd% ^
     update
