#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "GUI.h"
#include "PauseMenu.h"
#include "TileMap.h"
#include "TextSystem.h"

class State;
class Gui;
class PauseMenu;
class TileMap;

class GameState : public State
{
private:
	//Változók
	sf::SoundBuffer pbuffer;
	sf::SoundBuffer ebuffer;
	sf::Sound placementSound;
	sf::Sound endgameSound;
	sf::View view;
	float cameraSpeed;

	sf::Font font;
	PauseMenu* pmenu;
	PauseMenu* emenu;
	std::map<std::string, GUI::Button*> buttons;

	TileMap* tileMap;
	sf::Vector2u maxSize;
	sf::RectangleShape border;

	sf::IntRect textureRect;
	sf::RectangleShape selectorRect;

	sf::Transform transform;
	int rotate;

	std::map<std::pair<unsigned short, unsigned short>, unsigned short> occurrence;
	unsigned short randX;
	unsigned short randY;

	sf::Text playerNameText;
	std::string playerName;
	sf::Text countText;
	unsigned short points;
	unsigned short tiles;

	TXT::TextSystem* tts;
	sf::VideoMode& vm;

	//Inicializálás
	void initvariables();
	void initView();
	void initFonts();
	void initText();
	void initKeybinds();
	void initPauseMenu();
	void initButtons();
	void initTileMap();
	void initGui();
	void initSystems();

public:
	//Constructors / Deconstructors
	GameState(StateData* state_data, const std::string& player_name);
	virtual ~GameState();

	//Függvények
	void checkIsEnd();
	void updateInput(const float& dt);
	void updateGameInput(const float& dt);
	void updateButtons();
	void updateGui(const float& dt);
	void updatePauseMenuButtons();
	void update(const float& dt);
	void renderButtons(sf::RenderTarget& target);
	void renderGui(sf::RenderTarget& target);
	void render(sf::RenderTarget* target = nullptr);
};

#endif
