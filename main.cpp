#include<Windows.h>
#include<SDL.h>
#include<string>
#include<SDL_image.h>

using namespace std;\

SDL_Surface* LoadImage(string file_path)
{
	SDL_Surface *load_image = NULL;
	SDL_Surface *optimize_image = NULL;

	load_image = IMG_Load(file_path.c_str());
	if (load_image != NULL)
	{
		optimize_image = SDL_DisplayFormat(load_image);
		SDL_FreeSurface(load_image);
	}

	return optimize_image;
}

int main(int arc, char *argv[])
{

	SDL_Surface *screen;
	SDL_Surface *image;

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return 1;

	screen = SDL_SetVideoMode(1920, 1080, 32, SDL_SWSURFACE);
	image = LoadImage("Background.png");

	SDL_BlitSurface(image, NULL, screen, NULL);

	SDL_Flip(screen);
	SDL_Delay(5000);
	SDL_FreeSurface(image);
	SDL_Quit();

	return 0;
}
