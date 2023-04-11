#include "stdafx.h"
#include "GraphicsSettings.h"

//======Constructor======
GraphicsSettings::GraphicsSettings()
{
	this->title = "ALAPÉRTELMEZETT";
	this->resolution = sf::VideoMode::getDesktopMode();
	this->fullscreen = false;
	this->vsync = false;
	this->framRateLimit = 120;
	this->contextSettings.antialiasingLevel = 0;
	this->videoModes = sf::VideoMode::getFullscreenModes();
	this->changed = false;
	if (!icon.loadFromFile("Resources/Images/Backgrounds/puzzle.png"))
		std::cout << "DEBUG::GraphicsSettings::Ikon betoltese sikertelen!" << "\n";
}

//======Függvények======
void GraphicsSettings::saveToFile(const std::string path)
{
	std::ofstream ofs(path);
	if (ofs.is_open())
	{
		ofs << this->title <<"\n"
			<< this->resolution.width << " " << this->resolution.height << "\n"
			<< this->fullscreen << "\n"
			<< this->framRateLimit << "\n"
			<< this->vsync << "\n"
			<< this->contextSettings.antialiasingLevel;
	}
	else std::cout << "DEBUG::Game.H::Sikertelen grafikai fajl kiiras" << "\n";
	ofs.close();
}

void GraphicsSettings::loadFromFile(const std::string path)
{
	std::ifstream ifs(path);
	if (ifs.is_open())
	{
		std::getline(ifs, this->title);
		ifs >> this->resolution.width >> this->resolution.height;
		ifs >> this->fullscreen;
		ifs >> this->framRateLimit;
		ifs >> this->vsync;
		ifs >> this->contextSettings.antialiasingLevel;

	}
	else std::cout << "DEBUG::Game.H::Sikertelen grafikai fajl megnyitas" << "\n";
	ifs.close();
}
