#include<iostream>
#include<conio.h>
#include<fstream>
#include<string>
#include<sstream>
#include"C:\SDL2-devel-2.26.1-VC\include\SDL.h"		//for SDL GUI
#include"C:\SDL2-devel-2.26.1-VC\include\SDL_ttf.h"	//for SDL GUI Font
#include"C:\SDL2-devel-2.26.1-VC\include\SDL2_gfxPrimitives.h"	//for SDL GUI
#include"Trie Data Structure/Trie_Tree.h"	//My Trie Data Structure
#include"List/list.h"	//list for the line that appears behind Pressed Letters
#include"Generic_List/Generic_list.h"
int Transparency = 120;
SDL_Window* window = SDL_CreateWindow("Boggle Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 840/*width*/, 571, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
TTF_Font* font;
Trie_Tree Word_Dictionary_Trie_Tree;
using namespace std;
struct Size {
	int height;
	int width;
	Size() : height(0), width(0) {}
	Size(int h, int w) : height(h), width(w) {}
};
class Text_Box {
protected:
	string text_for_Box{};
	COORD Position{};
	SDL_Surface* buttonTextSurface{};
	SDL_Texture* buttonTextTexture{};
	SDL_Color Box_Color;
	Size Box_Size;
public:
	static double scale;
	static int Rounding_Radius;
	SDL_Rect Box;
	void set_Box_Text_Char(char Text, int Font_Size, SDL_Color Text_Color, int style) {
		if (buttonTextSurface) {
			SDL_FreeSurface(buttonTextSurface);
			buttonTextSurface = nullptr;
		}
		if (buttonTextTexture) {
			SDL_DestroyTexture(buttonTextTexture);
			buttonTextTexture = nullptr;
		}
		text_for_Box = "";
		text_for_Box = Text;
		TTF_SetFontSize(font, Font_Size);
		TTF_SetFontStyle(font, style);

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		buttonTextSurface = TTF_RenderText_Blended(font, text_for_Box.c_str(), Text_Color); //text Color
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	}
	void set_Box_Text(const char* Text, int Font_Size, SDL_Color Text_Color, bool BOLD) {
		if (buttonTextSurface) {
			SDL_FreeSurface(buttonTextSurface);
			buttonTextSurface = nullptr;
		}
		if (buttonTextTexture) {
			SDL_DestroyTexture(buttonTextTexture);
			buttonTextTexture = nullptr;
		}
		text_for_Box = Text;
		TTF_SetFontSize(font, Font_Size);
		if (BOLD)
			TTF_SetFontStyle(font, TTF_STYLE_BOLD);
		else
			TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
		buttonTextSurface = TTF_RenderText_Blended(font, text_for_Box.c_str(), Text_Color); //text Color
		buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
	}
	void set_Text_Box(const char* Text, int Font_Size, SDL_Color Text_Color, COORD Position, Size Box_Size, SDL_Color Box_Color, bool BOLD) {
		this->Position = Position;
		this->Box_Size = Box_Size;		Box = { Position.X, Position.Y, Box_Size.height, Box_Size.width };
		this->Box_Color = Box_Color;
		set_Box_Text(Text, Font_Size, Text_Color, BOLD);
	}
	void set_Box_Color(SDL_Color Box_Color1) { Box_Color = Box_Color1; }
	void set_Box_Size(Size New_Size) { Box_Size = New_Size; }

	void Display_Text_Box(SDL_Color newcolor, bool usenewcolor_instead_boxcolor) const {
		if (usenewcolor_instead_boxcolor)
			roundedBoxRGBA(renderer, Box.x, Box.y, Box.x + Box.w, Box.y + Box.h, Rounding_Radius, newcolor.r, newcolor.g, newcolor.b, 255);
		else
			roundedBoxRGBA(renderer, Box.x, Box.y, Box.x + Box.w, Box.y + Box.h, Rounding_Radius, Box_Color.r, Box_Color.g, Box_Color.b, 255);
		//scale = .65;//1.5
		int w, h;
		SDL_QueryTexture(buttonTextTexture, nullptr, nullptr, &w, &h);
		double x, y;
		x = Box.x + (Box.w - double(w) * scale) / 2;
		y = Box.y + (Box.h - double(h) * scale) / 2;
		SDL_Rect dst;
		dst = { int(x), int(y), int(w * scale),int(h * scale) };
		SDL_RenderCopy(renderer, buttonTextTexture, nullptr, &dst);
	}

	string get_text_for_Box() const { return text_for_Box; }
	SDL_Rect get_Main_Box() const { return Box; }
	SDL_Color get_Box_Color() const { return Box_Color; }
	Size get_Box_Size() const { return Box_Size; }
	COORD get_Box_Position() const { return Position; }
	SDL_Surface* get_buttonTextSurface() const { return buttonTextSurface; }
	SDL_Texture* get_buttonTextTexture() const { return buttonTextTexture; }
	~Text_Box() {
		//SDL_DestroyTexture(buttonTextTexture);
		//SDL_FreeSurface(buttonTextSurface);
		cout << "Called Text des\n";
	}
};
double Text_Box::scale = 0.65;
int Text_Box::Rounding_Radius = 20;
class Button {
protected:
	Text_Box Main_Text_Box;
	SDL_Rect Shadow_box;	bool Button_Pushed;		bool Button_Hovered;	int Shadow_offset;
public:
	Button() : Shadow_box({ 0,0,0,0 }), Button_Pushed(0), Button_Hovered(0), Shadow_offset(5) {}
	void Set_Button(const char* Alphabet, SDL_Color Text_Color, COORD Position, Size Button_Size, int Font_Size, SDL_Color Box_Color, bool BOLD) {
		Main_Text_Box.set_Text_Box(Alphabet, Font_Size, Text_Color, Position, Button_Size, Box_Color, BOLD);
		Shadow_box = { Position.X + Shadow_offset, Position.Y + Shadow_offset, Button_Size.height, Button_Size.width };
	}
	void set_char_of_button(char Alphabet) {
		//char ch = &Alphabet;
		Main_Text_Box.set_Box_Text_Char(Alphabet, 100, { 0, 0, 255 }, TTF_STYLE_BOLD);
	}
	void set_Button_Color(SDL_Color newColor) { Main_Text_Box.set_Box_Color(newColor); }
	void set_Button_Size(Size Button_Size) { this->Main_Text_Box.set_Box_Size(Button_Size); }
	void set_Button_Pushed(bool Button_pushed) { this->Button_Pushed = Button_pushed; }
	void set_Button_Hovered(bool Button_hovered) { this->Button_Hovered = Button_hovered; }
	bool Check_if_Mouse_in_Button_Area(int x, int y) const {
		return (x >= Main_Text_Box.get_Box_Position().X && x < Main_Text_Box.get_Box_Position().X + Main_Text_Box.get_Box_Size().height && y >= Main_Text_Box.get_Box_Position().Y && y < Main_Text_Box.get_Box_Position().Y + Main_Text_Box.get_Box_Size().width);
	}
	bool operator==(Button Button2) {
		return (Main_Text_Box.get_Box_Position().Y == Button2.Main_Text_Box.get_Box_Position().Y && Main_Text_Box.get_Box_Position().X == Button2.Main_Text_Box.get_Box_Position().X);
	}
	void Diplay_Shadow() const {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		if (Button_Pushed)//inner shadow
			roundedBoxRGBA(renderer, Main_Text_Box.get_Box_Position().X, Main_Text_Box.get_Box_Position().Y, Shadow_box.x + Main_Text_Box.get_Box_Size().height, Shadow_box.y + Main_Text_Box.get_Box_Size().width, 20, Main_Text_Box.get_Box_Color().r * (0.55), Main_Text_Box.get_Box_Color().g * (0.55), Main_Text_Box.get_Box_Color().b * (0.55), Transparency + 30);
		else
			roundedBoxRGBA(renderer, Main_Text_Box.get_Box_Position().X, Main_Text_Box.get_Box_Position().Y, Shadow_box.x + Main_Text_Box.get_Box_Size().height, Shadow_box.y + Main_Text_Box.get_Box_Size().width, 20, 75, 75, 75, Transparency);
	}
	void Display_Button(bool Makes_a_Word) {
		if (Button_Pushed) {
			Main_Text_Box.Box = { Main_Text_Box.get_Box_Position().X + 5, Main_Text_Box.get_Box_Position().Y + 5, Main_Text_Box.get_Box_Size().height - Shadow_offset, Main_Text_Box.get_Box_Size().width - Shadow_offset };
			Shadow_box.x = Main_Text_Box.get_Box_Position().X;
			Shadow_box.y = Main_Text_Box.get_Box_Position().Y;
		}
		else
		{
			Main_Text_Box.Box = { Main_Text_Box.get_Box_Position().X, Main_Text_Box.get_Box_Position().Y , Main_Text_Box.get_Box_Size().height , Main_Text_Box.get_Box_Size().width };
			Shadow_box.x = Main_Text_Box.get_Box_Position().X + Shadow_offset;
			Shadow_box.y = Main_Text_Box.get_Box_Position().Y + Shadow_offset;
		}
		Diplay_Shadow();
		if (Button_Hovered)
			roundedBoxRGBA(renderer, Main_Text_Box.get_Main_Box().x, Main_Text_Box.get_Main_Box().y, Main_Text_Box.get_Main_Box().x + Main_Text_Box.get_Main_Box().w, Main_Text_Box.get_Main_Box().y + Main_Text_Box.get_Main_Box().h, 20, Main_Text_Box.get_Box_Color().r * 0.80, Main_Text_Box.get_Box_Color().g * 0.80, Main_Text_Box.get_Box_Color().b * 0.80, 255);
		else
			roundedBoxRGBA(renderer, Main_Text_Box.get_Main_Box().x, Main_Text_Box.get_Main_Box().y, Main_Text_Box.get_Main_Box().x + Main_Text_Box.get_Main_Box().w, Main_Text_Box.get_Main_Box().y + Main_Text_Box.get_Main_Box().h, 20, Main_Text_Box.get_Box_Color().r, Main_Text_Box.get_Box_Color().g, Main_Text_Box.get_Box_Color().b, 255);
		if (!Button_Pushed)
			filledCircleRGBA(renderer, (Main_Text_Box.get_Box_Position().X + 50), Main_Text_Box.get_Box_Position().Y + 50, 43, 0, 255, 0, 255);

		int w, h;
		SDL_QueryTexture(Main_Text_Box.get_buttonTextTexture(), nullptr, nullptr, &w, &h);
		double x, y;
		x = Main_Text_Box.get_Main_Box().x + (Main_Text_Box.get_Main_Box().w - double(w) * Main_Text_Box.scale) / 2;
		y = Main_Text_Box.get_Main_Box().y + (Main_Text_Box.get_Main_Box().h - double(h) * Main_Text_Box.scale) / 2;
		SDL_Rect dst;
		dst = { int(x), int(y), int(w * Main_Text_Box.scale),int(h * Main_Text_Box.scale) };
		SDL_RenderCopy(renderer, Main_Text_Box.get_buttonTextTexture(), nullptr, &dst);
	}
	void Display_Text_Button() {
		if (Button_Pushed) {
			Main_Text_Box.Box =
			{ Main_Text_Box.get_Box_Position().X + 5, Main_Text_Box.get_Box_Position().Y + 5, Main_Text_Box.get_Box_Size().height - Shadow_offset, Main_Text_Box.get_Box_Size().width - Shadow_offset };
			Shadow_box.x = Main_Text_Box.get_Box_Position().X;
			Shadow_box.y = Main_Text_Box.get_Box_Position().Y;
		}
		else
		{
			Main_Text_Box.Box = { Main_Text_Box.get_Box_Position().X, Main_Text_Box.get_Box_Position().Y , Main_Text_Box.get_Box_Size().height , Main_Text_Box.get_Box_Size().width };
			Shadow_box.x = Main_Text_Box.get_Box_Position().X + Shadow_offset;
			Shadow_box.y = Main_Text_Box.get_Box_Position().Y + Shadow_offset;
		}
		SDL_Color temp = Main_Text_Box.get_Box_Color();
		Diplay_Shadow();
		if (Button_Hovered) {
			temp.r = temp.r * 0.85;
			temp.g = temp.g * 0.85;
			temp.b = temp.b * 0.85;
			temp.a = 255;
			Main_Text_Box.Display_Text_Box(temp, 1);
			return;
		}
		Main_Text_Box.Display_Text_Box({ 0 }, 0);
	}

	COORD get_Position() const { return Main_Text_Box.get_Box_Position(); }
	bool get_Button_Pushed() const { return Button_Pushed; }
	bool get_Button_Hovered() const { return Button_Hovered; }
	Size get_Button_Size() const { return Main_Text_Box.get_Box_Size(); }
	char get_char_of_button() const { return Main_Text_Box.get_text_for_Box().front(); }
	string get_text_of_button() const { return Main_Text_Box.get_text_for_Box(); }
	Text_Box get_Main_Text_Box() const { return Main_Text_Box; }
};
Text_Box Invalid_Word_Mess, Score_Board, Current_Word_Board;
class Players
{
protected:
	char Player_Name[50] = "";			int High_score{};		bool isActive{ false };
	int No_of_Player_Profiles{};
public:
	int get_high_score() const { return High_score; }
	void update_high_Score(int new_high_score) { High_score = High_score + (new_high_score - High_score); }
	char* get_Player_Name() { return Player_Name; }
	bool get_IsActive_Status()const { return isActive; }

};
Generic_list <string> Game_Settings_List;
class Board {
	Button Alphabets[16]; //using 1D array as it faster than 2D
	int Score;
	Trie_Tree Words_Made;
	string Current_Word;
	Button Last_Pressed_Button;
	bool Word_Made;
	Trie_Node* Current_Letter_Node;
	list Buttons_coord;
public:
	Board() : Score(0), Word_Made(0), Current_Word(""), Current_Letter_Node(NULL) {}
	void Set_Board(string Set_Board_Letters) {
		for (int i = 0; i < 16; i++) {
			string temp(1, Set_Board_Letters[i]);
			Alphabets[i].Set_Button(temp.c_str(), { 0, 0, 255 },
				{ short(10 + (i % 4) * (100 + 10)), short((10 + (i / 4) * (100 + 10) + 49)) },
				{ 100,100 }, 100, { 204, 0, 204, 255 }, 1);
		}
	}
	bool check_if_WordMade(char c) {
		if (Current_Letter_Node == NULL)
			Current_Letter_Node = Word_Dictionary_Trie_Tree.get_Tree_Root();

		c = tolower(c);
		int index = c - 'a';
		if (!(index >= 0 && index <= 25))//kind of exception handling
			exit(0);
		if (Current_Letter_Node->children[index] == nullptr) {

			Invalid_Word_Mess.Display_Text_Box({}, 0);
			SDL_RenderPresent(renderer);	//Final Output to SDL window
			SDL_Delay(750);

			Reset_Pressed_Letters();
			return false;
			//Word invalid and such suffix(word) will never be found
		}
		if (Current_Letter_Node->children[index]->children == NULL)
			return 0;
		Current_Letter_Node = Current_Letter_Node->children[index];

		if (Current_Letter_Node->is_end_of_word == 1 && Current_Letter_Node->is_registerd == 0) {
			//GOT END OF STRING & return 1;
			Word_Made = 1;	return 1;
		}
		Word_Made = 0;
		return 0;
	}
	bool check_if_Buttons_are_adjacent_in_grid(Button button2) {
		if (Last_Pressed_Button.get_Position().Y == button2.get_Position().Y
			&& abs(Last_Pressed_Button.get_Position().X - button2.get_Position().X) == Last_Pressed_Button.get_Button_Size().height + 10)
			return true;

		if (abs(Last_Pressed_Button.get_Position().X - button2.get_Position().X) == 100 + 10 &&
			abs(Last_Pressed_Button.get_Position().Y - button2.get_Position().Y) == 100 + 10)
			return true;

		// Check if the buttons are adjacent vertically
		if (Last_Pressed_Button.get_Position().X == button2.get_Position().X
			&& abs(Last_Pressed_Button.get_Position().Y - button2.get_Position().Y) == (Last_Pressed_Button.get_Button_Size()).width + 10)
			return true;

		return false;
	}
	void Check_for_Hovering(int MouseX, int MouseY, bool& Changes_Made) {
		for (int i = 0; i < 16; i++)
			if (Alphabets[i].Check_if_Mouse_in_Button_Area(MouseX, MouseY) != Alphabets[i].get_Button_Hovered())
			{
				Alphabets[i].set_Button_Hovered(Alphabets[i].Check_if_Mouse_in_Button_Area(MouseX, MouseY));
				Changes_Made = 1;
			}
	}
	void Reset_Pressed_Letters() {
		for (int i = 0; i < 16; i++)
			Alphabets[i].set_Button_Pushed(0);

		Current_Word = "";
		Word_Made = 0;
		Current_Letter_Node = NULL;
		Display_Board();
		while (!Buttons_coord.isEmpty())
			Buttons_coord.delete_Node(Buttons_coord.get_head()->get_Data());
	}
	void Reset_game() {
		Score = 0;
		Word_Dictionary_Trie_Tree.Reset_Registered_Words(Word_Dictionary_Trie_Tree.get_Tree_Root(), "");
		Reset_Pressed_Letters();
	}
	void Display_Board() {
		Display_Pressed_Buttons_LINE();
		for (int i = 0; i < 16; i++) {
			if (Word_Made && Alphabets[i].get_Button_Pushed())
				Alphabets[i].set_Button_Color({ 255,153,51,255 });
			else
				Alphabets[i].set_Button_Color({ 204,0,204,255 });
			Alphabets[i].Display_Button(Word_Made);
		}
	}
	void Display_Pressed_Buttons_LINE() {
		static COORD button1, button2;
		if (Buttons_coord.Number_of_Nodes() <= 1)
			return;
		for (int i = 1; i < Buttons_coord.Number_of_Nodes(); i++)
		{
			button1 = Buttons_coord.get_Node_by_Pos(i)->get_Data();
			button2 = Buttons_coord.get_Node_by_Pos(i + 1)->get_Data();
			if (Word_Made)
				thickLineRGBA(renderer, button1.X + 50, button1.Y + 50, button2.X + 50, button2.Y + 50, 15, 255, 153, 51, 255);
			else
				thickLineRGBA(renderer, button1.X + 50, button1.Y + 50, button2.X + 50, button2.Y + 50, 15, 204, 0, 204, 255);
		}
	}
	bool Check_for_Letters_input(int x, int y) {
		for (int i = 0; i < 16; i++)
			if (Alphabets[i].Check_if_Mouse_in_Button_Area(x, y))
			{
				Alphabets[i].set_Button_Pushed(!Alphabets[i].get_Button_Pushed());
				if (Alphabets[i].get_Button_Pushed()) {
					if (!Current_Word.empty())
						if (!check_if_Buttons_are_adjacent_in_grid(Alphabets[i]) && !(Alphabets[i] == Last_Pressed_Button)) // not adjacent to last pushed button
						{
							//Are NOT adjacent Buttons
							Alphabets[i].set_Button_Pushed(!Alphabets[i].get_Button_Pushed());
							break;
						}

					Buttons_coord.insert_end(Alphabets[i].get_Position());
					Current_Word += Alphabets[i].get_char_of_button();
					Last_Pressed_Button = Alphabets[i];
					if (check_if_WordMade(Alphabets[i].get_char_of_button())) {
						Word_Made = 1;
					}
					else
						Word_Made = 0;
				}
				else {	//if button released
					if (Word_Made) {
						Score += Score_Generator(Current_Word);
						Current_Letter_Node->is_registerd = 1;
						Reset_Pressed_Letters();
						break;
					}

					if (Alphabets[i].get_char_of_button() != Current_Word[Current_Word.size() - 1]) //if released button is in middle of already pressed buttons
					{
						//cout << "Lie in string and atempts to break\n";
						Reset_Pressed_Letters();
						break;
					}

					Current_Letter_Node = Word_Dictionary_Trie_Tree.get_Parent(Current_Letter_Node, Current_Word);
					Current_Word.erase(Current_Word.size() - 1, 1);
					Buttons_coord.delete_Node(Buttons_coord.get_Node_by_Pos(Buttons_coord.Number_of_Nodes())->get_Data());

					if (Current_Word == "")
						break;

					int index = tolower(Current_Word.front()) - 'a';
					if (Current_Letter_Node->is_end_of_word && Current_Letter_Node->is_registerd == 0) {
						Word_Made = 1;
					}
					else
						Word_Made = 0;
				}
				return 1;
			}
		return 0;
	}
	int Score_Generator(string Word) {
		switch (Word.size())
		{
		case 2:
		case 3:
		case 4:
			return 1;
		case 5:
			return 2;
		case 6:
			return 3;
		case 7:
			return 5;
		default:
			return 11;
		}
	}
	void Show_Registered_Words() {
		if (Score == 0)
			return;
		int smallest = INT_MAX; int Longest_String = 0;
		Word_Dictionary_Trie_Tree.get_smallest_longest_string(Word_Dictionary_Trie_Tree.get_Tree_Root(), "", smallest, Longest_String);
		if (smallest == Longest_String) {
		}
		Longest_String++;
		string text;
		int i = smallest, number_of_iterations_skiped = 0;
		Text_Box Registered_Word_list;
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		int x_correction = 0;
		Registered_Word_list.set_Text_Box("", 30, { 255,255,255, 255 }, { 450,150 }, { 100,324 }, { 43,31,143,0 }, 0);
		int x = Registered_Word_list.get_Box_Position().X + 5 + x_correction, y = Registered_Word_list.get_Box_Position().Y + 5;
		COORD org = { x ,y };
		static int w, h = 0;
		bool is_First_Word = 1;
		while (i != Longest_String) {
			Word_Dictionary_Trie_Tree.Display_Registered_Word(Word_Dictionary_Trie_Tree.get_Tree_Root(), "", text, i);
			// Get the size of the text
			TTF_SizeText(TTF_OpenFont("arial.ttf", 25), "", &w, &h);
			istringstream ss(text);
			int MAX_horizontal_spacing = 0;	static int Max_Box_Height = y;
			if (Score == 0)
				Max_Box_Height = y + 15;
			string word;
			int correction = 0;
			Registered_Word_list.Rounding_Radius = 15;
			while (getline(ss, word, ' ')) {

				SDL_Surface* surface = TTF_RenderText_Blended(TTF_OpenFont("arial.ttf", 25), word.c_str(), { 255,255,255,255 });
				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
				SDL_FreeSurface(surface);
				//if (word.size() <= 4 || is_First_Word) {
				//	correction = 25;
				//}
				//else
				//	correction = -5;
				if (is_First_Word || i + 1 == Longest_String)
					Registered_Word_list.Rounding_Radius = 20;
				else
					Registered_Word_list.Rounding_Radius = 0;
				//if (i != smallest)
					//is_First_Word = 0;
				SDL_QueryTexture(texture, NULL, NULL, &w, &h);
				if (MAX_horizontal_spacing < w)
					MAX_horizontal_spacing = w;
				if (Max_Box_Height < y)
					Max_Box_Height = y;
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
				if (i + 1 == Longest_String)
					Registered_Word_list.set_Text_Box("", 30, { 255,255,255, 255 }, { short(x - 5),short(y - 5) }, { 90,Max_Box_Height - y + 40 }, { 43,31,143,0 }, 0);
				else
					Registered_Word_list.set_Text_Box("", 30, { 255,255,255, 255 }, { short(x - 5),short(y - 5) }, { 100,Max_Box_Height - y + 40 }, { 43,31,143,0 }, 0);
				//Registered_Word_list.set_Text_Box("", 30, { 255,255,255, 255 }, { short(x - 5),short(y - 5) }, {100,w + correction }, { 43,31,143,0 }, 0);
				//boxSIZE: width, height
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
				Registered_Word_list.Display_Text_Box({ 0 }, 0);
				SDL_Rect rect = { x, y, w, h };
				//roundedBoxRGBA(renderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, 20, 75, 75, 75, 255);
				SDL_RenderCopy(renderer, texture, NULL, &rect);
				y += h;
				SDL_DestroyTexture(texture);
			}
			is_First_Word = 0;
			Registered_Word_list.Rounding_Radius = 20;
			text = "";
			if (word == "")
				x = (x + MAX_horizontal_spacing + i + 1 / 2), y = org.Y;
			//x = (x + MAX_horizontal_spacing + i/2), y = org.Y;
			else
				x = (x + MAX_horizontal_spacing + i + 1 / 2), y = org.Y;
			//x = (x + MAX_horizontal_spacing + 12), y = org.Y;
			i++;
			MAX_horizontal_spacing = 0;
			//x_correction = (3 + h);
			if (x > 840)
				SDL_SetWindowSize(window, x + 8, 571);
			SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		}
	}
	void Display_Current_Word() const {
		if (Current_Word == "")
			return;
		Current_Word_Board.set_Text_Box(Current_Word.c_str(), 40, { 255,255,255,255 }, { 470,70 }, { 150,60 }, { 75, 75, 75, 255 }, 0);
		Current_Word_Board.Display_Text_Box({ 0 }, 0);
	}
	void Display_Score() const {
		string score_mess = "Score " + to_string(Score);
		COORD Center;
		Center.X = (Alphabets[1].get_Position().X + Alphabets[2].get_Position().X) / 2 - 20;
		Center.Y = (Alphabets[1].get_Position().Y + Alphabets[2].get_Position().Y) / 2 - 73;
		Score_Board.set_Text_Box(score_mess.c_str(), 40, { 255,255,255,255 }, Center, { 150,60 }, { 75, 75, 75, 255 }, 0);
		Score_Board.Display_Text_Box({ 0 }, 0);
	}
	void Board_rotate() {
		char temp[4];
		for (size_t i = 0; i < 4; i++)
			temp[i] = Alphabets[i].get_char_of_button();
		Alphabets[3].set_char_of_button(Alphabets[0].get_char_of_button());
		Alphabets[0].set_char_of_button(Alphabets[12].get_char_of_button());
		Alphabets[1].set_char_of_button(Alphabets[8].get_char_of_button());
		Alphabets[2].set_char_of_button(Alphabets[4].get_char_of_button());

		Alphabets[12].set_char_of_button(Alphabets[15].get_char_of_button());
		Alphabets[8].set_char_of_button(Alphabets[14].get_char_of_button());
		Alphabets[4].set_char_of_button(Alphabets[13].get_char_of_button());

		{char temp_Inner_Sq = Alphabets[5].get_char_of_button();
		Alphabets[5].set_char_of_button(Alphabets[9].get_char_of_button());
		Alphabets[9].set_char_of_button(Alphabets[10].get_char_of_button());
		Alphabets[10].set_char_of_button(Alphabets[6].get_char_of_button());
		Alphabets[6].set_char_of_button(temp_Inner_Sq);
		}
		Alphabets[15].set_char_of_button(temp[3]);
		Alphabets[13].set_char_of_button(Alphabets[11].get_char_of_button());
		Alphabets[14].set_char_of_button(Alphabets[7].get_char_of_button());
		Alphabets[11].set_char_of_button(temp[2]);
		Alphabets[7].set_char_of_button(temp[1]);

	}
	int get_Game_Score() const { return Score; }
	void New_Game() {
		Reset_game();
		static Generic_Node <string>* Temp = Game_Settings_List.get_head();
		Set_Board(Temp->get_Data());
		Temp = Temp->next;
	}
	~Board() {
		delete Current_Letter_Node;
	}
};
void delete_trie(Trie_Node* node) {
	if (node == nullptr) return;

	// Recursively delete all children
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		cout << "deleting " << char('a' + i);
		delete_trie(node->get_child(i));
	}

	// Delete the current node
	delete node;
}
int Number_of_W_Read = 0;
void Read_Board_Letter() {
	ifstream file("Board_Letter.txt");
	if (!file) {
		cout << "File(Board_Letter.txt) Not Found !!\n"; _getch(); exit(1);
	}
	string* Temp = new string;
	while (!file.eof()) {
		file >> *Temp;
		Game_Settings_List.insert_end(*Temp);
	}
	delete Temp;
	cout << "Board_Letter.txt found - DONE READING\n";
}
void Read_fr_File_and_store_in_Trie_Tree(Trie_Tree& Word_Dictionary) {
	ifstream file("Words Dictionary.txt");
	if (!file) {
		cout << "File(Words Dictionary.txt) Not Found !!\n"; _getch(); exit(1);
	}
	string* temp = new string;
	while (!file.eof())
	{
		file >> *temp;		//cout << temp << endl;
		Word_Dictionary.Insert(*temp);
		Number_of_W_Read++;
	}
	delete temp;
	cout << "Words Dictionary.txt found - DONE READING\n";
	//Word_Dictionary.Display();
}
int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();		font = TTF_OpenFont("arial.ttf", 100);//16  //max : 7332 /1000
	if (font == NULL)
		cout << "ERROR!!!\n(Arial.ttf) Font Not Found - unable to render text" << endl;
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_Event event;
	Board Boggle_Game;
	Read_fr_File_and_store_in_Trie_Tree(Word_Dictionary_Trie_Tree);
	Button Rotate_Button, Reset_Button, New_Game_Button;
	Rotate_Button.Set_Button("Rotate", { 255,255,255,255 }, { 23 + 15,507 }, { 99, 50 }, 40, { 48, 68, 193, 255 }, 0);
	Reset_Button.Set_Button("Reset", { 255,255,255,255 }, { 23 + 130,507 }, { 99, 50 }, 40, { 48, 68, 193, 255 }, 0);
	New_Game_Button.Set_Button("New Game", { 255,255,255,255 }, { 23 + 244,507 }, { 140, 50 }, 40, { 255, 128, 0, 255 }, 0);
	Invalid_Word_Mess.set_Text_Box("Invalid Word", 40, { 255,255,255,255 }, { 470, 70 }, { 150,60 }, { 255,0,0, 255 }, 0);
	Players Game_Player;
	string Player_name;
	Word_Dictionary_Trie_Tree.Write_SORTED_To_File_fr_Trie_Tree();
	Read_Board_Letter();
	Boggle_Game.New_Game();
	int MouseX, MouseY;
	bool Changes_Made = 1;
	while (true) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				//SDL_DestroyTexture(buttonTextTexture);
				//SDL_FreeSurface(buttonTextSurface);
				//event.key.keysym.sym == SDLK_ESCAPE
				Boggle_Game.~Board();
				TTF_CloseFont(font);
				TTF_Quit();
				delete_trie(Word_Dictionary_Trie_Tree.get_Tree_Root());
				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				SDL_Quit();
				exit(0);
				return 0;
			}
			if (event.type == SDL_MOUSEMOTION)	//Mouse is hovering
			{
				SDL_GetMouseState(&MouseX, &MouseY);
				if (Rotate_Button.Check_if_Mouse_in_Button_Area(MouseX, MouseY) != Rotate_Button.get_Button_Hovered())
				{
					Changes_Made = 1;
					Rotate_Button.set_Button_Hovered(Rotate_Button.Check_if_Mouse_in_Button_Area(MouseX, MouseY));
					Rotate_Button.Display_Text_Button();
				}
				else if (Reset_Button.Check_if_Mouse_in_Button_Area(MouseX, MouseY) != Reset_Button.get_Button_Hovered())
				{
					Changes_Made = 1;
					Reset_Button.set_Button_Hovered(Reset_Button.Check_if_Mouse_in_Button_Area(MouseX, MouseY));
				}
				else if (New_Game_Button.Check_if_Mouse_in_Button_Area(MouseX, MouseY) != New_Game_Button.get_Button_Hovered())
				{
					Changes_Made = 1;
					New_Game_Button.set_Button_Hovered(New_Game_Button.Check_if_Mouse_in_Button_Area(MouseX, MouseY));
				}
				Boggle_Game.Check_for_Hovering(MouseX, MouseY, Changes_Made);
			}
			if (event.type == SDL_MOUSEBUTTONUP)	//mouse click on Button
			{
				Changes_Made = 1;
				SDL_GetMouseState(&MouseX, &MouseY);
				if (Boggle_Game.Check_for_Letters_input(MouseX, MouseY)) {
					break;
				}
				if (Rotate_Button.Check_if_Mouse_in_Button_Area(MouseX, MouseY)) {
					//RELEASED
					Boggle_Game.Board_rotate();
					Rotate_Button.set_Button_Pushed(0);
					break;
				}
				if (Reset_Button.Check_if_Mouse_in_Button_Area(MouseX, MouseY)) {
					//RELEASED
					Boggle_Game.Reset_game();
					Reset_Button.set_Button_Pushed(0);
					break;
				}
				if (New_Game_Button.Check_if_Mouse_in_Button_Area(MouseX, MouseY)) {
					//RELEASED
					Boggle_Game.New_Game();
					New_Game_Button.set_Button_Pushed(0);
					break;
				}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {	//mouse click on Button
				SDL_GetMouseState(&MouseX, &MouseY);

				if (Rotate_Button.Check_if_Mouse_in_Button_Area(MouseX, MouseY)) {
					Rotate_Button.set_Button_Pushed(!Rotate_Button.get_Button_Pushed());
					Rotate_Button.Display_Text_Button();
					break;
				}
				if (Reset_Button.Check_if_Mouse_in_Button_Area(MouseX, MouseY)) {
					Changes_Made = 1;
					Reset_Button.set_Button_Pushed(!Reset_Button.get_Button_Pushed());
					Reset_Button.Display_Text_Button();
					break;
				}
				if (New_Game_Button.Check_if_Mouse_in_Button_Area(MouseX, MouseY)) {
					Changes_Made = 1;
					New_Game_Button.set_Button_Pushed(!New_Game_Button.get_Button_Pushed());
					New_Game_Button.Display_Text_Button();
					break;
				}
			}
		}
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 130, 214, 240, 50);
		SDL_RenderClear(renderer);
		if (Changes_Made) {
			Boggle_Game.Display_Board();
			Rotate_Button.Display_Text_Button();
			Reset_Button.Display_Text_Button();
			New_Game_Button.Display_Text_Button();
			Boggle_Game.Show_Registered_Words();
			Boggle_Game.Display_Current_Word();
			Boggle_Game.Display_Score();
			if (Boggle_Game.get_Game_Score() > Game_Player.get_high_score())
				Game_Player.update_high_Score(Boggle_Game.get_Game_Score()); //updating Player high score at runtime
			cout << "printing";
			SDL_RenderPresent(renderer);	//Final Output to SDL window
		}
		Changes_Made = 0;
	}
}