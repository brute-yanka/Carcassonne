#ifndef GRAPHICSSETTINGS_H
#define GRAPHICSSETTINGS_H

class GraphicsSettings
{
public:
	//Változók
	sf::Image icon;
	std::string title;
	sf::VideoMode resolution;
	bool fullscreen;
	bool vsync;
	unsigned framRateLimit;
	sf::ContextSettings contextSettings;
	std::vector<sf::VideoMode> videoModes;
	bool changed;

	//Constructors / Deconstructors
	GraphicsSettings();

	//Függvények
	void saveToFile(const std::string path);
	void loadFromFile(const std::string path);
};

#endif