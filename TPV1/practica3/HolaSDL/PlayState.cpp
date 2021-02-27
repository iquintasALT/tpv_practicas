#include "PlayState.h"
#include "SDLApplication.h"

//constructora de PlayState
PlayState::PlayState(SDLApplication* appPtr, string const path) : GameState(appPtr) {
	if (path == "")
		//creacion de los objetos por defecto
		loadLevelFile(level);
	else //carga de archivo (puede fallar)
		loadFromFile(path);

	//creacion de la barra de informacion 
	infoBar = new InfoBar(app, &lives, pacman->getEnergyRef(), maxFood - foodCount,
		Point2D(winWidth - infoBarWidth, 0), Point2D(infoBarWidth, winHeight));
	stage.push_back(infoBar);

	srand(time(nullptr)); //semilla aleatoria
}

//destructora de PlayState
PlayState::~PlayState() {
	//eliminamos los elementos de premios
	for (auto elem : prizes) delete elem;
	//vaciamos las listas, aunque no es necesario porque se sale de ambito y se destruyen
	ghosts.clear();
	prizes.clear();
	objsToErase.clear();
}

//metodo para la carga de los elementos del juego de un nivel
void PlayState::loadLevelFile(int const level) {
	ifstream input;
	//guardamos la ruta del nivel
	string mapFile = "..\\maps\\level" + to_string(level) + ".dat";
	input.open(mapFile);
	if (!input.is_open())
		throw FileNotFoundError(mapFile);

	int fils, cols;
	input >> fils >> cols;

	//asignamos tamaño celda en base a las filas y columnas
	cellSize = Point2D(winHeight / fils, (winWidth - infoBarWidth) / cols);

	//creacion del mapa
	gameMap = new GameMap(fils, cols, app);
	stage.push_back(gameMap);

	//colocacion elementos mapa [x --> cols; y --> fils]
	//cada GameObject creado se añade a la lista de GameObjects stage de GameState
	//cada EventHandler creado se añade a la lista de EventHandlers eventHandlers de GameState
	int value;
	for (int i = 0; i < fils; i++) { //filas
		for (int j = 0; j < cols; j++) { //columnas
			input >> value;
			if (value < 4) gameMap->writeCell(j, i, value, foodCount); //celdas
			else {
				if (value == 9) { //pacman
					pacman = new Pacman(Point2D(j * cellSize.getXCoor(), i * cellSize.getYCoor()), app, Point2D(0, 10), cellSize, this);
					pacman->setItList(stage.insert(stage.end(), pacman));
					eventHandlers.insert(eventHandlers.end(), pacman);
				}
				else if (value > 4 && value < 9) { //ghosts
					ghosts.push_back(new Ghost(Point2D(j * cellSize.getXCoor(), i * cellSize.getYCoor()), app, Point2D(0, (value - 5) * 2), cellSize, this));
					ghosts.back()->setItList(stage.insert(stage.end(), ghosts.back()));
				}
				else { //smartghosts
					ghosts.push_back(new SmartGhost(Point2D(j * cellSize.getXCoor(), i * cellSize.getYCoor()), app, Point2D(0, 8), cellSize, this));
					ghosts.back()->setItList(stage.insert(stage.end(), ghosts.back()));
				}
				gameMap->writeCell(j, i, 0, foodCount);
			}
		}
	}
	//asignamos la comida maxima del nivel, guardando toda la comida que se ha obtenido en niveles anteriores
	maxFood += foodCount;

	input.close();
}

//metodo para la carga del estado de Pacman y Ghosts de un archivo de guardado
void PlayState::loadFromFile(string const path) {
	ifstream loadFile;
	//lanzamos excepcion si no se encuentra el archivo
	loadFile.open("..\\savedGame\\" + path);
	if (!loadFile.is_open())
		throw FileNotFoundError(path);

	int lineCount = 1;
	loadFile >> level >> maxFood >> lives >> cellSize;
	//lanzamos excepcion si la lectura no ha sido correcta
	if (!loadFile)
		throw FileFormatError("Integer expected. Line " + to_string(lineCount) + ", file " + path + ".");
	//creacion del mapa
	gameMap = new GameMap(loadFile, app);
	stage.push_back(gameMap);

	for (int i = 0; i < gameMap->getFils(); i++) { //filas
		for (int j = 0; j < gameMap->getCols(); j++) { //columnas
			int value;
			loadFile >> value;
			if (!loadFile)
				throw FileFormatError("Integer expected. Line " + to_string(lineCount) + ", file " + path + ".");
			if (value < 4) gameMap->writeCell(j, i, value, foodCount); //celdas
		}
		lineCount++;
	}

	int numObjs;
	loadFile >> numObjs;
	while (numObjs > 0) {
		char identifier;
		//segun su identificador, creamos un GameCharacter distinto
		loadFile >> identifier;
		if (identifier == 'P') { //pacman
			pacman = new Pacman(loadFile, app, this);
			pacman->setItList(stage.insert(stage.end(), pacman));
			eventHandlers.insert(eventHandlers.end(), pacman);
		}
		else if (identifier == 'G') { //ghosts
			ghosts.push_back(new Ghost(loadFile, app, this));
			ghosts.back()->setItList(stage.insert(stage.end(), ghosts.back()));
		}
		else if (identifier == 'S') { //smartghosts
			ghosts.push_back(new SmartGhost(loadFile, app, this));
			ghosts.back()->setItList(stage.insert(stage.end(), ghosts.back()));
		}
		else
			throw FileFormatError("Wrong object identifier. Line " + to_string(lineCount) + ", file " + path + ".");
		numObjs--; lineCount++;
	}

	//cerramos el archivo
	loadFile.close();
}


