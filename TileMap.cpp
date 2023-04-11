//Precomplied header csatol�sa
#include "stdafx.h"
#include "TileMap.h"

//======Inicializ�l�s======
//Lerakand� elemr�l minta k�sz�t�se
void TileMap::initSample(std::pair<unsigned short, unsigned short> rands)
{
	this->tileSample.top = this->tileData[rands.second][rands.first].top;
	this->tileSample.right = this->tileData[rands.second][rands.first].right;
	this->tileSample.bottom = this->tileData[rands.second][rands.first].bottom;
	this->tileSample.left = this->tileData[rands.second][rands.first].left;
	this->tileSample.isEndRoad = this->tileData[rands.second][rands.first].isEndRoad;
	this->tileSample.isCloister = this->tileData[rands.second][rands.first].isCloister;
}

//======Constructor======
TileMap::TileMap(float gridSize, unsigned width, unsigned height, std::string texture_file)
{
	//Alapvet� j�t�kfel�let be�ll�t�sok
	this->gridSizeF = gridSize;
	this->gridSizeU = static_cast<unsigned>(this->gridSizeF);
	this->maxSize.x = width;
	this->maxSize.y = height;
	this->textureFile = texture_file;
	//J�t�kfel�let deklar�l�sa
	this->map.reserve(maxSize.x);
	for (size_t i = 0; i < this->maxSize.x; i++)
	{
		this->map.push_back(std::vector<Tile*>());
		for (size_t j = 0; j < this->maxSize.y; j++)
		{
			this->map[i].push_back(nullptr);
		}
	}
	//Text�ra bet�lt�se
	if (!this->tileSheet.loadFromFile(texture_file))
		throw("DEBUG::TileMap::Textura fajl megnyitasa sikertelen");
	//Elemek adatainak bet�lt�se & elt�rol�sa
	std::ifstream ifs("Resources/Images/Tiles/tiles.txt");
	if (ifs.is_open())
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 6; j++)
			{
				ifs >> this->tileData[i][j].top >> this->tileData[i][j].right >> this->tileData[i][j].bottom >> this->tileData[i][j].left >> this->tileData[i][j].isEndRoad >> this->tileData[i][j].isCloister;
			}
		}
	}
	else throw("DEBUG::TileMap::Az elem adatokat tartalmazo fajl megnyitasa sikertelen");
	ifs.close();
}

//======Deconstructor======
TileMap::~TileMap()
{
	for (size_t i = 0; i < this->maxSize.x; i++)
	{
		for (size_t j = 0; j < this->maxSize.y; j++)
		{
			delete this->map[i][j];
			this->map[i][j] = nullptr;
		}
	}
	this->map.clear();
}

//======Seg�d F�ggv�nyek======
const sf::Texture* TileMap::getTileSheet() const
{
	return &this->tileSheet;
}

//======F�ggv�nyek======
//Jobbra forgat�s
const void TileMap::rotateRight()
{
	char tmp = this->tileSample.top;
	this->tileSample.top = this->tileSample.left;
	this->tileSample.left = this->tileSample.bottom;
	this->tileSample.bottom = this->tileSample.right;
	this->tileSample.right = tmp;
}

//Balra forgat�s
const void TileMap::rotateLeft()
{
	char tmp = this->tileSample.top;
	this->tileSample.top = this->tileSample.right;
	this->tileSample.right = this->tileSample.bottom;
	this->tileSample.bottom = this->tileSample.left;
	this->tileSample.left = tmp;
}

//Els� elem deklar�l�sa
const bool TileMap::firstTile(const unsigned x, const unsigned y, const sf::IntRect& texture_rect, const int& rotate)
{
	if (x < this->maxSize.x && x >= 0 &&
		y < this->maxSize.y && y >= 0 &&
		this->map[x][y] == nullptr)
	{
		this->map[x][y] = new Tile(x, y, this->gridSizeF, this->tileSheet, texture_rect, rotate, this->tileSample.top, this->tileSample.right, this->tileSample.bottom, this->tileSample.left, this->tileSample.isEndRoad, this->tileSample.isCloister);
		return true;
	}
	return false;
}

