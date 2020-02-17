#include <windows.h>
#include <vector>
#include <map>
#include <string>
#include <SFML\Graphics.hpp>
#include "aquarium.h"

using std::map;
using std::vector;
using std::string;
using std::pair;

// main types::
using aquarium::Aquarium;				// Aquarium class
using aquarium::ObjectType;				// type of fish class
using aquarium::Object;					// class of fish

// for Aquarium:
using aquarium::XMax;					// aquarium width class
using aquarium::YMax;					// aquarium height class
using aquarium::ObjectSize;				// fish size class

// for fish type::
using aquarium::ObjectTypeName;			// fish type name class
using aquarium::SpeedWalk;				// speed class while calm
using aquarium::SpeedRun;				// speed class while chasing
using aquarium::Vision;					// vision radius class
using aquarium::BirthFrequency;			// class of frequency of birth of new fish
using aquarium::Lifetime;				// life expectancy class
using aquarium::MaxCount;				// class of maximum number of fish of a certain type
using aquarium::Settled;				// fish type settled class

// for one fish:
using aquarium::Location;				// class location
using aquarium::X;						// class X coordinate
using aquarium::Y;						// class Y coordinate
using aquarium::A;						// class angle (in degrees)

// for diplomacy:
using aquarium::DiplomaticStatus;		// class of relationships between types of fish

// for conversions between degrees and radians
using aquarium::degree_to_radian;
using aquarium::radian_to_degree;

// aquarium dimensions
const double x_m = 1300, y_m = 650;
// fish size
const double Object_size = 10;
int day = 0;


namespace n1 {
	std::vector<int> v;
}


int main() {
	// Aquarium
	Aquarium a1{ XMax(x_m), YMax(y_m), ObjectSize(Object_size) };

	// add fish types
	a1.add_type(ObjectTypeName("hunter_fish"), SpeedWalk(7), SpeedRun(8.5), Vision(140), BirthFrequency(600), Lifetime(1200), MaxCount(4), Settled(false));
	a1.add_type(ObjectTypeName("fish_peaceful"), SpeedWalk(6), SpeedRun(8), Vision(110), BirthFrequency(270), Lifetime(800), MaxCount(20), Settled(false));
	a1.add_type(ObjectTypeName("trav"), SpeedWalk(5), SpeedRun(7.5), Vision(85), BirthFrequency(150), Lifetime(600), MaxCount(40), Settled(true));

	// add the fish
	a1.add_Object(ObjectTypeName("hunter_fish"), Location(X(1000), Y(400), A(180)));
	for (unsigned u = 0; u != 2; ++u) {
		a1.add_Object(ObjectTypeName("fish_peaceful"), Location(X(300), Y(400), A(90)));
		a1.add_Object(ObjectTypeName("fish_peaceful"), Location(X(300), Y(400), A(111)));
		a1.add_Object(ObjectTypeName("trav"), Location(X(900), Y(70), A(0)));
		a1.add_Object(ObjectTypeName("trav"), Location(X(900), Y(60), A(0)));
	}

	// establish relationships between types of fish
	// the player is hunted by ObjectTypeName ("fish_peaceful")
	a1.set_diplomatic_status(ObjectTypeName("hunter_fish"), ObjectTypeName("fish_peaceful"), DiplomaticStatus(2));
	a1.set_diplomatic_status(ObjectTypeName("fish_peaceful"), ObjectTypeName("trav"), DiplomaticStatus(1));
	a1.set_diplomatic_status(ObjectTypeName("fish_peaceful"), ObjectTypeName("hunter_fish"), DiplomaticStatus(-1));
	a1.set_diplomatic_status(ObjectTypeName("trav"), ObjectTypeName("fish_peaceful"), DiplomaticStatus(-1));




	//a1.add_type(ObjectTypeName("hunter_fish"), SpeedWalk(7), SpeedRun(8.5), Vision(240), BirthFrequency(600), Lifetime(1200), MaxCount(2), Settled(false));
	//a1.add_type(ObjectTypeName("fish_peaceful"), SpeedWalk(6), SpeedRun(8), Vision(210), BirthFrequency(320), Lifetime(800), MaxCount(5), Settled(false));
	//a1.add_type(ObjectTypeName("trav"), SpeedWalk(5), SpeedRun(7.5), Vision(185), BirthFrequency(120), Lifetime(600), MaxCount(9), Settled(true));
	//a1.add_Object(ObjectTypeName("hunter_fish"), Location(X(400), Y(300), A(0)));
	//a1.add_Object(ObjectTypeName("fish_peaceful"), Location(X(500), Y(300), A(0)));
	//a1.add_Object(ObjectTypeName("trav"), Location(X(600), Y(300), A(0)));
	//a1.set_diplomatic_status(ObjectTypeName("hunter_fish"), ObjectTypeName("fish_peaceful"), DiplomaticStatus(2));
	//a1.set_diplomatic_status(ObjectTypeName("fish_peaceful"), ObjectTypeName("trav"), DiplomaticStatus(1));
	//a1.set_diplomatic_status(ObjectTypeName("fish_peaceful"), ObjectTypeName("hunter_fish"), DiplomaticStatus(-1));
	//a1.set_diplomatic_status(ObjectTypeName("trav"), ObjectTypeName("fish_peaceful"), DiplomaticStatus(-1));


	// Window
	sf::RenderWindow window(sf::VideoMode(x_m, y_m), "_aquarium");

	while (window.isOpen()) {
		// window close processing
		sf::Event Event;
		while (window.pollEvent(Event)) {
			if (Event.type == sf::Event::Closed)
				window.close();
		}
		// the rest of the fish take a step
		a1.step();

		// get fish locations
		// and the names of their types to know what to draw
		map<string, vector<Location>> Objectes = a1.get_Object_locations();

		// clear old image
		window.clear(sf::Color::Blue);

		// picture of fish (square)
		sf::RectangleShape Object_piture(sf::Vector2f(Object_size, Object_size));
		// fish display
		for (const pair<string, vector<Location>> &one_type : Objectes) {
			// determine what type of fish we are dealing with
			// and select the appropriate color
			if (one_type.first == "hunter_fish") {
				Object_piture.setFillColor(sf::Color::Red);
			}
			else if (one_type.first == "fish_peaceful") {
				Object_piture.setFillColor(sf::Color::Black);
			}
			else if (one_type.first == "trav") {
				Object_piture.setFillColor(sf::Color::Green);
			}
			// draw them
			for (const Location &one_Object : one_type.second) {
				Object_piture.setPosition(one_Object.x - Object_size / 2, y_m - (one_Object.y + Object_size / 2));
				window.draw(Object_piture);
			}
		}
		// display new frame
		window.display();

		// time delay
		Sleep(26);
		day++;
		printf("\r Day # %d", day/100);
	}
	return 0;
}