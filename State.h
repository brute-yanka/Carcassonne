#ifndef STATE_H
#define STATE_H

#include "GraphicsSettings.h"

class Player;
class GraphicsSettings;
class State;

class StateData
{
public:
	//Változók
	float gridSize;
	sf::RenderWindow* window;
	GraphicsSettings* gfxSettings;
	std::map<std::string, int>* supportedKeys;
	std::stack<State*>* states;
};

class State
{
private:


protected:
	//Változók
	StateData* stateData;
	std::stack<State*>* states;
	sf::RenderWindow* window;
	std::map<std::string, int>* supportedKeys;
	std::map<std::string, int> keybinds;
	bool quit;
	bool paused;
	bool end;
	float keyTime;
	float keyTimeMax;
	float gridSize;

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	sf::Vector2u mousePosGrid;

	sf::Event* sfEvent;
	std::map<std::string, sf::Texture> textures;

	//Inicializálás
	virtual void initKeybinds() = 0;

public:
	//Constructors / Deconstructors
	State(StateData* state_data);
	virtual ~State();

	//Segéd függvények
	const bool& getQuit() const;
	const bool getKeytime();

	virtual void setEvent(sf::Event* sfEvent);

	//Függvények
	void endState();
	void pauseState();
	void unpauseState();

	virtual void updateMousePositions(sf::View* view = nullptr);
	virtual void updateKeytime(const float& dt);
	virtual void updateInput(const float& dt) = 0;
	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;
};

#endif