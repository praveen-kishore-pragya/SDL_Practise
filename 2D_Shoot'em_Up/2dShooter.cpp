//Command to Run - 
// g++ 2dShooter.cpp -o prog -lmingw32 -lSDL2main -lSDL2_image -lSDL2
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#define SCREEN_HEIGHT 720
#define SCREEN_WIDTH 1280


#define IMAGE_HEIGHT 100
#define IMAGE_WIDTH 100


void printError()
{
	std::cout << " Error occured: " << SDL_GetError() << std::endl;
}

int init()
{
    if( SDL_Init(SDL_INIT_EVERYTHING) != 0 )
    {
        std::cout << " Unable to initialize: " << SDL_GetError() << std::endl;

        return 1;
    }
    return 0;
}


SDL_Texture* createTexture(SDL_Renderer* renderer, std::string& filePath)
{
    SDL_Texture* texture = nullptr;

    // SDL_Surface* surface = SDL_LoadBMP(filePath.c_str());
	//NOTE - We will texture directly from the image, not with the surface although that's also a way


    texture = IMG_LoadTexture(renderer, filePath.c_str());
    // SDL_FreeSurface(surface);

    return texture;
}


void renderScreen(SDL_Renderer* renderer, SDL_Texture* texture, int x = 0, int y = 0, int w = SCREEN_WIDTH, int h = SCREEN_HEIGHT, int delay = 1000)
{
    SDL_Rect destination;
    destination.x = x;
    destination.y = y;
    destination.w = w;
    destination.h = h;

	
	
    
	SDL_RenderClear(renderer);



    SDL_RenderCopy(renderer, texture, NULL, &destination);
	
	SDL_RenderPresent(renderer);

    SDL_Delay(delay);

}

void cleanup(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Window* window)
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}


int main(int argc, char* argv[])
{
	if( init() == 1)
        return 1;
	//Initialise IMG - also add the header file
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    SDL_Window* window = SDL_CreateWindow("inputWindow",
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SCREEN_WIDTH,
                                            SCREEN_HEIGHT,
                                            SDL_WINDOW_SHOWN);
    
    

    SDL_Renderer* renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_ACCELERATED |
                                                SDL_RENDERER_PRESENTVSYNC);

    

	//color the window
	SDL_SetRenderDrawColor(renderer, 47, 77, 76, 255);

    
	std::string filePath = "./../resources/shoes.jpg";
    
    
    SDL_Texture* texture = createTexture(renderer, filePath);
    if(texture == nullptr)
    {
        std::cout << " Texture was not created: " << SDL_GetError() << std::endl;
        SDL_Quit();

        return 1;
    }

	SDL_Event event;
	bool quit = false;
	//initial position of the ship/shooter
	int directionX = 100;
	int directionY = 100;
	while( quit != true)
	{
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				quit = true;
			if(event.type == SDL_KEYDOWN)
			{
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE : quit = true;
										break;

					case SDLK_UP : directionY -= 5;
									//handling outbounds condition
									directionY = directionY < 0 ? directionY + 5 : directionY;
										break;

					case SDLK_DOWN : directionY += 5;
									//handling outbounds condition
									//NOTE : (SCREEN_HEIGHT - IMAGE_HEIGHT) checks that image stops fully before its front touches the boundary
									directionY = directionY > (SCREEN_HEIGHT - IMAGE_HEIGHT) ? directionY - 5 : directionY;
										break;

					case SDLK_LEFT : directionX -= 5;
									//handling outbounds condition
									directionX = directionX < 0 ? directionX + 5 : directionX;
										break;

					case SDLK_RIGHT : directionX += 5;
									//handling outbounds condition
									//NOTE : (SCREEN_HEIGHT - IMAGE_WIDTH) checks that image stops fully before its front touches the boundary
									directionX = directionX > (SCREEN_WIDTH - IMAGE_WIDTH) ? directionX - 5 : directionX;
										break;
					
							default :
										break;
				}
			}
		}
		//render screen
		renderScreen(renderer, texture, directionX, directionY, 100, 100, 16);

	}



	return 0;
}