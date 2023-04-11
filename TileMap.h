#ifndef TILEMAP_H
#define TILEMAP_H

#include "Tile.h"
#include "TextSystem.h"

//Elemek adatstruktúrája
struct Data
{
	char top;
	char right;
	char bottom;
	char left;
	bool isEndRoad;
	bool isCloister;
};

class TileMap
{
private:
	//Változók
	float gridSizeF;
	unsigned gridSizeU;
	sf::Vector2u maxSize;
	std::vector<std::vector<Tile*>> map;
	std::string textureFile;
	sf::Texture tileSheet;

	Data tileData[4][6];
	Data tileSample;

	unsigned short rp;
	std::set<std::pair<unsigned short, unsigned short>> roadPoints;
	std::set<std::pair<unsigned short, unsigned short>> cityPoints;

	std::map<std::pair<unsigned short, unsigned short>, unsigned short> occurrence;
public:
	//Constructors / Deconstructors
	TileMap(float gridSize, unsigned width, unsigned height, std::string texture_file);
	virtual ~TileMap();

	//Segéd függvények
	const sf::Texture* getTileSheet() const;

	//Függvények
	void initSample(std::pair<unsigned short, unsigned short> rands);
	const void rotateRight();
	const void rotateLeft();
	const bool firstTile(const unsigned x, const unsigned y, const sf::IntRect& texture_rect, const int& rotate);
	const bool isValid(const unsigned x, const unsigned y);
	const bool addTile(const unsigned x, const unsigned y, const sf::IntRect& texture_rect, const int& rotate);

	unsigned short searchEndpoints(unsigned int x, unsigned int y, TileDirection direction);
	unsigned short searchCloister(unsigned int x, unsigned int y);
	void searchCycle(unsigned int x, unsigned int y, TileDirection direction, bool& closed);
	void searchCity(unsigned int x, unsigned int y, TileDirection direction, bool& closed);
	const unsigned short calculateScore();

	void saveToFile(const std::string playerName, const unsigned short points, const unsigned short tiles, const std::string file_name, std::map<std::pair<unsigned short, unsigned short>, unsigned short>& occurrence);
	std::pair<std::pair<std::string, unsigned short>, std::map<std::pair<unsigned short, unsigned short>, unsigned short>*> loadFromFile(const std::string file_name);

	void render(sf::RenderTarget& target);
};

#endif