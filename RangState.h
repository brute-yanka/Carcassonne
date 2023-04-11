#ifndef RANGSTATE_H
#define RANGSTATE_H

#include "State.h"
#include "GUI.h"

struct stats {
	std::string playerName;
	unsigned points;
};

class RangState : public State
{
private:
	//Változók
	sf::Texture backgroundTexture;
	sf::RectangleShape background;
	sf::Font font;

	sf::RectangleShape containerBackground;
	std::map<std::string, GUI::Button*> buttons;
	sf::Text rangText;
	sf::Text playerNameText;
	sf::Text pointsText;
	std::vector<stats> data;
	stats dataSample;

	//Inicializálás
	void initBackground();
	void initFonts();
	void initKeybinds();
	void initGui();
	void initPlayers();

public:
	//Constructors / Deconstructors
	RangState(StateData* state_data);
	virtual ~RangState();

	//Függvények
	void updateInput(const float& dt);
	void updateGui(const float& dt);
	void update(const float& dt);
	void renderGui(sf::RenderTarget& target);
	void render(sf::RenderTarget* target = nullptr);

};

#endif