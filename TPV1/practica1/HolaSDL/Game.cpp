#include "Game.h"

//constructora de Game
Game::Game(int level) {
	//creacion de la ventana de juego
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Pac-Man", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		winWidth, winHeight, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (window == nullptr || renderer == nullptr)
		throw string("ERROR: SDL error, either window or renderer aren't loading properly");

	//creacion de las texturas y barra informacion
	for (int i = 0; i < NUM_TEXTURE; i++)
		textures[i] = new Texture(renderer, textureAtribs[i].fileName, textureAtribs[i].fils, textureAtribs[i].cols);

	//creacion de los objetos
	loadObjects(level);

	//creacion de la barra de informacion 
	infoBar = new InfoBar(this, pacman->getLivesPoint(), pacman->getEnergyPoint());

	//asignacion de variables de control
	exit = false;
	victory = true;
	srand(time(nullptr)); //semilla aleatoria
}

//destructora de Game
Game::~Game() {
	//eliminamos los punteros a Pacman, GameMap, Ghosts y Textures
	delete gameMap;
	delete pacman;
	for (Ghost*& elem : ghosts) delete elem;
	for (Texture*& elem : textures) delete elem;
	delete infoBar;
	//eliminamos lo necesario de SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

//metodo del bucle principal de juego
void Game::run() {
	//mientras no salga
	while (!exit) {
		update(); //actualizamos
		render(); //renderizamos
		handleEvents(); //manejamos eventos
	}
}

//metodo para guardar la partida, guardando un fichero por entidad a guardar
void Game::saveGame() {
	ofstream output;
	gameMap->save(output); //guardamos archivo GameMap
	pacman->save(output); //guardamos archivo Pacman
	output.open("..\\savedGame\\Ghosts.txt");  //guardamo archivo Ghosts
	if (!output.is_open()) throw string("ERROR: Ghost saved file not created correctly");
	for (Ghost* ghost : ghosts) ghost->save(output);
	output.close();
}

//metodo para comprobar que en la posicion de Pacman haya o no un Ghost
bool Game::collisionGhost(Point2D const pacmanPos, int const energia) const {
	int i = 0; //buscamos si coindcide con alguno
	while (i < ghosts.size() && !(pacmanPos == ghosts[i]->getPos())) i++;

	//si no coincide
	if (i == ghosts.size()) return false;
	else {
		//si coincide pero tiene powerUp
		if (energia != 0) {
			//matamos al Ghost
			ghosts[i]->death();
			return false;
		}
		//en caso contrario
		else {
			for (Ghost* ghost : ghosts) ghost->death();
			return true;
		}
	}
}

//metodo para eliminar comida del GameMap cuando el Pacman pase por una casilla
bool Game::eraseCellFood(Point2D const point) {
	int prevFoodCount = gameMap->getFoodCount(); //pillamos la comida antes de interaccionar con la celda
	//compruebo que haya encontrado comida o vitamina
	bool eatsFood = gameMap->eraseCellContent(point.getXCoor(), point.getYCoor());
	if (prevFoodCount == gameMap->getFoodCount() + 1) //si ha comido comida
		infoBar->addPuntuation(); //aumentamos la puntuacion 
	if (gameMap->getFoodCount() == 0) exit = true; //si se ha comido todo, salimos (ganando)
	return eatsFood;
}

//metodo de impresion de la pantalla final
void Game::endScreen() {
	if (gameMap->getFoodCount() == 0 || *(pacman->getLivesPoint()) == 0) {
		SDL_RenderClear(renderer);
		Texture* endScreen;
		if (victory) endScreen = new Texture(renderer, "..\\images\\youwin.png");
		else endScreen = new Texture(renderer, "..\\images\\game_over.png");
		SDL_Rect dstRect;
		dstRect.w = winWidth; dstRect.h = winHeight;
		dstRect.x = dstRect.y = 0;
		endScreen->render(dstRect);
		SDL_RenderPresent(renderer);

		removeSaveFile();

		delete endScreen;

		SDL_Delay(endScreenTime);
	}
}

//metodo para vaciar el buffer de eventos 
void Game::emptyBuffer() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) //por si se sale al morir
		if (event.type == SDL_QUIT) exit = true;
}

