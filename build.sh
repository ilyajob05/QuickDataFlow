mkdir build
cd build
cmake -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=DEBUG ..
cmake --build . --target all
./QickDataFlow_test
cd ..
