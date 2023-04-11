//Precomplied header csatol�sa
#include "stdafx.h"
#include "GameState.h"

//======Inicializ�l�s======
//Alapvet� v�ltoz�k deklar�l�sa
void GameState::initvariables()
{
	//P�lya m�reteinek be�ll�t�sa
	this->maxSize.x = 8;
	this->maxSize.y = 5;
	//Lerakand� elem deklar�l�sa
	this->textureRect = sf::IntRect(0, 0,static_cast<int>(this->stateData->gridSize), static_cast<int>(this->stateData->gridSize));
	//Elforgatotts�g m�rt�ke
	this->rotate = 0;
	//Lerakott elemek sz�ma
	this->tiles = 0;
	//Szerzett pontok sz�ma
	this->points = 0;
	//Kameramozgat�s sebess�ge
	this->cameraSpeed = GUI::p2pX(25.f, this->vm);
	srand((unsigned int)time(0));
}

//N�zet deklar�l�sa
void GameState::initView()
{
	//Ablaknak megfelel� m�ret
	this->view.setSize(sf::Vector2f(static_cast<float>(this->stateData->gfxSettings->resolution.width), static_cast<float>(this->stateData->gfxSettings->resolution.height)));
	//K�z�ppont behat�rol�sa
	this->view.setCenter(this->stateData->gfxSettings->resolution.width / 2.f, this->stateData->gfxSettings->resolution.height / 2.f);
}

//Bet�t�pus bet�lt�se
void GameState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/Kingthings_Calligraphica_2.ttf"))
		throw("DEBUG::GameState::Font betoltese sikertelen");
}

//Megjelen� sz�veg deklar�l�sa
void GameState::initText()
{
	//J�t�kos neve
	this->playerNameText.setPosition(sf::Vector2f(GUI::p2pX(1.f, vm), GUI::p2pY(1.f, vm)));
	this->playerNameText.setCharacterSize(GUI::calcCharSize(vm, 80));
	this->playerNameText.setFont(this->font);
	this->playerNameText.setFillColor(sf::Color::White);
	this->playerNameText.setString("J�t�kos n�v: " + this->playerName);
	//Pontok sz�ma
	this->countText.setPosition(sf::Vector2f(GUI::p2pX(1.f, vm), GUI::p2pY(6.f, vm)));
	this->countText.setFont(this->font);
	this->countText.setCharacterSize(GUI::calcCharSize(this->vm, 80));
	this->countText.setFillColor(sf::Color::White);
}

