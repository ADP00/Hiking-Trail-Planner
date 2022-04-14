#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
using namespace std;

//Defines what a point is. Consists of its name, elevation in meters, and distance from the start in meters
struct Point {
	float dist;
	float alt;
	string name;
};

//Formats the output of a single point
void printPoint(Point a) {
	cout << a.name << " has an altitude of " << a.alt << " meters and a distance from the start of " << a.dist << endl;
}

//Goes through an array of points, using the printPoint function to output the array
void printList(Point list[], int size) {
	for (int i = 0; i < size; i++) {
		printPoint(list[i]);
	}
}

//Sorts the points in the array based on their distance from the start. It will go from least to greatest. This is done recursively using the fusion sort method.
void sort (Point list[], int size) {
	if (size <= 1) {
		
	} else {
		int s1 = size/2;
		int s2 = size-s1;
		Point a1[s1];
		Point a2[s2];
		for (int i = 0; i < s1; i++) {
			a1[i] = list[i];
		}
		for (int i = 0; i < s2; i++) {
			a2[i] = list[s1 + i];
		}
		sort(a1, s1);
		sort(a2, s2);
		int j = 0, k = 0;
		for (int i = 0; i < size; i++) {
			if (j >= s1) {
				list[i] = a2[k];
				k++;
			} else if (k >= s2) {
				list[i] = a1[j];
				j++;
			} else if (a1[j].dist < a2[k].dist) {
				list[i] = a1[j];
				j++;
			} else if (a1[j].dist > a2[k].dist) {
				list[i] = a2[k];
				k++;
			} else {
				list[i] = a1[j];
				j++;
			}
		}
	}
}

//Gives the horizontal distance, assuming the last point in the array is the furthest
float totalHorzDist (Point a[], int size) {
	return a[size-1].dist;
}

//Gives the net change in elevation, comparing the altitude of the starting and end points
float totalElevChange (Point a[], int size) {
	return a[size-1].alt - a[0].alt;
}

//Returns the highest point by comparing the altitudes of each point
Point highestPoint (Point a[], int size) {
	int highI = 0;
	for (int i = 1; i < size; i++) {
		if (a[i].alt > a[highI].alt) {
			highI = i;
		}
	}
	return a[highI];
}

//Compares the altitude of two points, returns 1 if the first is greater, -1 for lesser, and 0 for equal
int compareAlt (Point a, Point b) {
	if (a.alt > b.alt) {
		return 1;
	} else if (a.alt < b.alt) {
		return -1;
	} else {
		return 0;
	}
}

//Looks at where the altitude change is positive, and then sums the distance between those points
float totalClimbed (Point a[], int size) {
	float sum = 0;
	for (int i = 1; i < size; i++) {
		if (compareAlt(a[i-1], a[i]) == -1) {
			sum += a[i].alt - a[i-1].alt;
		}
	}
	return sum;
}

//Looks at where the altitude change is negative, and sums the distance between those points
float totalDescended (Point a[], int size) {
	float sum = 0;
	for (int i = 1; i < size; i++) {
		if (compareAlt(a[i-1], a[i]) == 1) {
			sum += a[i-1].alt - a[i].alt;
		}
	}
	return sum;
}

//Calculates the direct, straight distance between two points using the pathagorean theorem.
float pointDistance (Point a, Point b) {
	float horz = b.dist - a.dist;
	float vert = b.alt - a.alt;
	float dist = sqrt(horz*horz + vert*vert);
	return dist;
}

//Calculates the distance between consecutive points and sums them.
float actualDist (Point a[], int size) {
	float dist = 0;
	for (int i = 1; i < size; i++) {
		dist += pointDistance(a[i-1], a[i]);
	}
	return dist;
}

//Calculates the angle from one point to another using arctan and the change in altitude and change in distance. Calculates the angle going from a to b.
float angle (Point a, Point b) {
	float rise = b.alt-a.alt;
	float run = b.dist-a.dist;
	return (float)atan(rise/run);
}

//Calculates the steepest angle between two points in the array and returns it
float steepestSlope (Point a[], int size) {
	float big = angle(a[0], a[1]);
	for (int i = 2; i < size; i++) {
		float current = angle(a[i-1], a[i]);
		if (current < 0) {
			current *= -1;
		}
		if (current > big) {
			big = current;
		}
	}
	return big*180/(atan(1)*4);//atan(1)*4 equals Pi
}

