headache : headache.c
	cc headache.c -o headache

.PHONY : test debug

test : headache
	./headache sum.bf

debug : headache.c
	cc -D DEBUG headache.c -o headache-debug
	./headache-debug sum.bf 
