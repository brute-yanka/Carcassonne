//Precomplied header csatolása
#include "stdafx.h"
#include "RangState.h"

//======Inicializálás======
//Háttér deklarálása
void RangState::initBackground()
{
	this->background.setSize(sf::Vector2f(static_cast<float>(this->window->getSize().x), static_cast<float>(this->window->getSize().y)));
	if (!this->backgroundTexture.loadFromFile("Resources/Images/Backgrounds/bg3.jpg"))
		throw("DEBUG::SettingsState::Hatterkep betoltese sikertelen");
	this->background.setTexture(&this->backgroundTexture);
}

//Betûtípus deklarálása
void RangState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/Kingthings_Calligraphica_2.ttf"))
		throw("DEBUG::SettingsState::Font betoltese sikertelen");
}

//Elõre meghatározott billentyûk betöltése
void RangState::initKeybinds()
{
	std::ifstream ifs("Config/mainmenustate_keybinds.ini");
	if (ifs.is_open())
	{
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2)
		{
			this->keybinds[key] = this->supportedKeys->at(key2);
		}
	}
	else throw("DEBUG::SettingsState.cpp::Tamogatott billentyuk betoltese sikertelen");
	ifs.close();
}

//Megjelenõ tartalom deklarálása
void RangState::initGui()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;
	//Gomb deklarálása
	this->buttons["BACK"] = new GUI::Button(
		GUI::p2pX(45.f, vm), GUI::p2pY(80.f, vm), GUI::p2pX(10.f, vm), GUI::p2pY(6.f, vm),
		&this->font, "Vissza", GUI::calcCharSize(vm),
		sf::Color(255, 255, 255, 255), sf::Color(140, 190, 30, 255), sf::Color(180, 190, 180, 255),
		sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
	);
	//Háttér deklarálása
	this->containerBackground.setSize(sf::Vector2f(static_cast<float>(vm.width / 2), static_cast<float>(vm.height)));
	this->containerBackground.setPosition(GUI::p2pX(25.f, vm), 0.f);
	this->containerBackground.setFillColor(sf::Color(10, 10, 10, 220));
	//Szöveg deklarálása
	this->rangText.setFont(this->font);
	this->rangText.setPosition(sf::Vector2f(GUI::p2pX(45.f, vm), GUI::p2pY(9.f, vm)));
	this->rangText.setCharacterSize(GUI::calcCharSize(vm));
	this->rangText.setFillColor(sf::Color(255, 255, 255, 200));
	this->rangText.setString("Ranglista");
	//---
	this->playerNameText.setPosition(sf::Vector2f(GUI::p2pX(40.f, vm), GUI::p2pY(18.f, vm)));
	this->playerNameText.setCharacterSize(GUI::calcCharSize(vm, 80));
	this->playerNameText.setFont(this->font);
	this->playerNameText.setFillColor(sf::Color::White);
	//---
	this->pointsText.setPosition(sf::Vector2f(GUI::p2pX(55.f, vm), GUI::p2pY(18.f, vm)));
	this->pointsText.setCharacterSize(GUI::calcCharSize(vm, 80));
	this->pointsText.setFont(this->font);
	this->pointsText.setFillColor(sf::Color::White);
}

//Pont összehasonlító függvény
bool comparePoints(const stats& a, const stats& b)
{
	return a.points > b.points;
}

//Ranglista betöltése
void RangState::initPlayers()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;
	std::ifstream ifs("Maps/Ranglista.txt");
	if (ifs.is_open())
	{
		while (!ifs.eof()) {
			ifs >> this->dataSample.playerName >> this->dataSample.points;
			this->data.push_back(this->dataSample);
		}
	}
	else std::cout << "DEBUG::RangState::Fajl kiiras sikertelen" << "\n";
	ifs.close();
	std::sort(this->data.begin(), this->data.end()-1, comparePoints);
	std::stringstream spn, spo;
	spn << "Játékos név\n"; spo << "Pontok\n";
	for (size_t i = 0; i < std::min((unsigned)this->data.size()-1, (unsigned)10); i++)
	{
		spn << this->data[i].playerName << "\n";
		spo << this->data[i].points << "\n";
	}
	this->playerNameText.setString(spn.str());
	this->pointsText.setString(spo.str());
}

//======Constructor======
RangState::RangState(StateData* state_data)
	: State(state_data)
{
	this->initBackground();
	this->initFonts();
	this->initKeybinds();
	this->initGui();
	this->initPlayers();
}

//======Deconstructor======
RangState::~RangState()
{
	//Button list
	auto itr = this->buttons.begin();
	for (; itr != this->buttons.end(); ++itr)
	{
		delete itr->second;
	}
}

//======Függvények======
void RangState::updateInput(const float& dt)
{

}

void RangState::updateGui(const float& dt)
{
	//Gombok frissítése
	for (auto& itr : this->buttons)
	{
		itr.second->update(this->mousePosWindow);
	}

	//Vissza
	if (this->buttons["BACK"]->isPressed() || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		this->endState();
	}
}

void RangState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateInput(dt);
	this->updateGui(dt);
}

void RangState::renderGui(sf::RenderTarget& target)
{
	//Gombok megjelenítése
	for (auto& itr : this->buttons)
	{
		itr.second->render(target);
	}
	target.draw(this->playerNameText);
	target.draw(this->pointsText);
}

void RangState::render(sf::RenderTarget* target)
{
	if (!target) target = this->window;
	target->draw(this->background);
	target->draw(this->containerBackground);
	this->renderGui(*target);
	target->draw(rangText);
}