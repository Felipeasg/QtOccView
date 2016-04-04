# Simple opencascade demo 

This is a simple opencascade demo based on oce/6.8.0 community version and in  [occQt](https://github.com/eryar/occQt/wiki/A-Simple-OpenCASCADE-Qt-Demo).

# To compile and run (linux)

First is needed donwload the [opencascade](https://github.com/tpaviot/oce) srcs, compile and install and download the qt5 development tools using you preferred package manager.


This example use qmake-qt5 to generate Makefile and compile the sources. Is a good practice create a build dir to compile.

```
$ cd QtOccWidget  
$ mkdir build  
$ cd build  
$ qmake-qt5 ../OccWidget.pro  
$ make   
$ ./OccWidget  
```