//metodo para la carga de los elementos de juego
void Game::loadObjects(int level) {
	ifstream input("..\\savedGame\\GameMap.txt"); //comprobamos que haya partida guardada
	bool saveFile = false;
	if (!input.fail()) {
		input.close();
		input.open("..\\savedGame\\GameMap.txt");
		if (!input.is_open()) throw string("ERROR: Map save file isn't loading properly");
		saveFile = true; //indicamos que estamos cargando partida
	}
	else {
		string mapFile = "..\\maps\\level" + to_string(level) + ".dat";
		input.open(mapFile);
		if (!input.is_open()) throw "ERROR: map file " + mapFile + " not found";
	}

	//creacion del mapa
	int fils, cols;
	input >> fils >> cols;
	gameMap = new GameMap(fils, cols, this);

	//colocacion elementos mapa [x --> cols; y --> fils]
	int value;
	for (int i = 0; i < fils; i++) { //filas
		for (int j = 0; j < cols; j++) { //columnas
			input >> value;
			if (value < 4) gameMap->writeCell(j, i, value); //celdas
			else {
				if (value == 9) //pacman
					pacman = new Pacman(Point2D(j, i), textures[(int)TextureName::CharactersTexture], this, maxLives);
				else if (value != 4) //ghosts
					ghosts[value - 5] = new Ghost(Point2D(j, i), value - 5, textures[(int)TextureName::CharactersTexture], this);
				gameMap->writeCell(j, i, 0);
			}
		}
	}

	input.close();
	//si estamos cargando partida, cargamos estado de Pacman y Ghosts
	if (saveFile) loadFromFile();
	
	//asignamos tamaño celda en base a las filas y columnas
	cellHeight = winHeight / fils;
	cellWidth = (winWidth - 150) / cols;
}

//metodo para la carga del estado de Pacman y Ghosts de un archivo de guardado
void Game::loadFromFile() {
	ifstream input;
	input.open("..\\savedGame\\Pacman.txt");
	if (!input.is_open()) throw string("ERROR: Pacman save file isn't loading properly.");
	pacman = new Pacman(Point2D(0,0), textures[(int)TextureName::CharactersTexture], this, 3);
	pacman->load(input);
	input.open("..\\savedGame\\Ghosts.txt");
	if (!input.is_open()) throw string("ERROR: Ghosts save file isn't loading properly.");
	for (int i = 0; i < NUM_GHOSTS; ++i) {
		ghosts[i] = new Ghost(Point2D(0,0), i, textures[(int)TextureName::CharactersTexture], this);
		ghosts[i]->load(input);
	}
	input.close();
}

//metodo de actualizacion (actualiza Pacman y Ghostst en ese orden)
void Game::update() {
	pacman->update();
	for (Ghost* ghost : ghosts) ghost->update();
	SDL_Delay(timeFrame);
}

//metodo de renderizado (GameMap --> Pacman --> Ghosts)
void Game::render() const {
	SDL_RenderClear(renderer);
	gameMap->render();
	infoBar->render();
	pacman->render();
	for (Ghost* ghost : ghosts) ghost->render();
	SDL_RenderPresent(renderer);
}

//metodo para manejar eventos
void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if ((event.type == SDL_QUIT) || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
			exit = true;
			saveGame(); //si sale, guardamos partida
		}
		else pacman->handleEvent(event);
	}
}

//metodo para eliminar la partida guardada en caso de ganar/perder
void Game::removeSaveFile() {
	remove("..\\savedGame\\Pacman.txt");
	remove("..\\savedGame\\GameMap.txt");
	remove("..\\savedGame\\Ghosts.txt");
}