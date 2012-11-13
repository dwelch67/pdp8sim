

all : test.bin pdp8diss pdp8sim


test.bin : test.pal
	palbart test.pal


pdp8diss : pdp8diss.c
	gcc -O2 -Wall pdp8diss.c -o pdp8diss


pdp8sim : pdp8sim.c
	gcc -O2 -Wall pdp8sim.c -o pdp8sim

clean :
	rm -f test.bin
	rm -f test.lst
	rm -f pdp8diss
	rm -f pdp8sim


