//Precomplied header csatol�sa
#include "stdafx.h"
#include "SettingsState.h"

//======Inicializ�l�s======
//Alapvet� v�ltoz�k deklar�l�sa
void SettingsState::initvariables()
{
	this->modes = sf::VideoMode::getFullscreenModes();
	this->activeRes = 0;
	this->activeFull = 0;
}

//Bet�t�pus deklar�l�sa
void SettingsState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/Kingthings_Calligraphica_2.ttf"))
		throw("DEBUG::SettingsState::Font betoltese sikertelen");
}

//El�re meghat�rozott billenty�k bet�lt�se
void SettingsState::initKeybinds()
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

//Megjelen� tartalom deklar�l�sa
void SettingsState::initGui()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;
	//H�tt�r deklar�l�sa
	this->background.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
	if (!this->backgroundTexture.loadFromFile("Resources/Images/Backgrounds/bg2.jpg"))
		throw("DEBUG::SettingsState::Hatterkep betoltese sikertelen");
	this->background.setTexture(&this->backgroundTexture);
	//Gombok deklar�l�sa
	this->buttons["APPLY"] = new GUI::Button(
		GUI::p2pX(35.f, vm), GUI::p2pY(80.f, vm), GUI::p2pX(10.f, vm), GUI::p2pY(6.f, vm),
		&this->font, "Alkalmaz", GUI::calcCharSize(vm),
		sf::Color(255, 255, 255, 255), sf::Color(140, 190, 30, 255), sf::Color(180, 190, 180, 255),
		sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
	);
	this->buttons["BACK"] = new GUI::Button(
		GUI::p2pX(55.f, vm), GUI::p2pY(80.f, vm), GUI::p2pX(10.f, vm), GUI::p2pY(6.f, vm),
		&this->font, "Vissza", GUI::calcCharSize(vm),
		sf::Color(255, 255, 255, 255), sf::Color(140, 190, 30, 255), sf::Color(180, 190, 180, 255),
		sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
	);
	//Kont�ner h�tt�r deklar�l�sa
	this->containerBackground.setSize(sf::Vector2f(static_cast<float>(vm.width / 2), static_cast<float>(vm.height)));
	this->containerBackground.setPosition(GUI::p2pX(25.f, vm), 0.f);
	this->containerBackground.setFillColor(sf::Color(10, 10, 10, 220));
	//Leg�rd�l� lista deklar�l�sa
	std::string bmodes[] = {"Nem", "Igen"};
	std::vector<std::string> modes_str;
	for (auto& i : this->modes) modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.height));
	//Lementett grafikai be�ll�t�sok bet�lt�se akt�v elemk�nt
	if (this->stateData->gfxSettings->fullscreen) this->activeFull = 1;
	else this->activeFull = 0;
	auto it = std::find(modes_str.begin(), modes_str.end(), std::to_string(this->stateData->gfxSettings->resolution.width) + "x" + std::to_string(this->stateData->gfxSettings->resolution.height));
	this->activeRes = (unsigned int)std::distance(modes_str.begin(), it);
	//---
	this->dropDownLists["RESOLUTION"] = new GUI::DropDownList(GUI::p2pX(35.f, vm), GUI::p2pY(15.f, vm), GUI::p2pX(10.f, vm), GUI::p2pY(6.f, vm), this->font, GUI::calcCharSize(vm), modes_str.data(), 8, this->activeRes);
	this->dropDownLists["FULLSCREEN"] = new GUI::DropDownList(GUI::p2pX(50.f, vm), GUI::p2pY(15.f, vm), GUI::p2pX(10.f, vm), GUI::p2pY(6.f, vm), this->font, GUI::calcCharSize(vm), bmodes, 2, this->activeFull);
	//Megjelen� sz�veg deklar�l�sa
	this->optionsText.setFont(this->font);
	this->optionsText.setPosition(sf::Vector2f(GUI::p2pX(35.f, vm), GUI::p2pY(10.f, vm)));
	this->optionsText.setCharacterSize(GUI::calcCharSize(vm, 70));
	this->optionsText.setFillColor(sf::Color(255, 255, 255, 200));
	this->optionsText.setString("\tFelbont�s\t\t\t\tTeljes k�perny�");
}

