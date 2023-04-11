//Precomplied header csatolása
#include "stdafx.h"
#include "GameState.h"

//======Inicializálás======
//Alapvetõ változók deklarálása
void GameState::initvariables()
{
	//Pálya méreteinek beállítása
	this->maxSize.x = 8;
	this->maxSize.y = 5;
	//Lerakandó elem deklarálása
	this->textureRect = sf::IntRect(0, 0,static_cast<int>(this->stateData->gridSize), static_cast<int>(this->stateData->gridSize));
	//Elforgatottság mértéke
	this->rotate = 0;
	//Lerakott elemek száma
	this->tiles = 0;
	//Szerzett pontok száma
	this->points = 0;
	//Kameramozgatás sebessége
	this->cameraSpeed = GUI::p2pX(25.f, this->vm);
	srand((unsigned int)time(0));
}

//Nézet deklarálása
void GameState::initView()
{
	//Ablaknak megfelelõ méret
	this->view.setSize(sf::Vector2f(static_cast<float>(this->stateData->gfxSettings->resolution.width), static_cast<float>(this->stateData->gfxSettings->resolution.height)));
	//Középpont behatárolása
	this->view.setCenter(this->stateData->gfxSettings->resolution.width / 2.f, this->stateData->gfxSettings->resolution.height / 2.f);
}

//Betûtípus betöltése
void GameState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/Kingthings_Calligraphica_2.ttf"))
		throw("DEBUG::GameState::Font betoltese sikertelen");
}

//Megjelenõ szöveg deklarálása
void GameState::initText()
{
	//Játékos neve
	this->playerNameText.setPosition(sf::Vector2f(GUI::p2pX(1.f, vm), GUI::p2pY(1.f, vm)));
	this->playerNameText.setCharacterSize(GUI::calcCharSize(vm, 80));
	this->playerNameText.setFont(this->font);
	this->playerNameText.setFillColor(sf::Color::White);
	this->playerNameText.setString("Játékos név: " + this->playerName);
	//Pontok száma
	this->countText.setPosition(sf::Vector2f(GUI::p2pX(1.f, vm), GUI::p2pY(6.f, vm)));
	this->countText.setFont(this->font);
	this->countText.setCharacterSize(GUI::calcCharSize(this->vm, 80));
	this->countText.setFillColor(sf::Color::White);
}

//Elõre meghatározott billentyûk betöltése
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

//Szünet menü deklarálása
void GameState::initPauseMenu()
{
	this->pmenu = new PauseMenu(this->stateData->gfxSettings->resolution, this->font, "SZÜNETELTETVE", "A játékmenet szüneteltetve van!");
	this->emenu = new PauseMenu(this->stateData->gfxSettings->resolution, this->font, "JÁTÉK VÉGE", "A lap további lerakása nem lehetséges!");
}

//Gombok deklarálása
void GameState::initButtons()
{
	//Szünet menü gombok
	this->pmenu->addButton("QUIT", GUI::p2pY(74.f, this->vm), GUI::p2pX(10.f, this->vm), GUI::p2pY(6.f, this->vm), GUI::calcCharSize(this->vm), "Kilépés");
	this->pmenu->addButton("SAVE", GUI::p2pY(55.f, this->vm), GUI::p2pX(10.f, this->vm), GUI::p2pY(6.f, this->vm), GUI::calcCharSize(this->vm), "Mentés");
	this->pmenu->addButton("LOAD", GUI::p2pY(37.f, this->vm), GUI::p2pX(10.f, this->vm), GUI::p2pY(6.f, this->vm), GUI::calcCharSize(this->vm), "Betöltés");
	//Játék vége gombok
	this->emenu->addButton("QUIT", GUI::p2pY(74.f, this->vm), GUI::p2pX(10.f, this->vm), GUI::p2pY(6.f, this->vm), GUI::calcCharSize(this->vm), "Kilépés");
}

