#include<iostream>
#include<Windows.h>
#include<conio.h>
#include<fstream>
#include"C:\SDL2-devel-2.26.1-VC\include\SDL.h"
#include"C:\SDL2-devel-2.26.1-VC\include\SDL_ttf.h"
#include"C:\SDL2-devel-2.26.1-VC\include\SDL2_gfxPrimitives.h"
#include"Trie Data Structure/Trie_Tree.h"	//My Trie Data Structure
int Transparency = 120;
SDL_Window* window = SDL_CreateWindow("Boggle Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
TTF_Font* font;
Trie_Tree Word_Dictionary;
using namespace std;
struct Size {
	int height;
	int width;
	Size() : height(0), width(0) {}
	Size(int h, int w) : height(h), width(w) {}
};
class Text_Box {
protected:
	string text_for_button;
	SDL_Rect Main_Button;		SDL_Color Button_Color;			COORD Position;
	SDL_Surface* buttonTextSurface{};
	SDL_Texture* buttonTextTexture{};
	Size Button_Size;
public:
	void set_Text_Box(string Text, int Font_Size, SDL_Color Text_Color, COORD Position, Size Button_Size, SDL_Color Button_Color) {
		this->Position = Position;
		this->Button_Size = Button_Size;		Main_Button = { Position.X, Position.Y, Button_Size.height, Button_Size.width };
		this->Button_Color = Button_Color;
		text_for_button = Text;
		TTF_Font* font = TTF_OpenFont("arial.ttf", Font_Size);
		//TTF_SetFontStyle(font, TTF_STYLE_BOLD);
		//buttonTextSurface = TTF_RenderText_Blended(font, text_for_button.c_str(), { 255,255, 255, 255 }); //text Color
		buttonTextSurface = TTF_RenderText_Blended(font, text_for_button.c_str(), Text_Color); //text Color
		buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
	}
	void Display_Text_Box() {
		roundedBoxRGBA(renderer, Main_Button.x, Main_Button.y, Main_Button.x + Main_Button.w, Main_Button.y + Main_Button.h, 20, Button_Color.r, Button_Color.g, Button_Color.b, 255);

		double scale = .65;//1.5
		int w, h;
		SDL_QueryTexture(buttonTextTexture, nullptr, nullptr, &w, &h);
		double x = Main_Button.x + (Main_Button.w - double(w) * scale) / 2;
		double y = Main_Button.y + (Main_Button.h - double(h) * scale) / 2;
		SDL_Rect dst = { int(x), int(y), int(w * scale),int(h * scale) };
		SDL_RenderCopy(renderer, buttonTextTexture, nullptr, &dst);
		cout << "dis\n";
	}
};
class Button {
protected:
	Size Button_Size;
	SDL_Rect Main_Button;		SDL_Color Button_Color;		SDL_Rect Shadow_box;
	bool Button_Pushed;		bool Button_Hovered;	int Shadow_offset;
	COORD Position;
	SDL_Surface* buttonTextSurface{};// = TTF_RenderText_Solid(font, text_for_button, { 0, 0, 255 }); //text Color
	SDL_Texture* buttonTextTexture{};// = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
public:
	Button() : Main_Button({ 0,0,0,0 }), Shadow_box({ 0,0,0,0 }), Button_Color({ 0,0,0 }), Button_Size({ 0,0 }), Button_Pushed(0), Button_Hovered(0), Position({ 0,0 }), Shadow_offset(5) {}
	//Button() : Main_Button({ 0,0,0,0 }), Shadow_box({ 0,0,0,0 }), Button_Color({ 0,0,0 }), Button_Pushed(0), Button_Hovered(0), Position({ 0,0 }), Shadow_offset(5), Button_Size(0) {}
	void set_Position(COORD Position) { this->Position = Position; }
	void set_Button_Size(Size Button_Size) { this->Button_Size = Button_Size; }
	void set_Button_Pushed(bool Button_pushed) { this->Button_Pushed = Button_pushed; }
	void set_Button_Hovered(bool Button_hovered) { this->Button_Hovered = Button_hovered; }
	COORD get_Position() const { return Position; }
	bool get_Button_Pushed() const { return Button_Pushed; }
	bool get_Button_Hovered() const { return Button_Hovered; }
	bool operator==(Button Button2) {
		if (Position.Y == Button2.Position.Y && Position.X == Button2.Position.X)
			return 1;
		return 0;
	}
	bool Check_if_Mouse_in_Button_Area(int x, int y) const {
		return (x >= Position.X && x < Position.X + Button_Size.height && y >= Position.Y && y < Position.Y + Button_Size.width);
	}
	Size get_Button_Size() const { return Button_Size; }
	void Diplay_Shadow() const {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		if (Button_Pushed)//inner shadow
			roundedBoxRGBA(renderer, Position.X, Position.Y, Shadow_box.x + Button_Size.height, Shadow_box.y + Button_Size.width, 20, Button_Color.r * (0.55), Button_Color.g * (0.55), Button_Color.b * (0.55), Transparency + 30);
		else
			roundedBoxRGBA(renderer, Position.X, Position.Y, Shadow_box.x + Button_Size.height, Shadow_box.y + Button_Size.width, 20, 75, 75, 75, Transparency);
	}
};
class Letter_Button_Sq_button : public Button {
	char char_for_button{};
public:
	void Set_Button(char Alphabet, COORD Position, Size Button_Size, SDL_Color RGB_Color) {
		//text_for_button = Alphabet;	
		this->Position = Position;
		this->Button_Size = Button_Size;		Main_Button = { Position.X, Position.Y, Button_Size.height, Button_Size.width };
		Button_Color = RGB_Color;
		Shadow_box = { Position.X + Shadow_offset, Position.Y + Shadow_offset, Button_Size.height, Button_Size.width };
		set_char_of_button(Alphabet);
	}
	void set_char_of_button(char Alphabet) {
		char_for_button = Alphabet;
		TTF_Font* font = TTF_OpenFont("arial.ttf", 100);
		TTF_SetFontStyle(font, TTF_STYLE_BOLD);
		buttonTextSurface = TTF_RenderText_Blended(font, &char_for_button, { 0, 0, 255 }); //text Color
		buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
	}
	char get_char_of_button() const { return char_for_button; }
	void Display_Button(bool Makes_a_Word) {
		if (!Makes_a_Word)
			Button_Color = { 204,0,204,255 };
		if (Button_Pushed) {
			Main_Button =
			{ Position.X + 5, Position.Y + 5, Button_Size.height - Shadow_offset, Button_Size.width - Shadow_offset };
			Shadow_box.x = Position.X;
			Shadow_box.y = Position.Y;
			if (Makes_a_Word)
				Button_Color = { 255,153,51,255 };
			else
				Button_Color = { 204,0,204,255 };
		}
		else
		{
			Main_Button = { Position.X, Position.Y , Button_Size.height , Button_Size.width };
			Shadow_box.x = Position.X + Shadow_offset;
			Shadow_box.y = Position.Y + Shadow_offset;
		}
		Diplay_Shadow();
		if (Button_Hovered)
			roundedBoxRGBA(renderer, Main_Button.x, Main_Button.y, Main_Button.x + Main_Button.w, Main_Button.y + Main_Button.h, 20, Button_Color.r * 0.80, Button_Color.g * 0.80, Button_Color.b * 0.80, 255);
		//roundedBoxRGBA(renderer, Main_Button.x, Main_Button.y, Main_Button.x + Main_Button.w, Main_Button.y + Main_Button.h, 20, 0, 255, 255, 255);
		else
			roundedBoxRGBA(renderer, Main_Button.x, Main_Button.y, Main_Button.x + Main_Button.w, Main_Button.y + Main_Button.h, 20, Button_Color.r, Button_Color.g, Button_Color.b, 255);
		if (!Button_Pushed)
			filledCircleRGBA(renderer, (Position.X + 50), Position.Y + 50, 43, 0, 255, 0, 255);

		double scale = .65;//1.5
		int w, h;
		SDL_QueryTexture(buttonTextTexture, nullptr, nullptr, &w, &h);
		double x = Main_Button.x + (Main_Button.w - double(w) * scale) / 2;
		double y = Main_Button.y + (Main_Button.h - double(h) * scale) / 2;
		SDL_Rect dst = { int(x), int(y), int(w * scale),int(h * scale) };
		SDL_RenderCopy(renderer, buttonTextTexture, nullptr, &dst);
	}
};
class Text_Button : public Button
{
	string text_for_button;
public:
	Text_Button() : text_for_button("") {}
	void Set_Button(string Alphabet, COORD Position, Size Button_Size, int Font_Size, SDL_Color RGB_Color) {
		//text_for_button = Alphabet;	
		this->Position = Position;
		this->Button_Size = Button_Size;		Main_Button = { Position.X, Position.Y, Button_Size.height, Button_Size.width };
		Button_Color = RGB_Color;
		Shadow_box = { Position.X + Shadow_offset, Position.Y + Shadow_offset, Button_Size.height, Button_Size.width };
		set_text_of_button(Alphabet, Font_Size);
	}
	void set_text_of_button(string Alphabet, int Font_Size) {
		text_for_button = Alphabet;
		TTF_Font* font = TTF_OpenFont("arial.ttf", Font_Size);
		//TTF_SetFontStyle(font, TTF_STYLE_BOLD);
		buttonTextSurface = TTF_RenderText_Blended(font, text_for_button.c_str(), { 255,255, 255, 255 }); //text Color
		//buttonTextSurface = TTF_RenderText_Blended(font, text_for_button.c_str(), { 0, 0, 255 }); //text Color
		buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
	}
	string get_text_of_button() const { return text_for_button; }
	void Display_Button() {
		if (Button_Pushed) {
			Main_Button =
			{ Position.X + 5, Position.Y + 5, Button_Size.height - Shadow_offset, Button_Size.width - Shadow_offset };
			Shadow_box.x = Position.X;
			Shadow_box.y = Position.Y;
		}
		else
		{
			Main_Button = { Position.X, Position.Y , Button_Size.height , Button_Size.width };
			Shadow_box.x = Position.X + Shadow_offset;
			Shadow_box.y = Position.Y + Shadow_offset;
		}
		Diplay_Shadow();
		if (Button_Hovered)
			roundedBoxRGBA(renderer, Main_Button.x, Main_Button.y, Main_Button.x + Main_Button.w, Main_Button.y + Main_Button.h, 20, Button_Color.r * 0.85, Button_Color.g * 0.85, Button_Color.b * 0.85, 255);
		//roundedBoxRGBA(renderer, Main_Button.x, Main_Button.y, Main_Button.x + Main_Button.w, Main_Button.y + Main_Button.h, 20, 0, 255, 255, 255);
		else
			roundedBoxRGBA(renderer, Main_Button.x, Main_Button.y, Main_Button.x + Main_Button.w, Main_Button.y + Main_Button.h, 20, Button_Color.r, Button_Color.g, Button_Color.b, 255);

		double scale = .65;//1.5
		int w, h;
		SDL_QueryTexture(buttonTextTexture, nullptr, nullptr, &w, &h);
		double x = Main_Button.x + (Main_Button.w - double(w) * scale) / 2;
		double y = Main_Button.y + (Main_Button.h - double(h) * scale) / 2;
		SDL_Rect dst = { int(x), int(y), int(w * scale),int(h * scale) };
		SDL_RenderCopy(renderer, buttonTextTexture, nullptr, &dst);
	}
};
class Board {
	Letter_Button_Sq_button Alphabets[16];
	int Score;
	Trie_Tree Words_Made;
	string Current_Word;
	Letter_Button_Sq_button Last_Pressed_Button;
	bool Word_Made;
	Trie_Node* Current_Letter_Node;
public:
	Board() : Score(0), Word_Made(0), Current_Word(""), Current_Letter_Node(NULL) {}
	void Set_Board() {
		for (int i = 0; i < 16; i++) {
			int x = 10 + (i % 4) * (100 + 10);	//making Grid 
			int y = 10 + (i / 4) * (100 + 10);	//making Grid 
			Alphabets[i].set_Button_Size({ 100, 100 });
			Alphabets[i].Set_Button(' ', { short(x),short(y) }, Alphabets[i].get_Button_Size(), { 204, 0, 204, 255 });
		}
		Alphabets[15].get_Position();
		//string Set_Board_Letters = "FEBNETVMVSEEQZNT";
		string Set_Board_Letters = "ATEHHDSEVTMFWLIA";
		for (int i = 0; i < 16; i++)
			Alphabets[i].set_char_of_button(Set_Board_Letters[i]);
		{//Alphabets[0].set_text_of_button('T');		//Alphabets[1].set_text_of_button('T');
		//Alphabets[2].set_text_of_button('T');		//Alphabets[3].set_text_of_button('T');	
		//Alphabets[4].set_text_of_button('T');		//Alphabets[5].set_text_of_button('T');		
		//Alphabets[6].set_text_of_button('T');		//Alphabets[7].set_text_of_button('T');	
		//Alphabets[8].set_text_of_button('T');		//Alphabets[9].set_text_of_button('T');	
		//Alphabets[10].set_text_of_button('T')		//Alphabets[11].set_text_of_button('T')		
		//Alphabets[12].set_text_of_button('T')		//Alphabets[13].set_text_of_button('T')	
		//Alphabets[14].set_text_of_button('T')		//Alphabets[15].set_text_of_button('T')
		//Alphabets[16].set_text_of_button('T')		//Alphabets[17].set_text_of_button('T')
		}
	}
	bool WordMade(char c) {
		if (Current_Letter_Node == NULL) {
			cout << "TO root\n";
			Current_Letter_Node = Word_Dictionary.root;
		}
		c = tolower(c);
		int index = c - 'a';
		if (!(index >= 0 && index <= 25))//kind of exception handling
			exit(0);
		cout << index << endl;
		if (Current_Letter_Node->children[index] == nullptr) {
			static int Disappear = 5000;
			cout << "nullPTR returned false\n";
			//Text_Button Invalid_Word_message;
			//Invalid_Word_message.Set_Button("Invalid Word")


			//SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Invalid Word", { 0, 0, 255 });
			//SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		/*	SDL_Rect textRect;
			textRect.x = (640 - textSurface->w) / 2;
			textRect.y = (480 - textSurface->h) / 2;
			textRect.w = textSurface->w;
			textRect.h = textSurface->h;	*/		//TTF_OpenFont("arial.ttf", 100);
			//SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, "Invalid Word", { 0, 0, 255 }));
			/*SDL_RenderCopy(renderer,
				SDL_CreateTextureFromSurface(renderer,
					TTF_RenderText_Solid(TTF_OpenFont("arial.ttf", 500), "Invalid Word", { 0, 0, 0,255 })),
				nullptr, &dst);*/
				//SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
			cout << "INVALID\n";
			Text_Box Inva;
			Inva.set_Text_Box("INVALID", 40, { 255,255,255,255 }, { 500,100 }, { 100,60 }, { 75, 75, 75, 255 });
			Inva.Display_Text_Box();
			SDL_RenderPresent(renderer);	//Final Output to SDL window
			SDL_Delay(500);
			//roundedBoxRGBA(renderer, 0, 0, 0 + 100, Main_Button.y + Main_Button.h, 20, Button_Color.r, Button_Color.g, Button_Color.b, 255);

			//double scale = .65;//1.5
			//int w, h;
			//SDL_QueryTexture(buttonTextTexture, nullptr, nullptr, &w, &h);
			//double x = Main_Button.x + (Main_Button.w - double(w) * scale) / 2;
			//double y = Main_Button.y + (Main_Button.h - double(h) * scale) / 2;
			//SDL_Rect dst = { int(x), int(y), int(w * scale),int(h * scale) };
			//SDL_RenderCopy(renderer, buttonTextTexture, nullptr, &dst);


			Reset_Pressed_Letters();
			return false;
			//Word invalid and such suffix(word) will never be found
		}
		if (Current_Letter_Node->children[index]->children == NULL)
			return 0;
		Current_Letter_Node = Current_Letter_Node->children[index];

		cout << "Searching: " << char(index + 'a') << endl;
		if (Current_Letter_Node->is_end_of_word == 1) {
			cout << "GOT END OF STRING :" << Current_Letter_Node->is_end_of_word << endl;
			cout << "return 1\n";
			Word_Made = 1;	return 1;
		}
		cout << "default Returing false\n";
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
							cout << "are NOT adjacent trigered\n";
							Alphabets[i].set_Button_Pushed(!Alphabets[i].get_Button_Pushed());
							break;
						}
					if (!(Current_Word == ""))
						thickLineRGBA(renderer, Last_Pressed_Button.get_Position().X + 50, Last_Pressed_Button.get_Position().Y + 50, Alphabets[i].get_Position().X + 50, Alphabets[i].get_Position().Y + 50, 15, 204, 0, 204, 255);
					SDL_RenderPresent(renderer);
					Current_Word += Alphabets[i].get_char_of_button();
					Last_Pressed_Button = Alphabets[i];
					if (WordMade(Alphabets[i].get_char_of_button())) {
						Word_Made = 1;		cout << "Setting to 1\n";
					}
					else
						Word_Made = 0;
				}
				else {	//if button released
					bool exits_in_string = 0;
					/*for (int i = 0; i < Current_Word.size() - 1; i++)
						if (Alphabets[i].get_char_of_button())
						exits_in_string = 1;*/
					if (Alphabets[i].get_char_of_button() != Current_Word[Current_Word.size() - 1]) //if released button is in middle of already pressed buttons
					{
						cout << "Lie in string and atempts to break\n";
						Reset_Pressed_Letters();
						break;
					}
					else
						cout << "does Not lie in string\n";

					Current_Letter_Node = Word_Dictionary.get_Parent(Current_Letter_Node, Current_Word);
					Current_Word.erase(Current_Word.size() - 1, 1);
					int index = tolower(Current_Word.front()) - 'a';
					
					//if (Current_Letter_Node->children[index]->is_end_of_word) {
					if (Current_Letter_Node->is_end_of_word) {
						Word_Made = 1;		cout << "Setting to 1\n";
					}
					else
						Word_Made = 0;


					if (Current_Word == "")
						Word_Made = 0;
				}
				if (Current_Word == "")
					cout << "Current Word  <empty>" << Current_Word << endl;
				else
					cout << "Current Word " << Current_Word << endl;
				return 1;
				//cout << "Current Word " << Current_Word << endl;
			}
		return 0;
	}
	void Show_Registered_Words() {
		//roundedBoxRGBA(renderer, Main_Button.x, Main_Button.y, Main_Button.x + Main_Button.w, Main_Button.y + Main_Button.h, 20, Button_Color.r, Button_Color.g, Button_Color.b, 255);
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
	Read_fr_File_and_store_in_Trie_Tree(Word_Dictionary);
	Text_Button Rotate_Button;
	Rotate_Button.Set_Button("Rotate", { 500,25 }, { 99, 60 }, 40, { 48, 68, 193, 255 });
	Word_Dictionary.Write_SORTED_To_File_fr_Trie_Tree();
	//cout << "All words in Trie Tree: " << endl;
	//Word_Dictionary.Display();
	//cout << "Number_of_W_Read" << Number_of_W_Read << "\t\tNumber_of_W_in_Tree:" << Word_Dictionary.get_Number_of_Words_in_Tree() << endl;
	//Word_Dictionary.get_Parent(Word_Dictionary.get_Tree_Root().)
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
				//Rotate_Button.Check_for_Hovering(MouseX, MouseY);
				Rotate_Button.set_Button_Hovered(Rotate_Button.Check_if_Mouse_in_Button_Area(MouseX, MouseY));

			}
			if (event.type == SDL_MOUSEBUTTONUP)	//mouse click on Button
			{
				SDL_GetMouseState(&MouseX, &MouseY);
				if (Boggle_Game.Check_for_Letters_input(MouseX, MouseY)) {
					//cout << "breaking";
					break;
				}
				if (Rotate_Button.Check_if_Mouse_in_Button_Area(MouseX, MouseY)) {
					//cout << "RELEASED\n";
					Boggle_Game.Board_rotate();
					Rotate_Button.set_Button_Pushed(0);
				}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {	//mouse click on Button
				SDL_GetMouseState(&MouseX, &MouseY);
				if (Rotate_Button.Check_if_Mouse_in_Button_Area(MouseX, MouseY)) {
					Rotate_Button.set_Button_Pushed(!Rotate_Button.get_Button_Pushed());
					Rotate_Button.Display_Button();
					break;
				}
			}
		}
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		Boggle_Game.Display_Board();
		Rotate_Button.Display_Button();
		SDL_RenderPresent(renderer);	//Final Output to SDL window
	}
}