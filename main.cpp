#include <algorithm>
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>
#include <random>  // For random number generation
#include <SFML/Graphics.hpp>

using namespace std::chrono;
using namespace  std;

class Game {
private:
	int rank; //You'll have to convert from string to int after reading since all file stuff is strings
	string name;
	string platform;
	string year; //I don't think year actually need to be an int because its not being compared
	string genre;
	string publisher;
	float northAmericaSales;
	float europeSales;
	float japanSales;
	float otherSales;
	float globalSales;
public:
	Game() {
		rank = 0;
		name = "";
		platform = "";
		year = "";
		genre = "";
		publisher = "";
		northAmericaSales = 0;
		europeSales = 0;
		japanSales = 0;
		otherSales = 0;
		globalSales = 0;
	}

	Game(int rank, string name, string platform, string year, string genre, string publisher,
		float northAmericaSales, float europeSales, float japanSales, float otherSales, float globalSales) {
		this -> rank = rank;
		this -> name = name;
		this -> platform = platform;
		this -> year = year;
		this -> genre = genre;
		this -> publisher = publisher;
		this -> northAmericaSales = northAmericaSales;
		this -> europeSales = europeSales;
		this -> japanSales = japanSales;
		this -> otherSales = otherSales;
		this -> globalSales = globalSales;

	}
	int getRank() {
		return rank;
	}
	string getName() {
		return name;
	}
	string getPlatform() {
		return platform;
	}
	string getYear() {
		return year;
	}
	string getGenre() {
		return genre;
	}
	string getPublisher() {
		return publisher;
	}
	float getNA() {
		return northAmericaSales;
	}
	float getEU() {
		return europeSales;
	}
	float getJP() {
		return japanSales;
	}
	float getOther() {
		return otherSales;
	}
	float getGlobal() {
		return globalSales;
	}

	float getSales(const string& sortType) const {
		if (sortType == "NA") {
			return northAmericaSales;
		} else if (sortType == "EU") {
			return europeSales;
		} else if (sortType == "JP") {
			return japanSales;
		} else if (sortType == "Other") {
			return otherSales;
		} else {
			return globalSales; // Default case for "Global"
		}
	}

};

void merge(vector<Game> &games, string sortType, int left, int middle, int right) {
	int subArrayOne = middle - left + 1;
	int subArrayTwo = right - middle;

	// Create temp vector
	vector<Game> leftVector(subArrayOne);
	vector<Game> rightVector(subArrayTwo);

	// Copy data to temp vector
	for (int i = 0; i < subArrayOne; i++)
		leftVector[i] = games[left + i];
	for (int j = 0; j < subArrayTwo; j++)
		rightVector[j] = games[middle + 1 + j];

	int indexSub1 = 0;
	int indexSub2 = 0;
	int indexMerged = left;

	// Merge the temp arrays back into
	while (indexSub1 < subArrayOne && indexSub2 < subArrayTwo) {
		bool condition;
		if (sortType == "NA") {
			condition = leftVector[indexSub1].getNA() >= rightVector[indexSub2].getNA();
		} else if (sortType == "EU") {
			condition = leftVector[indexSub1].getEU() >= rightVector[indexSub2].getEU();
		} else if (sortType == "JP") {
			condition = leftVector[indexSub1].getJP() >= rightVector[indexSub2].getJP();
		} else if (sortType == "Other") {
			condition = leftVector[indexSub1].getOther() >= rightVector[indexSub2].getOther();
		} else {
			condition = leftVector[indexSub1].getGlobal() >= rightVector[indexSub2].getGlobal();
		}

		if (condition) {
			games[indexMerged] = leftVector[indexSub1];
			indexSub1++;
		} else {
			games[indexMerged] = rightVector[indexSub2];
			indexSub2++;
		}
		indexMerged++;
	}

	// Copy the remaining elements
	while (indexSub1 < subArrayOne) {
		games[indexMerged] = leftVector[indexSub1];
		indexSub1++;
		indexMerged++;
	}
	while (indexSub2 < subArrayTwo) {
		games[indexMerged] = rightVector[indexSub2];
		indexSub2++;
		indexMerged++;
	}
}

void mergeSort (vector<Game> &games, string sortType, int start, int end) {
	if (start >= end) {
		return;
	}
	int middle = start + (end-start)/2;
	mergeSort(games, sortType, start, middle);
	mergeSort(games, sortType, middle + 1, end);
	merge(games, sortType, start, middle, end);


}

// Swap function
void swap(Game &a, Game &b) {
	Game temp = a;
	a = b;
	b = temp;
}

// Partition function for Quicksort
int partition(vector<Game> &games, int low, int high, const string& sortType) {
	float pivotValue = games[high].getSales(sortType); // Choosing the last element's sales as pivot
	int i = low - 1; // Index of smaller element

	for (int j = low; j < high; j++) {
		if (games[j].getSales(sortType) >= pivotValue) {
			i++;
			swap(games[i], games[j]);
		}
	}
	swap(games[i + 1], games[high]);
	return i + 1;
}

// Quicksort function
void quicksort(vector<Game> &games, int low, int high, const string& sortType) {
	if (low < high) {
		int pi = partition(games, low, high, sortType); // Partitioning index

		// Recursively sort elements before and after partition
		quicksort(games, low, pi - 1, sortType);
		quicksort(games, pi + 1, high, sortType);
	}
}

// function to help parse the data
// referenced: https://www.geeksforgeeks.org/working-csv-files-python/
vector<Game> parseCSV(const string& filePath) {
    vector<Game> objects;
    ifstream file(filePath);
    string line;

    if (!file.is_open()) {
        cout << "error" << endl;
    }

    getline(file, line);

    while(getline(file, line)) {
        istringstream ss(line);
        string Srank, name, platform, year, genre, publisher;
        string SnaSale, SeuSale, SjpSale, SotherSale, SglobalSale; //string : int for stoi later sorry for weird name convention
        // Rank,Name,Platform,Year,Genre,Publisher,NA_Sales,EU_Sales,JP_Sales,Other_Sales,Global_Sales (just for my reference)
        // using getline to take in each line, then check for comma before going to next variable
        getline(ss, Srank,',');
        getline(ss, name, ',');
        getline(ss, platform, ',');
        getline(ss, year, ',');
        getline(ss, genre, ',');
        getline(ss, publisher, ',');
        getline(ss, SnaSale, ',');
        getline(ss, SeuSale, ',');
        getline(ss, SjpSale, ',');
        getline(ss, SotherSale, ',');
        getline(ss, SglobalSale, ',');

        // convert from string to int for the variables that need it
        try {
            int rank = stoi(Srank);
            float naSale = stof(SnaSale);
            float euSale = stof(SeuSale);
            float jpSale = stof(SjpSale);
            float otherSale = stof(SotherSale);
            float globalSale = stof(SglobalSale);

            // make the object and then push back into object vector
            Game games(rank, name, platform, year, genre, publisher, naSale, euSale, jpSale, otherSale, globalSale);
            objects.push_back(games);
        } catch (invalid_argument& e) {
            continue; // there is a conversion error but i am skipping it here, im not 100
        }
    }
    return objects;
}

