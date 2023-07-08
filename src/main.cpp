#include <SDL2/SDL.h> 
#include <iostream>
using namespace std;




#include "../include/class.hpp"



bool check_collision( SDL_Rect &A, SDL_Rect &B );
int check_ligne(int nb_block[50]);
void sup_case(int i,int &indice, block tab_block[1000]);




int main(int argc, char *argv[]) { // Initialise SDL if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { printf("Erreur lors de l'initialisation de SDL: %s\n", SDL_GetError()); return 1; }

SDL_Window* win = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);

Uint32 render_flags = SDL_RENDERER_ACCELERATED;
SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

SDL_Rect plateau;
plateau.h=720;
plateau.w=303;
plateau.x=248;
plateau.y=100;


block b;
b.dest.w = 30;
b.dest.h = 30;
b.dest.x = 400;
b.dest.y = 150;

int nb_block[50] = {0};

block tab_block[1000];
int indice =0;

int vitesse= 1;

bool close = false,bas = true;
int speed = 300;

while (!close) {
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                close = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        b.dest.y -= 30;
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        b.dest.x -= 30;
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                    if (bas == true)
                        {b.dest.y += 15;}
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        b.dest.x += 30;
                        break;
                    default:
                        break;
                }
                break;
        }
    }
    
    if (b.dest.x + 30 > 551) {
        b.dest.x = 549 - b.dest.w;
    }
    
    if (b.dest.x - 30 < 250) {
        b.dest.x = 250;
    }
    
    if (b.dest.y + b.dest.h > 810) {
        b.dest.y = 810 - b.dest.h;
        b.move=false;

    }
    
    if (b.dest.y < 0) {
        b.dest.y = 0;
    }
    
    b.dest.y+=vitesse;

    SDL_SetRenderDrawColor(rend,0,0,0,255);
    SDL_RenderClear(rend);

    SDL_SetRenderDrawColor(rend,255,255,255,255);
    SDL_RenderDrawRect(rend, &plateau);

    SDL_SetRenderDrawColor(rend,0,0,255,255);
    SDL_RenderDrawRect(rend, &b.dest);

    
    

    for(int y=0;y<=indice;y++)
    {
        
        SDL_SetRenderDrawColor(rend,0,0,255,255);
        SDL_RenderDrawRect(rend, &tab_block[y].dest);
        if (check_collision(b.dest,tab_block[y].dest)==true)
        {
            b.move=false;
            
            
        }
        b.dest.y+=30;
        if (check_collision(b.dest,tab_block[y].dest)==true)
        {
            bas = false;
            
            
            
        }
        
        b.dest.y -=30;

    }

    if (b.move==false)
    {
        
        nb_block[b.dest.y / 30] +=1;
        b.nbLigne = b.dest.y / 30 ;
        b.dest.y = 30  * b.nbLigne;
        tab_block[indice]=b;
        indice+=1;
        
        

        bas = true;
        block a;
        
        a.dest.w = 30;
        a.dest.h = 30;
        a.dest.x = 400;
        a.dest.y = 150;
        a.move=true;
        b=a;
    }
    

    int ligne = check_ligne(nb_block);
    
   
    if (ligne != -1)
    {
        
        for(int i=0;i<=indice;i++)
        {
            
            if (tab_block[i].nbLigne +1 < ligne)
            {
               
                tab_block[i].nbLigne+=1;
                
                nb_block[tab_block[i].nbLigne ]+=1;
                if ((tab_block[i].dest.y) %30 == 0)
                {
                    tab_block[i].dest.y +=30;
                }
                else
                {
                    
                    tab_block[i].dest.y = 30 * tab_block[i].nbLigne;
                }
                
            }

            else if (tab_block[i].nbLigne +1 == ligne)
            {
                sup_case(i, indice, tab_block);
                i-=1;
                indice-=1;
            }
        }
    }

    SDL_RenderPresent(rend);
    SDL_Delay(1000 / 60);
}

SDL_DestroyRenderer(rend);
SDL_DestroyWindow(win);
SDL_Quit();

return 0;
}













bool check_collision( SDL_Rect &A, SDL_Rect &B )
{
    //Les cotes des rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
 
    //Calcul les cotes du rectangle A
    leftA = A.x +3;
    rightA = A.x - 3 + A.w;
    topA = A.y;
    bottomA = A.y + A.h;
 
    //Calcul les cotes du rectangle B
    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;

    if( bottomA <= topB )
    {
        return false;
    }
 
    if( topA >= bottomB )
    {
        return false;
    }
 
    if( rightA <= leftB )
    {
        return false;
    }
 
    if( leftA >= rightB )
    {
        return false;
    }
 
    //Si conditions collision detectee
    return true;
}

int check_ligne(int nb_block[50])
{
    for (int i=1 ;i<49;++i)
    {
        if (nb_block[i]==10)
        {
            nb_block[i]=0;
            return i+1;
        }
    }
    return -1;
}


void sup_case(int i,int &indice, block tab_block[1000])
{
    for (int y=i;y<=indice;y++)
    {
        tab_block[y] = tab_block[y+1];
        
    }
}