//El�re meghat�rozott billenty�k bet�lt�se
void GameState::initKeybinds()
{
	std::ifstream ifs("Config/gamestate_keybinds.ini");
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

//Sz�net men� deklar�l�sa
void GameState::initPauseMenu()
{
	this->pmenu = new PauseMenu(this->stateData->gfxSettings->resolution, this->font, "SZ�NETELTETVE", "A j�t�kmenet sz�neteltetve van!");
	this->emenu = new PauseMenu(this->stateData->gfxSettings->resolution, this->font, "J�T�K V�GE", "A lap tov�bbi lerak�sa nem lehets�ges!");
}

//Gombok deklar�l�sa
void GameState::initButtons()
{
	//Sz�net men� gombok
	this->pmenu->addButton("QUIT", GUI::p2pY(74.f, this->vm), GUI::p2pX(10.f, this->vm), GUI::p2pY(6.f, this->vm), GUI::calcCharSize(this->vm), "Kil�p�s");
	this->pmenu->addButton("SAVE", GUI::p2pY(55.f, this->vm), GUI::p2pX(10.f, this->vm), GUI::p2pY(6.f, this->vm), GUI::calcCharSize(this->vm), "Ment�s");
	this->pmenu->addButton("LOAD", GUI::p2pY(37.f, this->vm), GUI::p2pX(10.f, this->vm), GUI::p2pY(6.f, this->vm), GUI::calcCharSize(this->vm), "Bet�lt�s");
	//J�t�k v�ge gombok
	this->emenu->addButton("QUIT", GUI::p2pY(74.f, this->vm), GUI::p2pX(10.f, this->vm), GUI::p2pY(6.f, this->vm), GUI::calcCharSize(this->vm), "Kil�p�s");
}

//J�t�kfel�let deklar�l�sa
void GameState::initTileMap()
{
	//J�t�kfel�let l�trehoz�sa
	this->tileMap = new TileMap(this->stateData->gridSize, this->maxSize.x, this->maxSize.y, "Resources/Images/Tiles/tilesheet.png");
	//K�rtyalapok deklar�l�sa (18 f�le k�rtya van, j�t�kfel�letnek megfelel�en elosztva)
	for (unsigned short i = 0; i < 6; i++)
		for (unsigned short j = 0; j < 3; j++)
			this->occurrence[std::make_pair(i, j)] = (unsigned short)std::round(this->maxSize.x * this->maxSize.y / 18) + 1;
	//Hat�r deklar�l�sa
	this->border.setSize(sf::Vector2f(this->stateData->gridSize * this->maxSize.x, this->stateData->gridSize * this->maxSize.y));
	this->border.setFillColor(sf::Color::Transparent);
	this->border.setOutlineThickness(3.f);
	this->border.setOutlineColor(sf::Color::White);
	//Kezdetleges elem deklar�l�sa (Egy olyan elem, amelyre a k�vetkez� elem biztosan lerakhat�)
	this->textureRect = sf::IntRect(1 * 100, 2 * 100, static_cast<int>(this->gridSize), static_cast<int>(this->gridSize));
	this->tileMap->initSample(std::make_pair(1, 2));
	this->occurrence[std::make_pair(1, 2)]--;
}

//Lerakand� elem deklar�l�sa
void GameState::initGui()
{
	this->selectorRect.setSize(sf::Vector2f(this->stateData->gridSize, this->stateData->gridSize));
	this->selectorRect.setFillColor(sf::Color(255, 255, 255, 100));
	this->selectorRect.setOutlineThickness(2.f);
	this->selectorRect.setOutlineColor(sf::Color::White);
	this->selectorRect.setTexture(this->tileMap->getTileSheet());
	this->selectorRect.setTextureRect(this->textureRect);
	this->selectorRect.setOrigin(this->gridSize / 2.f, this->gridSize / 2.f);
	this->transform.translate(this->gridSize / 2.f, this->gridSize / 2.f);
}

//Pontsz�m�t�sn�l felugr� anim�ci� deklar�l�sa
void GameState::initSystems()
{
	this->tts = new TXT::TextSystem("Fonts/Kingthings_Calligraphica_2.ttf");
	//Elem lerak�st jelz� hang bet�lt�se
	if (!this->pbuffer.loadFromFile("Sounds/place.wav"))
		std::cout << "DEBUG::GameState::Elem lerakast jelzo hang betoltese sikertelen!" << "\n";
	this->placementSound.setBuffer(this->pbuffer);
	//J�t�k v�g�t jelz� hang bet�lt�se
	if (!this->ebuffer.loadFromFile("Sounds/end.wav"))
		std::cout << "DEBUG::GameState::Jatek vege hang betoltese sikertelen!" << "\n";
	this->endgameSound.setBuffer(this->ebuffer);
}

//======Constructor======
GameState::GameState(StateData* state_data, const std::string& player_name)
	: State(state_data), vm(stateData->gfxSettings->resolution)
{
	this->playerName = player_name;
	this->initvariables();
	this->initView();
	this->initFonts();
	this->initText();
	this->initKeybinds();
	this->initPauseMenu();
	this->initButtons();
	this->initTileMap();
	this->initGui();
	this->initSystems();
}

//======Deconstructor======
GameState::~GameState()
{
	auto itr = this->buttons.begin();
	for (; itr != this->buttons.end(); ++itr)
	{
		delete itr->second;
	}
	delete this->pmenu;
	delete this->emenu;
	delete this->tileMap;
	delete this->tts;
}

//======F�ggv�nyek======
//J�t�k v�ge ellen�rz�se
void GameState::checkIsEnd()
{
	//Betelt j�t�kfel�let ellen�rz�se
	if(this->tiles == this->maxSize.x * this->maxSize.y)
	{
		this->points += 10;
		this->end = true;
	}
	//Lerakhat�s�g ellen�rz�se
	unsigned db = 0;
	//Sorok bej�r�sa
	for (size_t i = 0; i < this->maxSize.x; i++)
	{
		//Oszlopok bej�r�sa
		for (size_t j = 0; j < this->maxSize.y; j++)
		{
			//Elem forgat�sa
			for (size_t k = 0; k < 4; k++)
			{
				if (this->tileMap->isValid((unsigned)i, (unsigned)j)) db++;
				this->tileMap->rotateRight();
			}
		}
	}
	if (db == 0)
		this->end = true;
	//Extra pontok ellen�rz�se
	if (this->end)
	{
		this->endgameSound.play();
		std::cout << "===========PONTOK===========" << "\n";
		this->points += this->tileMap->calculateScore();
	}
}

//Bemenet ellen�rz�se
void GameState::updateInput(const float& dt)
{
	//Sz�net men� kezel�se
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeytime())
	{
		if (!this->paused && !this->end)
			this->pauseState();
		else if(this->paused)
			this->unpauseState();
	}
}

