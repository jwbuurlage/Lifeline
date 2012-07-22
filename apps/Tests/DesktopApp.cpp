#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
extern "C"{
#include "LRecognizer.h"
#include "LListener.h"
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

void callbackBest_match(char character, void* obj);
void callbackResult_set(LResultSet* result, void* obj);
void callbackSource_image(LImage* src, void* obj);

LRecognizer recognizer;

LImage* result_image;

int main(){
	std::cout << "---------------------------------\n";
	std::cout << "----- Lifeline test station -----\n";
	std::cout << "--- Press ESC to clear screen ---\n";
	std::cout << "------ Press enter to send ------\n";
	std::cout << "---------------------------------\n";
	
	recognizer.listener.char_found = callbackBest_match;
	recognizer.listener.result_set = callbackResult_set;
	recognizer.listener.source_image = callbackSource_image;
	recognizer_load_data(&recognizer);

	window = new sf::RenderWindow(sf::VideoMode(900, 400, 32), "Testing Station");
	timer = new sf::Clock;

	sf::Event Event;

	drawing = false;
	strokeList.clear();
	timer->Reset();
	while( window->IsOpened() ){
		while(window->GetEvent(Event)) {
			switch( Event.Type ){
			case sf::Event::KeyPressed: break;
			case sf::Event::KeyReleased: 
				if (Event.Key.Code == sf::Key::Escape){
					if( strokeList.empty() ) window->Close();
					else strokeList.clear();
				}else if(Event.Key.Code == sf::Key::Return){
					sendInput();
					strokeList.clear();
				}
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
		window->Clear(sf::Color(170,220, 250));

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
		
		if( result_image ){
			int n = result_image->size;
			
			const int px = 5;
			window->Draw(sf::Shape::Rectangle(10,10,px*n+30, px*n+30, sf::Color(255,255,255)));
			sf::Color pixelColor;
			for(int i = 0; i < n; ++i)
			{
				for(int j = 0; j < n; ++j)
				{
					LGridPoint gridPoint = result_image->grid[i*n+j];
					if( gridPoint.enabled ){
						int color = gridPoint.type;
						if( color == 0 && gridPoint.dummy == 0) pixelColor = sf::Color(120,120,120);
						else if( color == 1 && gridPoint.dummy == 0) pixelColor = sf::Color(255,0,0);
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
						window->Draw(sf::Shape::Rectangle(20+px*j,20+px*i, 20+px*j+px-1, 20+px*i+px-1, pixelColor));
					} 
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
		if( strokeList.empty() ) timer->Reset();
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
	if( strokeList.empty() ) return;
	
	LPointData point_data;
	list_init(&point_data, free);
	
	for( std::vector< std::vector<Touch> >::iterator lineIter = strokeList.begin(); lineIter != strokeList.end(); ++lineIter ){
		for( std::vector<Touch>::iterator touchIter = lineIter->begin(); touchIter != lineIter->end(); ++touchIter ){
			LPoint* point = (LPoint*)malloc(sizeof(LPoint));
			point->x = touchIter->x;
			point->y = touchIter->y;
			point->t = touchIter->time;
			list_insert_next( &point_data, point_data.tail, point);
		}
	}
	
	recognizer.source_points = 0;
	recognizer_set_data(&recognizer, &point_data);
	list_destroy(&point_data);
}

void callbackBest_match(char character, void* obj)
{

}

void callbackResult_set(LResultSet* result, void* obj)
{

}

void callbackSource_image(LImage* src, void* obj)
{
	if( result_image ){
		free(result_image->grid);
		free(result_image);
	}
	result_image = src;
}

