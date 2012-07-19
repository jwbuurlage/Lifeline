#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
extern "C"{
#include "LRecognizer.h"
}

void mousePress(sf::Mouse::Button button, bool down, int x, int y);
void mouseMoved(int x, int y);

void sendInput();

sf::RenderWindow* window;
sf::Clock* timer;

bool drawing;
int previousX, previousY;

struct Touch{
	float x, y;
	float time;
	Touch(float _x, float _y, float _time) : x(_x), y(_y), time(_time) {};
	~Touch(){};
};

std::vector< std::vector<Touch> > strokeList; //each element contains a list of points which is a stroke

int main(){
	std::cout << "---------------------------------\n";
	std::cout << "----- Lifeline test station -----\n";
	std::cout << "--- Press ESC to clear screen ---\n";
	std::cout << "------ Press enter to send ------\n";
	std::cout << "---------------------------------\n";

	window = new sf::RenderWindow(sf::VideoMode(400, 300, 32), "Testing Station");
	timer = new sf::Clock;

	sf::Event Event;

	drawing = false;
	strokeList.clear();
	while( window->IsOpened() ){
		while(window->GetEvent(Event)) {
			switch( Event.Type ){
			case sf::Event::KeyPressed: break;
			case sf::Event::KeyReleased: 
				if (Event.Key.Code == sf::Key::Escape) strokeList.clear();
				else if(Event.Key.Code == sf::Key::Return) sendInput();
				break;
			case sf::Event::MouseWheelMoved: break;
			case sf::Event::MouseMoved:
				mouseMoved( Event.MouseMove.X, Event.MouseMove.Y );
				break;
			case sf::Event::MouseButtonPressed:
				mousePress( Event.MouseButton.Button, true, Event.MouseButton.X, Event.MouseButton.Y );
				break;
			case sf::Event::MouseButtonReleased:
				mousePress( Event.MouseButton.Button, false, Event.MouseButton.X, Event.MouseButton.Y );
				break;
			case sf::Event::Closed:
				window->Close();
				break;
			default:
				break;
			}
		}
		window->Clear(sf::Color(150,200, 230));

		if( strokeList.empty() == false ){
			for( std::vector< std::vector<Touch> >::iterator lineIter = strokeList.begin(); lineIter != strokeList.end(); ++lineIter ){
				if( lineIter->empty() ) continue;
				float prevX, prevY;
				std::vector<Touch>::iterator iter = lineIter->begin();
				prevX = iter->x;
				prevY = iter->y;
				++iter;
				while(iter != lineIter->end()){
					window->Draw(sf::Shape::Line(prevX, prevY, iter->x, iter->y, 2.0f, sf::Color(0,0,0)));
					prevX = iter->x;
					prevY = iter->y;
					++iter;
				}
			}
		}

		window->Display();
		sf::Sleep(0.020f);
	}

	delete window;
	return 0;
}

void mousePress(sf::Mouse::Button button, bool down, int x, int y){
	if( down == true ){
		timer->Reset();
		previousX = x; 
		previousY = y;
		drawing = true;
		strokeList.push_back(std::vector<Touch>());
	}else{
		if( strokeList.empty() == false ) strokeList.back().push_back(Touch((float)x,(float)y,timer->GetElapsedTime()));
		drawing = false;
	}
}

void mouseMoved(int x, int y){
	if( drawing ){
		if( (x-previousX) > 2 || (x-previousX) < -2 || (y-previousY) > 2 || (y-previousY) < -2 ){
			strokeList.back().push_back(Touch((float)x,(float)y,timer->GetElapsedTime()));
			previousX = x;
			previousY = y;
		}
	}
}

void sendInput(){
	LRecognizer recognizer;
	LPointData point_data;
	recognizer_set_data(&recognizer, &point_data);
}
