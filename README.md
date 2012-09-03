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

On the client's end data should be recorded as a linked list of
`LPoint`s. For readability reasons there is a `LPointData` typedeffed as
a List (see _used datastructures_). You can insert points like in the
following snippet

` LPoint* point = (LPoint*)malloc(sizeof(LPoint));
` point->x = touchIter->x;
` point->y = touchIter->y;
` point->t = touchIter->time;
` list_insert_next( &point_data, point_data.tail, point); 

For compatibility reasons Lifeline is built in C. To use the recognizer
system you should first create a recognizer struct. 

` LRecognizer recog



` LRecognizer* recog = malloc(sizeof(LRecognizer));
` recognizer_set_data(recog, data_set, ...); 

## Authors

- Tom Bannink
- Jan-Willem Buurlage
- Erik Lumens

## Contact: 

- buurlage.byn@gmail.com
		[[LPoint]]
