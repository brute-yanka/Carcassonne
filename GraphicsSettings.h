#ifndef GRAPHICSSETTINGS_H
#define GRAPHICSSETTINGS_H

class GraphicsSettings
{
public:
	//V�ltoz�k
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

	//F�ggv�nyek
	void saveToFile(const std::string path);
	void loadFromFile(const std::string path);
};

#endif