void SettingsState::resetGui()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
	this->buttons.clear();

	auto it2 = this->dropDownLists.begin();
	for (it2 = this->dropDownLists.begin(); it2 != this->dropDownLists.end(); ++it2)
	{
		delete it2->second;
	}
	this->dropDownLists.clear();

	this->initGui();
}

//======Constructor======
SettingsState::SettingsState(StateData* state_data)
	: State(state_data)
{
	this->initvariables();
	this->initFonts();
	this->initKeybinds();
	this->initGui();
}

//======Deconstructor======
SettingsState::~SettingsState()
{
	auto itr = this->buttons.begin();
	for (; itr != this->buttons.end(); ++itr)
	{
		delete itr->second;
	}
	auto itr2 = this->dropDownLists.begin();
	for (; itr2 != this->dropDownLists.end(); ++itr2)
	{
		delete itr2->second;
	}
}

//======F�ggv�nyek======
void SettingsState::updateInput(const float& dt)
{

}

//Megjelen�tett tartalom friss�t�se
void SettingsState::updateGui(const float& dt)
{
	//Minden gomb friss�t�se
	for (auto& itr : this->buttons)
	{
		itr.second->update(this->mousePosWindow);
	}
	//Kil�p�s
	if (this->buttons["BACK"]->isPressed() || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		this->endState();
	//Kiv�lasztott be�ll�t�sok alkalmaz�sa
	if (this->buttons["APPLY"]->isPressed())
	{
		this->stateData->gfxSettings->resolution = this->modes[this->dropDownLists["RESOLUTION"]->getActiveElementId()];
		this->stateData->gfxSettings->fullscreen = this->dropDownLists["FULLSCREEN"]->getActiveElementId();
		//---
		if (this->stateData->gfxSettings->fullscreen)
			this->window->create(this->stateData->gfxSettings->resolution, this->stateData->gfxSettings->title, sf::Style::Fullscreen, this->stateData->gfxSettings->contextSettings);
		else
			this->window->create(this->stateData->gfxSettings->resolution, this->stateData->gfxSettings->title, sf::Style::Close | sf::Style::Titlebar, this->stateData->gfxSettings->contextSettings);
		//Ikon be�ll�t�sa
		this->window->setIcon(this->stateData->gfxSettings->icon.getSize().x, this->stateData->gfxSettings->icon.getSize().y, this->stateData->gfxSettings->icon.getPixelsPtr());
		//Ablak friss�t�se -> v�ltoztat�sok alkalmaz�sa
		this->resetGui();
		//V�ltoztat�sok elment�se
		this->stateData->gfxSettings->saveToFile("Config/graphics.ini");
		this->stateData->gfxSettings->changed = true;
	}
	//Leg�rd�l� lista friss�t�se
	for (auto& itr2 : this->dropDownLists)
	{
		itr2.second->update(this->mousePosWindow, dt);
	}
}

//�rt�kek friss�t�se
void SettingsState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateInput(dt);
	this->updateGui(dt);
}

//Tartalom megjelen�t�se
void SettingsState::renderGui(sf::RenderTarget& target)
{
	//Gombok megjelen�t�se
	for (auto& itr : this->buttons)
	{
		itr.second->render(target);
	}
	//Leg�rd�l� lista megjelen�t�se
	for (auto& itr : this->dropDownLists)
	{
		itr.second->render(target);
	}
}

void SettingsState::render(sf::RenderTarget* target)
{
	if (!target) target = this->window;
	target->draw(this->background);
	target->draw(this->containerBackground);
	this->renderGui(*target);
	target->draw(optionsText);
}