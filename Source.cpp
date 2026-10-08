#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

// Keeps the information for one labeled object together.
struct Annotation {
	string imageName;
	string label;
	int x;
	int y;
	int width;
	int height;

};

// Keeps asking until the user enters a valid whole number
int readNumber(const string& prompt, int minimum, int maximum) {
	int number;
	
	while (true) {
		cout << prompt;

		if (cin >> number && number >= minimum && number <= maximum) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return number;
		}

		cout << "Enter a whole number from"
			<< minimum << " to " << maximum << ".\n";

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

	}
}

int main() {
	Annotation object;
	int imageWidth;
	int imageHeight;
	int choice;

	cout << "== ROAD SCENE LABELER==\N\N";

	// CSV columns use commas, so reject commas in the filename.
	do {
		cout << " Image filename, such as street01.jpg:";
		getline(cin, object.imageName);
	} while (object.imageName.empty() ||
		object.imageName.find_first_of(",\"") != string::npos);

	imageWidth = readNumber("Image width in pixels:", 1, 100000);
	imageHeight = readNumber("Image height in pixels:", 1, 100000);

	int labelAnother;
	do {



		cout << "\nWhat object are you labeling?\n";
		cout << "1. Car\n";
		cout << "2. Pedestrain \n";
		cout << "3. Bicycle\n";
		cout << "4. Traffic light\n";

		choice = readNumber("Choose 1-4:", 1, 4);

		switch (choice) {
		case 1: object.label = "car"; break;
		case 2: object.label = "pedestrain"; break;
		case 3: object.label = "bicycle"; break;
		case 4: object.label = "traffic_light"; break;
		}

		// Limit coordinates and dimensions to keep the box inside the image.
		cout << "\nEnter the object's bounding box.\n";

		object.x = readNumber("Left postition (x):", 0, imageWidth - 1);
		object.y = readNumber("Top position (y):", 0, imageHeight - 1);

		object.width = readNumber(
			"Box width:", 1, imageWidth - object.x
		);

		object.height = readNumber(
			"Box height:", 1, imageHeight - object.y
		);

		//  Check whether the file needs column headings.
		ifstream existingFile("annotations.csv.");
		bool needsHeader =
			!existingFile || existingFile.peek() == ifstream::traits_type::eof();

		// Append keeps previously saved annotations.
		ofstream outputFile("annotations.csv", ios::app);

		if (!outputFile) {
			cerr << "Cloud not open annotations.csv.\n";
			return 1;
		}
		if (needsHeader) {
			outputFile << "image,label,x,y,width,height\n";
		}

		outputFile << object.imageName << ","
			<< object.label << ","
			<< object.x << ","
			<< object.y << ","
			<< object.width << ","
			<< object.height << "\n";

		outputFile.close();

		if (!outputFile) {
			cerr << "The annotation could not be saved.\n";

			return 1;

		}

		cout << "\nSaved one " << object.label
			<< "annotation to annotations.csv!\n";

		labelAnother = readNumber(
			"\nLabel another object? (1 = Yes , 0 = No): ", 0, 1
		);
	} while (labelAnother == 1);

	return 0;


}