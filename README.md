to build:

> g++ app.cpp -o netswitch.exe -mwindows


For Manifest

> windres app.rc app.res

> g++ app.cpp app.res -o netswitch.exe -mwindows