//metodo para guardar la partida, guardando un fichero con todos los datos necesarios
void PlayState::save(string const saveFile) {
	ofstream save;
	save.open("..//savedGame//" + saveFile);
	save << level << " " << maxFood << " " << lives << " " << cellSize << endl;


	gameMap->saveToFile(save); //guardamos archivo GameMap
	save << stage.size() - 2 << endl; //-2 para eliminar infobar y gamemap del contador de elementos en el mapa
	//guardamos los GameCharacters
	for (auto obj : stage) {
		GameCharacter* gm = dynamic_cast<GameCharacter*>(obj);
		if (gm != nullptr)
			gm->saveToFile(save);
	}
	//cerramos el archivo
	save.close();
}

void PlayState::update() {
	//actualiza los gObs
	GameState::update();
	//se borran los objetos apuntados para ser borrados
	for (auto elem : objsToErase) {
		delete* elem;
		stage.erase(elem);
	}
	//vaciamos la lista
	objsToErase.clear();
	//si se ha completado el nivel
	if (foodCount == 0) {
		//si hay mas niveles jugables
		if (level < maxLevels) {
			//compruebo si hay siguiente nivel cuando se ha ganado
			ifstream levelPath;
			levelPath.open("..\\maps\\level" + to_string(level + 1) + ".dat");
			//compruebo que existe el siguiente nivel
			if (levelPath.is_open()) {
				levelPath.close();
				loadNextLevel();
			}
			//en caso contrario, salimos del juego
			//else exit = true; //CORREGIR, push estado de fin de juego
		} //en caso contrario se termina el juego ganando
		else app->endGame(true);
	}
	//si el pacman muere termina el juego perdiendo
	if (lives <= 0) {
		app->endGame(false);
	}

	//tiempo de espera entre updates
	SDL_Delay(timeFrame);
}

//metodo para comprobar que en la posicion de Pacman haya o no un Ghost
bool PlayState::collisionGhost(SDL_Rect& const pacmanPos) {
	list<Ghost*>::const_iterator it = ghosts.begin(); //buscamos si coincide con alguno
	while (it != ghosts.end() && !(SDL_HasIntersection(&pacmanPos, &(*it)->getDestRect()))) ++it;

	//si no coincide
	if (it == ghosts.end()) return false;
	else {
		//si coincide pero tiene powerUp
		if (pacman->getEnergy() != 0) {
			//llamamos al metodo para crear un premio con cierta probabilidad
			if (rand() % 100 <= 70 && timePrice <= 0) createPrize((*it)->getDestRect(), (*it)->getDir());
			//matamos al Ghost
			(*it)->death();
			return false;
		}
		//en caso contrario
		else {
			SmartGhost* sm = dynamic_cast<SmartGhost*>((*it));
			//si es fantasma o es fantasma evolucionado no ninyo
			if (sm == nullptr || (sm != nullptr && sm->getState() != State::Child)) {
				lives--;
				return true;
			}
			return false;
		}
	}
}

//metodo que comprueba si el pacman puede moverse en ciera direccion
bool PlayState::tryMove(SDL_Rect& const charRect, Vector2D dir, Point2D& newPos) {
	//obtenemos las dimensiones del mapa
	SDL_Rect map = gameMap->getDestRect();
	//calculamos la nueva posicion
	newPos = Point2D(charRect.x, charRect.y) + dir;
	//se realiza un moviemiento toroidal en caso de ser posible
	toroidalMove(newPos, Point2D(charRect.w, charRect.h), map);
	//se devuelve un booleano indicando si el movimiento es posible o no
	return !(gameMap->intersectWall(SDL_Rect({ newPos.getXCoor(), newPos.getYCoor(), charRect.w, charRect.h })));
}

