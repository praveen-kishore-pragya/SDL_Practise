//Command to Run -
// g++ 2dShooter.cpp -o prog -lpthread -lmingw32 -lSDL2main -lSDL2_image -lSDL2
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//creeating thread to maintain movement of fired bullet on another thread,
//bcoz the movemnt of thread will be in a loop, and it will hinder other activities of the main thread
// #include <thread>
#include <pthread.h>

#include <vector>

//for pairs
#include <utility>

#define SCREEN_HEIGHT 720
#define SCREEN_WIDTH 1280


#define IMAGE_HEIGHT 100
#define IMAGE_WIDTH 100

#define BULLET_VELOCITY 3
#define TERRORIST_VELOCITY 3


void printError()
{
	std::cout << " Error occured: " << SDL_GetError() << std::endl;
}

void checkPointer(void* ptr)
{
	if(ptr == nullptr)
		printError();
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



// void moveBullets (SDL_Rect* bullet, vector<void*> args)
// {
//     while(bullet->w < (SCREEN_WIDTH - bullet->w))
//     {
//         bullet->x += BULLET_VELOCITY;
//         SDL_RenderCopy(renderer, textureBullet, NULL, bullet);

//         SDL_RenderPresent(renderer);
//     }
// }



void renderScreen(SDL_Renderer* renderer, SDL_Texture* texturePolice,  SDL_Texture* textureBullet, std::vector< std::pair<int,int> >& bulletList, SDL_Texture* textureTerrorist, std::vector< std::pair<int,int> >& terroristList, int x = 0, int y = 0, int w = SCREEN_WIDTH, int h = SCREEN_HEIGHT, int delay = 1000, bool fire = false)
{
	//creating police
    SDL_Rect police;
    police.x = x;
    police.y = y;
    police.w = w;
    police.h = h;


	SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texturePolice, NULL, &police);



    //NOTE : Wrap only till adding a new bullet, when pressed SPACE or fire == true
	if(fire == true)
	{
		//creating bullet
		//height and width of bullet is hard coded to be 10 each
        // Initialize bullet position
		int bulletX = police.x + w;
        int bulletY = police.y + police.h / 2;

        bulletList.push_back( std::make_pair(bulletX, bulletY) );

    }


    //Now, for each bullet, create a bullet SDL_Rect and render it on the screen

    for(auto it = bulletList.begin(); it != bulletList.end();)
    {
        SDL_Rect bullet;
        bullet.x = it->first; //NEVER assign bulletX, as this is a list of bullets new and old, so initialise them from the vector of pairs only
        bullet.y = it->second; //NEVER assign bulletY, as this is a list of bullets new and old, so initialise them from the vector of pairs only
        bullet.w = 10;
        bullet.h = 10;

        //render the created bullet
        SDL_RenderCopy(renderer, textureBullet, NULL, &bullet);

        //update the position at each frame or, render for a bullet
        it->first += BULLET_VELOCITY;

        //Now, check whether the movement of bullet is within the bounds or not
        if (it->first < SCREEN_WIDTH - bullet.w)
        {
            //Go to next bullet
            it++;
        }
        else
        {
            bulletList.erase(it);
        }
    }


    //rendering terrorist
    //initial position of terrorist
    static int enemyCreationTimer = 0;
    if(--enemyCreationTimer <= 0)
    {
        terroristList.push_back(std::make_pair(10 * police.x, rand() % (SCREEN_HEIGHT - h) ));
        enemyCreationTimer = 30 + rand() % 60;
    }

    for(auto it = terroristList.begin(); it != terroristList.end();)
    {
        SDL_Rect terrorist;

        terrorist.x = it->first;
        terrorist.y = it->second;
        terrorist.w = w;
        terrorist.h = w;

        SDL_RenderCopy(renderer, textureTerrorist, NULL, &terrorist);

        it->first -= TERRORIST_VELOCITY;

        if(it->first > w)
        {
            it++;
        }
        else
        {
            terroristList.erase(it);
        }
    }


    SDL_RenderPresent(renderer);

    SDL_Delay(delay);

}

void cleanup(SDL_Renderer* renderer, SDL_Texture* texturePolice, SDL_Texture* textureBullet, SDL_Window* window)
{
    SDL_DestroyTexture(texturePolice);
    SDL_DestroyTexture(textureBullet);
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
                                            SDL_WINDOW_SHOWN);  //can put  | SDL_WINDOW_RESIZABLE, BUT then have to calculate the resized window for character movement



    SDL_Renderer* renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_ACCELERATED |
                                                SDL_RENDERER_PRESENTVSYNC);



	//color the window
	SDL_SetRenderDrawColor(renderer, 47, 77, 76, 255);


	std::string policePath = "./../resources/characters/Police.png";
	std::string bulletPath = "./../resources/props/bullet.png";
	std::string bulletShotPath = "./../resources/props/bullet_shot.png";

	std::string terroristPath = "./../resources/characters/Terrorist.png";

    SDL_Texture* texturePolice = createTexture(renderer, policePath);
	checkPointer(texturePolice);

    SDL_Texture* textureBullet = createTexture(renderer, bulletPath);
	checkPointer(textureBullet);


    SDL_Texture* textureTerrorist = createTexture(renderer, terroristPath);
    checkPointer(textureTerrorist);


    // if(texture == nullptr)
    // {
    //     std::cout << " Texture was not created: " << SDL_GetError() << std::endl;
    //     SDL_Quit();

    //     return 1;
    // }

	SDL_Event event;
	bool quit = false;

	//keep track of firing bullets
	bool fire = false;

	//initial position of the Police
	int policeX = 100;
	int policeY = 100;

    //list of bullets fired
    // pair<int,int> -> <x-position, y-position>
    std::vector< std::pair<int,int> > bulletList;

    //list of terrorists
    // pair<int,int> -> <x-position, y-position>
    std::vector< std::pair<int,int> > terroristList;


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

					case SDLK_UP : policeY -= 5;
									//handling outbounds condition
									policeY = policeY < 0 ? policeY + 5 : policeY;
										break;

					case SDLK_DOWN : policeY += 5;
									//handling outbounds condition
									//NOTE : (SCREEN_HEIGHT - IMAGE_HEIGHT) checks that image stops fully before its front touches the boundary
									policeY = policeY > (SCREEN_HEIGHT - IMAGE_HEIGHT) ? policeY - 5 : policeY;
										break;

					case SDLK_LEFT : policeX -= 5;
									//handling outbounds condition
									policeX = policeX < 0 ? policeX + 5 : policeX;
										break;

					case SDLK_RIGHT : policeX += 5;
									//handling outbounds condition
									//NOTE : (SCREEN_HEIGHT - IMAGE_WIDTH) checks that image stops fully before its front touches the boundary
									policeX = policeX > (SCREEN_WIDTH - IMAGE_WIDTH) ? policeX - 5 : policeX;
										break;

							default :
										break;
				}

				if(event.key.keysym.sym == SDLK_SPACE)
				{
					fire = true;

				}
			}



		}
		//render police
		//height and width is kept 100 and 100 for the police
		// std::thread threadMainRender(&renderPolice, renderer, texturePolice, textureBullet, policeX, policeY, 100, 100, 16, fire);
		renderScreen(renderer, texturePolice, textureBullet, bulletList, textureTerrorist, terroristList, policeX, policeY, 100, 100, 16, fire);

        //NOTE : Ensure to reset the flag, otherwise the player will keep firing
        fire = false;
	}



	return 0;
}