//Sorts the array from the largest altitude to the least in order for it to be displayed. It is done recursively using the fusion method.
void sortByAlt (Point list[], int size) {
	if (size <= 1) {
		
	} else {
		int s1 = size/2;
		int s2 = size-s1;
		Point a1[s1];
		Point a2[s2];
		for (int i = 0; i < s1; i++) {
			a1[i] = list[i];
		}
		for (int i = 0; i < s2; i++) {
			a2[i] = list[s1 + i];
		}
		sortByAlt(a1, s1);
		sortByAlt(a2, s2);
		int j = 0, k = 0;
		for (int i = 0; i < size; i++) {
			if (j >= s1) {
				list[i] = a2[k];
				k++;
			} else if (k >= s2) {
				list[i] = a1[j];
				j++;
			} else if (a1[j].alt > a2[k].alt) {
				list[i] = a1[j];
				j++;
			} else if (a1[j].alt < a2[k].alt) {
				list[i] = a2[k];
				k++;
			} else {
				list[i] = a1[j];
				j++;
			}
		}
	}
}

//Outputs the names of the points in a roughly to-scale representation of the trail by sorting the array from highest altitude to smallest, and then 
//based on each points distance from the start adding spaces horizontaly, and then based on the altitude difference to the next point add blank lines.
string display (Point list[], int size) {
	string build = "";
	sortByAlt(list,size);
	int div = 100;
	if (totalHorzDist(list, size) > 9000) {
		div = 500;
	} else if (totalHorzDist(list, size) > 50000) {
		div = 1000;
	}
	for (int i = 0; i < size - 1; i++) {
		float scale = list[i].dist/div;
		for (int x = 0; x < scale; x++) {
			build += "  ";
		}
		build += list[i].name;
		float altScale = (list[i].alt-list[i + 1].alt)/div;
		if (altScale < 2) {
			build += "\n";
		} else {
			for (int x = 0; x < altScale; x++) {
				build += "\n";
			}
		}
	}
	for (int i = 0; i < list[size-1].dist/div; i++) {
		build += "  ";
	}
	build += list[size-1].name;
	return build;
}

//For flat or very gradual trails, it will take 30 minutes to hike one mile. For every 1000 feet gained in elevation, another 30 minutes is added.
//Going downhill is assumed to take as long as a flat path.
float estTime (Point list[], int size) {
	float time = 0;
	for (int i = 1; i < size; i++) {
		float a = angle(list[i-1],list[i])*180/(atan(1)*4);
		if (a <= 5) {
			float miles = pointDistance(list[i-1],list[i])*0.000621371;
			time += miles*30;
		} else {
			float altChangeFeet = (list[i].alt - list[i-1].alt)*3.28084;
			float miles = pointDistance(list[i-1],list[i])*0.000621371;
			time += miles*30 + altChangeFeet/1000 * 30;
		}
	}
	return time;
}

//Calculates time starting from the end of the trail. The angle is still calculted from the point closer to the start to the further, but the if statement has been changed to accomodate this.
//If the angle is greater than -5 degrees, then that means it's a negative or gradual slope going in the reverse direction, and anything else would be a positive slope in the reverse direction.
float revTime (Point list[], int size) {
	float time = 0;
	for (int i = size - 2; i >= 0; i--) {
		float a = angle(list[i], list[i+1])*180/(atan(i)*4);
		if(a >= -5) {
			float miles = pointDistance(list[i+1],list[i])*0.000621371;
			time += miles*30;
		} else {
			float altChangeFeet = (list[i].alt - list[i+1].alt)*3.28084;
			float miles = pointDistance(list[i+1],list[i])*0.000621371;
			time += miles*30 + altChangeFeet/1000 * 30;
		}
	}
	return time;
}

//This function will read a file with a path given by the user. The file also needs to be formatted so that each line has the name, distance,
//and altitude in that order separated by spaces. 
void readFile (Point list[], string path) {
	ifstream in(path);
	while (!in) {
		cerr << "File not found or unable to open." << endl;
	}
	int i = 0;
	while (!in.eof()) {
		in >> list[i].name >> list[i].dist >> list[i].alt;
		i++;
	}
}

