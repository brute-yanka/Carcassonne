#ifndef GAME_H
#define GAME_H

#include "MainMenuState.h"

class Game
{
private:
	//Változók
	GraphicsSettings gfxSettings;
	StateData stateData;
	sf::RenderWindow *window;
	sf::Event sfEvent;

	sf::Clock dtClock;
	float dt;

	std::stack<State*> states;
	std::map<std::string, int> supportedKeys;
	float gridSize;

	//Inicializálás
	void initVariables();
	void initGraphicsSettings();
	void initWindow();
	void initKeys();
	void initStateData();
	void initStates();

public:
	//Constructors / Deconstructors
	Game();
	virtual ~Game();

	//Függvények
	void updateDt();
	void updateSFMLEvents();
	void update();
	void render();
	void run();
};

#endif