//Játékfelület deklarálása
void GameState::initTileMap()
{
	//Játékfelület létrehozása
	this->tileMap = new TileMap(this->stateData->gridSize, this->maxSize.x, this->maxSize.y, "Resources/Images/Tiles/tilesheet.png");
	//Kártyalapok deklarálása (18 féle kártya van, játékfelületnek megfelelõen elosztva)
	for (unsigned short i = 0; i < 6; i++)
		for (unsigned short j = 0; j < 3; j++)
			this->occurrence[std::make_pair(i, j)] = (unsigned short)std::round(this->maxSize.x * this->maxSize.y / 18) + 1;
	//Határ deklarálása
	this->border.setSize(sf::Vector2f(this->stateData->gridSize * this->maxSize.x, this->stateData->gridSize * this->maxSize.y));
	this->border.setFillColor(sf::Color::Transparent);
	this->border.setOutlineThickness(3.f);
	this->border.setOutlineColor(sf::Color::White);
	//Kezdetleges elem deklarálása (Egy olyan elem, amelyre a következõ elem biztosan lerakható)
	this->textureRect = sf::IntRect(1 * 100, 2 * 100, static_cast<int>(this->gridSize), static_cast<int>(this->gridSize));
	this->tileMap->initSample(std::make_pair(1, 2));
	this->occurrence[std::make_pair(1, 2)]--;
}

//Lerakandó elem deklarálása
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

//Pontszámításnál felugró animáció deklarálása
void GameState::initSystems()
{
	this->tts = new TXT::TextSystem("Fonts/Kingthings_Calligraphica_2.ttf");
	//Elem lerakást jelzõ hang betöltése
	if (!this->pbuffer.loadFromFile("Sounds/place.wav"))
		std::cout << "DEBUG::GameState::Elem lerakast jelzo hang betoltese sikertelen!" << "\n";
	this->placementSound.setBuffer(this->pbuffer);
	//Játék végét jelzõ hang betöltése
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

//======Függvények======
//Játék vége ellenõrzése
void GameState::checkIsEnd()
{
	//Betelt játékfelület ellenõrzése
	if(this->tiles == this->maxSize.x * this->maxSize.y)
	{
		this->points += 10;
		this->end = true;
	}
	//Lerakhatóság ellenõrzése
	unsigned db = 0;
	//Sorok bejárása
	for (size_t i = 0; i < this->maxSize.x; i++)
	{
		//Oszlopok bejárása
		for (size_t j = 0; j < this->maxSize.y; j++)
		{
			//Elem forgatása
			for (size_t k = 0; k < 4; k++)
			{
				if (this->tileMap->isValid((unsigned)i, (unsigned)j)) db++;
				this->tileMap->rotateRight();
			}
		}
	}
	if (db == 0)
		this->end = true;
	//Extra pontok ellenõrzése
	if (this->end)
	{
		this->endgameSound.play();
		std::cout << "===========PONTOK===========" << "\n";
		this->points += this->tileMap->calculateScore();
	}
}

//Bemenet ellenõrzése
void GameState::updateInput(const float& dt)
{
	//Szünet menü kezelése
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeytime())
	{
		if (!this->paused && !this->end)
			this->pauseState();
		else if(this->paused)
			this->unpauseState();
	}
}

