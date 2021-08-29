# Conaud
A easy-to-use console audio-playing tool.
Dependencies:
+ [Bass 2.4](http://www.un4seen.com/bass.html)
+ [Filesystem(C++17)](https://en.cppreference.com/w/cpp/filesystem)
+ [MediaInfoLib](https://github.com/mediaarea/mediainfolib)

How to build it(under GCC):
```
              (Bass 2.4)                     (using C++17 Filesystem)
g++ conaud.cpp bass.dll -o conaud.exe -lstdc++fs --std=c++17
```

How to use it:
```
 Usage: conaud.exe argument <file> option
 Arguments:
        -ld         =   list devices
        -v          =   version
        -h          =   print this message
        <file>      =   play the file
 Options:
        --playlist  =   play files from a directory
        --online    =   play file from the web
        --info      =   Get the file's info
```
