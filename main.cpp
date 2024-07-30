#include <algorithm>
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



	//all nick stuff

	//Go through all of the games and add based on specifications
	// for (int i = 0; i <gamesVector.size(); i++) {
	// 	if (userPlatform == gamesVector[i].getPlatform() || userGenre == gamesVector[i].getGenre() || userYear == gamesVector[i].getYear()) {
	// 		userVector.push_back(gamesVector[i]);
	// 	}
	// }
	//
	// //Mergesort the userVector and time it
	// auto mergeStart = high_resolution_clock::now();
	// mergeSort(gamesVector, "NA", 0, gamesVector.size()-1);
	// auto mergeStop = high_resolution_clock::now();
	// auto mergeTime = duration_cast<milliseconds>(mergeStop - mergeStart);
	// cout << "Merge Sort Took: " << mergeTime.count() << " milliseconds" << endl;
	// for (int i=0; i <50; i++) {
	// 	cout << i+1 << ". " << gamesVector[i].getName() << "(" << gamesVector[i].getNA() << ")" << endl;
	// }
	//
	// auto quicksortStart = high_resolution_clock::now();
	// quicksort(gamesVector, 0, gamesVector.size() - 1, "NA");
	// auto quicksortStop = high_resolution_clock::now();
	// auto quicksortTime = duration_cast<milliseconds>(quicksortStop - quicksortStart);
	// cout << "Quicksort Took: " << quicksortTime.count() << " milliseconds" << endl;
	// for (int i=0; i <50; i++) {
	// 	cout << i+1 << ". " << gamesVector[i].getName() << "(" << gamesVector[i].getNA() << ")" << endl;
	// }


	return 0;

}
