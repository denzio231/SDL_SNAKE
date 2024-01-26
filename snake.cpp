#include<SDL2/SDL.h>
#include<iostream>
#include<math.h>
using namespace std;
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* sprite = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface* character_appearance = NULL;
int block_len = 50;
int SCREEN_WIDTH_BLOCKS = 15;
int SCREEN_HEIGHT_BLOCKS = 15;
int SCREEN_WIDTH = SCREEN_WIDTH_BLOCKS*block_len;
int SCREEN_HEIGHT = SCREEN_HEIGHT_BLOCKS*block_len;
int LENGTH = 15;
class node{
    public:
        SDL_Rect value;
        SDL_Rect* front;
        SDL_Rect* back;
};
class vector2{
    public:
        float x;
        float y;
        void set(float x_set,float y_set){
            x = x_set;
            y = y_set;
        }
        void normalize(){
            float unit_length = 1;
            float cos_x = acos(x/y);
            float sin_y = asin(x/y);
            float x = cos_x*unit_length;
            float y = sin_y*unit_length;

        }      
};
vector2 player_velocity;
vector2 player_position;
SDL_Rect apple;
bool init(){
    
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        cout << SDL_GetError() << endl;
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
            //SDL_FillRect(character_appearance, c, SDL_MapRGB(character_appearance->format, 255, 0, 0));
            
            renderer =  SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED);
        }
    }
    return success;
}
void close()
{
	//Deallocate surface
	//SDL_FreeSurface( gHelloWorld );
	//gHelloWorld = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}
int ACTUAL_LENGTH = 1;
SDL_Rect arr[10000];
SDL_Rect c;
bool repeated_coordinate(int x,int y){
    int counted = 0;
    for(int i = 0;i<ACTUAL_LENGTH;i++){
        if(arr[i].x == x and arr[i].y == y){
            counted++;
        }
        if(counted > 1){
            return true;
        }
    }
    return false;
}
void random_apple(){
    int len = 0;
    pair<int,int> valid_coordinates[SCREEN_HEIGHT_BLOCKS*SCREEN_WIDTH_BLOCKS];
            for(int i = 0;i<ACTUAL_LENGTH;i++){
                for(int v_x = 0;v_x<SCREEN_WIDTH_BLOCKS;v_x++){
                    for(int v_y = 0;v_y<SCREEN_HEIGHT_BLOCKS;v_y++){
                        if(arr[i].x != v_x and arr[i].y != v_y){
                            pair<int,int> a;
                            a.first = v_x;
                            a.second = v_y;
                            valid_coordinates[len] = a;
                            len++;
                        }       
                    }
                }
                 

            }
    int idx = (rand()%len);
    apple.x = valid_coordinates[idx].first*block_len;
    apple.y = valid_coordinates[idx].second*block_len;
}
int main(int argc, char** argv){
    srand(time(NULL));
    apple.h = 50;
    apple.w = 50;
    random_apple();
    c.h = 50;
    c.w = 50;
    arr[0] = c;
    player_velocity.x = 0;
    player_velocity.y = 1;
    /**SDL_Rect* r = NULL;
    r->h = 100;
    r->w = 100;
    SDL_FillRect(s,r);**/
    if(!init()){
        return 1;
    }
    SDL_Event e;
    bool running = true;
    while (running){
        while (SDL_PollEvent(&e)){
            switch (e.type){
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym)
                    {
                        case SDLK_w:
                            if(player_velocity.y == 1){
                                break;
                            }
                            player_velocity.set(0,-1);
                            break;
                        case SDLK_s:
                            if(player_velocity.y == -1){
                                break;
                            }
                            player_velocity.set(0,1);
                            break;
                        case SDLK_d:
                            if(player_velocity.x == -1){
                                break;
                            }
                            player_velocity.set(1,0);
                            break;
                        case SDLK_a:
                            if(player_velocity.x == 1){
                                break;
                            }
                            player_velocity.set(-1,0);
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }


        }
        if(ACTUAL_LENGTH<LENGTH){
            SDL_Rect n = arr[ACTUAL_LENGTH-1];
            arr[ACTUAL_LENGTH] = n;
            ACTUAL_LENGTH++;
            cout << ACTUAL_LENGTH;
        }
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
        SDL_RenderClear( renderer );
        SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
        SDL_RenderFillRects(renderer,arr,ACTUAL_LENGTH);
        SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
        SDL_RenderFillRect(renderer,&apple);
        SDL_RenderPresent(renderer);
        player_position.x += player_velocity.x;
        player_position.y += player_velocity.y;
        SDL_Rect prev = arr[0];
        arr[0].x = player_position.x*block_len;
        arr[0].y = player_position.y*block_len;
        for(int i = 1;i<ACTUAL_LENGTH;i++){
            SDL_Rect hold = arr[i];
            arr[i] = prev;
            prev = hold;
            
        }
        for(int i = 1;i<ACTUAL_LENGTH;i++){
            if(repeated_coordinate(arr[i].x,arr[i].y)){
                exit(0);
            }
        }
        if(player_position.x > SCREEN_WIDTH_BLOCKS or player_position.x < 0 or player_position.y > SCREEN_HEIGHT_BLOCKS or player_position.y < 0){
            exit(0);
        }
        if(apple.x==arr[0].x and apple.y == arr[0].y){
            random_apple();
            LENGTH++;
        }
        SDL_Delay(100);
    }
    close();
    
}