#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "json.hpp"

using json = nlohmann::json;

namespace fs = std::filesystem;

struct Man
{
	std::string name;
	int age;
	float rost;
	float ves;
	bool deti;
};

std::istream& operator >> (std::istream& in, Man& man)
{
	std::string age, rost, ves, deti;
	in >> man.name >> age >> rost >> ves >> deti;

//корректность возраста
	if (age.find_first_not_of("0123456789") == std::string::npos)
		man.age = stoi(age);
	else {
		std::cout << "The age is incorrect" << std::endl;
		man.age = 0;
	}
// корректность роста
	if (rost.find_first_not_of("0123456789,") == std::string::npos)
		man.rost = atof(rost.c_str());
	else {
		std::cout << "The growth is incorrect" << std::endl;
		man.rost = 0;
	}

//корректность веса
	if (ves.find_first_not_of("0123456789,") == std::string::npos)
		man.ves = atof(ves.c_str());
	else {
		std::cout << "The weight is incorrect" << std::endl;
		man.ves = 0;
	}

//наличие детей
	if (deti == "y" || deti == "Y") man.deti = true;
	else man.deti = false;

	return in;
}


void to_json(json& j_obj, const Man& man)
{
	j_obj = json{
		{ "name", man.name },
		{ "age", man.age },
		{ "rost", man.rost },
		{ "ves", man.ves },
		{ "deti", man.deti }
	};
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
	std::vector <Man> people;
	Man tmp;
	int kol = 0;

	std::cout << "How many people do you want to enter information about? ";
	std::cin >> kol;
	std::cout << "Enter the data about the person separated by a space: name, age(full years), growth, weight, do you have children(y/n)?" << std::endl;
	for (int i = 0; i < kol; i++) {
		std::cin.ignore();
		std::cout << i+1 << ": ";
		std::cin >> tmp;
		people.push_back(tmp);
	}

	std::vector <json> j_object;

	fs::create_directories("JSON");

	std::ofstream fout;

	for (int i = 0; i < kol; i++) {
		j_object.push_back( { people[i] } );
		fout.open("JSON/person" + std::to_string(i+1) + ".txt", std::ios::trunc);
		fout << j_object[i];
		fout.close();
	}

}

