all:
g++ -std=c++17 main.cpp Source/*.cpp -o orbit_os
./orbit_os

clean:
rm orbit_os
