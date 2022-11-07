#include <SFML/Graphics.hpp>  //Библиотека для отрисовки
#include <iostream>
#include <ctime> 
#define MAX_POLE 100
using namespace std;
using namespace sf;  //Подключение пространство имён sf (чтобы не писать sf::)
int WIDTH = 1600, HEIGHT = 800;

RenderWindow window(VideoMode(WIDTH, HEIGHT), "Input", Style::Default, ContextSettings(0,0,8));

bool CLICK = false, IN_POLE = false;
int MAIN_CHOOSE_INPUT = -1;

string test_string[MAX_POLE];
RectangleShape INPUT_RECT;	

class Input_string{
	public:
		int pos_x1, 
		pos_x2, 
		pos_y1, 
		pos_y2,
		font_size;
		
		Color clr;
		bool is_click = false, lock = false;
		string *txt;
		
		//Input_string(int x1, int x2, int y1, int y2, int size, string *text);
		void setFirstValue(int x1, int x2, int y1, int y2, int size, string *text);
		void setColor(Color color);
		void Draw();
};
/*
Input_string::Input_string(int x1, int y1, int x2, int y2, int size, string *text){
	pos_x1 = x1;
	pos_y1 = y1;
	pos_x2 = x2;
	pos_y2 = y2;
	font_size = size;
	txt = text;
}*/

void Input_string::setFirstValue(int x1, int y1, int x2, int y2, int size, string *text){
	pos_x1 = x1;
	pos_y1 = y1;
	pos_x2 = x2;
	pos_y2 = y2;
	font_size = size;
	txt = text;
}
	
	
void Input_string::setColor(Color color){
	clr = color;
}

void Input_string::Draw(){	
	//RectangleShape rectangle(Vector2f(pos_x2 - pos_x1, pos_y2 - pos_y1));	
	
	INPUT_RECT.setFillColor(clr);  	
	INPUT_RECT.setSize(Vector2f(pos_x2 - pos_x1, pos_y2 - pos_y1));
	INPUT_RECT.setPosition(pos_x1, pos_y1);  
	
 	//INPUT_RECT.setOutlineThickness(2);
 	//INPUT_RECT.setOutlineColor(Color::Black);
 	
	window.draw(INPUT_RECT);
	
	Font font;//шрифт 
	font.loadFromFile("arkhip.ttf");
		
	Text text(*txt, font, font_size);
	text.setFillColor(Color::Black);
	text.setPosition(pos_x1, pos_y1);//text.getGlobalBounds().width/2
	window.draw(text);
	
	if(pos_x1 + font_size * 0.7 + text.getGlobalBounds().width >= pos_x2)
		lock = true;
	
	if(is_click and (clock()/450) % 2 and !lock){
		RectangleShape line(Vector2f(font_size/2, 3));	
		line.setPosition(pos_x1 + font_size * 0.2 + text.getGlobalBounds().width, pos_y1 + font_size);  
		line.setFillColor(Color(180, 180, 180));  
	
		window.draw(line);
	}	
}

Input_string pole[MAX_POLE]; // x1, y1, x2, y2, font size 

void Check(int x, int y){
	IN_POLE = false;
	for(int i=0; i<MAX_POLE; i++){
		if(pole[i].pos_x1 <= x and x <= pole[i].pos_x2 and pole[i].pos_y1 <= y and y <= pole[i].pos_y2){
			pole[i].is_click = true;
			IN_POLE = true;
			MAIN_CHOOSE_INPUT = i;
		}
		else // если не на то нажали, то тогда делаем прошлые на false, если используем масиив то первоналально записываем i_choose а далее его обнуляем
			pole[i].is_click = false;
	}	
	//	IN_POLE = false;
	
}

int main()
{	
 	INPUT_RECT.setOutlineThickness(2);
 	INPUT_RECT.setOutlineColor(Color::Black);
	
	for(int i=0; i < MAX_POLE; i++){
		pole[i].setFirstValue(100 * (i % 10 + 1), 100 * (i/10 + 1), 100 * (i % 10 + 1) + 90, 100 * (i/10 + 1) + 50, 20, &test_string[i]);
		pole[i].setColor(Color::White);
	}

	while (window.isOpen()) 
	{
		Event event;  
		while (window.pollEvent(event))  
		{
			if (event.type == Event::Closed)  
				window.close(); 
				
			if (event.type == Event::Resized) 
			{
	            WIDTH = event.size.width;
	            HEIGHT = event.size.height;          
	            window.setView(
	                View(
	                   Vector2f(WIDTH / 2.0, HEIGHT / 2.0), 
	                   Vector2f(WIDTH, HEIGHT)
	                )
	            );
	        }
	        if (event.type == Event::KeyPressed){ //cout<<event.key.code<<endl;
				if(IN_POLE and pole[MAIN_CHOOSE_INPUT].is_click){ // ну или через i_switch  97 - 122
					if(!pole[MAIN_CHOOSE_INPUT].lock){
						if(event.key.code < 26)
							*pole[MAIN_CHOOSE_INPUT].txt += char(97 + event.key.code); //48 57
						else
							if(event.key.code < 36)
								*pole[MAIN_CHOOSE_INPUT].txt += char(48 + event.key.code - 26);
						
						if(event.key.code == 56)
							*pole[MAIN_CHOOSE_INPUT].txt += char(45);
							
						if(event.key.code == 57)
							*pole[MAIN_CHOOSE_INPUT].txt += char(32);
					}
						
					if(event.key.code == 59 and (*pole[MAIN_CHOOSE_INPUT].txt).size() != 0){
						*pole[MAIN_CHOOSE_INPUT].txt = (*pole[MAIN_CHOOSE_INPUT].txt).erase((*pole[MAIN_CHOOSE_INPUT].txt).size()-1, 1);
						pole[MAIN_CHOOSE_INPUT].lock = false;
					}
						
					if(event.key.code == 58) // если нажали на интер, то выводим выбранную строку
						cout<<test_string[MAIN_CHOOSE_INPUT]<<endl;
				}
			}  
	        if (event.type == Event::MouseButtonPressed and event.mouseButton.button == Mouse::Left){
		        int x = event.mouseButton.x, y = event.mouseButton.y;
		        if(x <= WIDTH)
		        	CLICK = !CLICK, Check(x, y);
			}				
		}
		window.clear(Color(52, 160, 209));  //Очисктка окна и заливка в цвет(RGBA) 
		
		for(int i=0; i < MAX_POLE; i++)
			pole[i].Draw();
			
		window.display();  //Отрисовка окна
	}
	return 0;
}