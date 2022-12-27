#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
//#include "SDL.h"
#include<conio.h>
using namespace sf;
int size = 152; //letters size
//don't do this, this is just an example
//SDL_Renderer* renderer;
bool isRunning;
bool fullscreen;
//void handleEvents();
//void update();
//void render();
//please don't put all your code in main like I did.
void Read_Words_Dictionary() {

}
Sprite f[26]; //figures
Vector2f offset(40, 40);
int main() {

	fullscreen = false;
	int flags = 0;
	RenderWindow window(VideoMode(1366, 700), "Boggle Word Game");
	sf::Texture t1, t2;
	//t1.loadFromFile("scrabble-letter-vector-pack.jpg");
	Sprite sBoard(t2);

	t1.loadFromFile("scrabble-letter-vector-pack.jpg");
	//t2.loadFromFile("images/board.png");
	for (int i = 0; i < 26; i++) f[i].setTexture(t1);
	float k = 0;
	int y_pos = 0;
	for (int i = 0; i < 26; i++)
	{
		f[i].setTextureRect(IntRect(k, y_pos, size, size));
		k += 152 +15;
		if (i % 5 == 0 && i != 0) {
			y_pos += 160+15;
			k = 2;
		}

	}
	y_pos = 1;
	short x_pos = 1;
	for (int i = 0; i <= 26; i++) {
		if (i % 7 == 0 && i != 0)
		{
			std::cout << "i is: " << i << std::endl;
			y_pos += 170;
			x_pos = 0;
		}
		f[i].setPosition((x_pos), y_pos);
		x_pos += 170;
	}
	//window.draw(sBoard);	
	///for (int i = 0; i < 26; i++) f[i].move(offset);
	for (int i = 0; i < 26; i++) window.draw(f[i]);
	//window.draw(f[5]);
	//for (int i = 0; i < 26; i++) f[i].move(-offset);
	window.display();
	_getch();
	//while (isRunning) {
	//	handleEvents();
	//	update();
	//	render();
	//}

	//frees memory associated with renderer and window
	//SDL_DestroyRenderer(renderer);
	//SDL_DestroyWindow(window);	//error here
	//SDL_Quit();
}

//handles any events that SDL noticed.
//void handleEvents() {
//	//the only event we'll check is the  SDL_QUIT event.
//	SDL_Event event;
//	SDL_PollEvent(&event);
//
//
//	switch (event.type) {
//	case SDL_QUIT:
//		isRunning = false;
//		break;
//	default:
//		break;
//	}
//}
//
////simple render function
//void render() {
//	SDL_SetRenderDrawColor(renderer, 121, 121, 121, 255);
//	SDL_RenderClear(renderer);
//
//	//your stuff to render would typically go here.
//	SDL_RenderPresent(renderer);
//}
//
////simple update function
//void update() {
//	//if things could update the code would go in here.
//}