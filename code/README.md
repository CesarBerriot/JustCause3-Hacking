# This is where the magic happens

### Contents

| Directory | Description                                                                                     |
|-----------|-------------------------------------------------------------------------------------------------|
| demos     | technical demonstrations of different hacking techniques and methods applicable to Just Cause 3 |

### Build Instructions
#### Prerequisites
I personally work with c++ 17, cmake 3.27, and mingw-w64 v11.2.0. Any subsequent version for any of these should do.
#### Dependencies
builtin with mingw :
- Win32 API
- glibc
#### Building
Just generate a project of your choice for your environment with cmake,
then compile it with your favorite IDE and that should be it. 
Once that's done just copy the built dll into your JC3 install folder.
The binary might depend on some mingw dlls, in which case just copy
the dlls mentioned in the errors when launching the game from your
mingw install directory's bin subdirectory into your JC3 install folder.  