# Virtualfish Redux
Virtualfish Redux is a ground up remake of the [virtualfish project](https://www.github.com/kirkseytc/virtualfish) that aims 
to function similar to the original while having new features to play around with.

## What is Virtualfish?
Virtualfish is an aquarium simulator that you run in your terminal built off of the ncurses library.

## Install
Currently Virtualfish Redux is only available on Linux. 
It might work on MacOS, but that is untested, but feel free to try it out! 
For Windows, You can use [WSL](https://learn.microsoft.com/en-us/windows/wsl/install) to run Linux programs.

> To build this program you need to have `gcc` and `make` installed.

> If you do not have ncurses-6.4 library installed already, either build it from [source](https://invisible-island.net/ncurses/#download)
> or install it with your package manager.
>
> Example with `apt`: `sudo apt install libncurses-dev`

Download the lastest [release](https://github.com/kirkseytc/virtualfish-redux/releases), unzip it, and change directory into the newly created folder.

Next run `sudo make install` to build and install Virtualfish Redux to your "/usr/local/bin" directory.

At this point, if you want to, you can remove the created folder and the .zip file you downloaded earlier.

_Note: if you have the non-redux version of Virtualfish installed, this will overwrite that install_

## Running
To run Virtualfish Redux either run `virtualfish` or `vfish`.

After starting execution you will be greeted with this title screen.  
Simply press space to continue.

<img src="imgs/title_screen.png" alt="Screenshot of the Title Screen" width="640">

### Commands
To run a command type a ':' (or ';').  
This will pause gameplay and bring up an input window.  
In this window you are able to type in commands to interact with the tank.  

|Command|Description|
|:-|:-|
|`clear`|Clears the tank of all fish|
|`fish`|Spawns one fish|
|`max`|Spawns the maximum number of fish|
||||
|`castle`|enables/disables the castle at the bottom of the screen|
|`crab`|enables/disables the crab at the bottom of the screen|
|`volcano`|enables/disables the volcano at the bottom of the screen|
||||
|`quit`|Quits the game|
|`seed`|Prints the seed|

_Note: If you only type the first few characters of a command in order, the intened command will still run. Example: `ca` will run `castle`_

### Flags
|Option|Description|
|:-----------------------------------|:-|
|`-h` or `--help`|Prints the help screen|
|`-v` or `--version`|Prints the version|
|`-bw` or `--black-and-white`|Starts game in Black and White mode|
|`-nt` or `--no-title`|Skips the title screen|
|`-rb` or `--rainbow`|Starts game in Rainbow Fish mode. If the game is in Black and White mode, then nothing happens|
|`-sm` or `--start-with-max`|Starts the game with the max amount of fish spawned in|
|`-cg` or `--colored-gravel`|Replaces the sand with colored gravel|

## Why Does Redux Exist?
Redux exists because I want to have a crack at this idea again with
new understanding of programming with C/C++ and with a vision of 
what the final product will look like from the beginning.

