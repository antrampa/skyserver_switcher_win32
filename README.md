to build:

> g++ app.cpp -o netswitch.exe -mwindows


For Manifest

> windres app.rc app.res

> g++ app.cpp app.res -o netswitch.exe -mwindows


Build a “Portable” Static EXE (Release)

> g++ app.cpp app_manifest.o -o netswitch.exe -mwindows -static -static-libgcc -static-libstdc++


EVEN BETTER “Professional Release Build”

> windres app.rc -O coff -o app_res.o
> g++ app.cpp app_res.o -o netswitch.exe -mwindows -static -static-libgcc -static-libstdc++ -O2 -s
