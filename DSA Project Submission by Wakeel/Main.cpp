#include<iostream>
#include<Windows.h>
#include<conio.h>
#include<fstream>
#include"C:\SDL2-devel-2.26.1-VC\include\SDL.h"
#include"C:\SDL2-devel-2.26.1-VC\include\SDL_ttf.h"
#include"C:\SDL2-devel-2.26.1-VC\include\SDL2_gfxPrimitives.h"
#include"Trie Data Structure/Trie_Tree.h"	//My Trie Data Structure
#include<string>
#include<sstream>
//#include<string.h>
int Transparency = 120;
SDL_Window* window = SDL_CreateWindow("Boggle Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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
public:
	string text_for_Box;
	SDL_Rect Box;		SDL_Color Box_Color;			COORD Position;
	SDL_Surface* buttonTextSurface{};
	SDL_Texture* buttonTextTexture{};
	Size Box_Size;
	double scale = 0.65;
	void set_Box_Text(const char* Text, int Font_Size, SDL_Color Text_Color, bool BOLD) {
		text_for_Box = Text;
		TTF_Font* font = TTF_OpenFont("arial.ttf", Font_Size);
		if (BOLD)
			TTF_SetFontStyle(font, TTF_STYLE_BOLD);

		buttonTextSurface = TTF_RenderText_Blended(font, text_for_Box.c_str(), Text_Color); //text Color
		buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
	}
	void set_Box_Text_Wrapped(const char* Text, int Font_Size, SDL_Color Text_Color, bool BOLD) {
		text_for_Box = Text;
		TTF_Font* font = TTF_OpenFont("arial.ttf", Font_Size);
		if (BOLD)
			TTF_SetFontStyle(font, TTF_STYLE_BOLD);

		buttonTextSurface = TTF_RenderText_Blended_Wrapped(font, text_for_Box.c_str(), Text_Color, 10); //text Color
		buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
	}
	void set_Text_Box(const char* Text, int Font_Size, SDL_Color Text_Color, COORD Position, Size Button_Size, SDL_Color Button_Color) {
		this->Position = Position;
		this->Box_Size = Button_Size;		Box = { Position.X, Position.Y, Button_Size.height, Button_Size.width };
		this->Box_Color = Button_Color;

		set_Box_Text(Text, Font_Size, Text_Color, 0);
	}

	void set_Box_Color(SDL_Color Box_Color1) { Box_Color = Box_Color1; }
	void Display_Text_Box(SDL_Color newcolor, bool usenewcolor_instead_boxcolor) const {
		if (usenewcolor_instead_boxcolor)
			roundedBoxRGBA(renderer, Box.x, Box.y, Box.x + Box.w, Box.y + Box.h, 20, newcolor.r, newcolor.g, newcolor.b, 255);
		else
			roundedBoxRGBA(renderer, Box.x, Box.y, Box.x + Box.w, Box.y + Box.h, 20, Box_Color.r, Box_Color.g, Box_Color.b, 255);

		//scale = .65;//1.5
		int w, h;
		SDL_QueryTexture(buttonTextTexture, nullptr, nullptr, &w, &h);
		double x = Box.x + (Box.w - double(w) * scale) / 2;
		double y = Box.y + (Box.h - double(h) * scale) / 2;
		SDL_Rect dst = { int(x), int(y), int(w * scale),int(h * scale) };
		SDL_RenderCopy(renderer, buttonTextTexture, nullptr, &dst);
	}
	SDL_Rect get_Main_Box() const { return Box; }
	SDL_Color get_Box_Color() const { return Box_Color; }
	Size get_Box_Size() const { return Box_Size; }
	COORD get_Box_Position() const { return Position; }
	SDL_Surface* get_buttonTextSurface() const { return buttonTextSurface; }
	SDL_Texture* get_buttonTextTexture() const { return buttonTextTexture; }
};
class Button {
protected:
public:
	Text_Box Main_Text_Box;
	SDL_Rect Shadow_box;	bool Button_Pushed;		bool Button_Hovered;	int Shadow_offset;

