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
        
        std::cout << " BMP image couldn't be loaded: " << SDL_GetError() << std::endl;
        
        SDL_Quit();
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if(texture == nullptr)
    {
        SDL_DestroyRenderer(renderer);
        
        std::cout << " Texture couldn't be created: " << SDL_GetError() << std::endl;
        
        SDL_Quit();
    }

    return texture;
}




void renderScreen(SDL_Renderer* renderer, SDL_Texture* texture, int x = 0, int y = 0, int w = SCREEN_WIDTH, int h = SCREEN_HEIGHT, int delay = 1000)
{
        // SDL_RenderClear(renderer);
        SDL_Rect destination;
        destination.x = x;
        destination.y = y;
        destination.w = w;
        destination.h = h;
        SDL_RenderCopy(renderer, texture, NULL, &destination);
        SDL_RenderPresent(renderer);

        SDL_Delay(delay);
}





void cleanup (SDL_Texture* texture1, SDL_Texture* texture2, SDL_Renderer* renderer,  SDL_Window* window)
{
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
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


    std::string backgroundfilePath = "./resources/sunset.bmp";
    std::string foregroundfilePath = "./resources/blackbuck.bmp";
    
    SDL_Texture* backgroundTexture = createTexture(backgroundfilePath, renderer);
    SDL_Texture* foregroundTexture = createTexture(foregroundfilePath, renderer);
    if(backgroundTexture == nullptr || foregroundTexture == nullptr)
    {
        std::cout << " Failed to create textures: " << SDL_GetError() << std::endl;
        SDL_Quit();

        return 1;
    }

    //Run the renderer on the texture for 3 times
    renderScreen(renderer, backgroundTexture, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderScreen(renderer, foregroundTexture, SCREEN_WIDTH/3, SCREEN_HEIGHT/3, SCREEN_WIDTH/4, SCREEN_HEIGHT/4, 2000);

    //clean-up
    cleanup(backgroundTexture, foregroundTexture, renderer, window);
    
    SDL_Quit();

    return 0;
}