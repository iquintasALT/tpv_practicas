#include "Game.h"

#pragma region Static_Consts
const TextureAtribs Game::textureAtribs[NUM_TEXTURE] = { {"..\\images\\wall3.png",1,1}, {"..\\images\\food2.png",1,1},
					{"..\\images\\vitamins.png",1,1}, {"..\\images\\characters1.png",4,14}, {"..\\images\\digits.png",1,10},
					{"..\\images\\background.jpg",1,1}, {"..\\images\\heart.png",1,1}, {"..\\images\\battery.png",1,1},
					{"..\\images\\youwin.png", 1,1}, {"..\\images\\game_over.png",1,1} };
#pragma endregion


//constructora de Game
Game::Game() {
	//obtenemos la opcion elegida por el jugador
	string path = menu();
	//borramos la pantalla
	system("cls");

	//creacion de la ventana de juego
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Pac-Man", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		winWidth, winHeight, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//si ha habido algun problema en la creacion de la ventana de SDL, se lanza excepcion
	if (window == nullptr || renderer == nullptr)
		throw SDLError(SDL_GetError());

	//creacion de las texturas y barra informacion
	for (int i = 0; i < NUM_TEXTURE; i++)
		textures[i] = new Texture(renderer, textureAtribs[i].fileName, textureAtribs[i].fils, textureAtribs[i].cols);

	//si no hay que cargar
	if (path == "")
		//creacion de los objetos por defecto
		loadLevelFile(level);
	else { //si hay que cargar
		try {
			loadFromFile(path);
		}
		catch (PacmanError const e) { //si falla la carga
			cout << e.what() << endl;
			//nos recuperamos del error cargando el nivel 1
			level = 1;
			loadLevelFile(level);
		}
	}

	//creacion de la barra de informacion 
	infoBar = new InfoBar(this, &lives, pacman->getEnergyRef(), maxFood - foodCount,
		Point2D(winWidth - infoBarWidth, 0), Point2D(infoBarWidth, winHeight));
	gObjs.push_back(infoBar);

	//asignacion de variables de control
	exit = false;
	srand(time(nullptr)); //semilla aleatoria
}

//destructora de Game
Game::~Game() {
	//borramos los elementos de la lista
	for (auto elem : gObjs) delete elem;
	//eliminamos los elementos de premios
	for (auto elem : prizes) delete elem;
	//vaciamos las listas
	gObjs.clear();
	ghosts.clear();
	prizes.clear();
	objsToErase.clear();
	for (Texture*& elem : textures) delete elem;
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
	//mostramos por pantalla si el jugador ha ganado o perdido
	endScreen();
}

//metodo para guardar la partida, guardando un fichero con todos los datos necesarios
void Game::save(string const saveFile){
	cout << "Insert save file number:" << endl;
	int saveNum;
	cin >> saveNum;

	ofstream save;
	save.open(saveFile + to_string(saveNum) + ".txt");
	save << level << " " << maxFood << " " << lives << " " << cellSize << endl;


	gameMap->saveToFile(save); //guardamos archivo GameMap
	save << gObjs.size() - 2 << endl; //-2 para eliminar infobar y gamemap del contador de elementos en el mapa
	//guardamos los GameCharacters
	for (auto obj : gObjs) {
		GameCharacter* gm = dynamic_cast<GameCharacter*>(obj);
		if (gm != nullptr)
			gm->saveToFile(save);
	}
	//cerramos el archivo
	save.close();
}

//metodo que crea una recompensa entre las disponibles de forma aleatoria
void Game::createPrize(SDL_Rect& const ghostPos, Point2D dir) {
	//obtenemos la poscion del fantasma
	Point2D ghostCell = gameMap->SDLPointToMapCoors(Point2D(ghostPos.x + (ghostPos.w / 2), ghostPos.y + (ghostPos.h / 2)));
	//obtenemos la posicion de la recompensa a partir de la posicion del ghost
	Point2D prizePos = gameMap->MapCoorsToSDLPoint(ghostCell - dir);

	//si no coincide con una pared
	if (!gameMap->intersectWall(SDL_Rect{ prizePos.getXCoor(), prizePos.getYCoor(), cellSize.getYCoor(), cellSize.getXCoor() })) {
		Prize* prize;
		//creamos la recompensa, con distinta probabilidad segun el tipo
		if (rand() % 100 <= 80)
			prize = new Prize(prizePos, this, Point2D(0, 0), cellSize, PrizeType::ExtraLive);
		else
			prize = new Prize(prizePos, this, Point2D(0, 0), cellSize, PrizeType::NextLevel);

		//la añadimos a la lista de recompensas
		prizes.push_back(prize);
		
		//establecemos un tiempo hasta poder crear otra recompensa
		timePrice = maxTimePrice;
	}
}

