headache : headache.c
	cc headache.c -o headache

.PHONY : test debug

test : headache
	./headache examples/hello-world.bf

debug : headache.c
	cc -D DEBUG headache.c -o headache-debug
	./headache-debug examples/hello-world.bf 