	Button() : Shadow_box({ 0,0,0,0 }), Button_Pushed(0), Button_Hovered(0), Shadow_offset(5) {}
	void Set_Button(const char* Alphabet, COORD Position, Size Button_Size, int Font_Size, SDL_Color RGB_Color) {
		Main_Text_Box.set_Text_Box(Alphabet, Font_Size, { 255,255,255,255 }, Position, Button_Size, RGB_Color);
		Shadow_box = { Position.X + Shadow_offset, Position.Y + Shadow_offset, Button_Size.height, Button_Size.width };
	}
	void set_char_of_button(char Alphabet) {
		Main_Text_Box.text_for_Box = Alphabet;
		Main_Text_Box.set_Box_Text(&Alphabet, 100, { 0, 0, 255 }, 1);
	}
	void set_Position(COORD Position) { this->Main_Text_Box.Position = Position; }
	void set_Button_Size(Size Button_Size) { this->Main_Text_Box.Box_Size = Button_Size; }
	void set_Button_Pushed(bool Button_pushed) { this->Button_Pushed = Button_pushed; }
	void set_Button_Hovered(bool Button_hovered) { this->Button_Hovered = Button_hovered; }

	bool Check_if_Mouse_in_Button_Area(int x, int y) const {
		return (x >= Main_Text_Box.get_Box_Position().X && x < Main_Text_Box.get_Box_Position().X + Main_Text_Box.get_Box_Size().height && y >= Main_Text_Box.get_Box_Position().Y && y < Main_Text_Box.get_Box_Position().Y + Main_Text_Box.get_Box_Size().width);
	}
	bool operator==(Button Button2) {
		if (Main_Text_Box.Position.Y == Button2.Main_Text_Box.Position.Y && Main_Text_Box.Position.X == Button2.Main_Text_Box.Position.X)
			return 1;
		return 0;
	}
	void Diplay_Shadow() const {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		if (Button_Pushed)//inner shadow
			roundedBoxRGBA(renderer, Main_Text_Box.get_Box_Position().X, Main_Text_Box.get_Box_Position().Y, Shadow_box.x + Main_Text_Box.get_Box_Size().height, Shadow_box.y + Main_Text_Box.get_Box_Size().width, 20, Main_Text_Box.Box_Color.r * (0.55), Main_Text_Box.Box_Color.g * (0.55), Main_Text_Box.Box_Color.b * (0.55), Transparency + 30);
		else
			roundedBoxRGBA(renderer, Main_Text_Box.get_Box_Position().X, Main_Text_Box.get_Box_Position().Y, Shadow_box.x + Main_Text_Box.get_Box_Size().height, Shadow_box.y + Main_Text_Box.get_Box_Size().width, 20, 75, 75, 75, Transparency);
	}
	void Display_Button(bool Makes_a_Word) {
		if (!Makes_a_Word)
			Main_Text_Box.Box_Color = { 204,0,204,255 };
		if (Button_Pushed) {
			Main_Text_Box.Box =
			{ Main_Text_Box.Position.X + 5, Main_Text_Box.Position.Y + 5, Main_Text_Box.Box_Size.height - Shadow_offset, Main_Text_Box.Box_Size.width - Shadow_offset };
			Shadow_box.x = Main_Text_Box.Position.X;
			Shadow_box.y = Main_Text_Box.Position.Y;
			if (Makes_a_Word)
				Main_Text_Box.Box_Color = { 255,153,51,255 };
			else
				Main_Text_Box.Box_Color = { 204,0,204,255 };
		}
		else
		{
			Main_Text_Box.Box = { Main_Text_Box.Position.X, Main_Text_Box.Position.Y , Main_Text_Box.Box_Size.height , Main_Text_Box.Box_Size.width };
			Shadow_box.x = Main_Text_Box.Position.X + Shadow_offset;
			Shadow_box.y = Main_Text_Box.Position.Y + Shadow_offset;
		}
		Diplay_Shadow();
		if (Button_Hovered)
			roundedBoxRGBA(renderer, Main_Text_Box.Box.x, Main_Text_Box.Box.y, Main_Text_Box.Box.x + Main_Text_Box.Box.w, Main_Text_Box.Box.y + Main_Text_Box.Box.h, 20, Main_Text_Box.Box_Color.r * 0.80, Main_Text_Box.Box_Color.g * 0.80, Main_Text_Box.Box_Color.b * 0.80, 255);
		else
			roundedBoxRGBA(renderer, Main_Text_Box.Box.x, Main_Text_Box.Box.y, Main_Text_Box.Box.x + Main_Text_Box.Box.w, Main_Text_Box.Box.y + Main_Text_Box.Box.h, 20, Main_Text_Box.Box_Color.r, Main_Text_Box.Box_Color.g, Main_Text_Box.Box_Color.b, 255);
		if (!Button_Pushed)
			filledCircleRGBA(renderer, (Main_Text_Box.Position.X + 50), Main_Text_Box.Position.Y + 50, 43, 0, 255, 0, 255);

		double scale = .65;//1.5
		int w, h;
		SDL_QueryTexture(Main_Text_Box.buttonTextTexture, nullptr, nullptr, &w, &h);
		double x = Main_Text_Box.Box.x + (Main_Text_Box.Box.w - double(w) * scale) / 2;
		double y = Main_Text_Box.Box.y + (Main_Text_Box.Box.h - double(h) * scale) / 2;
		SDL_Rect dst = { int(x), int(y), int(w * scale),int(h * scale) };
		SDL_RenderCopy(renderer, Main_Text_Box.buttonTextTexture, nullptr, &dst);
	}
	void Display_Text_Button() {
		if (Button_Pushed) {
			Main_Text_Box.Box =
			{ Main_Text_Box.Position.X + 5, Main_Text_Box.Position.Y + 5, Main_Text_Box.Box_Size.height - Shadow_offset, Main_Text_Box.Box_Size.width - Shadow_offset };
			Shadow_box.x = Main_Text_Box.Position.X;
			Shadow_box.y = Main_Text_Box.Position.Y;
		}
		else
		{
			Main_Text_Box.Box = { Main_Text_Box.Position.X, Main_Text_Box.Position.Y , Main_Text_Box.Box_Size.height , Main_Text_Box.Box_Size.width };
			Shadow_box.x = Main_Text_Box.Position.X + Shadow_offset;
			Shadow_box.y = Main_Text_Box.Position.Y + Shadow_offset;
		}
		SDL_Color temp = Main_Text_Box.Box_Color;
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

	COORD get_Position() const { return Main_Text_Box.Position; }
	bool get_Button_Pushed() const { return Button_Pushed; }
	bool get_Button_Hovered() const { return Button_Hovered; }
	Size get_Button_Size() const { return Main_Text_Box.Box_Size; }
	char get_char_of_button() const { return Main_Text_Box.text_for_Box.front(); }
	string get_text_of_button() const { return Main_Text_Box.text_for_Box; }
};
Text_Box Inva;
class Board {
	Button Alphabets[16];
	int Score;
	Trie_Tree Words_Made;
	string Current_Word;
	Button Last_Pressed_Button;
	bool Word_Made;
	Trie_Node* Current_Letter_Node;
public:
	Board() : Score(0), Word_Made(0), Current_Word(""), Current_Letter_Node(NULL) {}
	void Set_Board() {
		for (int i = 0; i < 16; i++) {
			int x = 10 + (i % 4) * (100 + 10);	//making Grid 
			int y = 10 + (i / 4) * (100 + 10);	//making Grid 
			Alphabets[i].set_Button_Size({ 100, 100 });
			Alphabets[i].Set_Button("", { short(x),short(y) }, Alphabets[i].get_Button_Size(), 100, { 204, 0, 204, 255 });
		}
		Alphabets[15].get_Position();
		//string Set_Board_Letters = "FEBNETVMVSEEQZNT";
		string Set_Board_Letters = "ATEHHDSEVTMFWLIA";
		for (int i = 0; i < 16; i++)
			Alphabets[i].set_char_of_button(Set_Board_Letters[i]);
	}
	bool WordMade(char c) {
		if (Current_Letter_Node == NULL) {
			//cout << "TO root\n";
			Current_Letter_Node = Word_Dictionary_Trie_Tree.root;
		}
		c = tolower(c);
		int index = c - 'a';
		if (!(index >= 0 && index <= 25))//kind of exception handling
			exit(0);
		//cout << index << endl;
		if (Current_Letter_Node->children[index] == nullptr) {

			//cout << "Invalid Word\n";
			Inva.set_Text_Box("Invalid Word", 40, { 255,255,255,255 }, { 475,90 }, { 150,60 }, { 75, 75, 75, 255 });
			Inva.Display_Text_Box({}, 0);
			SDL_RenderPresent(renderer);	//Final Output to SDL window
			SDL_Delay(700);

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
		//cout << "default Returing false\n";
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
	void Check_for_Hovering(int x, int y) {
		for (int i = 0; i < 16; i++)
			Alphabets[i].set_Button_Hovered(Alphabets[i].Check_if_Mouse_in_Button_Area(x, y));
	}
	void Reset_Pressed_Letters() {
		for (int i = 0; i < 16; i++)
		{
			Alphabets[i].set_Button_Pushed(0);
		}
		Current_Word = "";
		Word_Made = 0;
		Current_Letter_Node = NULL;
		Display_Board();
	}
	void Display_Board() {
		for (int i = 0; i < 16; i++)
			Alphabets[i].Display_Button(Word_Made);
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
							//Are NOT adjacent Buttons trigered
							Alphabets[i].set_Button_Pushed(!Alphabets[i].get_Button_Pushed());
							break;
						}
					//if (!(Current_Word == ""))
						//thickLineRGBA(renderer, Last_Pressed_Button.get_Position().X + 50, Last_Pressed_Button.get_Position().Y + 50, Alphabets[i].get_Position().X + 50, Alphabets[i].get_Position().Y + 50, 15, 204, 0, 204, 255);
					//SDL_RenderPresent(renderer);
					Current_Word += Alphabets[i].get_char_of_button();
					Last_Pressed_Button = Alphabets[i];
					if (WordMade(Alphabets[i].get_char_of_button())) {
						Word_Made = 1;
					}
					else
						Word_Made = 0;
				}
				else {	//if button released
					bool exits_in_string = 0;
					if (Word_Made) {
						Score++;
						Current_Letter_Node->is_registerd = 1;
						Reset_Pressed_Letters();
						break;
					}

					if (Alphabets[i].get_char_of_button() != Current_Word[Current_Word.size() - 1]) //if released button is in middle of already pressed buttons
					{
						cout << "Lie in string and atempts to break\n";
						Reset_Pressed_Letters();
						break;
					}

					Current_Letter_Node = Word_Dictionary_Trie_Tree.get_Parent(Current_Letter_Node, Current_Word);
					Current_Word.erase(Current_Word.size() - 1, 1);

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
	void Show_Registered_Words() {
		if (Score == 0)
			return;
		std::string text;
		Text_Box Registered_Word_list;
		Word_Dictionary_Trie_Tree.Display_Registered_Word(Word_Dictionary_Trie_Tree.get_Tree_Root(), "", text);
		Registered_Word_list.set_Text_Box("", 30, { 255,255,255, 255 }, { 450,150 }, { 180,324 }, { 43,31,143,255 });

		int font_size = 25;
		// Get the size of the text
		int w, h;
		TTF_SizeText(TTF_OpenFont("arial.ttf", font_size), "", &w, &h);
		// Draw the text on the screen
		int x = Registered_Word_list.get_Box_Position().X + 5, y = Registered_Word_list.get_Box_Position().Y + 5;
		istringstream ss(text);
		string word;
		Registered_Word_list.Display_Text_Box({ 0 }, 0);
		while (std::getline(ss, word, ' ')) {
			SDL_Surface* surface = TTF_RenderText_Blended(TTF_OpenFont("arial.ttf", font_size), word.c_str(), { 255,255,255,255 });
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);
			SDL_QueryTexture(texture, NULL, NULL, &w, &h);
			SDL_Rect rect = { x, y, w, h };
			SDL_RenderCopy(renderer, texture, NULL, &rect);
			y += h;
			SDL_DestroyTexture(texture);
		}

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
};
int Number_of_W_Read = 0;
void Read_fr_File_and_store_in_Trie_Tree(Trie_Tree& Word_Dictionary) {
	string temp;
	ifstream file("Words Dictionary.txt");
	if (!file) {
		cout << "File(Words Dictionary.txt) Not Found !!\n"; _getch(); exit(1);
	}
	while (!file.eof())
	{
		file >> temp;		//cout << temp << endl;
		Word_Dictionary.Insert(temp);
		Number_of_W_Read++;
	}
	cout << "DONE READING\n";
	Word_Dictionary.Display();
	//_getch();
}
int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();		font = TTF_OpenFont("arial.ttf", 100);//16  //max : 7332 /1000
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_Event event;
	Board Boggle_Game;
	Boggle_Game.Set_Board();
	Read_fr_File_and_store_in_Trie_Tree(Word_Dictionary_Trie_Tree);
	Button Rotate_Button;
	Rotate_Button.Set_Button("Rotate", { 500,25 }, { 99, 50 }, 40, { 48, 68, 193, 255 });
	Word_Dictionary_Trie_Tree.Write_SORTED_To_File_fr_Trie_Tree();

	int MouseX, MouseY;
	while (true) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
				//SDL_DestroyTexture(buttonTextTexture);
				//SDL_FreeSurface(buttonTextSurface);
				//Boggle_Game.~Boggle_Game();
				TTF_CloseFont(font);
				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				SDL_Quit();
				exit(0);
				return 0;
			}
			if (event.type == SDL_MOUSEMOTION)	//Mouse is hovering
			{
				SDL_GetMouseState(&MouseX, &MouseY);
				Boggle_Game.Check_for_Hovering(MouseX, MouseY);
				Rotate_Button.set_Button_Hovered(Rotate_Button.Check_if_Mouse_in_Button_Area(MouseX, MouseY));

			}
			if (event.type == SDL_MOUSEBUTTONUP)	//mouse click on Button
			{
				SDL_GetMouseState(&MouseX, &MouseY);
				if (Boggle_Game.Check_for_Letters_input(MouseX, MouseY)) {
					break;
				}
				if (Rotate_Button.Check_if_Mouse_in_Button_Area(MouseX, MouseY)) {
					//RELEASED
					Boggle_Game.Board_rotate();
					Rotate_Button.set_Button_Pushed(0);
				}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {	//mouse click on Button
				SDL_GetMouseState(&MouseX, &MouseY);
				if (Rotate_Button.Check_if_Mouse_in_Button_Area(MouseX, MouseY)) {
					Rotate_Button.set_Button_Pushed(!Rotate_Button.get_Button_Pushed());
					Rotate_Button.Display_Text_Button();
					break;
				}
			}
		}

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 130, 214, 240, 50);
		SDL_RenderClear(renderer);

		Boggle_Game.Display_Board();
		Rotate_Button.Display_Text_Button();
		Boggle_Game.Show_Registered_Words();
		SDL_RenderPresent(renderer);	//Final Output to SDL window
	}
}