// BogoSort function
// BogoSort function
bool isSorted(const vector<Game>& games, const string& sortType) {
	for (size_t i = 1; i < games.size(); ++i) {
		if (games[i].getSales(sortType) > games[i - 1].getSales(sortType)) {
			return false;
		}
	}
	return true;
}

void bogoSort(vector<Game>& games, const string& sortType) {
	std::default_random_engine rng(static_cast<unsigned int>(std::time(nullptr)));
	std::uniform_int_distribution<size_t> dist(0, games.size() - 1);
	int swapCount = 0;  // Counter for swaps

	while (!isSorted(games, sortType)) {
		std::shuffle(games.begin(), games.end(), rng);
		swapCount++;  // Increment swap count for each shuffle
		cout << "Number of shuffles (swaps): " << swapCount << endl;
	}

	cout << "Number of shuffles (swaps): " << swapCount << endl;
}

void textConfig (sf::Text &text, sf::Font &font, int size, float x, float y) {
	text.setFont(font);
	text.setCharacterSize(size);
	text.setPosition(x, y);
}

void setText (sf::Text &text) {
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
}

int main() {
	string filePath = "../vgsales.csv"; // not working yet idk whats wrong
	vector<Game> gamesVector = parseCSV(filePath); //Games get added here after data is parsed
	vector<Game> userVector; //This is the vector that will be sorted and returned
	string sortType; //This should be either NA, EU, JP, or Other
	string userPlatform;
	string userGenre;
	string userYear;



    cout << "Welcome to Team BOGO'S: Games, UNLIMTIED Games" << endl;
	//all alyssa stuff
    // this prints out all of the data
    // for (int i = 0; i < 50; i++) { // limit to 50 cuz so much data
    //
    //     // checking to see if everything is correctly a string and integer
    //     cout << "Rank (" << typeid(gamesVector[i].getRank()).name() << "): " << gamesVector[i].getRank() << endl
    //          << "Name (" << typeid(gamesVector[i].getName()).name() << "): " << gamesVector[i].getName() << endl
    //          << "Platform (" << typeid(gamesVector[i].getPlatform()).name() << "): " << gamesVector[i].getPlatform()
    //          << endl
    //          << "Year (" << typeid(gamesVector[i].getYear()).name() << "): " << gamesVector[i].getYear() << endl
    //          << "Genre (" << typeid(gamesVector[i].getGenre()).name() << "): " << gamesVector[i].getGenre() << endl
    //          << "Publisher (" << typeid(gamesVector[i].getPublisher()).name() << "): " << gamesVector[i].getPublisher()
    //          << endl
    //          << "NA Sales (" << typeid(gamesVector[i].getNA()).name() << "): " << gamesVector[i].getNA() << endl
    //          << "EU Sales (" << typeid(gamesVector[i].getEU()).name() << "): " << gamesVector[i].getEU() << endl
    //          << "JP Sales (" << typeid(gamesVector[i].getJP()).name() << "): " << gamesVector[i].getJP() << endl
    //          << "Other Sales (" << typeid(gamesVector[i].getOther()).name() << "): " << gamesVector[i].getOther()
    //          << endl
    //          << "Global Sales (" << typeid(gamesVector[i].getGlobal()).name() << "): " << gamesVector[i].getGlobal()
    //          << endl
    //          << "-----------------------------" << endl;
    // }


	//all jacob stuff
	// open SFML window #1
	sf::RenderWindow window(sf::VideoMode(800, 600), "Games Unlimited Games", sf::Style::Close);
	sf::RectangleShape background1(sf::Vector2f(800, 600));
	background1.setFillColor(sf::Color::Blue);

	sf::Font font;
	font.loadFromFile("../assets/font.ttf");
	sf::Text welcome1;
	sf::Text welcome2;
	sf::Text conti;
	textConfig(welcome1, font, 18, 400, 185);
	textConfig(welcome2, font, 18, 400, 215);
	textConfig(conti, font, 18, 400, 475);
	welcome1.setString("Welcome to Team BOGO's game sorting program.");
	welcome2.setString("Click continue to begin.");
	conti.setString("continue");
	setText(welcome1);
	setText(welcome2);
	setText(conti);
	conti.setColor(sf::Color::Black);

	sf::Sprite cont;
	sf::Texture unpressed1;
	unpressed1.loadFromFile("../assets/pngs/button1.png");
	sf::Texture pressed1;
	pressed1.loadFromFile("../assets/pngs/button2.png");
	cont.setPosition(250, 400);
	cont.setTexture(unpressed1);

	while(window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Button::Left) {
						if (event.mouseButton.x >= 250 && event.mouseButton.x <= 550) {
							if (event.mouseButton.y >= 400 && event.mouseButton.y <= 550) {
								if (cont.getTexture() == &unpressed1) {
									cont.setTexture(pressed1);
								}
							}
						}
					}
					break;
				case sf::Event::MouseButtonReleased:
					if (event.mouseButton.button == sf::Mouse::Button::Left)
						cont.setTexture(unpressed1);
					if (event.mouseButton.x >= 250 && event.mouseButton.x <= 550) {
						if (event.mouseButton.y >= 400 && event.mouseButton.y <= 550)
							window.close();
					}
					break;
			}
		}
		window.clear();
		window.draw(background1);
		window.draw(welcome1);
		window.draw(welcome2);
		window.draw(cont);
		window.draw(conti);
		window.display();
	}

	// region window
	sf::RenderWindow region_window(sf::VideoMode(800, 600), "Games Unlimited Games", sf::Style::Close);
	sf::RectangleShape background3(sf::Vector2f(800, 600));
	background3.setFillColor(sf::Color::Blue);
	vector<sf::Sprite> sprites1;
	vector<sf::Text> texts1;

	sf::Sprite button34;
	sf::Sprite button35;
	sf::Sprite button36;
	sf::Sprite button37;
	button34.setTexture(unpressed1);
	button35.setTexture(unpressed1);
	button36.setTexture(unpressed1);
	button37.setTexture(unpressed1);
	sprites1.push_back(button34);
	sprites1.push_back(button35);
	sprites1.push_back(button36);
	sprites1.push_back(button37);
	sf::Text text34;
	sf::Text text35;
	sf::Text text36;
	sf::Text text37;
	sf::Text text38;
	textConfig(text34, font, 24, 220, 195);
	textConfig(text35, font, 24, 220, 465);
	textConfig(text36, font, 24, 580, 195);
	textConfig(text37, font, 24, 580, 465);
	textConfig(text38, font, 18, 400, 60);
	text34.setString("North America");
	text35.setString("Europe");
	text36.setString("Japan");
	text37.setString("Other/All");
	text38.setString("Please select one of the following regions");
	text34.setColor(sf::Color::Black);
	text35.setColor(sf::Color::Black);
	text36.setColor(sf::Color::Black);
	text37.setColor(sf::Color::Black);
	setText(text34);
	setText(text35);
	setText(text36);
	setText(text37);
	setText(text38);
	texts1.push_back(text34);
	texts1.push_back(text35);
	texts1.push_back(text36);
	texts1.push_back(text37);
	texts1.push_back(text38);

	auto itr1 = sprites1.begin();

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			itr1->setPosition((360*i + 70), (270*j + 120));
			itr1++;
		}
	}

	while(region_window.isOpen()) {
		sf::Event event;

		while (region_window.pollEvent(event)) {
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				if (event.mouseButton.x >= 70 && event.mouseButton.x <= 370) {
					if (event.mouseButton.y >= 120 && event.mouseButton.y <= 270) {
						auto itr = sprites1.begin();
						itr->setTexture(pressed1);
					}
					if (event.mouseButton.y >= 390 && event.mouseButton.y <= 540) {
						auto itr = sprites1.begin()+1;
						itr->setTexture(pressed1);
					}
				}
				if (event.mouseButton.x >= 430 && event.mouseButton.x <= 730) {
					if (event.mouseButton.y >= 120 && event.mouseButton.y <= 270) {
						auto itr = sprites1.begin()+2;
						itr->setTexture(pressed1);
					}
					if (event.mouseButton.y >= 390 && event.mouseButton.y <= 540) {
						auto itr = sprites1.begin()+3;
						itr->setTexture(pressed1);
					}
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				if (event.mouseButton.x >= 70 && event.mouseButton.x <= 370) {
					if (event.mouseButton.y >= 120 && event.mouseButton.y <= 270) {
						sortType = "NA";
					}
					if (event.mouseButton.y >= 390 && event.mouseButton.y <= 540) {
						sortType = "EU";
					}
				}
				if (event.mouseButton.x >= 430 && event.mouseButton.x <= 730) {
					if (event.mouseButton.y >= 120 && event.mouseButton.y <= 270) {
						sortType = "JP";
					}
					if (event.mouseButton.y >= 390 && event.mouseButton.y <= 540) {
						sortType = "Other";
					}
				}
				if (!sortType.empty()) {
					region_window.close();
				}
				for (auto itr = sprites1.begin(); itr != sprites1.end(); itr++) {
					itr->setTexture(unpressed1);
				}
			}
		}

		region_window.clear();
		region_window.draw(background3);
		for (auto itr = sprites1.begin(); itr != sprites1.end(); itr++) {
			region_window.draw(*itr);
		}
		for (auto itr = texts1.begin(); itr != texts1.end(); itr++) {
			region_window.draw(*itr);
		}
		region_window.display();
	}

	// platform window
	sf::RenderWindow platform_window(sf::VideoMode(800, 600), "Games Unlimited Games", sf::Style::Close);
	sf::RectangleShape background2(sf::Vector2f(800, 600));
	background2.setFillColor(sf::Color::Blue);
	vector<sf::Sprite> sprites2;
	vector<sf::Text> texts2;

	sf::Texture unpressed2;
	unpressed2.loadFromFile("../assets/pngs/button3.png");
	sf::Texture pressed2;
	pressed2.loadFromFile("../assets/pngs/button4.png");

	sf::Sprite button1;
	sf::Sprite button2;
	sf::Sprite button3;
	sf::Sprite button4;
	sf::Sprite button5;
	sf::Sprite button6;
	sf::Sprite button7;
	sf::Sprite button8;
	sf::Sprite button9;
	sf::Sprite button10;
	sf::Sprite button11;
	sf::Sprite button12;
	sf::Sprite button13;
	sf::Sprite button14;
	sf::Sprite button15;
	sf::Sprite button16;
	sf::Sprite button17;
	sf::Sprite button18;
	sf::Sprite button19;
	sf::Sprite button20;
	sf::Sprite button21;
	sf::Sprite button22;
	sf::Sprite button23;
	sf::Sprite button24;
	sf::Sprite button25;
	sf::Sprite button26;
	sf::Sprite button27;
	sf::Sprite button28;
	sf::Sprite button29;
	sf::Sprite button30;
	sf::Sprite button31;
	sf::Sprite button32;
	button1.setTexture(unpressed2);
	button2.setTexture(unpressed2);
	button3.setTexture(unpressed2);
	button4.setTexture(unpressed2);
	button5.setTexture(unpressed2);
	button6.setTexture(unpressed2);
	button7.setTexture(unpressed2);
	button8.setTexture(unpressed2);
	button9.setTexture(unpressed2);
	button1.setTexture(unpressed2);
	button10.setTexture(unpressed2);
	button11.setTexture(unpressed2);
	button12.setTexture(unpressed2);
	button13.setTexture(unpressed2);
	button14.setTexture(unpressed2);
	button15.setTexture(unpressed2);
	button16.setTexture(unpressed2);
	button17.setTexture(unpressed2);
	button18.setTexture(unpressed2);
	button19.setTexture(unpressed2);
	button20.setTexture(unpressed2);
	button21.setTexture(unpressed2);
	button22.setTexture(unpressed2);
	button23.setTexture(unpressed2);
	button24.setTexture(unpressed2);
	button25.setTexture(unpressed2);
	button26.setTexture(unpressed2);
	button27.setTexture(unpressed2);
	button28.setTexture(unpressed2);
	button29.setTexture(unpressed2);
	button30.setTexture(unpressed2);
	button31.setTexture(unpressed2);
	button32.setTexture(unpressed2);
	sprites2.push_back(button1);
	sprites2.push_back(button2);
	sprites2.push_back(button3);
	sprites2.push_back(button4);
	sprites2.push_back(button5);
	sprites2.push_back(button6);
	sprites2.push_back(button7);
	sprites2.push_back(button8);
	sprites2.push_back(button9);
	sprites2.push_back(button10);
	sprites2.push_back(button11);
	sprites2.push_back(button12);
	sprites2.push_back(button13);
	sprites2.push_back(button14);
	sprites2.push_back(button15);
	sprites2.push_back(button16);
	sprites2.push_back(button17);
	sprites2.push_back(button18);
	sprites2.push_back(button19);
	sprites2.push_back(button20);
	sprites2.push_back(button21);
	sprites2.push_back(button22);
	sprites2.push_back(button23);
	sprites2.push_back(button24);
	sprites2.push_back(button25);
	sprites2.push_back(button26);
	sprites2.push_back(button27);
	sprites2.push_back(button28);
	sprites2.push_back(button29);
	sprites2.push_back(button30);
	sprites2.push_back(button31);
	sprites2.push_back(button32);

	sf::Text text1;
	sf::Text text2;
	sf::Text text3;
	sf::Text text4;
	sf::Text text5;
	sf::Text text6;
	sf::Text text7a;
	sf::Text text7b;
	sf::Text text8;
	sf::Text text9a;
	sf::Text text9b;
	sf::Text text10;
	sf::Text text11;
	sf::Text text12;
	sf::Text text13;
	sf::Text text14;
	sf::Text text15;
	sf::Text text16;
	sf::Text text17;
	sf::Text text18;
	sf::Text text19;
	sf::Text text20a;
	sf::Text text20b;
	sf::Text text21a;
	sf::Text text21b;
	sf::Text text22;
	sf::Text text23;
	sf::Text text24;
	sf::Text text25;
	sf::Text text26;
	sf::Text text27;
	sf::Text text28;
	sf::Text text29;
	sf::Text text30;
	sf::Text text31;
	sf::Text text32;
	sf::Text text33;
	textConfig(text1, font, 12, 100, 145);
	textConfig(text2, font, 12, 100, 205);
	textConfig(text3, font, 12, 100, 265);
	textConfig(text4, font, 12, 100, 325);
	textConfig(text5, font, 12, 100, 385);
	textConfig(text6, font, 12, 100, 445);
	textConfig(text7a, font, 12, 100, 500);
	textConfig(text7b, font, 12, 100, 510);
	textConfig(text8, font, 12, 100, 565);
	textConfig(text9a, font, 12, 300, 140);
	textConfig(text9b, font, 12, 300, 150);
	textConfig(text10, font, 12, 300, 205);
	textConfig(text11, font, 12, 300, 265);
	textConfig(text12, font, 12, 300, 325);
	textConfig(text13, font, 12, 300, 385);
	textConfig(text14, font, 12, 300, 445);
	textConfig(text15, font, 12, 300, 505);
	textConfig(text16, font, 12, 300, 565);
	textConfig(text17, font, 12, 500, 145);
	textConfig(text18, font, 12, 500, 205);
	textConfig(text19, font, 12, 500, 265);
	textConfig(text20a, font, 12, 500, 320);
	textConfig(text20b, font, 12, 500, 330);
	textConfig(text21a, font, 12, 500, 380);
	textConfig(text21b, font, 12, 500, 390);
	textConfig(text22, font, 12, 500, 445);
	textConfig(text23, font, 12, 500, 505);
	textConfig(text24, font, 12, 500, 565);
	textConfig(text25, font, 12, 700, 145);
	textConfig(text26, font, 12, 700, 205);
	textConfig(text27, font, 12, 700, 265);
	textConfig(text28, font, 12, 700, 325);
	textConfig(text29, font, 12, 700, 385);
	textConfig(text30, font, 12, 700, 445);
	textConfig(text31, font, 12, 700, 505);
	textConfig(text32, font, 12, 700, 565);
	textConfig(text33, font, 18, 400, 60);
	text1.setString("Atari 2600");
	text2.setString("3DO");
	text3.setString("3DS");
	text4.setString("DreamCast");
	text5.setString("DS");
	text6.setString("GameBoy");
	text7a.setString("GameBoy");
	text7b.setString("Advanced");
	text8.setString("GameCube");
	text9a.setString("Sega");
	text9b.setString("Genesis");
	text10.setString("GameGear");
	text11.setString("N64");
	text12.setString("NES");
	text13.setString("NeoGeo");
	text14.setString("PC");
	text15.setString("PCFX");
	text16.setString("PlayStation");
	text17.setString("PlayStation2");
	text18.setString("PlayStation3");
	text19.setString("PlayStation4");
	text20a.setString("PlayStation");
	text20b.setString("Portable");
	text21a.setString("PlayStation");
	text21b.setString("Vita");
	text22.setString("SegaSaturn");
	text23.setString("SegaCD");
	text24.setString("SNES");
	text25.setString("TurboGrafx16");
	text26.setString("WonderSwan");
	text27.setString("Wii");
	text28.setString("Wii U");
	text29.setString("XBox 360");
	text30.setString("XBox");
	text31.setString("XBox One");
	text32.setString("None/All");
	text33.setString("Please select one of the following platforms");
	text1.setColor(sf::Color::Black);
	text2.setColor(sf::Color::Black);
	text3.setColor(sf::Color::Black);
	text4.setColor(sf::Color::Black);
	text5.setColor(sf::Color::Black);
	text6.setColor(sf::Color::Black);
	text7a.setColor(sf::Color::Black);
	text7b.setColor(sf::Color::Black);
	text8.setColor(sf::Color::Black);
	text9a.setColor(sf::Color::Black);
	text9b.setColor(sf::Color::Black);
	text10.setColor(sf::Color::Black);
	text11.setColor(sf::Color::Black);
	text12.setColor(sf::Color::Black);
	text13.setColor(sf::Color::Black);
	text14.setColor(sf::Color::Black);
	text15.setColor(sf::Color::Black);
	text16.setColor(sf::Color::Black);
	text17.setColor(sf::Color::Black);
	text18.setColor(sf::Color::Black);
	text19.setColor(sf::Color::Black);
	text20a.setColor(sf::Color::Black);
	text20b.setColor(sf::Color::Black);
	text21a.setColor(sf::Color::Black);
	text21b.setColor(sf::Color::Black);
	text22.setColor(sf::Color::Black);
	text23.setColor(sf::Color::Black);
	text24.setColor(sf::Color::Black);
	text25.setColor(sf::Color::Black);
	text26.setColor(sf::Color::Black);
	text27.setColor(sf::Color::Black);
	text28.setColor(sf::Color::Black);
	text29.setColor(sf::Color::Black);
	text30.setColor(sf::Color::Black);
	text31.setColor(sf::Color::Black);
	text32.setColor(sf::Color::Black);
	setText(text1);
	setText(text2);
	setText(text3);
	setText(text4);
	setText(text5);
	setText(text6);
	setText(text7a);
	setText(text7b);
	setText(text8);
	setText(text9a);
	setText(text9b);
	setText(text10);
	setText(text11);
	setText(text12);
	setText(text13);
	setText(text14);
	setText(text15);
	setText(text16);
	setText(text17);
	setText(text18);
	setText(text19);
	setText(text20a);
	setText(text20b);
	setText(text21a);
	setText(text21b);
	setText(text22);
	setText(text23);
	setText(text24);
	setText(text25);
	setText(text26);
	setText(text27);
	setText(text28);
	setText(text29);
	setText(text30);
	setText(text31);
	setText(text32);
	setText(text33);
	texts2.push_back(text1);
	texts2.push_back(text2);
	texts2.push_back(text3);
	texts2.push_back(text4);
	texts2.push_back(text5);
	texts2.push_back(text6);
	texts2.push_back(text7a);
	texts2.push_back(text7b);
	texts2.push_back(text8);
	texts2.push_back(text9a);
	texts2.push_back(text9b);
	texts2.push_back(text10);
	texts2.push_back(text11);
	texts2.push_back(text12);
	texts2.push_back(text13);
	texts2.push_back(text14);
	texts2.push_back(text15);
	texts2.push_back(text16);
	texts2.push_back(text17);
	texts2.push_back(text18);
	texts2.push_back(text19);
	texts2.push_back(text20a);
	texts2.push_back(text20b);
	texts2.push_back(text21a);
	texts2.push_back(text21b);
	texts2.push_back(text22);
	texts2.push_back(text23);
	texts2.push_back(text24);
	texts2.push_back(text25);
	texts2.push_back(text26);
	texts2.push_back(text27);
	texts2.push_back(text28);
	texts2.push_back(text29);
	texts2.push_back(text30);
	texts2.push_back(text31);
	texts2.push_back(text32);
	texts2.push_back(text33);

	auto itr2 = sprites2.begin();

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			itr2->setPosition((200*i + 50), (60*j + 120));
			itr2++;
		}
	}

	userPlatform = "";

	while(platform_window.isOpen()) {
		sf::Event event;

		while (platform_window.pollEvent(event)) {
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				if (event.mouseButton.x >= 50 && event.mouseButton.x <= 150) {
					if (event.mouseButton.y >= 120 && event.mouseButton.y <= 170) {
						auto itr = sprites2.begin();
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 180 && event.mouseButton.y <= 230) {
						auto itr = sprites2.begin()+1;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 240 && event.mouseButton.y <= 290) {
						auto itr = sprites2.begin() + 2;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 300 && event.mouseButton.y <= 350) {
						auto itr = sprites2.begin() + 3;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 360 && event.mouseButton.y <= 410) {
						auto itr = sprites2.begin() + 4;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 420 && event.mouseButton.y <= 470) {
						auto itr = sprites2.begin() + 5;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 480 && event.mouseButton.y <= 530) {
						auto itr = sprites2.begin() + 6;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 540 && event.mouseButton.y <= 590) {
						auto itr = sprites2.begin() + 7;
						itr->setTexture(pressed2);
					}
				}
				if (event.mouseButton.x >= 250 && event.mouseButton.x <= 350) {
					if (event.mouseButton.y >= 120 && event.mouseButton.y <= 170) {
						auto itr = sprites2.begin() + 8;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 180 && event.mouseButton.y <= 230) {
						auto itr = sprites2.begin() + 9;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 240 && event.mouseButton.y <= 290) {
						auto itr = sprites2.begin() + 10;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 300 && event.mouseButton.y <= 350) {
						auto itr = sprites2.begin() + 11;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 360 && event.mouseButton.y <= 410) {
						auto itr = sprites2.begin() + 12;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 420 && event.mouseButton.y <= 470) {
						auto itr = sprites2.begin() + 13;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 480 && event.mouseButton.y <= 530) {
						auto itr = sprites2.begin() + 14;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 540 && event.mouseButton.y <= 590) {
						auto itr = sprites2.begin() + 15;
						itr->setTexture(pressed2);
					}
				}
				if (event.mouseButton.x >= 450 && event.mouseButton.x <= 550) {
					if (event.mouseButton.y >= 120 && event.mouseButton.y <= 170) {
						auto itr = sprites2.begin() + 16;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 180 && event.mouseButton.y <= 230) {
						auto itr = sprites2.begin() + 17;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 240 && event.mouseButton.y <= 290) {
						auto itr = sprites2.begin() + 18;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 300 && event.mouseButton.y <= 350) {
						auto itr = sprites2.begin() + 19;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 360 && event.mouseButton.y <= 410) {
						auto itr = sprites2.begin() + 20;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 420 && event.mouseButton.y <= 470) {
						auto itr = sprites2.begin() + 21;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 480 && event.mouseButton.y <= 530) {
						auto itr = sprites2.begin() + 22;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 540 && event.mouseButton.y <= 590) {
						auto itr = sprites2.begin() + 23;
						itr->setTexture(pressed2);
					}
				}
				if (event.mouseButton.x >= 650 && event.mouseButton.x <= 750) {
					if (event.mouseButton.y >= 120 && event.mouseButton.y <= 170) {
						auto itr = sprites2.begin() + 24;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 180 && event.mouseButton.y <= 230) {
						auto itr = sprites2.begin() + 25;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 240 && event.mouseButton.y <= 290) {
						auto itr = sprites2.begin() + 26;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 300 && event.mouseButton.y <= 350) {
						auto itr = sprites2.begin() + 27;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 360 && event.mouseButton.y <= 410) {
						auto itr = sprites2.begin() + 28;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 420 && event.mouseButton.y <= 470) {
						auto itr = sprites2.begin() + 29;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 480 && event.mouseButton.y <= 530) {
						auto itr = sprites2.begin() + 30;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 540 && event.mouseButton.y <= 590) {
						auto itr = sprites2.begin() + 31;
						itr->setTexture(pressed2);
					}
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				for (auto itr = sprites2.begin(); itr != sprites2.end(); itr++) {
					itr->setTexture(unpressed2);
				}
				if (event.mouseButton.x >= 50 && event.mouseButton.x <= 150) {
					if (event.mouseButton.y >= 120 && event.mouseButton.y <= 170)
						userPlatform = "2600";
					if (event.mouseButton.y >= 180 && event.mouseButton.y <= 230)
						userPlatform = "3DO";
					if (event.mouseButton.y >= 240 && event.mouseButton.y <= 290)
						userPlatform = "3DS";
					if (event.mouseButton.y >= 300 && event.mouseButton.y <= 350)
						userPlatform = "DC";
					if (event.mouseButton.y >= 360 && event.mouseButton.y <= 410)
						userPlatform = "DS";
					if (event.mouseButton.y >= 420 && event.mouseButton.y <= 470)
						userPlatform = "GB";
					if (event.mouseButton.y >= 480 && event.mouseButton.y <= 530)
						userPlatform = "GBA";
					if (event.mouseButton.y >= 540 && event.mouseButton.y <= 590)
						userPlatform = "GC";
				}
				if (event.mouseButton.x >= 250 && event.mouseButton.x <= 350) {
					if (event.mouseButton.y >= 120 && event.mouseButton.y <= 170)
						userPlatform = "GEN";
					if (event.mouseButton.y >= 180 && event.mouseButton.y <= 230)
						userPlatform = "GG";
					if (event.mouseButton.y >= 240 && event.mouseButton.y <= 290)
						userPlatform = "N64";
					if (event.mouseButton.y >= 300 && event.mouseButton.y <= 350)
						userPlatform = "NES";
					if (event.mouseButton.y >= 360 && event.mouseButton.y <= 410)
						userPlatform = "NG";
					if (event.mouseButton.y >= 420 && event.mouseButton.y <= 470)
						userPlatform = "PC";
					if (event.mouseButton.y >= 480 && event.mouseButton.y <= 530)
						userPlatform = "PCFX";
					if (event.mouseButton.y >= 540 && event.mouseButton.y <= 590)
						userPlatform = "PS";
				}
				if (event.mouseButton.x >= 450 && event.mouseButton.x <= 550) {
					if (event.mouseButton.y >= 120 && event.mouseButton.y <= 170)
						userPlatform = "PS2";
					if (event.mouseButton.y >= 180 && event.mouseButton.y <= 230)
						userPlatform = "PS3";
					if (event.mouseButton.y >= 240 && event.mouseButton.y <= 290)
						userPlatform = "PS4";
					if (event.mouseButton.y >= 300 && event.mouseButton.y <= 350)
						userPlatform = "PSP";
					if (event.mouseButton.y >= 360 && event.mouseButton.y <= 410)
						userPlatform = "PSV";
					if (event.mouseButton.y >= 420 && event.mouseButton.y <= 470)
						userPlatform = "SAT";
					if (event.mouseButton.y >= 480 && event.mouseButton.y <= 530)
						userPlatform = "SCD";
					if (event.mouseButton.y >= 540 && event.mouseButton.y <= 590)
						userPlatform = "SNES";
				}
				if (event.mouseButton.x >= 650 && event.mouseButton.x <= 750) {
					if (event.mouseButton.y >= 120 && event.mouseButton.y <= 170)
						userPlatform = "TG16";
					if (event.mouseButton.y >= 180 && event.mouseButton.y <= 230)
						userPlatform = "WS";
					if (event.mouseButton.y >= 240 && event.mouseButton.y <= 290)
						userPlatform = "Wii";
					if (event.mouseButton.y >= 300 && event.mouseButton.y <= 350)
						userPlatform = "WiiU";
					if (event.mouseButton.y >= 360 && event.mouseButton.y <= 410)
						userPlatform = "X360";
					if (event.mouseButton.y >= 420 && event.mouseButton.y <= 470)
						userPlatform = "XB";
					if (event.mouseButton.y >= 480 && event.mouseButton.y <= 530)
						userPlatform = "XOne";
					if (event.mouseButton.y >= 540 && event.mouseButton.y <= 590) {
						userPlatform.clear();
						if (userPlatform.empty())
							platform_window.close();
					}
				}
				if (!userPlatform.empty()) {
					platform_window.close();
				}
			}
		}

		platform_window.clear();
		platform_window.draw(background2);
		for (auto itr = sprites2.begin(); itr != sprites2.end(); itr++) {
			platform_window.draw(*itr);
		}
		for (auto itr = texts2.begin(); itr != texts2.end(); itr++) {
			platform_window.draw(*itr);
		}
		platform_window.display();
	}

	// genre window
	sf::RenderWindow genre_window(sf::VideoMode(800, 600), "Games Unlimited Games", sf::Style::Close);
	sf::RectangleShape background4(sf::Vector2f(800, 600));
	background3.setFillColor(sf::Color::Blue);
	vector<sf::Sprite> sprites3;
	vector<sf::Text> texts3;

	sf::Sprite button38;
	sf::Sprite button39;
	sf::Sprite button40;
	sf::Sprite button41;
	sf::Sprite button42;
	sf::Sprite button43;
	sf::Sprite button44;
	sf::Sprite button45;
	sf::Sprite button46;
	sf::Sprite button47;
	sf::Sprite button48;
	sf::Sprite button49;
	sf::Sprite button50;
	button38.setTexture(unpressed2);
	button39.setTexture(unpressed2);
	button40.setTexture(unpressed2);
	button41.setTexture(unpressed2);
	button42.setTexture(unpressed2);
	button43.setTexture(unpressed2);
	button44.setTexture(unpressed2);
	button45.setTexture(unpressed2);
	button46.setTexture(unpressed2);
	button47.setTexture(unpressed2);
	button48.setTexture(unpressed2);
	button49.setTexture(unpressed2);
	button50.setTexture(unpressed2);
	sprites3.push_back(button38);
	sprites3.push_back(button39);
	sprites3.push_back(button40);
	sprites3.push_back(button41);
	sprites3.push_back(button42);
	sprites3.push_back(button43);
	sprites3.push_back(button44);
	sprites3.push_back(button45);
	sprites3.push_back(button46);
	sprites3.push_back(button47);
	sprites3.push_back(button48);
	sprites3.push_back(button49);
	sprites3.push_back(button50);
	sf::Text text39;
	sf::Text text40;
	sf::Text text41;
	sf::Text text42;
	sf::Text text43;
	sf::Text text44;
	sf::Text text45;
	sf::Text text46;
	sf::Text text47;
	sf::Text text48;
	sf::Text text49;
	sf::Text text50;
	sf::Text text51;
	sf::Text text52;
	textConfig(text39, font, 12, 150, 145);
	textConfig(text40, font, 12, 150, 240);
	textConfig(text41, font, 12, 150, 335);
	textConfig(text42, font, 12, 150, 430);
	textConfig(text43, font, 12, 400, 145);
	textConfig(text44, font, 12, 400, 240);
	textConfig(text45, font, 12, 400, 335);
	textConfig(text46, font, 12, 400, 430);
	textConfig(text47, font, 12, 400, 525);
	textConfig(text48, font, 12, 650, 145);
	textConfig(text49, font, 12, 650, 240);
	textConfig(text50, font, 12, 650, 335);
	textConfig(text51, font, 12, 650, 430);
	textConfig(text52, font, 18, 400, 60);
	text39.setString("Action");
	text40.setString("Adventure");
	text41.setString("Fighting");
	text42.setString("Misc");
	text43.setString("Platform");
	text44.setString("Puzzle");
	text45.setString("Racing");
	text46.setString("Role-Playing");
	text47.setString("None/All");
	text48.setString("Simulation");
	text49.setString("Sports");
	text50.setString("Strategy");
	text51.setString("Shooter");
	text52.setString("Please select one of the following genres");
	text39.setColor(sf::Color::Black);
	text40.setColor(sf::Color::Black);
	text41.setColor(sf::Color::Black);
	text42.setColor(sf::Color::Black);
	text43.setColor(sf::Color::Black);
	text44.setColor(sf::Color::Black);
	text45.setColor(sf::Color::Black);
	text46.setColor(sf::Color::Black);
	text47.setColor(sf::Color::Black);
	text48.setColor(sf::Color::Black);
	text49.setColor(sf::Color::Black);
	text50.setColor(sf::Color::Black);
	text51.setColor(sf::Color::Black);
	setText(text39);
	setText(text40);
	setText(text41);
	setText(text42);
	setText(text43);
	setText(text44);
	setText(text45);
	setText(text46);
	setText(text47);
	setText(text48);
	setText(text49);
	setText(text50);
	setText(text51);
	setText(text52);
	texts3.push_back(text39);
	texts3.push_back(text40);
	texts3.push_back(text41);
	texts3.push_back(text42);
	texts3.push_back(text43);
	texts3.push_back(text44);
	texts3.push_back(text45);
	texts3.push_back(text46);
	texts3.push_back(text47);
	texts3.push_back(text48);
	texts3.push_back(text49);
	texts3.push_back(text50);
	texts3.push_back(text51);
	texts3.push_back(text52);

	auto itr3 = sprites3.begin();

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			itr3->setPosition((250*i + 100), (95*j + 120));
			itr3++;
		}
	}

	itr3++->setPosition(350, 500);

	while(genre_window.isOpen()) {
		sf::Event event;

		while (genre_window.pollEvent(event)) {
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				if (event.mouseButton.x >= 100 && event.mouseButton.x <= 200) {
					if (event.mouseButton.y >= 120 && event.mouseButton.y <= 170) {
						auto itr = sprites3.begin();
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 215 && event.mouseButton.y <= 265) {
						auto itr = sprites3.begin()+1;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 310 && event.mouseButton.y <= 360) {
						auto itr = sprites3.begin()+2;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 405 && event.mouseButton.y <= 455) {
						auto itr = sprites3.begin()+3;
						itr->setTexture(pressed2);
					}
				}
				if (event.mouseButton.x >= 350 && event.mouseButton.x <= 450) {
					if (event.mouseButton.y >= 120 && event.mouseButton.y <= 170) {
						auto itr = sprites3.begin()+4;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 215 && event.mouseButton.y <= 265) {
						auto itr = sprites3.begin()+5;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 310 && event.mouseButton.y <= 360) {
						auto itr = sprites3.begin()+6;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 405 && event.mouseButton.y <= 455) {
						auto itr = sprites3.begin()+7;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 500 && event.mouseButton.y <= 550) {
						auto itr = sprites3.begin()+12;
						itr->setTexture(pressed2);
					}
				}
				if (event.mouseButton.x >= 600 && event.mouseButton.x <= 700) {
					if (event.mouseButton.y >= 120 && event.mouseButton.y <= 170) {
						auto itr = sprites3.begin()+8;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 215 && event.mouseButton.y <= 265) {
						auto itr = sprites3.begin()+9;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 310 && event.mouseButton.y <= 360) {
						auto itr = sprites3.begin()+10;
						itr->setTexture(pressed2);
					}
					if (event.mouseButton.y >= 405 && event.mouseButton.y <= 455) {
						auto itr = sprites3.begin()+11;
						itr->setTexture(pressed2);
					}
				}
			}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				if (event.mouseButton.x >= 100 && event.mouseButton.x <= 200) {
					if (event.mouseButton.y >= 120 && event.mouseButton.y <= 170) {
						userGenre = "Action";
						genre_window.close();
					}
					if (event.mouseButton.y >= 215 && event.mouseButton.y <= 265) {
						userGenre = "Adventure";
						genre_window.close();
					}
					if (event.mouseButton.y >= 310 && event.mouseButton.y <= 360) {
						userGenre = "Fighting";
						genre_window.close();
					}
					if (event.mouseButton.y >= 405 && event.mouseButton.y <= 455) {
						userGenre = "Misc";
						genre_window.close();
					}
				}
				if (event.mouseButton.x >= 350 && event.mouseButton.x <= 450) {
					if (event.mouseButton.y >= 120 && event.mouseButton.y <= 170) {
						userGenre = "Platform";
						genre_window.close();
					}
					if (event.mouseButton.y >= 215 && event.mouseButton.y <= 265) {
						userGenre = "Puzzle";
						genre_window.close();
					}
					if (event.mouseButton.y >= 310 && event.mouseButton.y <= 360) {
						userGenre = "Racing";
						genre_window.close();
					}
					if (event.mouseButton.y >= 405 && event.mouseButton.y <= 455) {
						userGenre = "Role-Playing";
						genre_window.close();
					}
					if (event.mouseButton.y >= 500 && event.mouseButton.y <= 550) {
						userGenre.clear();
						genre_window.close();
					}
				}
				if (event.mouseButton.x >= 600 && event.mouseButton.x <= 700) {
					if (event.mouseButton.y >= 120 && event.mouseButton.y <= 170) {
						userGenre = "Simulation";
						genre_window.close();
					}
					if (event.mouseButton.y >= 215 && event.mouseButton.y <= 265) {
						userGenre = "Sports";
						genre_window.close();
					}
					if (event.mouseButton.y >= 310 && event.mouseButton.y <= 360) {
						userGenre = "Strategy";
						genre_window.close();
					}
					if (event.mouseButton.y >= 405 && event.mouseButton.y <= 455) {
						userGenre = "Shooter";
						genre_window.close();
					}
				}
				for (auto itr = sprites3.begin(); itr != sprites3.end(); itr++) {
					itr->setTexture(unpressed2);
				}
			}
		}

		genre_window.clear();
		genre_window.draw(background3);
		for (auto itr = sprites3.begin(); itr != sprites3.end(); itr++) {
			genre_window.draw(*itr);
		}
		for (auto itr = texts3.begin(); itr != texts3.end(); itr++) {
			genre_window.draw(*itr);
		}
		genre_window.display();
	}

	//all nick stuff

	//Go through all the games and add based on specifications
	for (int i = 0; i <gamesVector.size(); i++) {
	//	 	 if (userPlatform == gamesVector[i].getPlatform() || userGenre == gamesVector[i].getGenre() || userYear == gamesVector[i].getYear()) {
	//			  userVector.push_back(gamesVector[i]);
	//	 	 }
		if (!userPlatform.empty()) {
			if (!userGenre.empty()) { // both
				if (userPlatform == gamesVector[i].getPlatform() && userGenre == gamesVector[i].getGenre())
					userVector.push_back(gamesVector[i]);
			} else { // only platform
				if (userPlatform == gamesVector[i].getPlatform())
					userVector.push_back(gamesVector[i]);
			}
		} else {
			if (!userGenre.empty()) { // only genre
				if (userGenre == gamesVector[i].getGenre())
					userVector.push_back(gamesVector[i]);
			} else { // neither
				userVector = gamesVector;
				break;
			}
		}

	}

	 //Mergesort the userVector and time it
	 auto mergeStart = high_resolution_clock::now();
	 mergeSort(userVector, sortType, 0, userVector.size()-1);
	 auto mergeStop = high_resolution_clock::now();
	 auto mergeTime = duration_cast<milliseconds>(mergeStop - mergeStart);
	 cout << "Merge Sort Took: " << mergeTime.count() << " milliseconds" << endl;
	 if (userVector.size() >= 20) {
		 for (int i = 0; i < 20; i++) {
			 cout << i + 1 << ". " << userVector[i].getName() << endl;
		 }
	 } else {
		 int count = 1;
		 for (auto itr = userVector.begin(); itr != userVector.end(); itr++) {
			 cout << count++ << ". " << itr->getName() << endl;
		 }
	 }

	 auto quicksortStart = high_resolution_clock::now();
	 quicksort(userVector, 0, userVector.size() - 1, sortType);
	 auto quicksortStop = high_resolution_clock::now();
	 auto quicksortTime = duration_cast<milliseconds>(quicksortStop - quicksortStart);
	 cout << "Quicksort Took: " << quicksortTime.count() << " milliseconds" << endl;
	if (userVector.size() >= 20) {
		for (int i = 0; i < 20; i++) {
			cout << i + 1 << ". " << userVector[i].getName() << endl;
		}
	} else {
		int count = 1;
		for (auto itr = userVector.begin(); itr != userVector.end(); itr++) {
			cout << count++ << ". " << itr->getName() << endl;
		}
	}


