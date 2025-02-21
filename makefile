install_dir = /usr/local/bin
code_dir = ./src
build_dir = ./build

virtualfish: $(build_dir)/main.o
	@echo "Linking..."
	@gcc $^ -lncurses -o virtualfish
	@echo "Done!"

$(build_dir)/main.o: $(code_dir)/main.c $(code_dir)/main.h $(code_dir)/fish_graphic.h
	@if [ ! -d $(build_dir) ]; then echo "Creating build directory..." && mkdir $(build_dir); fi
	@echo "Compiling..."
	@gcc -c $< -o $(build_dir)/main.o -Wall -Wextra -Werror

install: virtualfish
	@echo "Installing..."
	@cp virtualfish $(install_dir)/virtualfish
	@cp $(install_dir)/virtualfish $(install_dir)/vfish
	@echo "'virtualfish' is installed."

uninstall:
	@if [ -f $(install_dir)/virtualfish ]; then rm $(install_dir)/virtualfish; else echo "'virtualfish' is not installed. Nothing happened"; fi;

clean:
	@if [ -d $(build_dir) ]; then echo "Cleaning objects..." && rm -rf $(build_dir); fi;
	@if [ -f virtualfish ]; then echo "Cleaning executable..." && rm virtualfish; fi;
	@echo "Done!"