//Játékhoz tartozó bemenet kezelése
void GameState::updateGameInput(const float& dt)
{
	//Kamera mozgatása
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
	//Elem lerakása ellenõrzése
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->getKeytime())
	{
		//Elsõ elem lerakása
		if (this->tiles == 0)
		{
			if (this->tileMap->firstTile(this->mousePosGrid.x, this->mousePosGrid.y, this->textureRect, this->rotate))
			{
				//Hang lejátszása
				this->placementSound.play();
				//Felugró animcáió
				this->tts->addTextTag(TAGTYPES::DEFAULT_TAG, this->mousePosView.x, this->mousePosView.y, GUI::calcCharSize(this->vm));
				//Az elem textúrájának változtatása (Elõfordulásnak megfelelõen)
				do {
					this->randX = (rand() % 6);
					this->randY = (rand() % 3);
				} while (this->occurrence[std::make_pair(this->randX, this->randY)] == 0);
				this->occurrence[std::make_pair(this->randX, this->randY)]--;
				this->textureRect = sf::IntRect(this->randX * 100, this->randY * 100, static_cast<int>(this->gridSize), static_cast<int>(this->gridSize));
				//Forgatottság nullázása
				this->rotate = 0;
				this->selectorRect.setRotation(0);
				//Új elem deklarálása
				this->tileMap->initSample(std::make_pair(this->randX, this->randY));
				//Lerakott elemek számának növelése
				this->tiles++;
				//Játék végének ellenõrzése
				this->checkIsEnd();
			}
		}
		//További elemek lerakása
		else if (this->tileMap->addTile(this->mousePosGrid.x, this->mousePosGrid.y, this->textureRect, this->rotate))
		{
			//Hang lejátszása
			this->placementSound.play();
			//Felugró animcáió
			this->tts->addTextTag(TAGTYPES::DEFAULT_TAG, this->mousePosView.x, this->mousePosView.y, GUI::calcCharSize(this->vm));
			//Az elem textúrájának változtatása (Elõfordulásnak megfelelõen)
			do {
				this->randX = (rand() % 6);
				this->randY = (rand() % 3);
			} while (this->occurrence[std::make_pair(this->randX, this->randY)] == 0);
			this->occurrence[std::make_pair(this->randX, this->randY)]--;
			this->textureRect = sf::IntRect(this->randX * 100, this->randY * 100, static_cast<int>(this->gridSize), static_cast<int>(this->gridSize));
			//Forgatottság nullázása
			this->rotate = 0;
			this->selectorRect.setRotation(0);
			//Új elem deklarálása
			this->tileMap->initSample(std::make_pair(this->randX, this->randY));
			//Lerakott elemek számának növelése
			this->tiles++;
			//Játék végének ellenõrzése
			this->checkIsEnd();
		}
	}
	//Elem forgatása jobbra
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("ROTATE_RIGHT"))) && this->getKeytime())
	{
		this->selectorRect.rotate(90.f);
		this->rotate += 90;
		this->tileMap->rotateRight();
	}
	//Elem forgatása balra
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("ROTATE_LEFT"))) && this->getKeytime())
	{
		this->selectorRect.rotate(-90.f);
		this->rotate -= 90;
		this->tileMap->rotateLeft();
	}
}

//Gombok frissítése
void GameState::updateButtons()
{
	for (auto& itr : this->buttons)
	{
		itr.second->update(this->mousePosWindow);
	}
}

//Megjelenített tartalom frissítése
void GameState::updateGui(const float& dt)
{
	//Lerakandó elem frissítése
	this->selectorRect.setTextureRect(this->textureRect);
	this->selectorRect.setPosition(this->mousePosGrid.x * this->stateData->gridSize, this->mousePosGrid.y * this->stateData->gridSize);
	//Pontszámítás & lerakott elemek számának frissítése
	this->countText.setString("Lerakott elemek: " + std::to_string(this->tiles));
}

