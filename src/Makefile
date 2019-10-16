src = $(wildcard modules/*.cpp) \
		$(wildcard peripherals/*.cpp) \
		$(wildcard *.cpp) 
obj = $(src:.cpp=.o)

LDFLAGS = -lwiringPi -pthread -lrt -lcrypt -llapack -lblas -larmadillo

main: $(obj)
	$(CXX) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) main