//metodo para comprobar que en la posicion de Pacman haya o no un Ghost
bool Game::collisionGhost(SDL_Rect& const pacmanPos) {
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
				if (lives == 0) exit = true;
				return true;
			}
			return false;
		}
	}
}

//metodo que comprueba si el pacman puede moverse en ciera direccion
bool Game::tryMove(SDL_Rect& const charRect, Vector2D dir, Point2D& newPos) {
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
void Game::cellInteraction(SDL_Rect& const rectPac) {
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
void Game::acquirePrize() {
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

//metodo que comprueba la distancia maxima y minima entre un fantasma y el pacman
void Game::checkModule(Point2D& dir, Point2D& const currentPos, vector<Point2D>& const posDirs, vector<Point2D>& const posMoves) {
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
bool Game::reproductionSmartGhost(SmartGhost* const ghost) {
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
					bornPlace.getYCoor() * cellSize.getYCoor()), this, Point2D(0, 8), cellSize));
				//se pone en cuarentena durante cierto tiempo
				sm->changeState(State::Quarantine);
				ghosts.back()->setItList(gObjs.insert(gObjs.end(), ghosts.back()));
			}
		}
		//si es un fantasma normal
		else {
			int index = rand() % 4; //color aleatorio
			//creacion del fantasma normal
			ghosts.push_back(new Ghost(Point2D(bornPlace.getXCoor() * cellSize.getXCoor(),
				bornPlace.getYCoor() * cellSize.getYCoor()), this, Point2D(0, index * 2), cellSize));
			ghosts.back()->setItList(gObjs.insert(gObjs.end(), ghosts.back()));
		}
	}
	return reproduced;
}

//metodo que anyade un fantasma a la lista de objetos que seran borrados
void Game::eraseGhost(list<GameObject*>::iterator it, Ghost* const ghost) {
	ghosts.remove(ghost);
	objsToErase.push_back(it);
}

//metodo para la carga de los elementos de juego
void Game::loadLevelFile(int const level) {
	ifstream input;
	string mapFile = "..\\maps\\level" + to_string(level) + ".dat";
	input.open(mapFile);
	if (!input.is_open())
		throw FileNotFoundError(mapFile);

	int fils, cols;
	input >> fils >> cols;

	//asignamos tamaño celda en base a las filas y columnas
	cellSize = Point2D(winHeight / fils, (winWidth - infoBarWidth) / cols);

	//creacion del mapa
	gameMap = new GameMap(fils, cols, this);
	gObjs.push_back(gameMap);

	//colocacion elementos mapa [x --> cols; y --> fils]
	int value;
	for (int i = 0; i < fils; i++) { //filas
		for (int j = 0; j < cols; j++) { //columnas
			input >> value;
			if (value < 4) gameMap->writeCell(j, i, value, foodCount); //celdas
			else {
				if (value == 9) { //pacman
					pacman = new Pacman(Point2D(j * cellSize.getXCoor(), i * cellSize.getYCoor()), this, Point2D(0, 10), cellSize);
					pacman->setItList(gObjs.insert(gObjs.end(), pacman));
				}
				else if (value > 4 && value < 9) { //ghosts
					ghosts.push_back(new Ghost(Point2D(j * cellSize.getXCoor(), i * cellSize.getYCoor()), this, Point2D(0, (value - 5) * 2), cellSize));
					ghosts.back()->setItList(gObjs.insert(gObjs.end(), ghosts.back()));
				}
				else { //smartghosts
					ghosts.push_back(new SmartGhost(Point2D(j * cellSize.getXCoor(), i * cellSize.getYCoor()), this, Point2D(0, 8), cellSize));
					ghosts.back()->setItList(gObjs.insert(gObjs.end(), ghosts.back()));
				}
				gameMap->writeCell(j, i, 0, foodCount);
			}
		}
	}

	maxFood += foodCount;

	input.close();
}

//metodo para la carga del estado de Pacman y Ghosts de un archivo de guardado
void Game::loadFromFile(string const path) {
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
	gameMap = new GameMap(loadFile, this);
	gObjs.push_back(gameMap);

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
			pacman = new Pacman(loadFile, this);
			pacman->setItList(gObjs.insert(gObjs.end(), pacman));
		}
		else if (identifier == 'G') { //ghosts
			ghosts.push_back(new Ghost(loadFile, this));
			ghosts.back()->setItList(gObjs.insert(gObjs.end(), ghosts.back()));
		}
		else if (identifier == 'S') { //smartghosts
			ghosts.push_back(new SmartGhost(loadFile, this));
			ghosts.back()->setItList(gObjs.insert(gObjs.end(), ghosts.back()));
		}
		else
			throw FileFormatError("Wrong object identifier. Line " + to_string(lineCount) + ", file " + path + ".");
		numObjs--; lineCount++;
	}

	//cerramos el archivo
	loadFile.close();
}