//J�t�khoz tartoz� bemenet kezel�se
void GameState::updateGameInput(const float& dt)
{
	//Kamera mozgat�sa
	//Felfele
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_UP"))))
		this->view.move(0.f, -this->cameraSpeed * dt);
	//Lefele
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_DOWN"))))
		this->view.move(0.f, this->cameraSpeed * dt);
	//Balra
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_LEFT"))))
		this->view.move(-this->cameraSpeed * dt, 0.f);
	//Jobbra
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_RIGHT"))))
		this->view.move(this->cameraSpeed * dt, 0.f);
	//Elem lerak�sa ellen�rz�se
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->getKeytime())
	{
		//Els� elem lerak�sa
		if (this->tiles == 0)
		{
			if (this->tileMap->firstTile(this->mousePosGrid.x, this->mousePosGrid.y, this->textureRect, this->rotate))
			{
				//Hang lej�tsz�sa
				this->placementSound.play();
				//Felugr� animc�i�
				this->tts->addTextTag(TAGTYPES::DEFAULT_TAG, this->mousePosView.x, this->mousePosView.y, GUI::calcCharSize(this->vm));
				//Az elem text�r�j�nak v�ltoztat�sa (El�fordul�snak megfelel�en)
				do {
					this->randX = (rand() % 6);
					this->randY = (rand() % 3);
				} while (this->occurrence[std::make_pair(this->randX, this->randY)] == 0);
				this->occurrence[std::make_pair(this->randX, this->randY)]--;
				this->textureRect = sf::IntRect(this->randX * 100, this->randY * 100, static_cast<int>(this->gridSize), static_cast<int>(this->gridSize));
				//Forgatotts�g null�z�sa
				this->rotate = 0;
				this->selectorRect.setRotation(0);
				//�j elem deklar�l�sa
				this->tileMap->initSample(std::make_pair(this->randX, this->randY));
				//Lerakott elemek sz�m�nak n�vel�se
				this->tiles++;
				//J�t�k v�g�nek ellen�rz�se
				this->checkIsEnd();
			}
		}
		//Tov�bbi elemek lerak�sa
		else if (this->tileMap->addTile(this->mousePosGrid.x, this->mousePosGrid.y, this->textureRect, this->rotate))
		{
			//Hang lej�tsz�sa
			this->placementSound.play();
			//Felugr� animc�i�
			this->tts->addTextTag(TAGTYPES::DEFAULT_TAG, this->mousePosView.x, this->mousePosView.y, GUI::calcCharSize(this->vm));
			//Az elem text�r�j�nak v�ltoztat�sa (El�fordul�snak megfelel�en)
			do {
				this->randX = (rand() % 6);
				this->randY = (rand() % 3);
			} while (this->occurrence[std::make_pair(this->randX, this->randY)] == 0);
			this->occurrence[std::make_pair(this->randX, this->randY)]--;
			this->textureRect = sf::IntRect(this->randX * 100, this->randY * 100, static_cast<int>(this->gridSize), static_cast<int>(this->gridSize));
			//Forgatotts�g null�z�sa
			this->rotate = 0;
			this->selectorRect.setRotation(0);
			//�j elem deklar�l�sa
			this->tileMap->initSample(std::make_pair(this->randX, this->randY));
			//Lerakott elemek sz�m�nak n�vel�se
			this->tiles++;
			//J�t�k v�g�nek ellen�rz�se
			this->checkIsEnd();
		}
	}
	//Elem forgat�sa jobbra
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("ROTATE_RIGHT"))) && this->getKeytime())
	{
		this->selectorRect.rotate(90.f);
		this->rotate += 90;
		this->tileMap->rotateRight();
	}
	//Elem forgat�sa balra
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("ROTATE_LEFT"))) && this->getKeytime())
	{
		this->selectorRect.rotate(-90.f);
		this->rotate -= 90;
		this->tileMap->rotateLeft();
	}
}

