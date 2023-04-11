#include "stdafx.h"
#include "PauseMenu.h"

//======Constructor======
PauseMenu::PauseMenu(sf::VideoMode& vm, sf::Font& font, std::string text1, std::string text2)
	:font(font)
{
	//Háttér beállítása
	this->background.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
	this->background.setFillColor(sf::Color(20, 20, 20, 100));
	//Konténer beállítása
	this->container.setSize(sf::Vector2f(static_cast<float>(vm.width) / 2.f, static_cast<float>(vm.height) - GUI::p2pY(9.f, vm)));
	this->container.setFillColor(sf::Color(20, 20, 20, 200));
	this->container.setPosition(static_cast<float>(vm.width) / 2.f - this->container.getSize().x / 2.f, GUI::p2pY(2.f, vm));
	//Szöveg1 deklarálása
	this->menuText1.setFont(font);
	this->menuText1.setFillColor(sf::Color(255, 255, 255, 255));
	this->menuText1.setCharacterSize(GUI::calcCharSize(vm));
	this->menuText1.setString(text1);
	this->menuText1.setPosition(
		this->container.getPosition().x + this->container.getSize().x / 2.f - this->menuText1.getGlobalBounds().width / 2.f,
		this->container.getPosition().y + GUI::p2pY(10.f, vm)
	);
	//Szöveg2 deklarálása
	this->menuText2.setFont(font);
	this->menuText2.setFillColor(sf::Color(255, 255, 255, 200));
	this->menuText2.setCharacterSize(GUI::calcCharSize(vm));
	this->menuText2.setString(text2);
	this->menuText2.setPosition(
		this->container.getPosition().x + this->container.getSize().x / 2.f - this->menuText2.getGlobalBounds().width / 2.f,
		this->container.getPosition().y + GUI::p2pY(20.f, vm)
	);
}

//======Deconstructor======
PauseMenu::~PauseMenu()
{
	auto itr = this->buttons.begin();
	for (; itr != this->buttons.end(); ++itr)
	{
		delete itr->second;
	}
}

//======Segéd Függvények======
std::map<std::string, GUI::Button*>& PauseMenu::getButtons()
{
	return this->buttons;
}

const bool PauseMenu::isButtonPressed(const std::string key)
{
	return this->buttons[key]->isPressed();
}

//======Függvények======
void PauseMenu::addButton(const std::string key, const float y, const float width, const float height, const unsigned char_size, const std::string text)
{
	float x = this->container.getPosition().x + this->container.getSize().x /2.f - width / 2.f;
	this->buttons[key] = new GUI::Button(
		x, y, width, height,
		&this->font, text, char_size,
		sf::Color(255, 255, 255, 255), sf::Color(140, 190, 30, 255), sf::Color(180, 190, 180, 255),
		sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
	);
}

//Megjelenítés
void PauseMenu::update(const sf::Vector2i& mousePosWindow)
{
	for (auto& i : this->buttons)
	{
		i.second->update(mousePosWindow);
	}
}

void PauseMenu::render(sf::RenderTarget& target)
{
	target.draw(this->background);
	target.draw(this->container);
	target.draw(this->tile);

	for (auto& i : this->buttons)
	{
		i.second->render(target);
	}
	target.draw(this->menuText1);
	target.draw(this->menuText2);
}