//	//It's BogoTime
//	auto bogoSortStart = high_resolution_clock::now();
//	bogoSort(userVector, sortType);
//	auto bogoSortStop = high_resolution_clock::now();
//	auto bogoSortTime = duration_cast<seconds>(bogoSortStop - bogoSortStart);
//	cout << "BogoSort Took: " << bogoSortTime.count() << " seconds" << endl;
//
//	// Print sorted games
//	for (int i = 0; i < 50; i++) {
//		cout << i + 1 << ". " << userVector[i].getName() << endl;
//	}

// Jacob again
// leaderboard window
	sf::RenderWindow final_window(sf::VideoMode(800, 600), "Games Unlimited Games", sf::Style::Close);
	sf::RectangleShape background5(sf::Vector2f(800, 600));
	background5.setFillColor(sf::Color::Blue);
	vector<sf::Text> texts4;

	sf::Text text53;
	sf::Text text54;
	sf::Text text55;
	sf::Text text56;
	sf::Text text57;
	sf::Text text58;
	sf::Text text59;
	sf::Text text60;
	sf::Text text61;
	sf::Text text62;
	sf::Text text63;
	sf::Text text64;
	sf::Text text65;
	sf::Text text66;
	sf::Text text67;
	sf::Text text68;
	sf::Text text69;
	sf::Text text70;
	sf::Text text71;
	sf::Text text72;
	sf::Text text73;
	sf::Text text74;
	textConfig(text53, font, 10, 25, 40);
	textConfig(text54, font, 10, 25, 140);
	textConfig(text55, font, 10, 25, 240);
	textConfig(text56, font, 10, 25, 340);
	textConfig(text57, font, 10, 25, 440);
	textConfig(text58, font, 10, 225,  60);
	textConfig(text59, font, 10, 225, 160);
	textConfig(text60, font, 10, 225, 260);
	textConfig(text61, font, 10, 225, 360);
	textConfig(text62, font, 10, 225, 460);
	textConfig(text63, font, 10, 425,  40);
	textConfig(text64, font, 10, 425, 140);
	textConfig(text65, font, 10, 425, 240);
	textConfig(text66, font, 10, 425, 340);
	textConfig(text67, font, 10, 425, 440);
	textConfig(text68, font, 10, 625,  60);
	textConfig(text69, font, 10, 625, 160);
	textConfig(text70, font, 10, 625, 260);
	textConfig(text71, font, 10, 625, 360);
	textConfig(text72, font, 10, 625, 460);
	textConfig(text73, font, 12, 200, 550);
	textConfig(text74, font, 12, 600, 550);
	try {
		text53.setString(" 1. " + userVector[0].getName());
	} catch (exception) {
		text53.setString(" 1. ");
	}
	try {
		text54.setString(" 2. " + userVector[1].getName());
	} catch (exception) {
		text54.setString(" 2. ");
	}
	try {
		text55.setString(" 3. " + userVector[2].getName());
	} catch (exception) {
		text55.setString(" 3. ");
	}
	try {
		text56.setString(" 4. " + userVector[3].getName());
	} catch (exception) {
		text56.setString(" 4. ");
	}
	try {
		text57.setString(" 5. " + userVector[4].getName());
	} catch (exception) {
		text57.setString(" 5. ");
	}
	try {
		text58.setString(" 6. " + userVector[5].getName());
	} catch (exception) {
		text58.setString(" 6. ");
	}
	try {
		text59.setString(" 7. " + userVector[6].getName());
	} catch (exception) {
		text59.setString(" 7. ");
	}
	try {
		text60.setString(" 8. " + userVector[7].getName());
	} catch (exception) {
		text60.setString(" 8. ");
	}
	try {
		text61.setString(" 9. " + userVector[8].getName());
	} catch (exception) {
		text61.setString(" 9. ");
	}
	try {
		text62.setString("10. " + userVector[9].getName());
	} catch (exception) {
		text62.setString("10. ");
	}
	try {
		text63.setString("11. " + userVector[10].getName());
	} catch (exception) {
		text63.setString("11. ");
	}
	try {
		text64.setString("12. " + userVector[11].getName());
	} catch (exception) {
		text64.setString("12. ");
	}
	try {
		text65.setString("13. " + userVector[12].getName());
	} catch (exception) {
		text65.setString("13. ");
	}
	try {
		text66.setString("14. " + userVector[13].getName());
	} catch (exception) {
		text66.setString("14. ");
	}
	try {
		text67.setString("15. " + userVector[14].getName());
	} catch (exception) {
		text67.setString("15. ");
	}
	try {
		text68.setString("16. " + userVector[15].getName());
	} catch (exception) {
		text68.setString("16. ");
	}
	try {
		text69.setString("17. " + userVector[16].getName());
	} catch (exception) {
		text69.setString("17. ");
	}
	try {
		text70.setString("18. " + userVector[17].getName());
	} catch (exception) {
		text70.setString("18. ");
	}
	try {
		text71.setString("19. " + userVector[18].getName());
	} catch (exception) {
		text71.setString("19. ");
	}
	try {
		text72.setString("20. " + userVector[19].getName());
	} catch (exception) {
		text72.setString("20. ");
	}
	text73.setString("Quicksort Took: " + to_string(quicksortTime.count()) + " milliseconds");
	text74.setString("Merge Sort Took: " + to_string(mergeTime.count()) + " milliseconds");
	setText(text73);
	setText(text74);
	texts4.push_back(text53);
	texts4.push_back(text54);
	texts4.push_back(text55);
	texts4.push_back(text56);
	texts4.push_back(text57);
	texts4.push_back(text58);
	texts4.push_back(text59);
	texts4.push_back(text60);
	texts4.push_back(text61);
	texts4.push_back(text62);
	texts4.push_back(text63);
	texts4.push_back(text64);
	texts4.push_back(text65);
	texts4.push_back(text66);
	texts4.push_back(text67);
	texts4.push_back(text68);
	texts4.push_back(text69);
	texts4.push_back(text70);
	texts4.push_back(text71);
	texts4.push_back(text72);
	texts4.push_back(text73);
	texts4.push_back(text74);

	while (final_window.isOpen()) {
		sf::Event event;
		while (genre_window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {}
			final_window.close();
		}
		final_window.clear();
		final_window.draw(background5);
		for (auto itr = texts4.begin(); itr != texts4.end(); itr++) {
			final_window.draw(*itr);
		}
		final_window.display();
	}

	return 0;

}
