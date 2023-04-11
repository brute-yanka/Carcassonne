#include "stdafx.h"
#include "MainMenuState.h"

//======Inicializ�l�s======
void MainMenuState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/Kingthings_Calligraphica_2.ttf"))
		throw("DEBUG::MainMenuState::Font betoltese sikertelen");
}

//El�re meghat�rozott billenty�k bet�lt�se
void MainMenuState::initKeybinds()
{
	//Reading the keybinds
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
	ifs.close();
}

//Megjelen� tartalom deklar�l�sa
void MainMenuState::initGui()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;
	//H�tt�r be�ll�t�sa
	this->background.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
	if (!this->backgroundTexture.loadFromFile("Resources/Images/Backgrounds/bg1.jpg"))
		throw("DEBUG::MainMenuState::Hatterkep betoltese sikertelen");
	this->background.setTexture(&this->backgroundTexture);
	//Gomb h�tt�r be�ll�t�sa
	this->btnBackground.setSize(sf::Vector2f(static_cast<float>(vm.width / 5), static_cast<float>(vm.height)));
	this->btnBackground.setPosition(GUI::p2pX(11.5f, vm), 0.f);
	this->btnBackground.setFillColor(sf::Color(10, 10, 10, 220));
	//Gombok l�trehoz�sa
	this->buttons["GAME_STATE"] = new GUI::Button(
		GUI::p2pX(16.f, vm), GUI::p2pY(30.f, vm), GUI::p2pX(10.f, vm), GUI::p2pY(6.f, vm),
		&this->font, "�j j�t�k", GUI::calcCharSize(vm),
		sf::Color(255, 255, 255, 255), sf::Color(140, 190, 30, 255), sf::Color(180, 190, 180, 255),
		sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
	);

	this->buttons["SETTINGS_STATE"] = new GUI::Button(
		GUI::p2pX(16.f, vm), GUI::p2pY(40.f, vm), GUI::p2pX(10.f, vm), GUI::p2pY(6.f, vm),
		&this->font, "Be�ll�t�sok", GUI::calcCharSize(vm),
		sf::Color(255, 255, 255, 255), sf::Color(140, 190, 30, 255), sf::Color(180, 190, 180, 255),
		sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
	);

	this->buttons["RANG_STATE"] = new GUI::Button(
		GUI::p2pX(16.f, vm), GUI::p2pY(50.f, vm), GUI::p2pX(10.f, vm), GUI::p2pY(6.f, vm),
		&this->font, "Ranglista", GUI::calcCharSize(vm),
		sf::Color(255, 255, 255, 255), sf::Color(140, 190, 30, 255), sf::Color(180, 190, 180, 255),
		sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
	);

	this->buttons["EXIT_STATE"] = new GUI::Button(
		GUI::p2pX(16.f, vm), GUI::p2pY(65.f, vm), GUI::p2pX(10.f, vm), GUI::p2pY(6.f, vm),
		&this->font, "Kil�p�s", GUI::calcCharSize(vm),
		sf::Color(255, 255, 255, 255), sf::Color(140, 190, 30, 255), sf::Color(180, 190, 180, 255),
		sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
	);
	//Sz�veges tartalom l�trehoz�sa
	this->text.setPosition(sf::Vector2f(GUI::p2pX(16.f, vm), GUI::p2pY(10.f, vm)));
	this->text.setCharacterSize(GUI::calcCharSize(vm));
	this->text.setFont(this->font);
	this->text.setFillColor(sf::Color::White);
	this->text.setString("J�t�kos n�v");
	this->textbox = new TXT::TextBox(sf::Vector2f(GUI::p2pX(15.f, vm), GUI::p2pY(15.f, vm)), GUI::calcCharSize(vm), this->font, sf::Color::White, true, 10);
}

//Megjelen� tartalom friss�t�se
void MainMenuState::resetGui()
{
	auto itr = this->buttons.begin();
	for (; itr != this->buttons.end(); ++itr)
	{
		delete itr->second;
	}
	this->buttons.clear();
	delete this->textbox;
	this->initGui();
}

//======Constructor======
MainMenuState::MainMenuState(StateData* state_data)
	: State(state_data)
{
	this->initFonts();
	this->initKeybinds();
	this->initGui();
}

//======Deconstructor======
MainMenuState::~MainMenuState()
{
	auto itr = this->buttons.begin();
	for (; itr != this->buttons.end(); ++itr)
	{
		delete itr->second;
	}
	delete this->textbox;
}

//======F�ggv�nyek======
void MainMenuState::updateInput(const float& dt)
{
	if (this->sfEvent)
	{
		if (this->sfEvent->type == sf::Event::TextEntered)
		{
			this->textbox->typedOn(this->sfEvent);
			this->sfEvent = nullptr;
		}
	}
	//Grafikai v�ltoztat�sok alkalmaz�sa
	if (this->stateData->gfxSettings->changed)
	{
		this->resetGui();
		this->stateData->gfxSettings->changed = false;
	}
}

void MainMenuState::updateButtons()
{
	//J�t�kosn�v megad�sa
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
	{
		this->textbox->setSelected(true);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		this->textbox->setSelected(false);
	}
	//Gombok friss�t�se
	for (auto &itr : this->buttons)
	{
		itr.second->update(this->mousePosWindow);
	}
	//J�t�k men�
	if (this->buttons["GAME_STATE"]->isPressed())
	{
		if (this->textbox->getText().length() > 0)
		{
			this->text.setFillColor(sf::Color::White);
			this->text.setStyle(sf::Text::Regular);
			this->states->push(new GameState(this->stateData, this->textbox->getText()));
		}
		else
		{
			this->text.setFillColor(sf::Color::Red);
			this->text.setStyle(sf::Text::Bold);
		}
	}
	//Be�ll�t�s men�
	if (this->buttons["SETTINGS_STATE"]->isPressed())
	{
		this->states->push(new SettingsState(this->stateData));
	}
	//Ranglista men�
	if (this->buttons["RANG_STATE"]->isPressed())
	{
		this->states->push(new RangState(this->stateData));
	}
	//Kil�p�s
	if (this->buttons["EXIT_STATE"]->isPressed())
	{
		this->endState();
	}
}

void MainMenuState::update(const float& dt)
{
	this->updateInput(dt);
	this->updateMousePositions();
	this->updateButtons();
}

//Megjelen�t�s
void MainMenuState::renderButtons(sf::RenderTarget& target)
{
	for (auto& itr : this->buttons)
	{
		itr.second->render(target);
	}
}

void MainMenuState::render(sf::RenderTarget* target)
{
	if (!target) target = this->window;
	target->draw(this->background);
	target->draw(this->btnBackground);
	this->renderButtons(*target);
	this->textbox->render(target);
	target->draw(this->text);
}