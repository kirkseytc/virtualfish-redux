# Virtualfish Redux
This is a "redux" of the [virtualfish project](https://www.github.com/kirkseytc/virtualfish), remaking it from scratch and impementing new features.

## Install

## Running
To run virtualfish-redux either run `virtualfish` or `vfish`.

After starting execution you will be greeted with this title screen.  
Simply press space to continue.

![Screenshot of the Title Screen](imgs/title_screen.png)

### Commands
To run a command type a ':' (or ';').  
This will pause gameplay and bring up an input window.  
In this window you are able to type in commands to interact with the tank.  

|Command|Alias|Argument(s)|Description|
|:-|:-|:-|:-|
|`fish`|`f`|`<amount> <color>`|Spawns one fish or the amount specified in `<amount>` with either a random color or a specified color in `<color>`|
|`max`|`m`|`<new_max>`|Spawns the maximum number of fish or sets the max to the amount passed in `<new_max>`|
|`clear`|`c`|N/A|Clears the tank of all fish|
|`seed`|`s`|N/A|prints the seed so it can be copied then waits for the user to press space to resume game|

### Flags
|Flag|Argument|Description|
|:-|:-|:-|
|`-s`|`<seed>`|Seeds the randomizer to the amount passed in `<seed>`|
|`-c`|`<count>`|Sets the starting amount of fish to the amount passed in `<count>`|
|`-m`|`<max>`|Sets the max amount of fish possible to the amount passed in `<max>`|
|`-bw`|N/A|Starts in Black and White mode|
|`-nt`|N/A|Skips the title screen|

Example: 
Start with 5 fish, a max of 20 fish, and skip the title screen

`virtualfish -c 5 -m 20 -nt` 

_Note: The order of the flags does not matter._

## Releases

## Why Does Redux Exist?
Redux exists because I want to have a crack at this idea again with
new understanding of programming with C/C++ and with a vision of 
what the final product will look like from the beginning.

