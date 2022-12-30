#include<iostream>
#include<Windows.h>
#include<conio.h>
#include"include/SDL.h"
#include"include/SDL_ttf.h"
//#include"include/SDL_render.h"
//#include"include/SDL_shape.h"
//#include"include/SDL2_gfxPrimitives.h"
//#include"include/"
//#include"include/SDL_"
//#include<include/SDL2_gfxPrimitives.h >
#include"include/SDL_assert.h"
#define DEFAULT_ELLIPSE_OVERSCAN 4
int Transparency = 90;
int pixel(SDL_Renderer* renderer, Sint16 x, Sint16 y)
{
	return SDL_RenderDrawPoint(renderer, x, y);
}
int vline(SDL_Renderer* renderer, Sint16 x, Sint16 y1, Sint16 y2)
{
	return SDL_RenderDrawLine(renderer, x, y1, x, y2);;
}
int hline(SDL_Renderer* renderer, Sint16 x1, Sint16 x2, Sint16 y)
{
	return SDL_RenderDrawLine(renderer, x1, y, x2, y);;
}
int _drawQuadrants(SDL_Renderer* renderer, Sint16 x, Sint16 y, Sint16 dx, Sint16 dy, Sint32 f)
{
	int result = 0;
	Sint16 xpdx, xmdx;
	Sint16 ypdy, ymdy;

	if (dx == 0) {
		if (dy == 0) {
			result |= pixel(renderer, x, y);
		}
		else {
			ypdy = y + dy;
			ymdy = y - dy;
			if (f) {
				result |= vline(renderer, x, ymdy, ypdy);
			}
			else {
				result |= pixel(renderer, x, ypdy);
				result |= pixel(renderer, x, ymdy);
			}
		}
	}
	else {
		xpdx = x + dx;
		xmdx = x - dx;
		ypdy = y + dy;
		ymdy = y - dy;
		if (f) {
			result |= vline(renderer, xpdx, ymdy, ypdy);
			result |= vline(renderer, xmdx, ymdy, ypdy);
		}
		else {
			result |= pixel(renderer, xpdx, ypdy);
			result |= pixel(renderer, xmdx, ypdy);
			result |= pixel(renderer, xpdx, ymdy);
			result |= pixel(renderer, xmdx, ymdy);
		}
	}

	return result;
}
int _ellipseRGBA(SDL_Renderer* renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Sint32 f)
{
	int result;
	Sint32 rxi, ryi;
	Sint32 rx2, ry2, rx22, ry22;
	Sint32 error;
	Sint32 curX, curY, curXp1, curYm1;
	Sint32 scrX, scrY, oldX, oldY;
	Sint32 deltaX, deltaY;
	Sint32 ellipseOverscan;

	/*
	* Sanity check radii
	*/
	if ((rx < 0) || (ry < 0)) {
		return (-1);
	}

	/*
	* Set color
	*/
	result = 0;
	result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
	result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);

	/*
	* Special cases for rx=0 and/or ry=0: draw a hline/vline/pixel
	*/
	if (rx == 0) {
		if (ry == 0) {
			return (pixel(renderer, x, y));
		}
		else {
			return (vline(renderer, x, y - ry, y + ry));
		}
	}
	else {
		if (ry == 0) {
			return (hline(renderer, x - rx, x + rx, y));
		}
	}

	/*
	 * Adjust overscan
	 */
	rxi = rx;
	ryi = ry;
	if (rxi >= 512 || ryi >= 512)
	{
		ellipseOverscan = DEFAULT_ELLIPSE_OVERSCAN / 4;
	}
	else if (rxi >= 256 || ryi >= 256)
	{
		ellipseOverscan = DEFAULT_ELLIPSE_OVERSCAN / 2;
	}
	else
	{
		ellipseOverscan = DEFAULT_ELLIPSE_OVERSCAN / 1;
	}

	/*
	 * Top/bottom center points.
	 */
	oldX = scrX = 0;
	oldY = scrY = ryi;
	result |= _drawQuadrants(renderer, x, y, 0, ry, f);

	/* Midpoint ellipse algorithm with overdraw */
	rxi *= ellipseOverscan;
	ryi *= ellipseOverscan;
	rx2 = rxi * rxi;
	rx22 = rx2 + rx2;
	ry2 = ryi * ryi;
	ry22 = ry2 + ry2;
	curX = 0;
	curY = ryi;
	deltaX = 0;
	deltaY = rx22 * curY;

	/* Points in segment 1 */
	error = ry2 - rx2 * ryi + rx2 / 4;
	while (deltaX <= deltaY)
	{
		curX++;
		deltaX += ry22;

		error += deltaX + ry2;
		if (error >= 0)
		{
			curY--;
			deltaY -= rx22;
			error -= deltaY;
		}

		scrX = curX / ellipseOverscan;
		scrY = curY / ellipseOverscan;
		if ((scrX != oldX && scrY == oldY) || (scrX != oldX && scrY != oldY)) {
			result |= _drawQuadrants(renderer, x, y, scrX, scrY, f);
			oldX = scrX;
			oldY = scrY;
		}
	}

	/* Points in segment 2 */
	if (curY > 0)
	{
		curXp1 = curX + 1;
		curYm1 = curY - 1;
		error = ry2 * curX * curXp1 + ((ry2 + 3) / 4) + rx2 * curYm1 * curYm1 - rx2 * ry2;
		while (curY > 0)
		{
			curY--;
			deltaY -= rx22;

			error += rx2;
			error -= deltaY;

			if (error <= 0)
			{
				curX++;
				deltaX += ry22;
				error += deltaX;
			}

			scrX = curX / ellipseOverscan;
			scrY = curY / ellipseOverscan;
			if ((scrX != oldX && scrY == oldY) || (scrX != oldX && scrY != oldY)) {
				oldY--;
				for (; oldY >= scrY; oldY--) {
					result |= _drawQuadrants(renderer, x, y, scrX, oldY, f);
					/* prevent overdraw */
					if (f) {
						oldY = scrY - 1;
					}
				}
				oldX = scrX;
				oldY = scrY;
			}
		}

		/* Remaining points in vertical */
		if (!f) {
			oldY--;
			for (; oldY >= 0; oldY--) {
				result |= _drawQuadrants(renderer, x, y, scrX, oldY, f);
			}
		}
	}

	return (result);
}
int filledEllipseRGBA(SDL_Renderer* renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	return _ellipseRGBA(renderer, x, y, rx, ry, r, g, b, a, 1);
}
int filledCircleColor(SDL_Renderer* renderer, Sint16 x, Sint16 y, Sint16 rad, Uint32 color)
{
	Uint8* c = (Uint8*)&color;
	return filledEllipseRGBA(renderer, x, y, rad, rad, c[0], c[1], c[2], c[3]);
}

