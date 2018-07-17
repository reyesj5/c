reyesj5

Exercise 0:

==27271==      possibly lost: 1,352 bytes in 18 blocks
==27271==    still reachable: 141,526 bytes in 3,489 blocks
==27271==                       of which reachable via heuristic:
==27271==                         newarray           : 1,536 bytes in 16 blocks

Exercise 1:

1. What problem is valgrind reporting in these three cases?

	It's reporting that there are uninitialized values in each of the functions which are used on a conditional. 

==21574== Conditional jump or move depends on uninitialised value(s)
==21574==    at 0x40091E: fun2 (vgme.c:43)
==21574==    by 0x400DE2: main (vgme.c:186)

2. As revealed by what valgrind output from "--track-origins=yes", which one of the three problems has a different origin from the other two? 

	The allocation in func0 has a different origin because it doesn't malloc memory from the heap for the uninitialized array, instead the memory is allocated from the stack.

3. The code has the same underlying problem, but what is recognizing the problem now? 

	Now the problem is just y and not the array x.

4. Is any error reported? Have we learned in this class why or why not? 

	No error is reported. This is the case because the space has already being allocated in the stack for this array and therefore it has some value, even it has not being initialized to a value that is useful or that it makes sense.

Exercise 2:

1. In terms of the regions of a process's address space that we have been learning about, what is the difference between the two x[SIZE] arrays?

	One is allocated in the stack and the other in the heap.

2. What is the bug in both functions? 

	They both are trying to reference a value past the memory allocated for the array.

3. Compare the results of running "valgrind ./vgme 4" and "valgrind ./vgme 5". Why are they so different? Which is more helpful to you during debugging? 

	They are different because one is in the stack, and the other must allocate and free memory in the heap, which is easier to detect memory leaks in that case. Therefore, function 5, which uses the heap, is more useful for debugging.

4. Why are there two errors reported for the single offending line of C code in fun5?

	There are two errors because one of them is accessing a memory location past the array, and the other is because this  value was supposed to be assigned to sink, which can't happen because x[SIZE] can't be accessed.

5. Run "valgrind ./vgme 6". How well does valgrind detect the error in this case? Again, make a mental note of what kind of array declaration prevents valgrind from doing its intended job. 

	Valgrind doesn't detect the error.

Exercise 3:

1. What is the bug in fun7?

	It fress the array x and then tries to dereference one of its values.

2. Comparing the results of running "valgrind ./vgme 5" and "valgrind ./vgme 7", 
what words clue you into the difference in bugs in fun5 and fun7?

	The use of "free'd" and "bloack was alloc'd at"

==8991==  Address 0x5204308 is 168 bytes inside a block of size 480 free'd
==8991==    at 0x4C2EDEB: free (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==8991==    by 0x400A21: fun7 (vgme.c:76)
==8991==    by 0x400DE2: main (vgme.c:186)
==8991==  Block was alloc'd at
==8991==    at 0x4C2DB8F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)

3. Hypothetically, if you didn't already know that 4 == sizeof(int), how could you learn that from either valgrind output (combined with referencing the code)?
	By looking at the reads and writes sizes of errors in valgrind and making sure that is talking about ints.

Exercise 4:

1. There are 6 frees and 5 valgrind error messages about (most of) them. Which is the only free() statement that didn't generate a valgrind error?
	
	free(same(x));

2. At least four of the 5 broken free() calls can be identified with one of the process's four memory regions that we talked about in class: which free calls are they, and which regions are involved?

	free(x + 42);		--> Heap
	free(same(fun1));	--> Text
	free(same(gxs));	--> static
	free(same(x));		--> Heap
	free(x); 			--> Heap

3. Try removing -g from the compilation ("CFLAGS = -Wall -std=c99") and describe what information is missing about where in fun8 the problems arise. Note that after changing a Makefile, you usually have to make clean;make, since make watches for changes in source files, but not Makefiles, when deciding whether recompilation is necessary.

	The line numbers are missing without -g.

4. Why was the same function introduced? Hint: try removing it in one or the other of the two lines:

          free(fun1);
          free(gxs);

and running make. 

	It was introduced because func1 points to a location in the text and cannot be free'd unless passed as apointer that the optimizing compiler can't recognize.

Exercise 5:

1. What is the bug? What is the line in the valgrind output that identifies it? (make sure you've turned -g back on in the Makefile).

The bug is that the memory allocated for the array is not free'd ever. 
==53855== HEAP SUMMARY:
==53855==     in use at exit: 480 bytes in 1 blocks
==53855==   total heap usage: 2 allocs, 1 frees, 960 bytes allocated
==53855==
==53855== LEAK SUMMARY:
==53855==    definitely lost: 480 bytes in 1 blocks

2. Does "valgrind --leak-check=full ./vgme 9" help isolate exactly where the memory bug is?

	Yes it does, it gives the line number of where the memory was allocated.

Exercise 6:

1. What is the bug? What is the line in the valgrind output that identifies it?
	
	The bug is that in the loop, the function returns a NULL pointer and tries to write this which causes a segmentation fault.
==12837== Invalid write of size 8
==12837==    at 0x400C32: fun10 (vgme.c:137)
==12837==    by 0x400DE2: main (vgme.c:186)
==12837==  Address 0x8 is not stack'd, malloc'd or (recently) free'd