//Szünet menü gombjainak frissítése
void GameState::updatePauseMenuButtons()
{
	//Szünet menü kilépés
	if (this->pmenu->isButtonPressed("QUIT") && this->getKeytime())
		this->endState();
	//Mentés
	if (this->pmenu->isButtonPressed("SAVE") && this->getKeytime())
		this->tileMap->saveToFile(this->playerName, this->points, this->tiles, "Maps/elozo.txt", this->occurrence); // Ha a játékos szeretné, elmentheti a játék végkimenetelét		
	//Betöltés
	if (this->pmenu->isButtonPressed("LOAD") && this->getKeytime()) {
		std::pair<std::pair<std::string, unsigned short>, std::map<std::pair<unsigned short, unsigned short>, unsigned short>*> datas;
		datas = this->tileMap->loadFromFile("Maps/elozo.txt"); // Elõzõ mentett játék végkimenetelének betöltése
		//Lementett játékadatok beállítása
		this->playerName = datas.first.first;
		this->playerNameText.setString("Játékos név: " + this->playerName);
		this->tiles = datas.first.second;
		//Felhasznált kártyák betöltése
		if (datas.second) this->occurrence = *datas.second;
		//DEBUG
		std::cout << "Rendelkezesre allo kartyak: " << "\n";
		for (auto itr = this->occurrence.begin(); itr != this->occurrence.end(); itr++)
			std::cout << itr->first.first << " " << itr->first.second << " " << itr->second << "\n";
	}
	//Játék végi kilépés
	if (this->emenu->isButtonPressed("QUIT"))
	{
		//Játékosnév & pontszám mentése egy egységes ranglista fájlba (fájl végére)
		std::time_t t = std::time(0);
		std::ofstream ofs("Maps/Ranglista.txt", std::ios_base::app);
		if (ofs.is_open())
		{
			ofs << this->playerName << " " << this->tiles << "\n";
		}
		else std::cout << "DEBUG::GameState::Fajl kiiras sikertelen" << "\n";
		ofs.close();
		//Kilépés
		this->endState();
	}
}

//További értékek frissítése
void GameState::update(const float& dt)
{
	//Egér pozíció frissítése
	this->updateMousePositions(&this->view);
	this->updateKeytime(dt);
	//Bemenet frissítése
	this->updateInput(dt);
	//Szünet & Játék vége menü inaktív
	if (!this->paused && !this->end)
	{
		this->updateButtons();
		this->updateGui(dt);
		this->updateGameInput(dt);
	}
	//Szünet menü aktív
	else if(this->paused)
	{
		this->pmenu->update(this->mousePosWindow);
		this->updatePauseMenuButtons();
	}
	//Játék vége menü aktív
	else if (this->end)
	{
		this->emenu->update(this->mousePosWindow);
		this->selectorRect.setFillColor(sf::Color(255, 255, 255, 255));
		this->selectorRect.setTextureRect(this->textureRect);
		this->selectorRect.setPosition(GUI::p2pX(48.f, this->vm), GUI::p2pY(30.f, this->vm));
		this->countText.setPosition(GUI::p2pX(45.f, this->vm), GUI::p2pY(55.f, this->vm));
		this->updatePauseMenuButtons();
	}
	//Pontozási animáció frissítése
	this->tts->update(dt);
}

//Gombok megjelenítése
void GameState::renderButtons(sf::RenderTarget& target)
{
	for (auto& itr : this->buttons)
	{
		itr.second->render(target);
	}
}

//Tartalom megjelenítése
void GameState::renderGui(sf::RenderTarget& target)
{
	//Nézet beállítása
	target.setView(this->view);
	//Lerakandó elem megjelenítése
	if (!this->end) target.draw(this->selectorRect, this->transform);
	//Ablakhoz viszonyított elemeknek nézet beállítása
	target.setView(this->window->getDefaultView());
	//Számolt értékek megjelenítése
	if (!this->end) target.draw(this->countText);
	//Játékos név megjelenítése
	target.draw(this->playerNameText);
	//Nézet beállítása
	target.setView(this->view);
}

void GameState::render(sf::RenderTarget* target)
{
	if (!target) target = this->window;
	target->setView(this->view);
	this->tileMap->render(*target);
	//Határ megjelenítése
	target->draw(this->border);
	target->setView(this->window->getDefaultView());
	//Gombok megjelenítése
	this->renderButtons(*target);
	//Látható tartalom megjelenítése
	this->renderGui(*target);
	//Pontozási animáció
	this->tts->render(*target);
	//Szünet menü
	if (this->paused)
	{
		target->setView(this->window->getDefaultView());
		this->pmenu->render(*target);
	}
	//Játék vége menü
	if (this->end)
	{
		target->setView(this->window->getDefaultView());
		this->emenu->render(*target);
		target->draw(this->selectorRect, this->transform);
		this->countText.setString("Pontok: " + std::to_string(this->points) + "\n" + "Lerakott elemek: " + std::to_string(this->tiles));
		target->draw(this->countText);
	}
}