

all : test.bin pdp8diss


test.bin : test.pal
	palbart test.pal


pdp8diss : pdp8diss.c
	gcc -O2 -Wall pdp8diss.c -o pdp8diss


clean :
	rm -f test.bin
	rm -f test.lst
	rm -f pdp8diss

