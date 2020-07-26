# boggle

## a 1v1 game for playing boggle on a linux terminal

- Boggle is a popular game that is basically searching for words in a grid but unlike normal wordsearches you can move a step in any direction (except the one direction you came from) to make valid words
- The original version was intended for windows and hence used conio.h but this version uses the curses.h library on linux. This is not part of the standard g++ package but can be installed by running "sudo apt-get install libncurses5-dev libncursesw5-dev" (for Debian-based/Ubuntu) or equivalent 
- This also uses pthreads which is part of the standard g++ package hence nothing new has to be installed
- A compiled version of the game is present called boggle which has been compiled using "g++ -o boggle boggle.cpp -pthread -lncurses"
- The wordlist_new.txt file contains a few default words that can be used to play a basic version of the game
- It is not recommended to use the whole dictionary as the trie structure generated in the beginning will run out of heap space
- The boggle.cpp contains the source code of the game written in C++
- The game allows a user to search the grid for 10sec before timing out the screen to allow the next player to play
- The game uses words in the length of 3-7 letters hence scoring would be in the range of 1-5 for the words found provided it is a valid word in the trie
- The game ends when both users have forfeited the game or if there are no more words to be found in the grid
- The game displays all the words that could be found in the end along with the winner of the game
- The board size can be increased by changing the #define bsize
