#!/bin/bash

#    检查操作系统版本，该脚本只能运行在 Centos 7.x 系统上

cat /etc/redhat-release |grep -i centos |grep '7.[[:digit:]]' >/dev/null

if [[ $? != 0 ]] 
then
    echo -e "不支持的操作系统，该脚本只适用于CentOS 7.x  x86_64 操作系统"
    exit 1
fi

#    配置无人值守的安装，定义安装过程中需要用到的一些信息

DBPassword=zabbix_db_password
CacheSize=256M
ZBX_SERVER_NAME=XXX-Zabbix-Server
http_port=59980

#    配置 Zabbix 4.2 YUM 源

rpm -Uvh https://mirrors.aliyun.com/zabbix/zabbix/4.2/rhel/7/x86_64/zabbix-release-4.2-1.el7.noarch.rpm
sed -i 's/repo.zabbix.com/mirrors.aliyun.com\/zabbix/g' /etc/yum.repos.d/zabbix.repo
yum clean all

#    安装 Zabbix 4.2 Server、Web、Agent、DB
yum -y install zabbix-server-mysql zabbix-web-mysql zabbix-agent mysql mariadb-server

#    配置 Zabbix 数据库（创建账号，并导入zabbix库结构）

systemctl enable mariadb.service
systemctl start mariadb.service
mysql -e "create database zabbix character set utf8 collate utf8_bin;"
mysql -e "grant all privileges on zabbix.* to zabbix@localhost identified by '$DBPassword';"
mysql -e "flush privileges;"
zcat /usr/share/doc/zabbix-server-mysql*/create.sql.gz | mysql zabbix 

#    配置 Zabbix Web Server

sed -i 's/# php_value date.timezone Europe\/Riga/php_value date.timezone Asia\/Shanghai/' /etc/httpd/conf.d/zabbix.conf
sed -i 's/Listen 80/Listen '$http_port'/' /etc/httpd/conf/httpd.conf

cp /usr/share/zabbix/conf/zabbix.conf.php.example /etc/zabbix/web/zabbix.conf.php
sed -i "10c \$DB[\"PASSWORD\"]    = '$DBPassword';" /etc/zabbix/web/zabbix.conf.php
sed -i "16c \$ZBX_SERVER_NAME   = '$ZBX_SERVER_NAME';" /etc/zabbix/web/zabbix.conf.php

#    配置 Zabbix Server

sed -i "/^# DBPassword=/a DBPassword=$DBPassword" /etc/zabbix/zabbix_server.conf
sed -i "/^# CacheSize=8M/a CacheSize=$CacheSize" /etc/zabbix/zabbix_server.conf
sed -i "/^# StartPingers=1/a StartPingers=10" /etc/zabbix/zabbix_server.conf

#    启动 Zabbix 相关服务，并设置开机自启动

systemctl enable zabbix-server zabbix-agent httpd
systemctl start zabbix-server zabbix-agent httpd

#    配置 Zabbix 防火墙

firewall-cmd --permanent --zone=public --add-port=10051/tcp
firewall-cmd --permanent --zone=public --add-port=162/udp
firewall-cmd --permanent --zone=public --add-port=$http_port/tcp
firewall-cmd --reload

#    禁用服务器 SELinux

etenforce 0
sed -i 's/SELINUX=enforcing/SELINUX=permissive/' /etc/sysconfig/selinux

#    安装成功，提示访问方式

echo -e  "\n您可以通过以下地址来访问 Zabbix Web Page\n"
for i in `ip a | grep 'inet ' | awk '{print $2}' | awk -F '/' '{print $1}'` ; do echo -e "\thttp://$i:$http_port/zabbix" ; done
