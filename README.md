## Lifeline

Lifeline is a library for handwriting recognition. It is specifically
tailored for touch-based input.

## Installation

Before installation please ensure you have the following libraries
installed (used exclusively for the testsuite, Lifeline itself has no
dependencies for now):

- OpenGL with GLEW
- SFML 2.0 (RC as of September 3 2012)
 
## Usage

Lifeline does not come with a pre-recorded alphabet. You can use the
testsuite to record characters. Lifeline will then use these characters
in the future to test against input. 

` LRecognizer* recog = malloc(sizeof(LRecognizer));
` recognizer_set_data(recog, data_set, ...); 

## Authors

- Tom Bannink
- Jan-Willem Buurlage
- Erik Lumens

## Contact: 

- buurlage.byn@gmail.com

