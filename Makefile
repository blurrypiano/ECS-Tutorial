CFLAGS = -std=c++17 -I. -O3
LDFLAGS = `pkg-config --static --libs glfw3`

a.out: *.cpp *.hpp
	g++ $(CFLAGS) -o a.out *.cpp $(LDFLAGS)

.PHONY: test clean

clean:
	rm -f a.out
