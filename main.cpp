#include <iostream>
using namespace std;

class Game {
private:
	int rank; //You'll have to convert from string to int after reading since all file stuff is strings
	string name;
	string platform;
	string year; //I don't think year actually need to be an int because its not being compared
	string genre;
	string publisher;
	string northAmericaSales; //I don't think sales actually need to be an int because its not being compared
	string europeSales;
	string japanSales;
	string otherSales;
	string globalSales;
};

int main() {

    cout << "Welcome to Team BOGO'S: Games, UNLIMTIED Games" << endl;
    return 0;
    // test AE
	// test JA

}
