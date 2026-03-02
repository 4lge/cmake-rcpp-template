all:


clean:
	rm -f src/*.o src/*.so src/*~
	rm -rf 	src/backend/CMakeFiles/ \
  src/backend/OpenCL_CLHPP-prefix/ \
  src/backend/OpenCL_Headers-prefix/ \
  src/backend/build/ \
  src/backend/lib/ 


