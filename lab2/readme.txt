Ex1

multStrLen

For the multiStrLen function, I created a breakpoint of the start of the function in the library.c file, created watchoints for the p and q values and then compared the results before the return statement. 

break library.c:15
info locals
watch p
watch q
cont
...
step
print p-first (gave me a result of 3)
print q-second (gave me a result of 3)

After I noticed that the address of the p and q values was one more than it should be, I realized this was caused by the fact that in the while contional expression, the code uses ++ after checking the pointer and thus, we need to subract one before adding the length of the strings.


twoFingerSort

For twoFingerSort, I created a breakpoint at the start pf the function and check the result before implementing any switches. Then wente through the first few switched and noticed that the first character was not being checked. Therefore, I changed i=1 to i=0 and j=i to j=i+1.

break library.c:34
watch result
next
...
step
step
break library.c:54
print result
finish
print result

Ex2

The array is correupted when the third loop tries to reference a memory by an offset way bigger than the length of data4 and changing the value at the location of data4[i] which starts at data[200]. This happens on line 27.

What are the values of the local variables at one point where a corrupted value is written? 
offset = 66
i = 132
data = "Xfmm- !nbzc"
stringToPrint = "Some additional text."
data3 = "A final bit of text."
data4 = Well, maybe just a little uis to eat xl,ijtst loe un!"

What debugger commands did you use?
I used the folowing commands:
break main.c:16
break main.c:20
break main.c:24
step
info locals
cont
