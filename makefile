install_dir = /usr/local/bin
code_dir = ./src
build_dir = ./build
CC = gcc

virtualfish: $(build_dir)/virtualfish.o
	@echo "Linking..."
	@$(CC) $^ -lncurses -o virtualfish
	@echo "Done!"

$(build_dir)/virtualfish.o: $(code_dir)/virtualfish.c $(code_dir)/*.h
	@if [ ! -d $(build_dir) ]; then echo "Creating build directory..." && mkdir $(build_dir); fi
	@echo "Compiling..."
	@$(CC) -c -std=c99 $< -o $(build_dir)/virtualfish.o -Wall -Werror

install: virtualfish
	@echo "Installing..."
	@cp virtualfish $(install_dir)/virtualfish
	@cp $(install_dir)/virtualfish $(install_dir)/vfish
	@echo "'virtualfish' is installed."

uninstall:
	@if [ -f $(install_dir)/virtualfish ]; then rm $(install_dir)/virtualfish $(install_dir)/vfish; else echo "'virtualfish' is not installed. Nothing happened"; fi;

clean:
	@if [ -d $(build_dir) ]; then echo "Cleaning objects..." && rm -rf $(build_dir); fi;
	@if [ -f virtualfish ]; then echo "Cleaning executables..." && rm virtualfish; fi;
	@echo "Done!"

release:
	@zip -q -r virtualfish-redux.zip src makefile README.md
	@echo "Done!"
