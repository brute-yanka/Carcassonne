#include "stdafx.h"
#include "TextSystem.h"

/*========= Text Tag ==========*/
//======Privát Függvények======
void TXT::TextSystem::initFonts(std::string font_file)
{
	if (!this->font.loadFromFile(font_file))
		throw("DEBUG::TextTagSystem::Font betoltese sikertelen");
}

void TXT::TextSystem::initTagTemplates()
{
	this->tagTemplates[DEFAULT_TAG] = new TextTag(this->font, "", 100.f, 100.f, 0.f, -1.f, sf::Color(255, 215, 0, 255), 50, 200.f, 300.f);
}

//======Constructor======
TXT::TextSystem::TextSystem(std::string font_file)
{
	this->initFonts(font_file);
	this->initTagTemplates();
}

//======Deconstructor======
TXT::TextSystem::~TextSystem()
{
	for (auto* tag : this->tags)
	{
		delete tag;
	}

	for (auto& tag : this->tagTemplates)
	{
		delete tag.second;
	}
}

//======Függvények======
void TXT::TextSystem::addTextTag(const unsigned tag_type, const float pos_x, const float pos_y, const unsigned font_size)
{
	this->tags.push_back(new TextTag(this->tagTemplates[tag_type], pos_x, pos_y, "+1 elem", font_size));
}

void TXT::TextSystem::update(const float& dt)
{
	for (size_t i = 0; i < this->tags.size(); ++i)
	{
		this->tags[i]->update(dt);
		if (this->tags[i]->isExpired())
		{
			delete this->tags[i];
			this->tags.erase(this->tags.begin() + i);
		}
	}
}

void TXT::TextSystem::render(sf::RenderTarget& target)
{
	for (auto& tag : this->tags)
	{
		tag->render(target);
	}
}

/*========= Text Input ==========*/
//======Privát Függvények======
void TXT::TextBox::inputLogic(int char_typed)
{
	if (char_typed != DELETE_KEY && char_typed != ENTER_KEY && char_typed != ESCAPE_KEY)
	{
		this->text << static_cast<char>(char_typed);
	}
	else if (char_typed == DELETE_KEY)
	{
		if (this->text.str().length() > 0)
		{
			deleteLastChar();
		}
	}
	this->textbox.setString(text.str() + "_");
}

void TXT::TextBox::deleteLastChar()
{
	std::string t = this->text.str();
	std::string newT = "";
	for (int i = 0; i < t.length() - 1; i++)
	{
		newT += t[i];
	}
	this->text.str("");
	this->text << newT;

	this->textbox.setString(this->text.str());
}

//======Constructor======
TXT::TextBox::TextBox(sf::Vector2f pos, unsigned short char_size, sf::Font& font, sf::Color color, bool selected, const unsigned limit)
	: isSelected(false)
{
	this->textbox.setPosition(pos);
	this->textbox.setFont(font);
	this->textbox.setCharacterSize(char_size);
	this->textbox.setFillColor(color);
	this->isSelected = selected;
	this->limit = limit - 1;
	if (selected)
	{
		textbox.setString("_");
	}
	else
	{
		textbox.setString("");
	}
}

//======Deconstructor======
TXT::TextBox::~TextBox()
{

}

//======Segéd Függvények======
const std::string TXT::TextBox::getText() const
{
	return this->text.str();
}

//======Függvények======
void TXT::TextBox::setSelected(bool selected)
{
	this->isSelected = selected;
	if (!selected)
	{
		std::string t = this->text.str();
		std::string newT = "";
		for (int i = 0; i < t.length(); i++)
		{
			newT += t[i];
		}
		this->textbox.setString(newT);
	}
}

void TXT::TextBox::typedOn(sf::Event* input)
{
	if (this->isSelected)
	{
		int charTyped = input->text.unicode;
		//ASCII Table check
		if ((charTyped > 64 && charTyped < 91) || (charTyped > 96 && charTyped < 123) || charTyped == 8)
		{
			if (this->text.str().length() <= this->limit)
			{
				inputLogic(charTyped);
			}
			else if (this->text.str().length() > limit && charTyped == DELETE_KEY)
			{
				deleteLastChar();
			}
		}
	}
}

void TXT::TextBox::render(sf::RenderTarget* target)
{
	target->draw(this->textbox);
}