//Ellen�rz�s
const bool TileMap::isValid(const unsigned x, const unsigned y)
{
	//Hat�rok ellen�rz�se & elem poz�ci�ja �res-e
	if (x < this->maxSize.x && x >= 0 &&
		y < this->maxSize.y && y >= 0 &&
		this->map[x][y] == nullptr)
	{
		//M�r lentl�v� elemek �sszehasonl�t�sa a lerakand� elemmel
		//Lefele
		if (y < this->maxSize.y - 1)
			if (this->map[x][y + 1] != nullptr)
				if (this->map[x][y + 1]->getType(TileDirection::North) != this->tileSample.bottom)
					return false;
		//Jobbra
		if (x < this->maxSize.x - 1)
			if (this->map[x + 1][y] != nullptr)
				if (this->map[x + 1][y]->getType(TileDirection::West) != this->tileSample.right)
					return false;
		//Felfele
		if (y > 0)
			if (this->map[x][y - 1] != nullptr)
				if (this->map[x][y - 1]->getType(TileDirection::South) != this->tileSample.top)
					return false;
		//Balra
		if (x > 0)
			if (this->map[x - 1][y] != nullptr)
				if (this->map[x - 1][y]->getType(TileDirection::East) != this->tileSample.left)
					return false;
		//Kritikus pontok ellen�rz�se (hat�rment�n)
		if ((x == 0 || this->map[x - 1][y] == nullptr) &&
			(x == this->maxSize.x - 1 || this->map[x + 1][y] == nullptr) &&
			(y == 0 || this->map[x][y - 1] == nullptr) &&
			(y == this->maxSize.y - 1 || this->map[x][y + 1] == nullptr))
			return false;
		//Ha id�ig el�r a vizsg�lat, akkor lerakhat� az elem
		return true;
	}
	return false;
}

//Tov�bbi elem lerak�s ellen�rz�se
const bool TileMap::addTile(const unsigned x, const unsigned y, const sf::IntRect& texture_rect, const int& rotate)
{
	if (this->isValid(x, y))
	{
		//Elem lerak�sa
		this->map[x][y] = new Tile(x, y, this->gridSizeF, this->tileSheet, texture_rect, rotate, this->tileSample.top, this->tileSample.right, this->tileSample.bottom, this->tileSample.left, this->tileSample.isEndRoad, this->tileSample.isCloister);
		return true;
	}
	return false;
}

//======Extra pontsz�m�t�s======
unsigned short TileMap::searchEndpoints(unsigned int x, unsigned int y, TileDirection direction) {
	if (x < 0 || x >= this->maxSize.x || y < 0 || y >= this->maxSize.y || !this->map[x][y])
	{
		this->rp += (unsigned short)this->roadPoints.size();
		this->roadPoints.clear();
		return 0;
	}
	//---
	switch (direction)
	{
	case North:
		if (this->map[x][y]->isVisited(South)) return 0;
		this->map[x][y]->setVisited(South);
		break;
	case East:
		if (this->map[x][y]->isVisited(West)) return 0;
		this->map[x][y]->setVisited(West);
		break;
	case South:
		if (this->map[x][y]->isVisited(North)) return 0;
		this->map[x][y]->setVisited(North);
		break;
	case West:
		if (this->map[x][y]->isVisited(East)) return 0;
		this->map[x][y]->setVisited(East);
		break;
	}
	//---
	this->roadPoints.insert(std::make_pair(x, y));
	//---
	if (this->map[x][y]->getisEndRoad())
	{
		std::cout << "Zart utszakasz! +2 pont" << "\n";
		this->rp += (unsigned short)this->roadPoints.size();
		this->roadPoints.clear();
		return 2;
	}
	//---
	if (this->map[x][y]->getType(North) == 'u' && direction != South) {
		this->map[x][y]->setVisited(North);
		searchEndpoints(x, y - 1, North);
	}
	if (this->map[x][y]->getType(East) == 'u' && direction != West) {
		this->map[x][y]->setVisited(East);
		searchEndpoints(x + 1, y, East);
	}
	if (this->map[x][y]->getType(South) == 'u' && direction != North) {
		this->map[x][y]->setVisited(South);
		searchEndpoints(x, y + 1, South);
	}
	if (this->map[x][y]->getType(West) == 'u' && direction != East) {
		this->map[x][y]->setVisited(West);
		searchEndpoints(x - 1, y, West);
	}
	return 0;
}