//metodo de interaccion con la celda
void PlayState::cellInteraction(SDL_Rect& const rectPac) {
	if (!collisionGhost(rectPac)) { //si no colisiona con un fantasma
		//si obtuvo vitamina, activamos powerUp y actualizamos frame
		if (eraseCellFood(rectPac)) {
			pacman->setEnergy(maxEnergy);
			pacman->setNewFrame(11);
		}
		//comprobamos si se esta intersectando con una recompensa
		acquirePrize();
	}
	//en caso contrario, el pacman muere
	else pacman->death();
}

//metodo que gestiona que efectos tienen las recompensas según su tipo
void PlayState::acquirePrize() {
	list<Prize*>::iterator it = prizes.begin();
	//busco si el pacman esta interseccionando con alguna recompensa
	while (it != prizes.end() && !SDL_HasIntersection(&pacman->getDestRect(), &(*it)->getDestRect())) ++it;

	//si se ha encontrado alguna recompensa
	if (it != prizes.end()) {
		//obtener una vida extra
		if ((*it)->getPrizeType() == PrizeType::ExtraLive) {
			if (lives < 3) lives++;
		}
		//pasar al siguiente nivel
		else foodCount = 0;
		//borramos el itreador y la recompensa
		delete (*it);
		prizes.erase(it);
	}
	//restamos al contador
	if (timePrice > 0) timePrice--;
}

//metodo que crea una recompensa entre las disponibles de forma aleatoria
void PlayState::createPrize(SDL_Rect& const ghostPos, Point2D dir) {
	//obtenemos la poscion del fantasma
	Point2D ghostCell = gameMap->SDLPointToMapCoors(Point2D(ghostPos.x + (ghostPos.w / 2), ghostPos.y + (ghostPos.h / 2)));
	//obtenemos la posicion de la recompensa a partir de la posicion del ghost
	Point2D prizePos = gameMap->MapCoorsToSDLPoint(ghostCell - dir);

	//si no coincide con una pared
	if (!gameMap->intersectWall(SDL_Rect{ prizePos.getXCoor(), prizePos.getYCoor(), cellSize.getYCoor(), cellSize.getXCoor() })) {
		Prize* prize;
		//creamos la recompensa, con distinta probabilidad segun el tipo
		if (rand() % 100 <= 70)
			prize = new Prize(prizePos, app, Point2D(0, 0), cellSize, PrizeType::ExtraLive, this);
		else
			prize = new Prize(prizePos, app, Point2D(0, 0), cellSize, PrizeType::NextLevel, this);

		//la añadimos a la lista de recompensas
		prizes.push_back(prize);

		//establecemos un tiempo hasta poder crear otra recompensa
		timePrice = maxTimePrice;
	}
}

//metodo que comprueba la distancia maxima y minima entre un fantasma y el pacman
void PlayState::checkModule(Point2D& dir, Point2D& const currentPos, vector<Point2D>& const posDirs, vector<Point2D>& const posMoves) {
	//coordenadas del fantasma
	Point2D mapCoors = gameMap->SDLPointToMapCoors(currentPos);
	//coordenadas del pacman
	Point2D posPac = Point2D(pacman->getDestRect().x, pacman->getDestRect().y);
	//valores exagerados para que se asignen en el bucle
	float min = INT_MAX, max = INT_MIN;
	int minIndex = -1, maxIndex = -1;
	for (int i = 0; i < posDirs.size(); i++) { //obtenemos el movimiento de menor y mayor distancia
		Point2D sdlPoint = gameMap->MapCoorsToSDLPoint(mapCoors + posDirs[i]);
		sdlPoint -= posPac;
		float modulo = sdlPoint.modulo();
		if (modulo < min) {
			min = modulo;
			minIndex = i;
		}
		if (modulo > max) {
			max = modulo;
			maxIndex = i;
		}
	}

	if (pacman->getEnergy() > 0) { //si tiene el powerUp activado, se queda con el camino de mayor longitud
		dir = posDirs[maxIndex];
		currentPos = posMoves[maxIndex];
	}
	else { //si tiene el powerUp desactivado, se queda con el camino de mayor longitud
		dir = posDirs[minIndex];
		currentPos = posMoves[minIndex];
	}
}