//Takes the input from the user by looping through the array that is created in the main and tells the user what form to give the information.
//The user is prompted if they have a file with the information and they are told how the file should be formatted. If yes, the path is asked for and the readFile function is called.
//If no, the information for each point is taken one at a time. In either case, the user is asked if the hike will be a round trip.
bool input (Point list[], int size) {
	cout << "Do you have a file with all the information? (Y/N)\n(In order for it to work, the file needs to have\n the name, distance, and altitude in that order\n on one line separated by spaces. Do this for\n each point. The file must be .txt) ";
	string f; cin >> f;
	if (f == "Y" || f == "y") {
		cout << "Please enter the file's path: ";
		string path; cin >> path;
		readFile(list, path);
	} else {
		cout << "Please enter all distance and elevation numbers in meters." << endl;
		for (int i = 0; i < size; i++) {
			cout << "Point " << i+1 << ":\n";
			cout << "Name: ";
			cin >> list[i].name;
			cout << "Distance from the starting point: ";
			cin >> list[i].dist;
			cout << "Elevation: ";
			cin >> list[i].alt;
		}
	}
	cout << "Will this be a round trip? (Y/N) ";
	string round; cin >> round;
	if (round == "Y" || round == "y") {
		return true;
	} else {
		return false;
	}
}

//Formats the ouput of all the necessary functions.
void output (Point list[], int size, bool trip) {
	sort(list, size);
	int twice = 1;
	float climbed = 0;
	float desc = 0;
	float time = 0;
	if (trip) {
		twice = 2;
		climbed = totalClimbed(list, size);
		desc = totalDescended(list, size);
		time = revTime(list, size);
	}	
	cout << "The total horizontal distance to travel is " << totalHorzDist(list, size)*twice << " meters." << endl;
	cout << "The total change in the elevation from the point " << list[0].name << " to the point " << list[size-1].name << " is " << totalElevChange(list, size) << " meters." << endl;
	Point high = highestPoint(list, size);
	cout << "The highest point is at " << high.name << " which has an altitude of " << high.alt << " meters." << endl;
	cout << "The total number of meters climbed is " << totalClimbed(list, size) + desc << " meters." << endl;
	cout << "The total number of meters descended is " << totalDescended(list, size) + climbed << " meters." << endl;
	cout << "The actual distance walked is " << actualDist(list, size)*twice << " meters." << endl;
	cout << "The steepest slope is " << steepestSlope(list, size) << " degrees." << endl;
	cout << "The entire trail will take about " << estTime(list, size) + time << " minutes to complete." << endl;
	cout << "Here is an approximate 2D visualization of what the trail will look like: " << endl;
	cout << display(list,size);
}

//The option for a round trip was added to the input and ouput functions later. The input function was changed to return a boolean value (true for roundtrip, flase for not)
//and in the output function a boolean is taken in, and several variable were added in the function that change based on if that boolean is true. One variable for the output values that 
//only needed to be multiplied by two for a round trip (twice), two for adding values to the number of meters cimbed and descended (climbed and desc) since you will be 
//climbing what you descended on the return trip and vice versa, and also one for time. Since the time claculation is based on the steepness of upward slopes, time needs 
//to be calculated starting from the end of the array, which is what revTime does. 

int main() {
	cout << "Welcome to Hiking Planner\nPlease enter the total number of locations on the hiking trail: ";
	int size; cin >> size;
	Point list[size];
	bool trip = input(list, size);
//	Point list[5] = {{0, 2000, "Start"}, {1500, 2100, "revine"}, {1000, 2300, "cliff"}, {2100, 1900, "lake"}, {2500, 2050, "end"}};
//	Point list[9] = {{0, 1120, "Start"}, {1500, 1190, "Pt2"}, {4300, 1350, "Soda Springs"}, {6400, 1650, "Pt4"}, {8200, 1886, "Red Crater"}, {10500, 1625, "Pt6"}, {13100, 1456, "Ketetahi"}, {16700, 1040, "Pt8"}, {19400, 760, "End"}};
//	printList(list, size);
	output(list, size, trip);
	return 0;
}
