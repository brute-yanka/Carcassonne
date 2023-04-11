#ifndef SETTINGSSTATE_H
#define SETTINGSSTATE_H

#include "State.h"
#include "GUI.h"
#include "MainMenuState.h"

class SettingsState : public State
{
private:
	//Változók
	sf::Texture backgroundTexture;
	sf::RectangleShape background;
	sf::Font font;

	std::map<std::string, GUI::Button*> buttons;
	std::map<std::string, GUI::DropDownList*> dropDownLists;
	sf::Text optionsText;
	std::vector<sf::VideoMode> modes;
	sf::RectangleShape containerBackground;

	unsigned int activeRes;
	unsigned int activeFull;

	//Inicializálás
	void initvariables();
	void initFonts();
	void initKeybinds();
	void initGui();
	void resetGui();

public:
	//Constructors / Deconstructors
	SettingsState(StateData* state_data);
	virtual ~SettingsState();

	//Függvények
	void updateInput(const float& dt);
	void updateGui(const float& dt);
	void update(const float& dt);
	void renderGui(sf::RenderTarget& target);
	void render(sf::RenderTarget* target = nullptr);

};

#endif