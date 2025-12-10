# THE APOCALIPSE

## INTRO

Hello everyone !

I am RederG, the developper of "The Apocalipse" game. This is my first big project made with C++.
I started programmation in 2023 in the Christmas vacation with Python when I was 16 years old. Then I started to learn C/C++, especially C++. 
This project was born in 21 July of 2024 and it still grow every day until now.

I hope you will give feedback for this project and that can help me a lot during developpement.

If you want to know, The Apocalipse is a game like "Shaterred Pixel Dungeon" but I want to add some functionalities like multiplayer system, hystory and "creative mode" (this is a kind of minecraft if you want).
These functionalities are not implemented now, but I will do that earlier(or maybe not, if I am lazy and I want to make other things instead of this game).
I will develop it step by step and I want it to work. Then everyone can play on it(if the game is awesome also). I don't think that this game will be popular or something like that but I hope just.

## SCREENSHOTS

![The game gameplay preview](/res/images/Screenshots/game_preview.png "This is the gameplay")

## TOOLS

So, this game was made with C++ but also, I used SFML for graphics. If you want to build the game, you will need to download the SFML libraries.
You can find them on this site :
- https://www.sfml-dev.org/download/sfml/3.0.2/

PLEASE RESPECT THAT SFML SAID ABOUT THE "COMPILER ISSUE" THAT IT MUST BE THE SAME FOR WINDOWS

## BUILDING THE APP

After the download, you can clone the project. 

After that, in the terminal, create the "build" folder. Then write the command and execute it (the last instruction -I"{path_to_SFML_include_files(like D:/SFML-3.0.2/include)}" is important, you must put the absolute/relative path to the include files of SFML) :
'''
g++ -c ../src/main.cpp ../src/window_menu/*.cpp ../src/Help/*.cpp ../src/Effects/*.cpp ../src/Entities/*.cpp ../src/Graphicals/*.cpp ../src/InteractiveObjects/*.cpp ../src/Items/*.cpp ../src/Items/Effect_items/*.cpp ../src/Items/Foods/*.cpp ../src/Items/Weapons/*.cpp ../src/Map/*.cpp ../src/Map/Structures/*.cpp ../src/Others/*.cpp -I"{path_to_SFML_include_files(like D:/SFML-3.0.2/include)}"
'''

This command is a bit long and takes time at execution. This is the only command that is very boring but you can optimize it on VS code with the "task concept" that I used. With that, this long phrase will take only 1min 30sec or 2min of execution.

The following command is the next step for building the game. This is short and takes a few seconds to execute (the instruction -L"{path_to_SFML_libraries(like D:/SFML-3.0.2/lib)}" - is important, you must put the absolute/relative path to the libraries files of SFML):
'''
g++ *.o -o THE_APOCALIPSE -L"{path_to_SFML_libraries(like D:/SFML-3.0.2/lib)}" -lsfml-graphics -lsfml-window -lsfml-system
'''

If this command was finished you can play the game now by running the executable "THE_APOCALIPSE".

ENJOY !!!!