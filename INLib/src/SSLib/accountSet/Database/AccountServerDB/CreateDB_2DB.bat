@echo off

set host=127.0.0.1
set name=root
set pwd=123456

echo %host%
echo %name%
echo %pwd%

::mysql -h%host% -u%name% -p%pwd% create database IF NOT EXISTS %dbname%;
::mysql -h%host% -u%name% -p%pwd% create database %dbname%;
mysql -h%host% -u%name% -p%pwd% < CreateDB_2DB.sql

IF NOT DEFINED NO_PAUSE PAUSE