unsigned short TileMap::searchCloister(unsigned int x, unsigned int y)
{
	const int dx[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	const int dy[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
	unsigned short pt = 0;
	for (int i = 0; i < 8; i++) {
		short nx = x + dx[i];
		short ny = y + dy[i];
		if (nx >= 0 && nx < (short)this->maxSize.x && ny >= 0 && ny < (short)this->maxSize.y && this->map[nx][ny] != nullptr)
			pt++;
	}
	std::cout << "Kolostor, pos: " << x << " " << y << ", pontok: " << pt << "\n";
	return pt;
}

void TileMap::searchCity(unsigned int x, unsigned int y, TileDirection direction, bool& closed)
{
	if (x < 0 || x >= this->maxSize.x || y < 0 || y >= this->maxSize.y || !this->map[x][y])
	{
		closed = false;
		return;
	}
	//---
	switch (direction)
	{
	case North:
		if (this->map[x][y]->isVisited(South)) return;
		this->map[x][y]->setVisited(South);
		break;
	case East:
		if (this->map[x][y]->isVisited(West)) return;
		this->map[x][y]->setVisited(West);
		break;
	case South:
		if (this->map[x][y]->isVisited(North)) return;
		this->map[x][y]->setVisited(North);
		break;
	case West:
		if (this->map[x][y]->isVisited(East)) return;
		this->map[x][y]->setVisited(East);
		break;
	}
	//---
	this->cityPoints.insert(std::make_pair(x, y));
	//---
	if (this->map[x][y]->getType(North) == 'v' && direction != South) {
		this->map[x][y]->setVisited(North);
		searchCity(x, y - 1, North, closed);
	}
	if (this->map[x][y]->getType(East) == 'v' && direction != West) {
		this->map[x][y]->setVisited(East);
		searchCity(x + 1, y, East, closed);
	}
	if (this->map[x][y]->getType(South) == 'v' && direction != North) {
		this->map[x][y]->setVisited(South);
		searchCity(x, y + 1, South, closed);
	}
	if (this->map[x][y]->getType(West) == 'v' && direction != East) {
		this->map[x][y]->setVisited(West);
		searchCity(x - 1, y, West, closed);
	}
}

void TileMap::searchCycle(unsigned int x, unsigned int y, TileDirection direction, bool& closed)
{
	if (x < 0 || x >= this->maxSize.x || y < 0 || y >= this->maxSize.y || !this->map[x][y])
	{
		closed = false;
		return;
	}
	//---
	switch (direction)
	{
	case North:
		if (this->map[x][y]->isVisited(South)) return;
		this->map[x][y]->setVisited(South);
		break;
	case East:
		if (this->map[x][y]->isVisited(West)) return;
		this->map[x][y]->setVisited(West);
		break;
	case South:
		if (this->map[x][y]->isVisited(North)) return;
		this->map[x][y]->setVisited(North);
		break;
	case West:
		if (this->map[x][y]->isVisited(East)) return;
		this->map[x][y]->setVisited(East);
		break;
	}
	//---
	this->roadPoints.insert(std::make_pair(x, y));
	//---
	if (this->map[x][y]->getType(North) == 'u' && direction != South) {
		this->map[x][y]->setVisited(North);
		searchCycle(x, y - 1, North, closed);
	}
	if (this->map[x][y]->getType(East) == 'u' && direction != West) {
		this->map[x][y]->setVisited(East);
		searchCycle(x + 1, y, East, closed);
	}
	if (this->map[x][y]->getType(South) == 'u' && direction != North) {
		this->map[x][y]->setVisited(South);
		searchCycle(x, y + 1, South, closed);
	}
	if (this->map[x][y]->getType(West) == 'u' && direction != East) {
		this->map[x][y]->setVisited(West);
		searchCycle(x - 1, y, West, closed);
	}
}

//Extra pontsz�m�t�s
const unsigned short TileMap::calculateScore()
{
	const int dx[] = { 0, 1, 0, -1 };
	const int dy[] = { -1, 0, 1, 0 };
	const char dir[4] = { 'n', 'e', 's', 'w' };
	unsigned short points = 0;
	bool closed = false;
	//---
	for (unsigned int x = 0; x < this->maxSize.x; x++)
	{
		for (unsigned int y = 0; y < this->maxSize.y; y++)
		{
			//V�g/kiindul�si ponttal rendelkez� utak ellen�rz�se
			if (this->map[x][y] && this->map[x][y]->getisEndRoad())
			{
				for (int k = 0; k < 4; k++) {
					int nx = x + dx[k];
					int ny = y + dy[k];
					if (this->map[x][y]->getType(North) == 'u' && dir[k] == 'n') {
						this->map[x][y]->setVisited(North);
						this->roadPoints.insert(std::make_pair(x, y));
						points += searchEndpoints(nx, ny, North);
					}
					if (this->map[x][y]->getType(East) == 'u' && dir[k] == 'e') {
						this->map[x][y]->setVisited(East);
						this->roadPoints.insert(std::make_pair(x, y));
						points += searchEndpoints(nx, ny, East);
					}
					if (this->map[x][y]->getType(South) == 'u' && dir[k] == 's') {
						this->map[x][y]->setVisited(South);
						this->roadPoints.insert(std::make_pair(x, y));
						points += searchEndpoints(nx, ny, South);
					}
					if (this->map[x][y]->getType(West) == 'u' && dir[k] == 'w') {
						this->map[x][y]->setVisited(West);
						this->roadPoints.insert(std::make_pair(x, y));
						points += searchEndpoints(nx, ny, West);
					}
				}
				this->roadPoints.clear();
			}
			//Kolostorok ellen�rz�se
			if (this->map[x][y] && this->map[x][y]->getIsCloister())
				points += searchCloister(x, y);
			//Egybef�gg� v�rosok ellen�rz�se
			if (this->map[x][y])
			{
				if ((this->map[x][y]->getType(North) == 'v' && !this->map[x][y]->isVisited(North)) ||
					(this->map[x][y]->getType(East) == 'v' && !this->map[x][y]->isVisited(East)) ||
					(this->map[x][y]->getType(South) == 'v' && !this->map[x][y]->isVisited(South)) ||
					(this->map[x][y]->getType(West) == 'v' && !this->map[x][y]->isVisited(West)))
				{
					closed = true;
					for (int k = 0; k < 4; k++) {
						int nx = x + dx[k];
						int ny = y + dy[k];
						if (this->map[x][y]->getType(North) == 'v' && !this->map[x][y]->isVisited(North) && dir[k] == 'n') {
							this->map[x][y]->setVisited(North);
							this->cityPoints.insert(std::make_pair(x, y));
							searchCity(nx, ny, North, closed);
						}
						if (this->map[x][y]->getType(East) == 'v' && !this->map[x][y]->isVisited(East) && dir[k] == 'e') {
							this->map[x][y]->setVisited(East);
							this->cityPoints.insert(std::make_pair(x, y));
							searchCity(nx, ny, East, closed);
						}
						if (this->map[x][y]->getType(South) == 'v' && !this->map[x][y]->isVisited(South) && dir[k] == 's') {
							this->map[x][y]->setVisited(South);
							this->cityPoints.insert(std::make_pair(x, y));
							searchCity(nx, ny, South, closed);
						}
						if (this->map[x][y]->getType(West) == 'v' && !this->map[x][y]->isVisited(West) && dir[k] == 'w') {
							this->map[x][y]->setVisited(West);
							this->cityPoints.insert(std::make_pair(x, y));
							searchCity(nx, ny, West, closed);
						}
					}
				}
				if (closed)
				{
					std::cout << "Zart varos! +5 pont" << "\n";
					points += 5;
					closed = false;
				}
			}
		}
	}
	//Az utak�rt kapott pontsz�m
	for (unsigned int x = 0; x < this->maxSize.x; x++)
	{
		for (unsigned int y = 0; y < this->maxSize.y; y++)
		{
			//�nmag�ba csatlakoz� utak ellen�rz�se
			if (this->map[x][y])
			{
				if ((this->map[x][y]->getType(North) == 'u' && !this->map[x][y]->isVisited(North)) ||
					(this->map[x][y]->getType(East) == 'u' && !this->map[x][y]->isVisited(East)) ||
					(this->map[x][y]->getType(South) == 'u' && !this->map[x][y]->isVisited(South)) ||
					(this->map[x][y]->getType(West) == 'u' && !this->map[x][y]->isVisited(West)))
				{
					closed = true;
					for (int k = 0; k < 4; k++) {
						int nx = x + dx[k];
						int ny = y + dy[k];
						if (this->map[x][y]->getType(North) == 'u' && !this->map[x][y]->isVisited(North) && dir[k] == 'n') {
							this->map[x][y]->setVisited(North);
							this->roadPoints.insert(std::make_pair(x, y));
							searchCycle(nx, ny, North, closed);
						}
						if (this->map[x][y]->getType(East) == 'u' && !this->map[x][y]->isVisited(East) && dir[k] == 'e') {
							this->map[x][y]->setVisited(East);
							this->roadPoints.insert(std::make_pair(x, y));
							searchCycle(nx, ny, East, closed);
						}
						if (this->map[x][y]->getType(South) == 'u' && !this->map[x][y]->isVisited(South) && dir[k] == 's') {
							this->map[x][y]->setVisited(South);
							this->roadPoints.insert(std::make_pair(x, y));
							searchCycle(nx, ny, South, closed);
						}
						if (this->map[x][y]->getType(West) == 'u' && !this->map[x][y]->isVisited(West) && dir[k] == 'w') {
							this->map[x][y]->setVisited(West);
							this->roadPoints.insert(std::make_pair(x, y));
							searchCycle(nx, ny, West, closed);
						}
					}
				}
				if (closed)
				{
					std::cout << "Magaba csatlakozo ut! +2 pont" << "\n";
					points += 2;
					closed = false;
				}
			}
		}
	}
	//A v�rosok�rt kapott pontsz�m
	std::cout << "A varosok darabszama: " << this->cityPoints.size() << "\n";
	points += (unsigned short)this->cityPoints.size() * 2;
	this->cityPoints.clear();
	//Utak�rt kapott pontsz�m
	this->rp += (unsigned)this->roadPoints.size();
	std::cout << "Az utak darabszama: " << this->rp << "\n";
	points += this->rp;
	return points;
}

//V�gkimenetel lement�se
void TileMap::saveToFile(const std::string playerName, const unsigned short points, const unsigned short tiles, const std::string file_name, std::map<std::pair<unsigned short, unsigned short>, unsigned short>& occurrence)
{
	std::ofstream ofs(file_name);
	if (ofs.is_open())
	{
		//Alapvet� j�t�kfel�let be�ll�t�sok lement�se
		ofs << playerName << " " << points << " " << tiles << "\n"
			<< this->maxSize.x << " " << this->maxSize.y << "\n"
			<< this->gridSizeU << "\n"
			<< this->textureFile << "\n";
		//Felhasznalt elemek elmentese
		for (auto itr = occurrence.begin(); itr != occurrence.end(); itr++)
		{
			ofs << itr->first.first << " " << itr->first.second << " " << itr->second << "\n";
		}
		//A j�t�kfel�leten tal�lhat� elemek adatainak lement�se
		for (size_t i = 0; i < this->maxSize.x; i++)
		{
			for (size_t j = 0; j < this->maxSize.y; j++)
			{
				if (this->map[i][j])
				{
					ofs << i << " " << j << " " << this->map[i][j]->getAsString() << "\n";
				}
			}
		}
	}
	else std::cout<< "DEBUG::TileMap::Fajl kiiras sikertelen" << "\n";
	ofs.close();
}

//El�z� lementett v�gkimenetel bet�lt�se
std::pair<std::pair<std::string, unsigned short>, std::map<std::pair<unsigned short, unsigned short>, unsigned short>*> TileMap::loadFromFile(const std::string file_name)
{
	std::ifstream ifs(file_name);
	if (ifs.is_open())
	{
		//V�ltoz�k
		std::string playerName;
		unsigned short points;
		unsigned short tiles;
		sf::Vector2u size;
		unsigned gridSize = 0;
		std::string texture_file = "";
		unsigned x = 0;
		unsigned y = 0;
		unsigned trX = 0;
		unsigned trY = 0;
		int rotate = 0;
		char top;
		char right;
		char bottom;
		char left;
		bool isEndRoad;
		bool isCloister;
		unsigned short piece = 0;
		//Elemek elofordulasa
		//Alapvet� �rt�kek beolvas�sa
		ifs >> playerName >> points >> tiles;
		ifs >> size.x >> size.y;
		ifs >> gridSize;
		ifs >> texture_file;
		//Alapvet� be�ll�t�sok
		this->gridSizeF = static_cast<float>(gridSize);
		this->gridSizeU = gridSize;
		this->maxSize.x = size.x;
		this->maxSize.y = size.y;
		this->textureFile = texture_file;
		//Felhasznalt elemek betoltese
		for (size_t i = 0; i < 18; i++)
		{
			ifs >> x >> y >> piece;
			this->occurrence[std::make_pair(x, y)] = piece;
		}
		//Jelenleg haszn�lt j�t�kfel�let lenull�z�sa
		for (size_t i = 0; i < this->maxSize.x; i++)
		{
			for (size_t j = 0; j < this->maxSize.y; j++)
			{
				delete this->map[i][j];
				this->map[i][j] = nullptr;
			}
		}
		this->map.clear();
		//K�v�nt j�t�kfel�let l�trehoz�sa
		this->map.reserve(maxSize.x);
		for (size_t i = 0; i < this->maxSize.x; i++)
		{
			this->map.push_back(std::vector<Tile*>());
			for (size_t j = 0; j < this->maxSize.y; j++)
			{
				this->map[i].push_back(nullptr);
			}
		}
		//Text�ra bet�lt�se
		if (!this->tileSheet.loadFromFile(texture_file))
			throw("DEBUG::TileMap::Textura fajl megnyitasa sikertelen");
		//Elemek bet�lt�se
		while (!ifs.eof())
		{
			ifs >> x >> y >> trX >> trY >> rotate >> top >> right >> bottom >> left >> isEndRoad >> isCloister;
			this->map[x][y] = new Tile(x, y, this->gridSizeF, this->tileSheet, sf::IntRect(trX, trY, this->gridSizeU, this->gridSizeU), rotate, top, right, bottom, left, isEndRoad, isCloister);
		}
		ifs.close();
		return std::make_pair(std::make_pair(playerName, tiles), &this->occurrence);
	}
	else std::cout << "DEBUG::TileMap::Fajl beolvasas sikertelen" << "\n";
	ifs.close();
	//DEBUG
	return std::make_pair(std::make_pair("NULL", 0), nullptr);
}

//Lerakott elemek megjelen�t�se
void TileMap::render(sf::RenderTarget& target)
{
	for (auto& i : this->map)
	{
		for (auto* j : i)
		{
			if (j != nullptr) j->render(target);
		}
	}
}