//metodo que comprueba si un fantasma evolucionado puede reproducirse con otro y en ese caso se reproduce
bool PlayState::reproductionSmartGhost(SmartGhost* const ghost) {
	list<Ghost*>::const_iterator it = ghosts.begin(); //buscamos si coincide con alguno
	bool reproduced = false;
	//comprobamos si se puede reproducir con otro fantasma que no sea el mismo
	while (it != ghosts.end() && !reproduced) {
		if ((*it) != ghost)
			reproduced = SDL_HasIntersection(&(ghost->getDestRect()), &((*it)->getDestRect()));
		if (!reproduced) ++it;
	}

	//si se reproduce
	if (reproduced) {
		//obtenemos su punto de nacimiento
		Point2D bornPlace = gameMap->SDLPointToMapCoors(Point2D(ghost->getDestRect().x, ghost->getDestRect().y));
		SmartGhost* sm = dynamic_cast<SmartGhost*>((*it));
		//si es fantasma o es fantasma evolucionado no ninyo
		if (sm != nullptr) {
			if (sm->getState() != State::Quarantine && sm->getState() != State::Dead) {
				//creacion del fantasma evolucionado
				ghosts.push_back(new SmartGhost(Point2D(bornPlace.getXCoor() * cellSize.getXCoor(),
					bornPlace.getYCoor() * cellSize.getYCoor()), app, Point2D(0, 8), cellSize, this));
				//se pone en cuarentena durante cierto tiempo
				sm->changeState(State::Quarantine);
				ghosts.back()->setItList(stage.insert(stage.end(), ghosts.back()));
			}
		}
		//si es un fantasma normal
		else {
			int index = rand() % 4; //color aleatorio
			//creacion del fantasma normal
			ghosts.push_back(new Ghost(Point2D(bornPlace.getXCoor() * cellSize.getXCoor(),
				bornPlace.getYCoor() * cellSize.getYCoor()), app, Point2D(0, index * 2), cellSize, this));
			ghosts.back()->setItList(stage.insert(stage.end(), ghosts.back()));
		}
	}
	return reproduced;
}

//metodo que anyade un fantasma a la lista de objetos que seran borrados
void PlayState::eraseGhost(list<GameObject*>::iterator it, Ghost* const ghost) {
	ghosts.remove(ghost);
	objsToErase.push_back(it);
}


//metodo para eliminar comida del GameMap cuando el Pacman pase por una casilla
bool PlayState::eraseCellFood(SDL_Rect& const point) {
	int prevFoodCount = foodCount; //pillamos la comida antes de interaccionar con la celda
	//compruebo que haya encontrado comida o vitamina
	bool eatsFood = gameMap->eraseCellContent(point, foodCount);
	if (prevFoodCount == foodCount + 1) //si ha comido comida
		infoBar->addPuntuation(); //aumentamos la puntuacion 
	return eatsFood;
}

//metodo para vaciar el buffer de eventos 
void PlayState::emptyBuffer() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) //por si se sale al morir
		if (event.type == SDL_QUIT) app->exitGame();
}

//metodo de renderizado (GameMap --> Pacman --> Ghosts --> Prizes)
void PlayState::render() const {
	GameState::render();
	for (auto elem : prizes) elem->render();
}

//metodo para manejar eventos
void PlayState::handleEvent(SDL_Event& event) {
	//si se quiere guardar la partida
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
		app->pauseGame();
	//comprobamos el input de los EventHandlers (Pacman)
	else GameState::handleEvent(event);
}

//metodo para cargar el siguiente nivel
void PlayState::loadNextLevel() {
	//se destruye los objetos de juego
	destroyObjects();
	//aumentamos el contador de nivel
	level++;
	//cargamos el siguiente nivel
	loadLevelFile(level);
	//creacion de la barra de informacion 
	infoBar->setPowerUpRef(pacman->getEnergyRef());
	stage.push_back(infoBar);

}

//metodo privado para simular el movimiento toroidal
void PlayState::toroidalMove(Point2D& nextPos, Point2D const size, SDL_Rect& const map) {
	//vuelta en la X (columnas)
	if (nextPos.getXCoor() < map.x)
		nextPos.setXCoor((map.x + map.w) - size.getXCoor());
	else if (nextPos.getXCoor() + size.getXCoor() > map.x + map.w)
		nextPos.setXCoor(map.x);
	//vuelta en la Y (filas)
	else if (nextPos.getYCoor() < map.y)
		nextPos.setYCoor((map.y + map.h) - size.getYCoor());
	else if (nextPos.getYCoor() + +size.getYCoor() > map.y + map.h)
		nextPos.setYCoor(map.y);
}

//metodo que destruye los objetos del juego
void PlayState::destroyObjects() {
	for (auto obj : stage) { //eliminamos todo salvo infobar
		GameCharacter* gm = dynamic_cast<GameCharacter*>(obj);
		if (gm != nullptr)
			delete obj;
	}
	delete gameMap; //eliminamos mapa
	//eliminamos premios
	for (auto elem : prizes) delete elem;
	//vaciamos las listas
	stage.clear();
	eventHandlers.clear();
	ghosts.clear();
	prizes.clear();
	objsToErase.clear();
}