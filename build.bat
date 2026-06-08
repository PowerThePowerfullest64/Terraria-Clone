g++ -std=c++23 -Iraylib/include -IcustomHeaders -Iinclude main.cpp ^
 ^
customHeaders/*.cpp ^
-Lraylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm -o main.exe