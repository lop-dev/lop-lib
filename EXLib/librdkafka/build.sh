#/usr/bin
#if [ $# != 1 ] ; then
#  echo "USAGE: $0 [debug | release], default is debug"
#
#  exit 1
#fi


cmake -DCMAKE_BUILD_TYPE=Release .
make


cmake -DCMAKE_BUILD_TYPE=Debug .
make

