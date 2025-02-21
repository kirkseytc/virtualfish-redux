install_dir = /usr/local/bin

virtualfish-rdx: main.o
	gcc $^ -lncurses -o virtualfish-rdx

main.o: main.c main.h fish_graphic
	gcc -c main.c -o main.o -Wall

install: virtualfish-rdx
	cp virtualfish-rdx $(install_dir)/virtualfish-rdx

clean:
	rm *.o virtualfish-rdx