all:


clean:
	rm -f src/*.o src/*.so src/*~
	rm -rf 	src/backend/CMakeFiles/ 
	rm -f src/backend/src/*~
	rm -f src/backend/src/internal/*~
	rm -f src/backend/include/backend/*~ \
  src/backend/OpenCL_CLHPP-prefix/ \
  src/backend/OpenCL_Headers-prefix/ \
  src/backend/build/ \
  src/backend/lib/ 


