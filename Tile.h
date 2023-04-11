#ifndef TILE_H
#define TILE_H

//Irányok
enum TileDirection { North, East, South, West };

class Tile
{
protected:
	//Változók
	sf::RectangleShape shape;
	sf::Transform transform;

	char top;
	bool isVisitedTop;
	char right;
	bool isVisitedRight;
	char bottom;
	bool isVisitedBottom;
	char left;
	bool isVisitedLeft;
	bool isEndRoad;
	bool isCloister;

public:
	//Constructors / Deconstructors
	Tile(unsigned grid_x, unsigned grid_y, float gridSizeF, const sf::Texture& texture, const sf::IntRect& texture_rect, const int& rotate, const char& top, const char& right, const char& bottom, const char& left, const bool& isEndRoad, const bool& isCloister);
	virtual ~Tile();

	//Segéd függvények
	const std::string getAsString() const;
	const char getType(TileDirection direction) const;
	const bool& isVisited(TileDirection direction) const;
	void setVisited(TileDirection direction);
	const bool& getisEndRoad() const;
	const bool& getIsCloister() const;

	//Függvények
	void update();
	void render(sf::RenderTarget& target);

};

#endif