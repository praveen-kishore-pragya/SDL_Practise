#include <iostream>
#include <SDL2/SDL.h>


//width and height of the window
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800


//printError
void printError()
{
    std::cout << " Error: " << SDL_GetError() << std::endl;
}


//initialise 
int init()
{
    if( SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printError();

        SDL_Quit();

        return 1;
    }
    return 0;
}


//create texture
SDL_Texture* createTexture(std::string& backgroundPath, SDL_Renderer* renderer)
{
    SDL_Texture* texture = nullptr;

    SDL_Surface* surface = SDL_LoadBMP(backgroundPath.c_str());

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}



//rendering the window, since we need not want to get parameters of source array and sourceIndex separate,
// thus, we take clip as a pointer to the source array with index = sourceIndex
void renderScreen(SDL_Renderer* renderer, SDL_Texture* texture, int x = 0, int y = 0, int w = SCREEN_WIDTH, int h = SCREEN_HEIGHT, SDL_Rect* clip = nullptr, int delay = 1000)
{    // SDL_RenderClear(renderer);
    
    SDL_Rect destination;
    destination.x = x;
    destination.y = y;
    destination.w = w;
    destination.h = h;
    
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, clip, clip);
    //SDL_RenderCopy() expects address or reference of source and destination windows,
    //hence, clip which is address or reference (since, name is passed and clip is a pointer, thus its value which is address is passed),
    //and reference or address of destination is passed

    SDL_RenderPresent(renderer);

    // SDL_Delay(delay);
}



//cleanup
void cleanup(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}



//main function
int main(int argc, char* argv[])
{
    //initialise
    if (init() != 0)
        return 1;
    
    
    //create a window
    SDL_Window* window = SDL_CreateWindow("Changing Clips on Input [0,1,2,3]",
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SCREEN_WIDTH,
                                            SCREEN_HEIGHT,
                                            SDL_WINDOW_SHOWN);

    
    SDL_Renderer* renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_ACCELERATED |
                                                SDL_RENDERER_PRESENTVSYNC);

    //filePath of image or sprite 
    std::string backgroundPath = "./resources/sunset.bmp";

    //create a texture
    SDL_Texture* texture = createTexture(backgroundPath, renderer);
    if(texture == nullptr)
    {
        printError();

        SDL_Quit();
        
        return 1;       
    }

    SDL_Rect source[4];
    //create 4 parts of source window
    for(int i=0; i<4; i++)
    {
        static float start = 0;
        
        source[i].x = SCREEN_WIDTH * start;
        // source[i].y = SCREEN_HEIGHT * start;
        source[i].y = 0;
        source[i].w = SCREEN_WIDTH * 0.25;
        source[i].h = SCREEN_HEIGHT;

        // start = end;
        // end = end + 0.25;

        start = start + 0.25;
    }

    //defines the sourceIndex or, the part of thw window to be shown at a keyboard press event
    int sourceIdx = -1;
    
    SDL_Event event;
    bool quit = false;
    while(quit != true)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                quit = true;
            if(event.type == SDLK_ESCAPE)
                quit = true;
            if(event.type == SDL_KEYDOWN)
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_0: sourceIdx = 0;
                                break;
                    case SDLK_1: sourceIdx = 1;
                                break;
                    case SDLK_2: sourceIdx = 2;
                                break;
                    case SDLK_3: sourceIdx = 3;
                                break;

                    default: quit = true; 
                            break;
                }
            }

            //load and render window
            renderScreen(renderer, texture, 0, 0, SCREEN_WIDTH, SCREEN_WIDTH, &source[sourceIdx]);
        }
    }

    

    cleanup(window, renderer, texture);
    SDL_Quit();


    return 0;   
}