::..\protobuf\bin\protoc --cpp_out=./ person.proto

::protoc -I=. --cpp_out=. person.proto

..\..\bin\protogen -i:Result.proto -o:Result.cs
..\..\bin\protogen -i:Person.proto -o:Person.cs

PAUSE