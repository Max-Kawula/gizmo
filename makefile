build:
	gcc src/*.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall -Wextra -Werror -pedantic -o output.o
run:
	./output.o
debug:
	gcc src/*.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o output.o -g
rDebug:
	gdb output.o
clean:
	rm output.o
