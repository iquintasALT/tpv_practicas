#include "MenuButton.h"

//metodo para manejo de eventos por ser EventHandler
bool MenuButton::handleEvent(SDL_Event& const event) {
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		//obtengo la posición del ratón
		int x, y;
		SDL_GetMouseState(&x, &y);
		const SDL_Point mousePoint{ x, y };
		//obtengo la posicion del boton
		const SDL_Rect buttonRect = getDestRect();
		//si el cursor del raton esta dentro del boton
		if (SDL_PointInRect(&mousePoint, &buttonRect)) {
			//se llama a su callback
			callback(app);
			//se devuelve true para no llamar a otros metodos handleEvents
			return true;
		}

		return false;
	}
}

//metodo que cambia el frame del boton si el cursor del raton esta encima
void MenuButton::update() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	const SDL_Point mousePoint{ x, y };
	const SDL_Rect buttonRect = getDestRect();
	if (SDL_PointInRect(&mousePoint, &buttonRect)) {
		frameY = 1;
	}
	else frameY = 0;
}
