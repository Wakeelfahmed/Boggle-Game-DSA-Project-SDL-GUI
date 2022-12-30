#include<iostream>
#include<Windows.h>
#include<conio.h>
#include"include/SDL.h"
#include"include/SDL_ttf.h"
#include"include/SDL2_gfxPrimitives.h"
int Transparency = 95;
SDL_Window* window = SDL_CreateWindow("Button Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
TTF_Font* font;
using namespace std;
class Button {
	SDL_Rect Letter_Button;
	char text_for_button;
	bool Button_pushed;
	bool Button_hovered;
	COORD Position;
	COORD Shadow_Position;
	int Shadow_offset;
	int Button_Size;
	SDL_Rect buttonShadow;
	SDL_Surface* buttonTextSurface = TTF_RenderText_Solid(font, &text_for_button, { 0, 0, 255 }); //text Color
	SDL_Texture* buttonTextTexture1 = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
public:
	void set_Position(COORD Position) { this->Position = Position; }
	void set_Button_Pushed(bool Button_pushed) { this->Button_pushed = Button_pushed; }
	void set_Button_Hovered(bool Button_hovered) { this->Button_hovered = Button_hovered; }
	COORD get_Position() const { return Position; }
	bool get_Button_Pushed() const { return Button_pushed; }
	bool get_Button_Hovered() const { return Button_hovered; }
	Button() :
		Button_pushed(0), Button_hovered(0), Position({ 0,0 }), Shadow_offset(5), Button_Size(0) {}
	Button(short x_axis, short y_axis) :
		Button_pushed(0), Button_hovered(0), Position({ x_axis, y_axis }), Shadow_offset(5){}
	void Set_Button(char Alphabet, COORD Position, int Button_Size) {
		text_for_button = Alphabet;
		this->Position = Position;
		this->Button_Size = Button_Size;
		Letter_Button = { Position.X, Position.Y, Button_Size, Button_Size };
		buttonShadow = { Position.X + Shadow_offset, Position.Y + Shadow_offset, Button_Size, Button_Size };
		buttonTextSurface = TTF_RenderText_Solid(font, &text_for_button, { 0, 0, 255 }); //text Color
		buttonTextTexture1 = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
	}
	void Diplay_Shadow() {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		//SDL_SetRenderDrawColor(renderer, 75, 75, 75, Transparency);
		//SDL_RenderFillRect(renderer, &buttonShadow);
		int Correction_if_pushed = 0;
		roundedBoxRGBA(renderer, Position.X, Position.Y, buttonShadow.x + Button_Size, buttonShadow.y + Button_Size, 20, 75, 75, 75, Transparency);
	}
	void Display_Button() {
		if (Button_pushed) {
			Letter_Button =
			{ Position.X + 5, Position.Y + 5, Button_Size - Shadow_offset, Button_Size - Shadow_offset };
			buttonShadow.x = Position.X;
			buttonShadow.y = Position.Y;
		}
		else
		{
			Letter_Button = { Position.X, Position.Y , Button_Size , Button_Size };
			buttonShadow.x = Position.X + Shadow_offset;
			buttonShadow.y = Position.Y + Shadow_offset;
		}
		Diplay_Shadow();
		//SDL_SetRenderDrawColor(renderer, 255, 65, 65, 250);
		//SDL_RenderFillRect(renderer, &Letter_Button);
		roundedBoxRGBA(renderer, Letter_Button.x, Letter_Button.y, Letter_Button.x + Letter_Button.w, Letter_Button.y + Letter_Button.h, 20, 204, 0, 204, 255);
		//if (!Button_pushed)
			//filledCircleRGBA(renderer, (Position.X + 50), Position.Y + 50, 50, 0, 255, 0, 255);
		SDL_RenderCopy(renderer, buttonTextTexture1, NULL, &Letter_Button);
		//if(Button_hover)			//change color
	}
};
int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();		Button Normal_Letters;	font = TTF_OpenFont("arial.ttf", 1000);//16  //max : 7332

	SDL_Surface* buttonTextSurface = TTF_RenderText_Solid(font, "A", { 0, 0, 255 }); //text Color
	// Create a texture from the surface
	SDL_Texture* buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
	SDL_Rect buttonRect{ 10, 10, 100, 100 };
	int Shadow_offset = 5;
	SDL_Rect button_Shadow{ 10 + Shadow_offset, 10 + Shadow_offset, 100, 100 };
	bool buttonHovered = false, Button_Pushed = 0;
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	//cout << test << "\n";
	//int result = filledCircleColor(renderer, 60, 60, 20, 0xFF0000FF);
	//aacircleColor(renderer, 60, 60, 20, 20);
		//SDL_RenderFillRect(renderer, Position.X, Position.Y, 25);
		//filledCircleColor();
	//circleColor(renderer, center_x, center_y, radius, 22);
	SDL_Event event;
	Normal_Letters.Set_Button('B', { 200, 200 }, 100);
	while (true) {
		while (SDL_PollEvent(&event)) {
		/*	if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
				SDL_DestroyTexture(buttonTextTexture);
				SDL_FreeSurface(buttonTextSurface);
				TTF_CloseFont(font);
				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				SDL_Quit();
				exit(0);
				return 0;
			}*/
			if (event.type == SDL_MOUSEMOTION) {
				// Check if the mouse is hovering over the button rectangle
				int x, y;
				SDL_GetMouseState(&x, &y);
				buttonHovered =
					(
						x >= buttonRect.x && x < buttonRect.x + buttonRect.w &&
						y >= buttonRect.y && y < buttonRect.y + buttonRect.h
						);
			}
			if (event.type == SDL_MOUSEBUTTONUP) {
				// Check if the mouse click was within the button rectangle
				int x, y;
				SDL_GetMouseState(&x, &y);
				if (x >= Normal_Letters.get_Position().X && x < Normal_Letters.get_Position().X + 100 &&
					y >= Normal_Letters.get_Position().Y && y < Normal_Letters.get_Position().Y + 100) {
					Normal_Letters.set_Button_Pushed(!Normal_Letters.get_Button_Pushed());
				}
				if (x >= buttonRect.x && x < buttonRect.x + buttonRect.w &&
					y >= buttonRect.y && y < buttonRect.y + buttonRect.h) {
					// The mouse click was within the button, so do something
					std::cout << "Button clicked!\n";
					Button_Pushed = !Button_Pushed;
					Shadow_offset = Shadow_offset * -1;
					if (Button_Pushed) {
						buttonRect = { 15, 15, 95, 95 };
						button_Shadow.x = 10;
						button_Shadow.y = 10;
					}
					else
					{
						buttonRect = { 10, 10, 100, 100 };
						button_Shadow.x = 10 + Shadow_offset;
						button_Shadow.y = 10 + Shadow_offset;
					}

				}
			}
		}
		// Render the button
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		if (_kbhit())
			_getch();// >> x;
		if (Button_Pushed || 1)//if not Pressed then show shadow
		{
			SDL_SetRenderDrawColor(renderer, 75, 75, 75, Transparency);
			//SDL_SetRenderDrawColor(renderer/*,*/ 204, 204, 0, Transparency);
			SDL_RenderFillRect(renderer, &button_Shadow);
		}
		if (buttonHovered) {
			// Use cyan and yellow for the button color when it is hovered
			SDL_SetRenderDrawColor(renderer, 0, 255, 255, Transparency+70);
			SDL_RenderFillRect(renderer, &buttonRect);
			//SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		}
		else 
			SDL_SetRenderDrawColor(renderer, 204, 204, 0, 255);
		
		SDL_RenderFillRect(renderer, &buttonRect);
		SDL_RenderCopy(renderer, buttonTextTexture, NULL, &buttonRect);
		//roundedBoxRGBA(renderer, 10+50, 10+50, 100+50, 100+50, 20, 255, 75, 65, 255);
		Normal_Letters.Display_Button();
		//SDL_SetRenderDrawColor(renderer, 47, 15, 154, Transparency);//255
		//filledCircleColor(renderer, 10 + 50, 10 + 50, 50, 0xFF0000FF);
		if (!Button_Pushed)
			filledCircleRGBA(renderer, 10 + 50, 10 + 50, 50, 0, 255, 0, 200);
		//filledCircleColor(renderer, (Normal_Letters.get_Position().X+50), Normal_Letters.get_Position().Y + 50, 15, 0xFF0000FF);
		SDL_RenderCopy(renderer, buttonTextTexture, NULL, &buttonRect);
		SDL_RenderPresent(renderer);

	}
}
