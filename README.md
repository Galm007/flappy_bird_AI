# flappy_bird_AI
A genetic algorithm implementation that learns to play flappy bird, forked from my [flappy bird game](https://github.com/Galm007/flappy_bird_raylib) made with the Raylib library, written in C.

This video demonstrates the AI learning to play the game with the difficulty ramped all the way up. The spacing between the pipes is minimal while the vertical gap is as small as it gets. The AI learned to master the game under these circumstances in only 8 generations.

https://github.com/user-attachments/assets/ea4f9cd1-5703-4039-979e-79bd000d9203

The shape of the neural network is 3x2x2x2.

The 3 input neurons correspond to:
- Y position of the next pipe (ratio to window height)
- Y position of the bird (ratio to window height)
- Bird's horizontal distance from the next pipe (ratio to window width)

The bird flaps only its wings when the first output neuron is greater than the second.

How to run:
```
git clone https://github.com/Galm007/flappy_bird_AI && cd flappy_bird_AI
mkdir build && cd build
cmake ..
make
./flappy_bird
```
