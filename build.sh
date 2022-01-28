mkdir build
cd build
cmake -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=DEBUG ..
cmake --build . --clean-first --target all --parallel 8
./QickDataFlow_test
cd ..