SDL_Window* window = SDL_CreateWindow("Button Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
// Create a renderer for the window
SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
TTF_Font* font;
using namespace std;
class Button {
	SDL_Rect Letter_Button;
	char text_for_button;
	bool Button_pushed;
	bool Button_hovered;
	COORD Position;
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
		Button_pushed(0), Button_hovered(0), Position({ x_axis, y_axis }) {}
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
		SDL_SetRenderDrawColor(renderer, 75, 75, 75, Transparency);
		SDL_RenderFillRect(renderer, &buttonShadow);
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
		SDL_SetRenderDrawColor(renderer, 255, 65, 65, 250);
		SDL_RenderFillRect(renderer, &Letter_Button);
		SDL_RenderCopy(renderer, buttonTextTexture1, NULL, &Letter_Button);
		//if(Button_hover)			//change color
	}
};
Button Normal_Letters;

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	font = TTF_OpenFont("arial.ttf", 1000);//16  //max : 7332
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

		if (_kbhit())
			_getch();
		if (Button_Pushed || 1)//if not Pressed then show shadow
		{
			SDL_SetRenderDrawColor(renderer, 75, 75, 75, Transparency);
			SDL_RenderFillRect(renderer, &button_Shadow);
		}
		if (buttonHovered) {
			// Use cyan and yellow for the button color when it is hovered
			SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
			SDL_RenderFillRect(renderer, &buttonRect);
			//SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		}
		else {
			// Use white for the button color when it is not hovered
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
		SDL_RenderFillRect(renderer, &buttonRect);
		SDL_RenderCopy(renderer, buttonTextTexture, NULL, &buttonRect);
		Normal_Letters.Display_Button();
		SDL_SetRenderDrawColor(renderer, 47, 15, 154, 255);
		filledCircleColor(renderer, 10 + 50, 10 + 50, 50, 0xFF0000FF);
		//filledCircleColor(renderer, (Normal_Letters.get_Position().X+50), Normal_Letters.get_Position().Y + 50, 15, 0xFF0000FF);
		SDL_RenderCopy(renderer, buttonTextTexture, NULL, &buttonRect);
		SDL_RenderPresent(renderer);
	}
}