//Gombok friss�t�se
void GameState::updateButtons()
{
	for (auto& itr : this->buttons)
	{
		itr.second->update(this->mousePosWindow);
	}
}

//Megjelen�tett tartalom friss�t�se
void GameState::updateGui(const float& dt)
{
	//Lerakand� elem friss�t�se
	this->selectorRect.setTextureRect(this->textureRect);
	this->selectorRect.setPosition(this->mousePosGrid.x * this->stateData->gridSize, this->mousePosGrid.y * this->stateData->gridSize);
	//Pontsz�m�t�s & lerakott elemek sz�m�nak friss�t�se
	this->countText.setString("Lerakott elemek: " + std::to_string(this->tiles));
}

//Sz�net men� gombjainak friss�t�se
void GameState::updatePauseMenuButtons()
{
	//Sz�net men� kil�p�s
	if (this->pmenu->isButtonPressed("QUIT") && this->getKeytime())
		this->endState();
	//Ment�s
	if (this->pmenu->isButtonPressed("SAVE") && this->getKeytime())
		this->tileMap->saveToFile(this->playerName, this->points, this->tiles, "Maps/elozo.txt", this->occurrence); // Ha a j�t�kos szeretn�, elmentheti a j�t�k v�gkimenetel�t		
	//Bet�lt�s
	if (this->pmenu->isButtonPressed("LOAD") && this->getKeytime()) {
		std::pair<std::pair<std::string, unsigned short>, std::map<std::pair<unsigned short, unsigned short>, unsigned short>*> datas;
		datas = this->tileMap->loadFromFile("Maps/elozo.txt"); // El�z� mentett j�t�k v�gkimenetel�nek bet�lt�se
		//Lementett j�t�kadatok be�ll�t�sa
		this->playerName = datas.first.first;
		this->playerNameText.setString("J�t�kos n�v: " + this->playerName);
		this->tiles = datas.first.second;
		//Felhaszn�lt k�rty�k bet�lt�se
		if (datas.second) this->occurrence = *datas.second;
		//DEBUG
		std::cout << "Rendelkezesre allo kartyak: " << "\n";
		for (auto itr = this->occurrence.begin(); itr != this->occurrence.end(); itr++)
			std::cout << itr->first.first << " " << itr->first.second << " " << itr->second << "\n";
	}
	//J�t�k v�gi kil�p�s
	if (this->emenu->isButtonPressed("QUIT"))
	{
		//J�t�kosn�v & pontsz�m ment�se egy egys�ges ranglista f�jlba (f�jl v�g�re)
		std::time_t t = std::time(0);
		std::ofstream ofs("Maps/Ranglista.txt", std::ios_base::app);
		if (ofs.is_open())
		{
			ofs << this->playerName << " " << this->tiles << "\n";
		}
		else std::cout << "DEBUG::GameState::Fajl kiiras sikertelen" << "\n";
		ofs.close();
		//Kil�p�s
		this->endState();
	}
}

