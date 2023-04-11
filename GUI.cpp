#include "stdafx.h"
#include "GUI.h"

/*========= RESZPONZIVIT�S ==========*/
const float GUI::p2pX(const float perc, const sf::VideoMode& vm)
{
	//Sz�zal�kos �rt�ket k�ppontokk� alak�t �t az x tengely aktu�lis felbont�s�hoz k�pest.
	return std::floor(static_cast<float>(vm.width) * (perc / 100.f));
}

const float GUI::p2pY(const float perc, const sf::VideoMode& vm)
{
	//A sz�zal�kos �rt�ket pixelekk� alak�tja az y tengely aktu�lis felbont�s�hoz k�pest.
	return std::floor(static_cast<float>(vm.height) * (perc / 100.f));
}

const unsigned GUI::calcCharSize(const sf::VideoMode& vm, const unsigned modifier)
{
	//Kisz�m�tja a sz�veg karakterm�ret�t az aktu�lis felbont�s �s konstans haszn�lat�val
	return static_cast<unsigned>((vm.width  + vm.height) / modifier);
}

/*========= BUTTON ==========*/
//Constructors / Deconstructors
GUI::Button::Button(float x, float y, float width, float height,
	sf::Font* font, std::string text, unsigned character_size,
	sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
	sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
	sf::Color outline_idle_color, sf::Color outline_hover_color, sf::Color outline_active_color,
	short unsigned id
)
{
	this->buttonState = BTN_IDLE;
	this->id = id;

	//Gomb be�ll�t�sok
	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setFillColor(idle_color);
	this->shape.setOutlineThickness(2.f);
	this->shape.setOutlineColor(outline_idle_color);
	//Sz�veg be�ll�t�sok
	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setFillColor(text_idle_color);
	this->text.setCharacterSize(character_size);
	//Sz�veg k�z�pre igaz�t�sa
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - (this->text.getGlobalBounds().width / 2.f),
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - (this->text.getGlobalBounds().height)
	);
	//Sz�veg sz�ne
	this->textIdleColor = text_idle_color;
	this->textHoverColor = text_hover_color;
	this->textActiveColor = text_active_color;
	//Doboz sz�ne
	this->idleColor = idle_color;
	this->hoverColor = hover_color;
	this->activeColor = active_color;
	//K�rvonal sz�ne
	this->outlineIdleColor = outline_idle_color;
	this->outlineHoverColor = outline_hover_color;
	this->outlineActiveColor = outline_active_color;
}

GUI::Button::~Button()
{

}

/*========= Seg�d F�ggv�nyek ==========*/
const bool GUI::Button::isPressed() const
{
	if (this->buttonState == BTN_ACTIVE) return true;
	return false;
}

const std::string GUI::Button::getText() const
{
	return this->text.getString();
}

const short unsigned& GUI::Button::getId() const
{
	return this->id;
}

//M�dos�t�k
void GUI::Button::setText(const std::string text)
{
	this->text.setString(text);
}

void GUI::Button::setId(const short unsigned id)
{
	this->id = id;
}

/*========= F�ggv�nyek ==========*/
void GUI::Button::update(const sf::Vector2i& mousePosWindow)
{
	//Alap �llapot
	this->buttonState = BTN_IDLE;
	//Eg�r felette �llapot
	if (this->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
	{
		this->buttonState = BTN_HOVER;
		//Megnyomott �llapot
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			this->buttonState = BTN_ACTIVE;
	}

	switch (this->buttonState)
	{
	case BTN_IDLE:
		this->shape.setFillColor(this->idleColor);
		this->text.setFillColor(this->textIdleColor);
		this->shape.setOutlineColor(this->outlineIdleColor);
		break;
	case BTN_HOVER:
		this->shape.setFillColor(this->hoverColor);
		this->text.setFillColor(this->textHoverColor);
		this->shape.setOutlineColor(this->outlineHoverColor);
		break;
	case BTN_ACTIVE:
		this->shape.setFillColor(this->activeColor);
		this->text.setFillColor(this->textActiveColor);
		this->shape.setOutlineColor(this->outlineActiveColor);
		break;
	//Kiv�tel - DEBUG
	default:
		this->shape.setFillColor(sf::Color::Red);
		this->text.setFillColor(sf::Color::Blue);
		this->shape.setOutlineColor(sf::Color::Green);
		break;
	}
}

void GUI::Button::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
	target.draw(this->text);
}

/*========= DROPDOWN LIST ==========*/
//Constructors / Deconstructors
GUI::DropDownList::DropDownList(float x, float y, float width, float height, sf::Font& font, unsigned character_size, std::string list[], unsigned nrOfElements, unsigned default_index)
	: font(font), showList(false), keytimeMax(1.f), keytime(0.f)
{
	this->activeElement = new GUI::Button(
		x, y, width, height,
		&this->font, list[default_index], character_size,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 255)
	);

	for (unsigned short i = 0; i < nrOfElements; ++i)
	{
		this->list.push_back(new GUI::Button(
			x, y + ((i+1) * height), width, height,
			&this->font, list[i], character_size,
			sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
			sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
			sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(20, 20, 20, 0), i
		));
	}
}

GUI::DropDownList::~DropDownList()
{
	delete this->activeElement;
	for (size_t i = 0; i < this->list.size(); ++i)
	{
		delete this->list[i];
	}
}

/*========= Seg�d F�ggv�nyek ==========*/
const bool GUI::DropDownList::getKeyTime()
{
	if (this->keytime >= this->keytimeMax)
	{
		this->keytime = 0.f;
		return true;
	}
	return false;
}

const unsigned short& GUI::DropDownList::getActiveElementId() const
{
	return this->activeElement->getId();
}

/*========= F�ggv�nyek ==========*/
void GUI::DropDownList::updateKeyTime(const float& dt)
{
	if (this->keytime < this->keytimeMax)
		this->keytime += 5.f * dt;
}

void GUI::DropDownList::update(const sf::Vector2i& mousePosWindow, const float& dt)
{
	this->updateKeyTime(dt);
	this->activeElement->update(mousePosWindow);

	//Leg�rd�l� men� megjelen�t�se & elrejt�se
	if (this->activeElement->isPressed() && this->getKeyTime())
	{
		if (!this->showList)
			this->showList = true;
		else
			this->showList = false;
	}
	//Lista friss�t�se
	if (this->showList)
	{
		for (auto& i : this->list)
		{
			i->update(mousePosWindow);

			if (i->isPressed() && this->getKeyTime())
			{
				this->showList = false;
				this->activeElement->setText(i->getText());
				this->activeElement->setId(i->getId());
			}
		}
	}
}

void GUI::DropDownList::render(sf::RenderTarget& target)
{
	this->activeElement->render(target);
	//Lista megjelen�t�se
	if (this->showList)
	{
		for (auto& i : this->list)
		{
			i->render(target);
		}
	}
}
