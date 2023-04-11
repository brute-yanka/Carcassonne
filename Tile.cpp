//Precomplied header csatolása
#include "stdafx.h"
#include "Tile.h"

//======Constructor======
Tile::Tile(unsigned grid_x, unsigned grid_y, float gridSizeF, const sf::Texture& texture, const sf::IntRect& texture_rect, const int& rotate, const char& top, const char& right, const char& bottom, const char& left, const bool& isEndRoad, const bool& isCloister)
{
	this->shape.setSize(sf::Vector2f(gridSizeF, gridSizeF));
	this->shape.setPosition(static_cast<float>(grid_x) * gridSizeF, static_cast<float>(grid_y) * gridSizeF);
	this->shape.setTexture(&texture);
	this->shape.setTextureRect(texture_rect);
	this->shape.setOrigin(gridSizeF / 2.f, gridSizeF / 2.f);
	this->transform.translate(gridSizeF / 2.f, gridSizeF / 2.f);
	this->shape.rotate((float)rotate);

	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->left = left;
	this->isEndRoad = isEndRoad;
	this->isCloister = isCloister;
	this->isVisitedTop = false;
	this->isVisitedRight = false;
	this->isVisitedBottom = false;
	this->isVisitedLeft = false;
}

//======Deconstructor======
Tile::~Tile()
{

}

//======Segéd Függvények======
const std::string Tile::getAsString() const
{
	std::stringstream ss;
	ss << this->shape.getTextureRect().left << " " << this->shape.getTextureRect().top << " " << this->shape.getRotation() << " " << this->top << " " << this->right << " " << this->bottom << " " << this->left << " " << this->isEndRoad << " " << this->isCloister;
	return ss.str();
}

const char Tile::getType(TileDirection direction) const
{
	switch (direction)
	{
	case TileDirection::North:
		return this->top;
	case TileDirection::East:
		return this->right;
	case TileDirection::South:
		return this->bottom;
	case TileDirection::West:
		return this->left;
	//Nem történik meg, csak DEBUG
	default:
		return this->top;
		break;
	}
}

const bool& Tile::isVisited(TileDirection direction) const
{
	switch (direction)
	{
	case TileDirection::North:
		return this->isVisitedTop;
	case TileDirection::East:
		return this->isVisitedRight;
	case TileDirection::South:
		return this->isVisitedBottom;
	case TileDirection::West:
		return this->isVisitedLeft;
	//Nem történik meg, csak DEBUG
	default:
		return this->isVisitedTop;
		break;
	}
}

void Tile::setVisited(TileDirection direction)
{
	switch (direction)
	{
	case TileDirection::North:
		this->isVisitedTop = true;
		break;
	case TileDirection::East:
		this->isVisitedRight = true;
		break;
	case TileDirection::South:
		this->isVisitedBottom = true;
		break;
	case TileDirection::West:
		this->isVisitedLeft = true;
		break;
	//Nem történik meg, csak DEBUG
	default:
		this->isVisitedTop = true;
		break;
	}
}

const bool& Tile::getisEndRoad() const
{
	return this->isEndRoad;
}

const bool& Tile::getIsCloister() const
{
	return this->isCloister;
}

//======Függvények======
void Tile::update()
{

}

void Tile::render(sf::RenderTarget& target)
{
	target.draw(this->shape, this->transform);
}