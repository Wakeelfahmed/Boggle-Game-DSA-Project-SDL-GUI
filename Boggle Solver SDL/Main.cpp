#include"include/SDL.h"
#include<iostream>
#include<Windows.h>
#include"include/SDL_ttf.h"

// Create a window
SDL_Window* window = SDL_CreateWindow("Button Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
// Create a renderer for the window
SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
TTF_Font* font;
using namespace std;
class Button {
public:
	SDL_Rect Letter_Button;
	char text_for_button;
	bool Button_pushed;
	bool Button_hovered;
	COORD Position;
	int Shadow_offset;
	int Button_Size;
	SDL_Rect buttonShadow;
	Button() :
		Button_pushed(0), Button_hovered(0), Position({ 0,0 }), Shadow_offset(5), Button_Size(0) {}
	Button(short x_axis, short y_axis) :
		Button_pushed(0), Button_hovered(0), Position({ x_axis, y_axis }) {}
	void Set_Button(char Alpahbet, COORD Position, int Button_Size) {
		text_for_button = Alpahbet;
		this->Position = Position;
		this->Button_Size = Button_Size;
		Letter_Button = { Position.X, Position.Y, Button_Size, Button_Size };
		buttonShadow = { Position.X + Shadow_offset, Position.Y + Shadow_offset, Button_Size, Button_Size };
	}
	void Diplay_Shadow() {
		SDL_SetRenderDrawColor(renderer, 75, 75, 75, 1);
		//SDL_ALPHA_OPAQUE(25);
		SDL_RenderFillRect(renderer, &buttonShadow);
	}
	void Display_Button() {
		Diplay_Shadow();
		SDL_Surface* buttonTextSurface = TTF_RenderText_Solid(font, &text_for_button, { 0, 0, 255 }); //text Color
		SDL_Texture* buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
		SDL_SetRenderDrawColor(renderer, 255, 65, 65, 1);
		SDL_RenderFillRect(renderer, &Letter_Button);
		SDL_RenderCopy(renderer, buttonTextTexture, NULL, &Letter_Button);
		//if(Button_pushed)
			//change color
		//if(Button_hover)
			//change color
		//SDL_RenderPresent(renderer);
	}
};
Button Normal_Letters[26];

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	font = TTF_OpenFont("arial.ttf", 2000);//16  //max : 7332
	// Load a font to use for the button text

	// Create a surface for the button text
	SDL_Surface* buttonTextSurface = TTF_RenderText_Solid(font, "A", { 0, 0, 255 }); //text Color

	// Create a texture from the surface
	SDL_Texture* buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);

	// Create a rectangle for the button position and dimensions
	SDL_Rect buttonRect{ 10, 10, 100, 100 };
	int Shadow_offset = 5;
	SDL_Rect button_Shadow{ 10 + Shadow_offset, 10 + Shadow_offset, 100, 100 };

	bool buttonHovered = false, Button_Pushed = 0;
	while (true) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
				// Close the window and quit the game
				SDL_DestroyTexture(buttonTextTexture);
				SDL_FreeSurface(buttonTextSurface);
				TTF_CloseFont(font);
				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				SDL_Quit();
				exit(0);
				return 0;
			}
			if (event.type == SDL_MOUSEMOTION) {
				// Check if the mouse is hovering over the button rectangle
				int x, y;
				SDL_GetMouseState(&x, &y);
				buttonHovered = (x >= buttonRect.x && x < buttonRect.x + buttonRect.w && y >= buttonRect.y && y < buttonRect.y + buttonRect.h);
			}
			if (event.type == SDL_MOUSEBUTTONUP) {
				// Check if the mouse click was within the button rectangle
				int x, y;
				SDL_GetMouseState(&x, &y);
				if (x >= buttonRect.x && x < buttonRect.x + buttonRect.w && y >= buttonRect.y && y < buttonRect.y + buttonRect.h) {
					// The mouse click was within the button, so do something
					std::cout << ("Button clicked!\n");
					Button_Pushed = !Button_Pushed;
					Shadow_offset = Shadow_offset * -1;
					button_Shadow.x = 10 + Shadow_offset;
					button_Shadow.y = 10 + Shadow_offset;
				}
			}
		}

		// Render the button
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		//for (size_t i = 0; i < 26; i++)
		//	Normal_Letters[i].text_for_button = char(i + 65);
		Normal_Letters[1].Set_Button('B', { 200, 200 }, 100);
		Normal_Letters[1].Display_Button();

		if (!Button_Pushed || 1)//if not Pressed then show shadow
		{
			SDL_SetRenderDrawColor(renderer, 75, 75, 75, 1);
			//SDL_ALPHA_OPAQUE(25);
			SDL_RenderFillRect(renderer, &button_Shadow);
		}
		if (buttonHovered) {
			// Use cyan and yellow for the button color when it is hovered
			SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
			SDL_RenderFillRect(renderer, &buttonRect);
			//SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			//SDL_RenderDrawRect(renderer, &buttonRect);
		}
		else {
			// Use white for the button color when it is not hovered
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
		SDL_RenderFillRect(renderer, &buttonRect);
		SDL_RenderCopy(renderer, buttonTextTexture, NULL, &buttonRect);
		SDL_RenderPresent(renderer);
	}
}
