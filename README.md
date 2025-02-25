# Virtualfish Redux
Virtualfish Redux is a ground up remake of the [virtualfish project](https://www.github.com/kirkseytc/virtualfish) that aims 
to function similar to the original while having new features to play around with.

## What is Virtualfish?
Virtualfish is a fish tank simulator that you run in your terminal built off of the ncurses library.

## Install
### Windows:
Currently there is no native Windows port, but you can use [WSL](https://learn.microsoft.com/en-us/windows/wsl/install) with the [Linux](#linux) instructions.
### MacOS:
Currently there is no planned native MacOS port.
### Linux:
> If you do not have ncurses-6.4 installed already, either build it from [source](https://invisible-island.net/ncurses/#download) or install it with your package manager
> (example with Debian: `sudo apt-get install libncurses-dev`).

Download the lastest [release](https://github.com/kirkseytc/virtualfish-redux/releases), unzip it, and change directory into the newly created folder.

Next run `sudo make install` to build and install Virtualfish Redux to your "/usr/local/bin" directory.

At this point, if you want to, you can remove the created folder and the .zip file you downloaded earlier.

_Note: if you have the non-redux version of Virtualfish installed, this will overwrite that install_

## Running
To run Virtualfish Redux either run `virtualfish` or `vfish`.

After starting execution you will be greeted with this title screen.  
Simply press space to continue.

<img src="imgs/title_screen.png" alt="Screenshot of the Title Screen" width="540">

### Commands
To run a command type a ':' (or ';').  
This will pause gameplay and bring up an input window.  
In this window you are able to type in commands to interact with the tank.  

|Command|Alias|Description|
|:-|:-|:-|
|`fish`|`f`|Spawns one fish|
|`max`|`m`|Spawns the maximum number of fish|
|`clear`|`c`|Clears the tank of all fish|
|`seed`|`s`|Prints the seed|
|`quit`|`q`|Quits the game|

### Flags
|Option|Description|
|:-|:-|
|`-sm`|Starts the game with the max amount of fish spawned in.|
|`-bw`|Starts in Black and White mode|
|`-nt`|Skips the title screen|
|`-rf`|Makes the fish shift colors like rainbow rgb

## Why Does Redux Exist?
Redux exists because I want to have a crack at this idea again with
new understanding of programming with C/C++ and with a vision of 
what the final product will look like from the beginning.

