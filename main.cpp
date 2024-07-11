#include <iostream>
#include <vector>
#include <chrono>
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

	// Create temp arrays
	vector<Game> leftArray(subArrayOne);
	vector<Game> rightArray(subArrayTwo);

	// Copy data to temp arrays leftArray[] and rightArray[]
	for (int i = 0; i < subArrayOne; i++)
		leftArray[i] = games[left + i];
	for (int j = 0; j < subArrayTwo; j++)
		rightArray[j] = games[middle + 1 + j];

	int indexOfSubArrayOne = 0;
	int indexOfSubArrayTwo = 0;
	int indexOfMergedArray = left;

	// Merge the temp arrays back into array[left..right]
	while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
		bool condition;
		if (sortType == "NA") {
			condition = leftArray[indexOfSubArrayOne].getNA() >= rightArray[indexOfSubArrayTwo].getNA();
		} else if (sortType == "EU") {
			condition = leftArray[indexOfSubArrayOne].getEU() >= rightArray[indexOfSubArrayTwo].getEU();
		} else if (sortType == "JP") {
			condition = leftArray[indexOfSubArrayOne].getJP() >= rightArray[indexOfSubArrayTwo].getJP();
		} else if (sortType == "Other") {
			condition = leftArray[indexOfSubArrayOne].getOther() >= rightArray[indexOfSubArrayTwo].getOther();
		} else {
			condition = leftArray[indexOfSubArrayOne].getGlobal() >= rightArray[indexOfSubArrayTwo].getGlobal();
		}

		if (condition) {
			games[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
			indexOfSubArrayOne++;
		} else {
			games[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
			indexOfSubArrayTwo++;
		}
		indexOfMergedArray++;
	}

	// Copy the remaining elements of left[], if there are any
	while (indexOfSubArrayOne < subArrayOne) {
		games[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
		indexOfSubArrayOne++;
		indexOfMergedArray++;
	}

	// Copy the remaining elements of right[], if there are any
	while (indexOfSubArrayTwo < subArrayTwo) {
		games[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
		indexOfSubArrayTwo++;
		indexOfMergedArray++;
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


int main() {
	vector<Game> gamesVector; //Games get added here after data is parsed
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



	//all jacob stuff
    cout << "Welcome to Team BOGO'S: Games, UNLIMTIED Games" << endl;



	//all nick stuff

	//Go through all of the games and add based on specifications
	for (int i = 0; i <gamesVector.size(); i++) {
		if (userPlatform == gamesVector[i].getPlatform() || userGenre == gamesVector[i].getGenre() || userYear == gamesVector[i].getYear()) {
			userVector.push_back(gamesVector[i]);
		}
	}

	//Mergesort the userVector and time it
	auto mergeStart = high_resolution_clock::now();
	mergeSort(userVector, sortType, 0, userVector.size()-1);
	auto mergeStop = high_resolution_clock::now();
	auto mergeTime = duration_cast<microseconds>(mergeStop - mergeStart);

	cout << "Merge time: " << mergeTime.count() << " nanoseconds" << endl;
	for (int i = 0; i < userVector.size(); i++) {
		cout << userVector[i].getName() << endl;
	}

	return 0;

}
