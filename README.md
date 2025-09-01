# flappy_bird_AI
A genetic algorithm implementation that learns to play flappy bird, forked from my [flappy bird game](https://github.com/Galm007/flappy_bird_raylib) made with the Raylib library, written in C.

This video demonstrates the AI learning to play the game with the difficulty set to high. The spacing between the pipes is small while the vertical gap is minimal. The AI learned to master the game under these circumstances in only 8 generations.



https://github.com/user-attachments/assets/d5083db7-b095-42bf-b3cc-83b48093ef69



The shape of the neural network is 3x2x2x2.

The 3 input neurons correspond to:
- Y position of the next pipe (ratio to window height)
- Y position of the bird (ratio to window height)
- Bird's horizontal distance from the next pipe (ratio to window width)

The bird flaps only its wings when the first output neuron is greater than the second.

The AI can still learn to play the game perfectly even without the third input (and in fact it would make training a lot simpler), but I still kept it because having only 2 input neurons is boring.

How to run:
```
git clone https://github.com/Galm007/flappy_bird_AI && cd flappy_bird_AI
mkdir build && cd build
cmake ..
make
./flappy_bird
```
