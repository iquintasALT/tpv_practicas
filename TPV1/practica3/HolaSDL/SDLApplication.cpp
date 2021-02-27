#include "SDLApplication.h"

SDLApplication::SDLApplication() {
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

	//creacion de texturas en base a un archivo
	readTextureFile();

	//inicializacion de la maquina de estados
	stateMachine = new GameStateMachine();
	stateMachine->pushState(new MainMenuState(this));
}

//destructora de SDLApplication
SDLApplication::~SDLApplication() {
	delete stateMachine; //eliminamos la maquina de estados
	//con el iterador, recorremos el mapa y eliminamos todas las texturas
	for (auto& it : textures) delete it.second;
	//eliminamos lo necesario de SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

//metodo que se ejecuta al presionar 'start' en el menu principal
void SDLApplication::startGame() {
	//creamos un PlayState por defecto (nivel 1)
	stateMachine->pushState(new PlayState(this, ""));
}

//metodo que se ejecuta tras presionar 'resume' en el menu de pausa
void SDLApplication::resumeGame() {
	//hacemos un popState de PauseState y volvemos al estado PlayState
	stateMachine->popState();
}

//metodo que se ejecuta tras presionar 'load' en el menu principal
void SDLApplication::loadGame() {
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Load File", //damos instrucciones al usuario
		"Insert the numeric value of your save file (don't use numPad). Then, press 'return'", nullptr);
	string path = getPath(); //obtenemos el codigo numerico del archivo
	try {
		stateMachine->pushState(new PlayState(this, path)); //creamos el PlayState en base al archivo
	}
	catch (PacmanError e) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "File error", e.what(), nullptr);
	}
}

//metodo que se ejecuta tras presionar 'save' en el menu de pausa
void SDLApplication::saveGame() {
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Save File", //damos instrucciones al usuario
		"Insert the numeric value of the file where you want your game to be saved. Then, press 'return'", nullptr);
	string path = getPath(); //obtenemos el codigo numerico del archivo
	stateMachine->popState(); //guardamos el estado de PlayState (por lo que salimos de PauseState)
	static_cast<PlayState*>(stateMachine->currentState())->save(path);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Confirmation",
		"Your game has been succesfully saved.", nullptr);
}

//metodo que se ejecuta tras presionar 'main menu' en el menu de pausa/pantalla final
void SDLApplication::returnMainMenu() {
	//borramos estados hasta llegar a MainMenuState
	while (dynamic_cast<MainMenuState*>(stateMachine->currentState()) == nullptr)
		stateMachine->popState();
}

//bucle principal de ejecucion de la aplicacion
void SDLApplication::run() {
	while (!exit) {
		//hacemos la actualizacion de los elementos del estado
		stateMachine->currentState()->update();
		render();
		handleEvents();
	}
}

//metodo de renderizado
void SDLApplication::render() {
	SDL_RenderClear(renderer);
	stateMachine->currentState()->render();
	SDL_RenderPresent(renderer);
}

//metodo de manejo de eventos
void SDLApplication::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) exit = true;
		else stateMachine->currentState()->handleEvent(event);
	}
}

//metodo para obtener un codigo numerico para lerr/esrcibir en un archivo
string SDLApplication::getPath() {
	string numPath = "";
	SDL_Event event;
	bool enter = false; //recogemos el input hasta que se presione 'return'
	while (SDL_PollEvent(&event) || !enter) {
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym < SDLK_9)
				numPath += to_string(event.key.keysym.sym - 48);
			else
				enter = (event.key.keysym.sym == SDLK_RETURN);
		}
	}

	return "s" + numPath + ".txt";
}

//metodo para cargar las texturas en base a un archivo
void SDLApplication::readTextureFile() {
	ifstream text; //abrimos flujo con el archivo de lectura
	text.open("textureInfo.txt");
	if (!text.is_open())
		throw FileNotFoundError("textureInfo.txt");

	int fils, cols;
	string directory;
	int countdown = 0;
	while (!text.eof()) { //recorremos el archivo
		text >> fils >> cols >> directory;
		//creamos nuevo nodo en el mapa con el TextureName y la textura acorde a los datos obtenidos
		textures.insert({ (TextureName)countdown, new Texture(renderer, directory, fils, cols) });
		countdown++;
	}
}