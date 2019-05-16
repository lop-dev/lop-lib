#/usr/bin
#if [ $# != 1 ] ; then
#  echo "USAGE: $0 [debug | release], default is debug"
#
#  exit 1
#fi

#./build_librdkafka.sh

rm -rf build
mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=Release ../src
make
cd ../

rm -rf build
mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=Debug ../src
make
cd ../

