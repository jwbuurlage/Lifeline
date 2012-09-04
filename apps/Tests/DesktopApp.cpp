#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <fstream>

#include "LRecognizer.h"
#include "LDatabase.h"

void mousePress(sf::Mouse::Button button, bool down, int x, int y);
void mouseMoved(int x, int y);

void sendInput();

sf::RenderWindow* window;
sf::Clock* timer;

bool mouseDown;
int previousX, previousY;

char symbol;

enum STATE {
	StateNothing = 0,
	StateRecording,
	StateChoosingSymbol
} State;

typedef Lifeline::Touch Touch;

std::vector< std::vector<Touch> > strokeList; //each element contains a list of points which is a stroke

Lifeline::Recognizer recognizer;

int main(){
	std::cout << "---------------------------------\n";
	std::cout << "----- Lifeline test station -----\n";
	std::cout << "-- Press ESC to clear screen ----\n";
	std::cout << "-- Press enter to send ----------\n";
	std::cout << "-- Press R to record ------------\n";
	std::cout << "---------------------------------\n";
	
	char* symbolbuffer = 0;
	unsigned int symbolbuffersize = 0;

	std::ifstream file("symbols.data", std::ifstream::binary);
	if( file.is_open() == false ){
		std::cout << "Unable to open symbol database\n";
	}else{
		file.seekg(0, std::ios::end);
		symbolbuffersize = file.tellg();
		file.seekg(0, std::ios::beg);
		symbolbuffer = new char[symbolbuffersize];
		file.read(symbolbuffer, symbolbuffersize);
		file.close();
	}

	if( symbolbuffer ){
		database_add_pointer(symbolbuffer, symbolbuffersize);
	}

	window = new sf::RenderWindow(sf::VideoMode(900, 400, 32), "Testing Station");
	timer = new sf::Clock;

	sf::Event Event;

	State = StateNothing;
	mouseDown = false;
	strokeList.clear();
	timer->restart();
	while( window->isOpen() ){
		while(window->pollEvent(Event)) {
			switch( Event.type ){
			case sf::Event::KeyPressed: break;
			case sf::Event::KeyReleased: 
				switch( State ){
					case StateNothing:
						if( Event.key.code == sf::Keyboard::Escape ){
							if( strokeList.empty() ) window->close();
							else strokeList.clear();
						}else if( Event.key.code == sf::Keyboard::Return ){
							sendInput();
							strokeList.clear();
						}else if( Event.key.code == sf::Keyboard::R ){
							State = StateChoosingSymbol;
							std::cout << "Now press the key of the symbol you want to record features for. (A-Z)\n";
						}
						break;
					case StateChoosingSymbol:
						if( Event.key.code == sf::Keyboard::Escape ){
							State = StateNothing;
							std::cout << "Recording canceled.\n";
						}else if( Event.key.code >= sf::Keyboard::A && Event.key.code <= sf::Keyboard::Z ){
							symbol = 'A' + Event.key.code - sf::Keyboard::A;
							//recognizer_clear_samples(&recognizer);
							State = StateRecording;
							std::cout << "Recording symbol: " << symbol << "\n";
							std::cout << "Press Enter after drawing each symbol. Press ESC when done.\n";
						}else{
							std::cout << "Invalid key, only A-Z supported\n";
						}
						break;
					case StateRecording:
						if( Event.key.code == sf::Keyboard::Escape ){
							if( strokeList.empty() ){
								State = StateNothing;
								char symbolstring[2] = {symbol, 0};
								//recognizer_save_samples(&recognizer, symbolstring);
								//The file buffer has been changed. Write it to the file
								if( symbolbuffer ){
									std::ofstream file("symbols.data", std::ifstream::binary);
									if( file.is_open() == false ){
										std::cout << "Unable to open symbol database\n";
									}else{
										file.write(symbolbuffer, symbolbuffersize);
										file.close();
										std::cout << "Symbol file has been updated.\n";
									}
								}
								std::cout << "Recording finished.\n";
							}else{
								strokeList.clear();
							}
						}else if( Event.key.code == sf::Keyboard::Return ){
							sendInput();
							strokeList.clear();
						}
						break;
					default:
						break;
				}
				break;
			case sf::Event::MouseWheelMoved: break;
			case sf::Event::MouseMoved:
				mouseMoved( Event.mouseMove.x, Event.mouseMove.y );
				break;
			case sf::Event::MouseButtonPressed:
				mousePress( Event.mouseButton.button, true, Event.mouseButton.x, Event.mouseButton.y );
				break;
			case sf::Event::MouseButtonReleased:
				mousePress( Event.mouseButton.button, false, Event.mouseButton.x, Event.mouseButton.y );
				break;
			case sf::Event::Closed:
				window->close();
				break;
			default:
				break;
			}
		}
		window->clear(sf::Color(170,220, 250));
				
		// const sf::Shader* shader = sf::RenderStates::Default.shader;
		// shader->setParameter("color", sf::Color(0, 0, 0, 255));	 
		sf::RenderStates states;
		
		if( strokeList.empty() == false ){
			for( std::vector< std::vector<Touch> >::iterator lineIter = strokeList.begin(); lineIter != strokeList.end(); ++lineIter ){
				if( lineIter->empty() ) continue;
				float prevX, prevY;
				std::vector<Touch>::iterator iter = lineIter->begin();
				prevX = iter->x;
				prevY = iter->y;
				++iter;
				while(iter != lineIter->end()){
					
					sf::Vertex line[2] = { sf::Vertex(sf::Vector2<float>(prevX, prevY), sf::Color::Black), 
										   sf::Vertex(sf::Vector2<float>(iter->x, iter->y), sf::Color::Black) };
					window->draw(line, 2, sf::Lines);
					
					prevX = iter->x;
					prevY = iter->y;
					++iter;
				}
			}
		}

#ifdef LDEBUG
    int n = recognizer.getImageDimension();
    Lifeline::GridPoint *grid = recognizer.getImageGrid();
    if( grid ){
			const int px = 5;
			sf::RectangleShape rect;
			rect.setOrigin(sf::Vector2<float>(10, 10));
			rect.setSize(sf::Vector2<float>(px*n+30, px*n+30));
			rect.setFillColor(sf::Color(255,255,255));
			window->draw(rect);
			
			sf::Color pixelColor;
						
			for(int i = 0; i < n; ++i)
			{
				for(int j = 0; j < n; ++j)
				{					
          Lifeline::GridPoint& gridPoint = grid[i*n+j];
					if( gridPoint.enabled ){
						int color = gridPoint.type;
						//if( color == 0 && gridPoint.dummy == 0 && gridPoint.curvature == 0) pixelColor = sf::Color(120,120,120);
						//if( color == 0 && gridPoint.dummy == 0 && gridPoint.curvature == 1) pixelColor = sf::Color(255, 255, 0);
						//if( color == 0 && gridPoint.dummy == 0 && gridPoint.curvature == 2) pixelColor = sf::Color(255, 165, 0);
						//if( color == 0 && gridPoint.dummy == 0 && gridPoint.curvature == 3) pixelColor = sf::Color(255, 69, 0);
						if( color == 1 && gridPoint.dummy == 0) pixelColor = sf::Color(255,0,0);
						else if( color == 2 && gridPoint.dummy == 0) pixelColor = sf::Color(0,0,255);
						else if( color == 3 && gridPoint.dummy == 0) pixelColor = sf::Color(0,255,0);
						else if(color == 0 && gridPoint.dummy == 1) pixelColor = sf::Color(124, 252, 0);
						else if(color == 1 && gridPoint.dummy == 1) pixelColor = sf::Color(124, 252, 0);
						else if(color == 2 && gridPoint.dummy == 1) pixelColor = sf::Color(124, 252, 0);
						else if(color == 3 && gridPoint.dummy == 1) pixelColor = sf::Color(124, 252, 0);
						else if(color == 0 && gridPoint.dummy == 2) pixelColor = sf::Color(255,255,0);
						else if(color == 1 && gridPoint.dummy == 2) pixelColor = sf::Color(255,255,0);
						else if(color == 2 && gridPoint.dummy == 2) pixelColor = sf::Color(255,255,0);
						else if(color == 3 && gridPoint.dummy == 2) pixelColor = sf::Color(255,255,0);
						else if(color == 0 && gridPoint.dummy == 3) pixelColor = sf::Color(40,79,79);
						else if(color == 1 && gridPoint.dummy == 3) pixelColor = sf::Color(40,79,79);
						else if(color == 2 && gridPoint.dummy == 3) pixelColor = sf::Color(40,79,79);
						else if(color == 3 && gridPoint.dummy == 3) pixelColor = sf::Color(40,79,79);
						
						rect.setOrigin(sf::Vector2<float>(-(20 + px*j), -(20 + px*i)));
						rect.setSize(sf::Vector2<float>(px, px));
						rect.setFillColor(pixelColor);
						window->draw(rect);
					}
				}
			}
		}
#endif

		window->display();
		sf::sleep(sf::milliseconds(20));
	}

	database_free_pointer(symbolbuffer);
	if( symbolbuffer ) delete[] symbolbuffer;

	delete window;
	return 0;
}

void mousePress(sf::Mouse::Button button, bool down, int x, int y){
	if( down == true ){
		if( strokeList.empty() ) timer->restart();
		previousX = x; 
		previousY = y;
		mouseDown = true;
		strokeList.push_back(std::vector<Touch>());
	}else{
		if( strokeList.empty() == false ) strokeList.back().push_back(Touch((float)x,(float)y,timer->getElapsedTime().asSeconds()));
		mouseDown = false;
	}
}

void mouseMoved(int x, int y){
	if( mouseDown ){
		if( (x-previousX) > 2 || (x-previousX) < -2 || (y-previousY) > 2 || (y-previousY) < -2 ){
			strokeList.back().push_back(Touch((float)x,(float)y,timer->getElapsedTime().asSeconds()));
			previousX = x;
			previousY = y;
		}
	}
}

void sendInput(){
	if( strokeList.empty() ) return;
	
  std::vector<Touch> allInput;
	
	for( std::vector< std::vector<Touch> >::iterator lineIter = strokeList.begin(); lineIter != strokeList.end(); ++lineIter ){
    allInput.insert(allInput.end(), lineIter->begin(), lineIter->end());
	}

  Lifeline::ResultSet results;
  recognizer.processPoints(allInput, results);
}

