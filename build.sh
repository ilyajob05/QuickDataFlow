mkdir build
cd build
cmake -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON -DCMAKE_BUILD_TYPE=DEBUG ..
cmake --build . --clean-first --target all --parallel 8
./QuickDataFlow_test
cpack
cd ..