//Tov�bbi �rt�kek friss�t�se
void GameState::update(const float& dt)
{
	//Eg�r poz�ci� friss�t�se
	this->updateMousePositions(&this->view);
	this->updateKeytime(dt);
	//Bemenet friss�t�se
	this->updateInput(dt);
	//Sz�net & J�t�k v�ge men� inakt�v
	if (!this->paused && !this->end)
	{
		this->updateButtons();
		this->updateGui(dt);
		this->updateGameInput(dt);
	}
	//Sz�net men� akt�v
	else if(this->paused)
	{
		this->pmenu->update(this->mousePosWindow);
		this->updatePauseMenuButtons();
	}
	//J�t�k v�ge men� akt�v
	else if (this->end)
	{
		this->emenu->update(this->mousePosWindow);
		this->selectorRect.setFillColor(sf::Color(255, 255, 255, 255));
		this->selectorRect.setTextureRect(this->textureRect);
		this->selectorRect.setPosition(GUI::p2pX(48.f, this->vm), GUI::p2pY(30.f, this->vm));
		this->countText.setPosition(GUI::p2pX(45.f, this->vm), GUI::p2pY(55.f, this->vm));
		this->updatePauseMenuButtons();
	}
	//Pontoz�si anim�ci� friss�t�se
	this->tts->update(dt);
}

//Gombok megjelen�t�se
void GameState::renderButtons(sf::RenderTarget& target)
{
	for (auto& itr : this->buttons)
	{
		itr.second->render(target);
	}
}

//Tartalom megjelen�t�se
void GameState::renderGui(sf::RenderTarget& target)
{
	//N�zet be�ll�t�sa
	target.setView(this->view);
	//Lerakand� elem megjelen�t�se
	if (!this->end) target.draw(this->selectorRect, this->transform);
	//Ablakhoz viszony�tott elemeknek n�zet be�ll�t�sa
	target.setView(this->window->getDefaultView());
	//Sz�molt �rt�kek megjelen�t�se
	if (!this->end) target.draw(this->countText);
	//J�t�kos n�v megjelen�t�se
	target.draw(this->playerNameText);
	//N�zet be�ll�t�sa
	target.setView(this->view);
}

void GameState::render(sf::RenderTarget* target)
{
	if (!target) target = this->window;
	target->setView(this->view);
	this->tileMap->render(*target);
	//Hat�r megjelen�t�se
	target->draw(this->border);
	target->setView(this->window->getDefaultView());
	//Gombok megjelen�t�se
	this->renderButtons(*target);
	//L�that� tartalom megjelen�t�se
	this->renderGui(*target);
	//Pontoz�si anim�ci�
	this->tts->render(*target);
	//Sz�net men�
	if (this->paused)
	{
		target->setView(this->window->getDefaultView());
		this->pmenu->render(*target);
	}
	//J�t�k v�ge men�
	if (this->end)
	{
		target->setView(this->window->getDefaultView());
		this->emenu->render(*target);
		target->draw(this->selectorRect, this->transform);
		this->countText.setString("Pontok: " + std::to_string(this->points) + "\n" + "Lerakott elemek: " + std::to_string(this->tiles));
		target->draw(this->countText);
	}
}