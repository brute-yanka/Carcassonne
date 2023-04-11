#include "stdafx.h"
#include "Game.h"

//======Inicializ�l�s======
void Game::initVariables()
{
	this->window = nullptr;
	this->dt = 0.f;
	//Mez�k nagys�ga
	this->gridSize = 100.f;
}

//Lementett grafikus be�ll�t�sok bet�lt�se
void Game::initGraphicsSettings()
{
	this->gfxSettings.loadFromFile("Config/graphics.ini");
}

void Game::initWindow()
{
	//Ablak be�ll�t�sa a lementett v�ltoz�knak megfelel�en
	if(this->gfxSettings.fullscreen)
		this->window = new sf::RenderWindow(this->gfxSettings.resolution, this->gfxSettings.title, sf::Style::Fullscreen, this->gfxSettings.contextSettings);
	else 
		this->window = new sf::RenderWindow(this->gfxSettings.resolution, this->gfxSettings.title, sf::Style::Titlebar | sf::Style::Close, this->gfxSettings.contextSettings);
	
	this->window->setFramerateLimit(this->gfxSettings.framRateLimit);
	this->window->setVerticalSyncEnabled(this->gfxSettings.vsync);
	//Ikon be�ll�t�sa
	this->window->setIcon(this->gfxSettings.icon.getSize().x, this->gfxSettings.icon.getSize().y, this->gfxSettings.icon.getPixelsPtr());
}

//El�re meghat�rozott billenty�k bet�lt�se
void Game::initKeys()
{
	std::ifstream ifs("Config/supported_keys.ini");
	if (ifs.is_open())
	{
		std::string key = "";
		int key_value = 0;

		while (ifs >> key >> key_value)
		{
			this->supportedKeys[key] = key_value;
		}
	}
	else throw("DEBUG::Game.cpp::Tamogatott billentyuk betoltese sikertelen");
	ifs.close();
}

void Game::initStateData()
{
	this->stateData.window = this->window;
	this->stateData.gfxSettings = &this->gfxSettings;
	this->stateData.supportedKeys = &this->supportedKeys;
	this->stateData.states = &this->states;
	this->stateData.gridSize = this->gridSize;
}

void Game::initStates()
{
	this->states.push(new MainMenuState(&this->stateData));
}

//======Constructor======
Game::Game()
{
	this->initVariables();
	this->initGraphicsSettings();
	this->initWindow();
	this->initKeys();
	this->initStateData();
	this->initStates();
}

//======Deconstructor======
Game::~Game()
{
	delete this->window;
	while (!this->states.empty())
	{
		delete this->states.top();
		this->states.pop();
	}
}

//======F�ggv�nyek======
void Game::updateDt()
{
	//Delta time friss�t�se
	this->dt = this->dtClock.restart().asSeconds();
}

void Game::updateSFMLEvents()
{
	while (this->window->pollEvent(this->sfEvent))
	{
		if (this->sfEvent.type == sf::Event::Closed)
		{
			this->window->close();
		}
		else if (this->sfEvent.type == sf::Event::TextEntered)
		{
			this->states.top()->setEvent(&this->sfEvent);
		}
	}
}

void Game::update()
{
	this->updateSFMLEvents();
	//Akt�v �llapot friss�t�se
	if (!this->states.empty())
	{
		this->states.top()->update(this->dt);
		//Nem sz�ks�ges �llapot t�rl�se
		if (this->states.top()->getQuit())
		{
			this->states.top()->endState();
			delete this->states.top();
			this->states.pop();
		}
	}
	//App bez�r�sa
	else
		this->window->close();
}

void Game::render()
{
	this->window->clear();
	//Az �llapot megjelen�t�se
	if (!this->states.empty())
		this->states.top()->render();
	this->window->display();
}

void Game::run()
{
	while (this->window->isOpen())
	{
		this->updateDt();
		this->update();
		this->render();
	}
}
