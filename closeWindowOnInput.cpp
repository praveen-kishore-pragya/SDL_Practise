#include <iostream>
#include <SDL2/SDL.h>


#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 600

int init()
{
    if( SDL_Init(SDL_INIT_EVERYTHING) != 0 )
    {
        std::cout << " Unablr to initialize: " << SDL_GetError() << std::endl;

        return 1;
    }
    return 0;
}


SDL_Texture* createTexture(SDL_Renderer* renderer, std::string& filePath)
{
    SDL_Texture* texture = nullptr;

    SDL_Surface* surface = SDL_LoadBMP(filePath.c_str());

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}


void renderScreen(SDL_Renderer* renderer, SDL_Texture* texture, int x = 0, int y = 0, int w = SCREEN_WIDTH, int h = SCREEN_HEIGHT, int delay = 1000)
{
    SDL_Rect destination;
    destination.x = x;
    destination.y = y;
    destination.w = w;
    destination.h = h;

    SDL_RenderCopy(renderer, texture, NULL, &destination);
    SDL_RenderPresent(renderer);

    // SDL_Delay(delay);

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

    
    std::string backgroundPath = "./resources/sunset.bmp";
    
    
    SDL_Texture* texture = createTexture(renderer, backgroundPath);
    if(texture == nullptr)
    {
        std::cout << " Texture was not created: " << SDL_GetError() << std::endl;
        SDL_Quit();

        return 1;
    }

    
    SDL_Event e;
    bool quit = false;
    while(quit != true)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
                quit = true;
            if(e.type == SDL_KEYDOWN)
                quit = true;
            if(e.type == SDL_MOUSEBUTTONDOWN)
                quit = true;
        }
        renderScreen(renderer, texture, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    cleanup(renderer, texture, window);
    SDL_Quit();   
    
    
    return 0;
}