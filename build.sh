sudo rm -r build # removes entire build directory for a clean build
mkdir build # creates a new build directory
cd build # enter build directory
cmake ../ # run cmake on parent directory
cd .. # enter the parent directory
cmake --build build # build with cmake