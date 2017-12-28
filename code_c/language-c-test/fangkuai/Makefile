.PHONY:clean
test: main.o block.o
	gcc -o test main.o block.o
%.c:%.o
	gcc -c $@ $<
clean:
	rm -rf main.o block.o test
