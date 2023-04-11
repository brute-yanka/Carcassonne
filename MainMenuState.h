#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "GameState.h"
#include "SettingsState.h"
#include "RangState.h"

class MainMenuState : public State
{
private:
	//V�ltoz�k
	sf::Texture backgroundTexture;
	sf::RectangleShape background;
	sf::Font font;

	sf::Text cursorText;
	sf::RectangleShape btnBackground;
	std::map<std::string, GUI::Button*> buttons;
	sf::Event* event;

	sf::Text text;
	TXT::TextBox* textbox;

	//Inicializ�l�s
	void initFonts();
	void initKeybinds();
	void initGui();
	void resetGui();

public:
	//Constructors / Deconstructors
	MainMenuState(StateData* state_data);
	virtual ~MainMenuState();

	//F�ggv�nyek
	void updateInput(const float& dt);
	void updateButtons();
	void update(const float& dt);
	void renderButtons(sf::RenderTarget& target);
	void render(sf::RenderTarget* target = nullptr);
};

#endif