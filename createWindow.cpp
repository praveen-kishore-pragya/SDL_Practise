#include <iostream>
#include <SDL2/SDL.h>

#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH 800


SDL_Texture* createTexture(std::string& filePath, SDL_Renderer* renderer)
{
    SDL_Texture* texture = nullptr;

    SDL_Surface *surface = SDL_LoadBMP(filePath.c_str());
    if(surface == nullptr)
    {
        SDL_DestroyRenderer(renderer);
        // SDL_DestroyWindow(window);
        
        std::cout << " BMP image couldn't be loaded: " << SDL_GetError() << std::endl;
        
        SDL_Quit();
        // return 1;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if(texture == nullptr)
    {
        SDL_DestroyRenderer(renderer);
        // SDL_DestroyWindow(window);
        
        std::cout << " Texture couldn't be created: " << SDL_GetError() << std::endl;
        
        SDL_Quit();
        // return 1;
    }

    return texture;
}






int main(int argc, char* argv[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0 )
    {
        std::cout << " SDL Init error: " << SDL_GetError() << std::endl;
        return 1; //pgm closed with error
    }

    SDL_Window *window = SDL_CreateWindow("Hello World!",
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SCREEN_WIDTH,
                                            SCREEN_HEIGHT,
                                            SDL_WINDOW_SHOWN);

    if(window == nullptr)
    {
        std::cout<< " Window failed to be created: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_ACCELERATED |
                                                SDL_RENDERER_PRESENTVSYNC);
    //Using all flags by OR'ing is NOT a good idea, I read it on perplexity.ai
    
    if(renderer == nullptr)
    {
        std::cout << " Error in creation of renderer: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }


    std::string filePath = "./resources/sunset.bmp";
    SDL_Texture* texture = createTexture(filePath, renderer);
    
    

    //Run the renderer on the texture for 3 times
    for(int i=0; i<3; i++)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Delay(1000);
    }

    // SDL_Delay(3000);


    //clean-up
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}