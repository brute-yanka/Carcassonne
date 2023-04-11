#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "GUI.h"

class PauseMenu
{
private:
	//Változók
	sf::RectangleShape tile;

	sf::Font& font;
	sf::Text menuText1;
	sf::Text menuText2;
	sf::RectangleShape background;
	sf::RectangleShape container;

	std::map<std::string, GUI::Button*> buttons;

public:
	//Constructors / Deconstructors
	PauseMenu(sf::VideoMode& vm, sf::Font& font, std::string text1, std::string text2);
	virtual ~PauseMenu();

	//Segéd Függvények
	std::map<std::string, GUI::Button*>& getButtons();

	//Függvények
	const bool isButtonPressed(const std::string key);
	void addButton(const std::string key, const float y, const float width, const float height, const unsigned charSize, const std::string text);
	void update(const sf::Vector2i& mousePosWindow);
	void render(sf::RenderTarget& target);
};

#endif