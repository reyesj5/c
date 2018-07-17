GC = gcc
GCFLAGS = -Wall -c
OBJS = foo.o bar.o main

main: bar.o foo.o
	$(GC) -Wall $^ -o $@

bar.o: bar.c bar.h
	$(GC) $(GCFLAGS) bar.c -o $@

foo.o: foo.c foo.h bar.h
	$(GC) $(GCFLAGS) foo.c -o $@

clean:
	rm -f $(OBJS)
