#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>
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
	int northAmericaSales;
	int europeSales;
	int japanSales;
	int otherSales;
	int globalSales;
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
		int northAmericaSales, int europeSales, int japanSales, int otherSales, int globalSales) {
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
	int getNA() {
		return northAmericaSales;
	}
	int getEU() {
		return europeSales;
	}
	int getJP() {
		return japanSales;
	}
	int getOther() {
		return otherSales;
	}
	int getGlobal() {
		return globalSales;
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
            int naSale = stoi(SnaSale);
            int euSale = stoi(SeuSale);
            int jpSale = stoi(SjpSale);
            int otherSale = stoi(SotherSale);
            int globalSale = stoi(SglobalSale);

            // make the object and then push back into object vector
            Game obj(rank, name, platform, year, genre, publisher, naSale, euSale, jpSale, otherSale, globalSale);
            objects.push_back(obj);
        } catch (invalid_argument& e) {
            cout << "conversion" << e.what() << endl;
        } catch (out_of_range& e) {
            cout << "oor" << e.what() << endl;
        }
    }
    return objects;
}

int main() {
    string filePath = "../vgsales.csv"; // not working yet idk whats wrong
	vector<Game> gamesVector = parseCSV(filePath); //Games get added here after data is parsed
	vector<Game> userVector; //This is the vector that will be sorted and returned
	string sortType; //This should be either NA, EU, JP, or Other
	string userPlatform;
	string userGenre;
	string userYear;
	//These were examples that I did to test the Sort
	// Game game1(1, "game1", "nickbox", "2004", "nickgenre", "nicktendo", 100, 10, 2, 4, 116);
	// Game game2(2, "game2", "nickbox", "2004", "nickgenre", "nicktendo", 600, 10, 2, 4, 116);
	// Game game3(2, "game3", "nickbox", "2004", "nickgenre", "nicktendo", 400, 10, 2, 4, 116);
	// gamesVector.push_back(game1);
	// gamesVector.push_back(game2);
	// gamesVector.push_back(game3);



	//all alyssa stuff
    // this prints out all of the data
    for (int i = 0; i < 40; i++) { // limit to 40 cuz so much data!!!!!
        cout << "Rank: " << gamesVector[i].getRank()
                  << ", Name: " << gamesVector[i].getName()
                  << ", Platform: " << gamesVector[i].getPlatform()
                  << ", Year: " << gamesVector[i].getYear()
                  << ", Genre: " << gamesVector[i].getGenre()
                  << ", Publisher: " << gamesVector[i].getPublisher()
                  << ", NA Sales: " << gamesVector[i].getNA()
                  << ", EU Sales: " << gamesVector[i].getEU()
                  << ", JP Sales: " << gamesVector[i].getJP()
                  << ", Other Sales: " << gamesVector[i].getOther()
                  << ", Global Sales: " << gamesVector[i].getGlobal()
                  << endl;
        cout << "-----------------------------" << endl; // this is bc i am blind af
    }




	//all jacob stuff
    cout << "Welcome to Team BOGO'S: Games, UNLIMTIED Games" << endl;



	//all nick stuff

	//Go through all of the games and add based on specifications
//	for (int i = 0; i <gamesVector.size(); i++) {
//		if (userPlatform == gamesVector[i].getPlatform() || userGenre == gamesVector[i].getGenre() || userYear == gamesVector[i].getYear()) {
//			userVector.push_back(gamesVector[i]);
//		}
//	}
//
//	//Mergesort the userVector and time it
//	auto mergeStart = high_resolution_clock::now();
//	mergeSort(userVector, sortType, 0, userVector.size()-1);
//	auto mergeStop = high_resolution_clock::now();
//	auto mergeTime = duration_cast<microseconds>(mergeStop - mergeStart);
//
//	cout << "Merge time: " << mergeTime.count() << " nanoseconds" << endl;
//	for (int i = 0; i < userVector.size(); i++) {
//		cout << userVector[i].getName() << endl;
//	}
//
	return 0;

}
