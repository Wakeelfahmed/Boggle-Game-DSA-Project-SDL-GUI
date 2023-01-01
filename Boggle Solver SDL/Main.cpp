#include<iostream>
#include<Windows.h>
#include<conio.h>
//#include"include/SDL.h"
#include"C:\SDL2-devel-2.26.1-VC\include\SDL.h"
#include"C:\SDL2-devel-2.26.1-VC\include\SDL_ttf.h"
#include"C:\SDL2-devel-2.26.1-VC\include\SDL2_gfxPrimitives.h"
//#include"C:\SDL2-devel-2.26.1-VC\include"
int Transparency = 100;
SDL_Window* window = SDL_CreateWindow("Button Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
TTF_Font* font;
using namespace std;
class Button {
	SDL_Rect Main_Button;	SDL_Rect Shadow_box;
	char text_for_button;
	bool Button_Pushed;		bool Button_Hovered;
	int Shadow_offset;
	int Button_Size;
	COORD Position;
	SDL_Surface* buttonTextSurface = TTF_RenderText_Solid(font, &text_for_button, { 0, 0, 255 }); //text Color
	SDL_Texture* buttonTextTexture1 = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
public:
	Button() :
		Main_Button({ 0,0,0,0 }), Shadow_box({ 0,0,0,0 }), Button_Pushed(0), Button_Hovered(0), Position({ 0,0 }), Shadow_offset(5), Button_Size(0) {}
	//Button(short x_axis, short y_axis) :
		//Button_Pushed(0), Button_Hovered(0), Position({ x_axis, y_axis }), Shadow_offset(5) {}
	char get_text_of_button() const { return text_for_button; }
	void set_Position(COORD Position) { this->Position = Position; }
	void set_Button_Pushed(bool Button_pushed) { this->Button_Pushed = Button_pushed; }
	void set_Button_Hovered(bool Button_hovered) { this->Button_Hovered = Button_hovered; }
	COORD get_Position() const { return Position; }
	//SDL_Rect get_Position() const { return Position; }
	bool get_Button_Pushed() const { return Button_Pushed; }
	bool get_Button_Hovered() const { return Button_Hovered; }
	int get_Button_Size()const { return Button_Size; }
	void Set_Button(char Alphabet, COORD Position, int Button_Size) {
		text_for_button = Alphabet;
		this->Position = Position;
		this->Button_Size = Button_Size;
		Main_Button = { Position.X, Position.Y, Button_Size, Button_Size };
		Shadow_box = { Position.X + Shadow_offset, Position.Y + Shadow_offset, Button_Size, Button_Size };
		TTF_Font* font = TTF_OpenFont("arial.ttf", 100);
		buttonTextSurface = TTF_RenderText_Blended(font, &text_for_button, { 0, 0, 255 }); //text Color
		//buttonTextSurface = TTF_RenderText_Solid(font, &text_for_button, { 0, 0, 255 }); //text Color	//old
		buttonTextTexture1 = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
	}
	void Diplay_Shadow() {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		roundedBoxRGBA(renderer, Position.X, Position.Y, Shadow_box.x + Button_Size, Shadow_box.y + Button_Size, 20, 75, 75, 75, Transparency);
	}
	void Display_Button() {
		if (Button_Pushed) {
			Main_Button =
			{ Position.X + 5, Position.Y + 5, Button_Size - Shadow_offset, Button_Size - Shadow_offset };
			Shadow_box.x = Position.X;
			Shadow_box.y = Position.Y;
		}
		else
		{
			Main_Button = { Position.X, Position.Y , Button_Size , Button_Size };
			Shadow_box.x = Position.X + Shadow_offset;
			Shadow_box.y = Position.Y + Shadow_offset;
		}
		Diplay_Shadow();
		if (Button_Hovered)
			roundedBoxRGBA(renderer, Main_Button.x, Main_Button.y, Main_Button.x + Main_Button.w, Main_Button.y + Main_Button.h, 20, 0, 255, 255, 255);
		else
			roundedBoxRGBA(renderer, Main_Button.x, Main_Button.y, Main_Button.x + Main_Button.w, Main_Button.y + Main_Button.h, 20, 204, 0, 204, 255);
		if (!Button_Pushed)
			filledCircleRGBA(renderer, (Position.X + 50), Position.Y + 50, 50, 0, 255, 0, 255);

		int scale = 1.5;
		TTF_Font* font = TTF_OpenFont("arial.ttf", 100);
		int w, h;
		SDL_QueryTexture(buttonTextTexture1, nullptr, nullptr, &w, &h);
		int x = Main_Button.x + (Main_Button.w - w * scale) / 2;
		int y = Main_Button.y + (Main_Button.h - h * scale) / 2;
		SDL_Rect dst = { x, y, w * scale, h * scale };
		SDL_RenderCopy(renderer, buttonTextTexture1, nullptr, &dst);
	}
};
class Board {
	Button Alphabets[8];
	int Score;
public:
	Board() : Score(0) {}
};
int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();		Button Normal_Letters[9];	font = TTF_OpenFont("arial.ttf", 100);//16  //max : 7332 /1000
	//font = TTF_OpenFontDPI("arial.ttf", 5, 100, 100);
	//cout << TTF_FontHeight(font) << endl;
	SDL_Surface* buttonTextSurface = TTF_RenderText_Solid(font, "A", { 0, 0, 255 }); //text Color
	// Create a texture from the surface
	SDL_Texture* buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
	SDL_Rect buttonRect{ 10, 10, 100, 100 };
	int Shadow_offset = 5;
	SDL_Rect button_Shadow{ 10 + Shadow_offset, 10 + Shadow_offset, 100, 100 };
	bool buttonHovered = false, Button_Pushed = 0;
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_Event event;
	Normal_Letters[0].Set_Button('A', { 0, 0 }, 100);
	Normal_Letters[1].Set_Button('B', { 105 + 5, 0 }, 100);
	Normal_Letters[2].Set_Button('C', { 210 + 10, 0 }, 100);
	Normal_Letters[3].Set_Button('D', { 315 + 15, 0 }, 100);
	Normal_Letters[4].Set_Button('E', { 420 + 20, 0 }, 100);
	Normal_Letters[5].Set_Button('F', { 0       , 105 + 5 }, 100);
	Normal_Letters[6].Set_Button('G', { 105 + 5 , 105 + 5 }, 100);
	Normal_Letters[7].Set_Button('H', { 210 + 10, 105 + 5 }, 100);
	Normal_Letters[8].Set_Button('I', { 315 + 15, 105 + 5 }, 100);
	while (true) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
				SDL_DestroyTexture(buttonTextTexture);
				SDL_FreeSurface(buttonTextSurface);
				TTF_CloseFont(font);
				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				SDL_Quit();
				exit(0);
				return 0;
			}
			if (event.type == SDL_MOUSEMOTION)	//Mouse is hovering
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				buttonHovered =
					(
						x >= buttonRect.x && x < buttonRect.x + buttonRect.w &&
						y >= buttonRect.y && y < buttonRect.y + buttonRect.h
						);
				for (int i = 0; i < 9; i++)
				{
					Normal_Letters[i].set_Button_Hovered((x >= Normal_Letters[i].get_Position().X && x < Normal_Letters[i].get_Position().X + Normal_Letters[i].get_Button_Size() &&
						y >= Normal_Letters[i].get_Position().Y && y < Normal_Letters[i].get_Position().Y + Normal_Letters[i].get_Button_Size()));
				}
			}
			if (event.type == SDL_MOUSEBUTTONUP)	//mouse click on Button
			{
				int x, y;	SDL_GetMouseState(&x, &y);
				if (x >= Normal_Letters[1].get_Position().X && x < Normal_Letters[1].get_Position().X + 100 &&
					y >= Normal_Letters[1].get_Position().Y && y < Normal_Letters[1].get_Position().Y + 100) {
					Normal_Letters[1].set_Button_Pushed(!Normal_Letters[1].get_Button_Pushed());
				}
				if (x >= buttonRect.x && x < buttonRect.x + buttonRect.w &&
					y >= buttonRect.y && y < buttonRect.y + buttonRect.h) {
					// The mouse click was within the button, so do something
					//std::cout << "Button clicked!\n";
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
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);


		Normal_Letters[0].Display_Button();
		Normal_Letters[1].Display_Button();
		Normal_Letters[2].Display_Button();
		Normal_Letters[3].Display_Button();
		Normal_Letters[4].Display_Button();
		Normal_Letters[5].Display_Button();
		Normal_Letters[6].Display_Button();
		Normal_Letters[7].Display_Button();
		Normal_Letters[8].Display_Button();

		SDL_RenderPresent(renderer);	//Final Output to SDL window
	}
}