//metodo para eliminar comida del GameMap cuando el Pacman pase por una casilla
bool Game::eraseCellFood(SDL_Rect& const point) {
	int prevFoodCount = foodCount; //pillamos la comida antes de interaccionar con la celda
	//compruebo que haya encontrado comida o vitamina
	bool eatsFood = gameMap->eraseCellContent(point, foodCount);
	if (prevFoodCount == foodCount + 1) //si ha comido comida
		infoBar->addPuntuation(); //aumentamos la puntuacion 
	return eatsFood;
}

//metodo para vaciar el buffer de eventos 
void Game::emptyBuffer() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) //por si se sale al morir
		if (event.type == SDL_QUIT) exit = true;
}

//metodo de actualizacion (actualiza Pacman y Ghostst en ese orden)
void Game::update() {
	//actualiza los gObs
	for (auto elem : gObjs) elem->update();
	//se borran los objetos apuntados para ser borrados
	for (auto elem : objsToErase) {
		delete* elem;
		gObjs.erase(elem);
	}
	//vaciamos la lista
	objsToErase.clear();
	//si se ha completado el nivel
	if (foodCount == 0) {
		//compruebo si hay siguiente nivel cuando se ha ganado
		ifstream levelPath;
		levelPath.open("..\\maps\\level" + to_string(level + 1) + ".dat");
		//compruebo que existe el siguiente nivel
		if (levelPath.is_open()) {
			levelPath.close();
			loadNextLevel(level + 1);
		}
		//en caso contrario, salimos del juego
		else exit = true;
	}
	//tiempo de espera entre updates
	SDL_Delay(timeFrame);
}

//metodo de renderizado (GameMap --> Pacman --> Ghosts --> Prizes)
void Game::render() const {
	SDL_RenderClear(renderer);
	for (auto elem : gObjs) elem->render();
	for (auto elem : prizes) elem->render();
	SDL_RenderPresent(renderer);
}

//metodo para manejar eventos
void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		//si se sale del juego
		if ((event.type == SDL_QUIT))
			exit = true;
		//si se quiere guardar la partida
		else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s) {
			save("..\\savedGame\\s");
			exit = true;
		}
		//comprobamos el input del Pacman
		else pacman->handleEvent(event);
	}
}

//metodo para cargar el siguiente nivel
void Game::loadNextLevel(int const level) {
	//se destruye los objetos de juego
	destroyObjects();
	//cargamos el siguiente nivel
	loadLevelFile(level);
	//creacion de la barra de informacion 
	infoBar->setPowerUpRef(pacman->getEnergyRef());
	gObjs.push_back(infoBar);
}

//metodo para mostrar un menu en consola al comienzo del juego
string Game::menu() const{
	//mostramos las opciones del menu
	cout << "PACMAN:\n1. NEW GAME\n2. LOAD GAME\n";

	//leemos la opcion
	int opcion;
	do {
		cin >> opcion;
	} while (opcion != 1 && opcion != 2);

	if (opcion == 1)
		return "";
	else {
		cout << "Insert file name:" << endl;
		string path;
		cin >> path;
		return path + ".txt";
	}
}

//metodo privado para simular el movimiento toroidal
void Game::toroidalMove(Point2D& nextPos, Point2D const size, SDL_Rect& const map) {
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
void Game::destroyObjects() {
	for (auto obj : gObjs) { //eliminamos todo salvo infobar
		GameCharacter* gm = dynamic_cast<GameCharacter*>(obj);
		if (gm != nullptr)
			delete obj;
	}
	delete gameMap; //eliminamos mapa
	//eliminamos premios
	for (auto elem : prizes) delete elem;
	//vaciamos las listas
	gObjs.clear();
	ghosts.clear();
	prizes.clear();
	objsToErase.clear();
}

//metodo de impresion de la pantalla final
void Game::endScreen() const{
	//si ha terminado el juego
	if ((foodCount <= 0 || lives <= 0)) {
		//se borra la pantalla de la ventana de SDL
		SDL_RenderClear(renderer);
		Texture* endScreen;
		//si se ha ganado
		if (foodCount == 0) endScreen = getTexture(TextureName::WinTexture);
		//si se ha perdido
		else endScreen = getTexture(TextureName::LoseTexture);
		//se muestra en pantalla el resultado
		SDL_Rect dstRect;
		dstRect.w = winWidth; dstRect.h = winHeight;
		dstRect.x = dstRect.y = 0;
		endScreen->render(dstRect);
		SDL_RenderPresent(renderer);
		SDL_Delay(endScreenTime);
	}
}