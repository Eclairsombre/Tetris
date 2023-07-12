#include <SDL2/SDL.h> 
#include <iostream>
using namespace std;


#include <string.h>



#include "../include/class.hpp"



bool check_collision( SDL_Rect &A, SDL_Rect &B );
int check_ligne(int nb_block[50]);
void sup_case(int i,int &indice, block tab_block[1000]);
void draw_forme(forme &f,SDL_Renderer* rend);
forme init_forme(forme &f, SDL_Color tab_color[15]);
void actualiser_forme(forme &f,int vitesse,int &indice,int nb_block[50],block tab_block[1000],SDL_Color tab_color[15],bool &bas);
void check_colission_plateau(forme &f);
void check_colision_block(forme &f,block tab_block[1000],int indice,bool &bas);
void rotation(forme &f);





int main(int argc, char *argv[]) { // Initialise SDL if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { printf("Erreur lors de l'initialisation de SDL: %s\n", SDL_GetError()); return 1; }

srand (time(NULL));
SDL_Window* win = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);

Uint32 render_flags = SDL_RENDERER_ACCELERATED;
SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

SDL_Rect plateau;
plateau.h=712;
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


forme f;


SDL_Color blue = SDL_Color();
blue.r = 0;
blue.g = 0;
blue.b = 255;
blue.a = 255;

SDL_Color red = SDL_Color();
red.r = 255;
red.g = 0;
red.b = 0;
red.a = 255;

SDL_Color green = SDL_Color();
green.r = 0;
green.g = 255;
green.b = 0;
green.a = 255;

SDL_Color orange = SDL_Color();
orange.r = 255;
orange.g = 128;
orange.b = 0;
orange.a = 255;

SDL_Color yellow = SDL_Color();
yellow.r = 255;
yellow.g = 255;
yellow.b = 0;
yellow.a = 255;

SDL_Color tab_color[15] = {blue,red,green,orange,yellow};

init_forme(f,tab_color);



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
                        rotation(f);
                        
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        for (int i=0;i<=f.indice_tab;i++)
                        {f.liste_block[i].dest.x -= 30;}
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                    if (bas == true)
                        {for (int i=0;i<=f.indice_tab;i++)
                        {f.liste_block[i].dest.y += 30;}}
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        for (int i=0;i<=f.indice_tab;i++)
                        {f.liste_block[i].dest.x += 30;}
                        break;
                    default:
                        break;
                }
                break;
        }
    }
    
     
    
  
    check_colission_plateau(f);
    actualiser_forme(f,vitesse,indice,nb_block,tab_block,tab_color,bas);
    


    SDL_SetRenderDrawColor(rend,0,0,0,255);
    SDL_RenderClear(rend);

    SDL_SetRenderDrawColor(rend,255,255,255,255);
    SDL_RenderDrawRect(rend, &plateau);

   
    draw_forme(f,rend);

    
    
    check_colision_block(f,tab_block, indice,bas);
    for(int y=0;y<=indice;y++)
    {
        
        SDL_SetRenderDrawColor(rend,tab_block[y].color.r,tab_block[y].color.g,tab_block[y].color.b,255);
        SDL_RenderFillRect(rend, &tab_block[y].dest);

        SDL_SetRenderDrawColor(rend,0,0,0,255);
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


void draw_forme(forme &f,SDL_Renderer* rend)
{
    
    for (int i=0;i<=f.indice_tab;i++)
    {
        SDL_SetRenderDrawColor(rend,f.color.r,f.color.g,f.color.b,255);
        SDL_RenderFillRect(rend, &f.liste_block[i].dest);

        SDL_SetRenderDrawColor(rend,0,0,0,255);
        SDL_RenderDrawRect(rend, &f.liste_block[i].dest);
    }
}


forme init_forme(forme &f, SDL_Color tab_color[15])
{
    
    f.lettre = f.choix_forme[rand()%6];
    f.color = tab_color[rand()%5];
    block g1,m1,d1,g2,m2,d2,g3,m3,d3;
    if (f.lettre == 'L')
    {
        
        g1.dest.w = 30;
        g1.dest.h = 30;
        g1.dest.x = 370;
        g1.dest.y = 150;

        g2.dest.w = 30;
        g2.dest.h = 30;
        g2.dest.x = 370;
        g2.dest.y = 180;

        g3.dest.w = 30;
        g3.dest.h = 30;
        g3.dest.x = 370;
        g3.dest.y = 210;

        m3.dest.w = 30;
        m3.dest.h = 30;
        m3.dest.x = 400;
        m3.dest.y = 210;
        
        f.liste_block[0]= g1;
        f.liste_block[1]= g2;
        f.liste_block[2]= g3;
        f.liste_block[3]= m3;

       
        strcpy(f.liste_block[0].emplacement, "g1");
        strcpy(f.liste_block[1].emplacement,"g2");
        strcpy(f.liste_block[2].emplacement, "g3");
        strcpy(f.liste_block[3].emplacement, "m3");


        f.liste_block[0].cote_droit = 2;
        f.liste_block[0].cote_gauche = 0;
        f.liste_block[1].cote_droit = 2;
        f.liste_block[1].cote_gauche = 0;
        f.liste_block[2].cote_droit = 2;
        f.liste_block[2].cote_gauche = 0;
        f.liste_block[3].cote_droit = 1;
        f.liste_block[3].cote_gauche = 1;

        f.indice_tab = 3;
    }
    else if (f.lettre == 'O')
    {
        
        g1.dest.w = 30;
        g1.dest.h = 30;
        g1.dest.x = 370;
        g1.dest.y = 150;

        m1.dest.w = 30;
        m1.dest.h = 30;
        m1.dest.x = 400;
        m1.dest.y = 150;

        g2.dest.w = 30;
        g2.dest.h = 30;
        g2.dest.x = 370;
        g2.dest.y = 180;

        m2.dest.w = 30;
        m2.dest.h = 30;
        m2.dest.x = 400;
        m2.dest.y = 180;
        
        f.liste_block[0]= g1;
        f.liste_block[1]= g2;
        f.liste_block[2]= m1;
        f.liste_block[3]= m2;


        strcpy(f.liste_block[0].emplacement, "g1");
        strcpy(f.liste_block[1].emplacement,"g2");
        strcpy(f.liste_block[2].emplacement, "m1");
        strcpy(f.liste_block[3].emplacement, "m2");

        f.liste_block[0].cote_droit = 2;
        f.liste_block[0].cote_gauche = 0;
        f.liste_block[1].cote_droit = 2;
        f.liste_block[1].cote_gauche = 0;
        f.liste_block[2].cote_droit = 1;
        f.liste_block[2].cote_gauche = 1;
        f.liste_block[3].cote_droit = 1;
        f.liste_block[3].cote_gauche = 1;

        f.indice_tab = 3;
    }
    else if (f.lettre == 'T')
    {
        
        g1.dest.w = 30;
        g1.dest.h = 30;
        g1.dest.x = 370;
        g1.dest.y = 150;

        m1.dest.w = 30;
        m1.dest.h = 30;
        m1.dest.x = 400;
        m1.dest.y = 150;

        d1.dest.w = 30;
        d1.dest.h = 30;
        d1.dest.x = 430;
        d1.dest.y = 150;

        m2.dest.w = 30;
        m2.dest.h = 30;
        m2.dest.x = 400;
        m2.dest.y = 180;
        
        f.liste_block[0]= g1;
        f.liste_block[1]= d1;
        f.liste_block[2]= m1;
        f.liste_block[3]= m2;

        strcpy(f.liste_block[0].emplacement, "g1");
        strcpy(f.liste_block[1].emplacement,"d1");
        strcpy(f.liste_block[2].emplacement, "m1");
        strcpy(f.liste_block[3].emplacement, "m2");

        f.liste_block[0].cote_droit = 3;
        f.liste_block[0].cote_gauche = 0;
        f.liste_block[1].cote_droit = 1;
        f.liste_block[1].cote_gauche = 2;
        f.liste_block[2].cote_droit = 2;
        f.liste_block[2].cote_gauche = 1;
        f.liste_block[3].cote_droit = 2;
        f.liste_block[3].cote_gauche = 1;
        

        f.indice_tab = 3;
    }
    else if (f.lettre == 'Z')
    {
        
        g1.dest.w = 30;
        g1.dest.h = 30;
        g1.dest.x = 370;
        g1.dest.y = 150;

        g2.dest.w = 30;
        g2.dest.h = 30;
        g2.dest.x = 370;
        g2.dest.y = 180;

        m3.dest.w = 30;
        m3.dest.h = 30;
        m3.dest.x = 400;
        m3.dest.y = 210;

        m2.dest.w = 30;
        m2.dest.h = 30;
        m2.dest.x = 400;
        m2.dest.y = 180;
        
        f.liste_block[0]= g1;
        f.liste_block[1]= g2;
        f.liste_block[2]= m3;
        f.liste_block[3]= m2;

      
        strcpy(f.liste_block[0].emplacement, "g1");
        strcpy(f.liste_block[1].emplacement,"g2");
        strcpy(f.liste_block[2].emplacement, "m3");
        strcpy(f.liste_block[3].emplacement, "m2");

        f.liste_block[0].cote_droit = 2;
        f.liste_block[0].cote_gauche = 0;
        f.liste_block[1].cote_droit = 2;
        f.liste_block[1].cote_gauche = 0;
        f.liste_block[2].cote_droit = 1;
        f.liste_block[2].cote_gauche = 1;
        f.liste_block[3].cote_droit = 1;
        f.liste_block[3].cote_gauche = 1;

        f.indice_tab = 3;
    }
    else if (f.lettre == 'S')
    {
        
        d1.dest.w = 30;
        d1.dest.h = 30;
        d1.dest.x = 430;
        d1.dest.y = 150;

        d2.dest.w = 30;
        d2.dest.h = 30;
        d2.dest.x = 430;
        d2.dest.y = 180;

        m3.dest.w = 30;
        m3.dest.h = 30;
        m3.dest.x = 400;
        m3.dest.y = 210;

        m2.dest.w = 30;
        m2.dest.h = 30;
        m2.dest.x = 400;
        m2.dest.y = 180;
        
        f.liste_block[0]= d2;
        f.liste_block[1]= d1;
        f.liste_block[2]= m3;
        f.liste_block[3]= m2;


        strcpy(f.liste_block[0].emplacement, "d2");
        strcpy(f.liste_block[1].emplacement,"d1");
        strcpy(f.liste_block[2].emplacement, "m3");
        strcpy(f.liste_block[3].emplacement, "m2");

        f.liste_block[0].cote_droit = 1;
        f.liste_block[0].cote_gauche = 1;
        f.liste_block[1].cote_droit = 1;
        f.liste_block[1].cote_gauche = 1;
        f.liste_block[2].cote_droit = 2;
        f.liste_block[2].cote_gauche = 0;
        f.liste_block[3].cote_droit = 2;
        f.liste_block[3].cote_gauche = 0;

        f.indice_tab = 3;
    }
    else if (f.lettre == 'I')
    {
        
        m1.dest.w = 30;
        m1.dest.h = 30;
        m1.dest.x = 400;
        m1.dest.y = 150;

        m2.dest.w = 30;
        m2.dest.h = 30;
        m2.dest.x = 400;
        m2.dest.y = 180;

        m3.dest.w = 30;
        m3.dest.h = 30;
        m3.dest.x = 400;
        m3.dest.y = 210;

        
        
        f.liste_block[0]= m1;
        f.liste_block[1]= m2;
        f.liste_block[2]= m3;
        
        strcpy(f.liste_block[0].emplacement, "m1");
        strcpy(f.liste_block[1].emplacement,"m2");
        strcpy(f.liste_block[2].emplacement, "m3");
        

        f.liste_block[0].cote_droit = 1;
        f.liste_block[0].cote_gauche = 0;
        f.liste_block[1].cote_droit = 1;
        f.liste_block[1].cote_gauche = 0;
        f.liste_block[2].cote_droit = 1;
        f.liste_block[2].cote_gauche = 0;


        f.indice_tab = 2;
    }
    
}


void actualiser_forme(forme &f,int vitesse,int &indice,int nb_block[50],block tab_block[1000],SDL_Color tab_color[15],bool &bas)
{
    bool new_forme = false;
    for (int i=0;i<=f.indice_tab;i++)
        {
            if (f.liste_block[i].move==true)
            {
                f.liste_block[i].dest.y+=vitesse;
                
            }
            else{
                f.liste_block[i].nbLigne = f.liste_block[i].dest.y / 30 ;
                f.liste_block[i].dest.y = 30  * f.liste_block[i].nbLigne;
                nb_block[f.liste_block[i].dest.y /30] +=1;
                f.liste_block[i].color = f.color;
                tab_block[indice]=f.liste_block[i];
                indice+=1;

                new_forme = true;

                
            }
        }
        if (new_forme==true)
        {
            forme g;
            bas =true;
            init_forme(g,tab_color);
            
            f=g;
        }
        
}


void check_colission_plateau(forme &f)
{
    
    for (int i=0;i<=f.indice_tab;i++)
        {

            if (f.liste_block[i].dest.x + 30*f.liste_block[i].cote_droit +30> 551 ) {
                f.liste_block[i].dest.x = 549 -30*f.liste_block[i].cote_droit;
                
            }
            
            if (f.liste_block[i].dest.x - 30*f.liste_block[i].cote_gauche< 250 ) {
                f.liste_block[i].dest.x = 250 + 30*f.liste_block[i].cote_gauche ;
                
            }
            
            if (f.liste_block[i].dest.y + f.liste_block[i].dest.h > 810) {
                f.liste_block[i].dest.y = 810 - f.liste_block[i].dest.h;
                
                for (int y=0;y<=f.indice_tab;y++)
                {
                    f.liste_block[y].move = false;
                    
                    
                }

            }
            
            if (f.liste_block[i].dest.y < 0) {
                f.liste_block[i].dest.y = 0;
            }
        }
    
}


void check_colision_block(forme &f,block tab_block[1000],int indice,bool &bas)
{
    bool test=true;
    for (int i=0;i<=f.indice_tab;i++)
    {
        for(int y=0;y<=indice;y++)
        {
            if (check_collision(f.liste_block[i].dest,tab_block[y].dest)==true)
            {
                test=false;
                
                
            }
            /*
            f.liste_block[i].dest.y+=30;
            if (check_collision(f.liste_block[i].dest,tab_block[y].dest)==true)
            {
                bas=false;
                
                
            }
            
            f.liste_block[i].dest.y-=30; */

        }
    }
    if (test==false)
    {
        for (int i=0;i<=f.indice_tab;i++)
        {
            f.liste_block[i].move=false;
        }
    }
}


void rotation(forme &f)
{
        
        if (f.lettre != 'O')
        {
            for (int i=0;i<=f.indice_tab;i++)
            {
                
                if (strcmp(f.liste_block[i].emplacement, "g0") == true)
                {
                   
                    f.liste_block[i].dest.x += 60;
                    strcpy(f.liste_block[i].emplacement, "d1");
                    
                }


                else if (strcmp(f.liste_block[i].emplacement , "m0")== true)
                    {
                   
                    f.liste_block[i].dest.x += 30;
                    f.liste_block[i].dest.y += 30;
                    strcpy(f.liste_block[i].emplacement, "d2");
                    
                    }
                    

                else if (strcmp(f.liste_block[i].emplacement , "d0")== true)
                    {
                        
                    
                    f.liste_block[i].dest.y += 60;
                    strcpy(f.liste_block[i].emplacement, "d3");
                    }
                   

                else if (strcmp(f.liste_block[i].emplacement , "g1")== true)
                    {
                    
                    f.liste_block[i].dest.x += 30;
                    f.liste_block[i].dest.y -= 30;
                    strcpy(f.liste_block[i].emplacement, "m1");
                   }

                else if (strcmp(f.liste_block[i].emplacement , "d1")== true)
                    {
          
                    f.liste_block[i].dest.x -= 30;
                    f.liste_block[i].dest.y += 30;
                    strcpy(f.liste_block[i].emplacement, "m3");}
                    
                

                else if (strcmp(f.liste_block[i].emplacement , "g2")== true)
                    
                    {
                    
                    f.liste_block[i].dest.y -= 60;
                    strcpy(f.liste_block[i].emplacement, "g1");}
                    

                else if (strcmp(f.liste_block[i].emplacement , "m2")== true)
                    {
              
                    f.liste_block[i].dest.x -= 30;
                    f.liste_block[i].dest.y -= 30;
                    strcpy(f.liste_block[i].emplacement, "g2");}
                    

                else if (strcmp(f.liste_block[i].emplacement , "d2")== true)
                   { 
           
                    f.liste_block[i].dest.x -= 60;
                   strcpy(f.liste_block[i].emplacement, "g3");}
                    
                   
                
                }
               

            }
}
            
            

            
        
        

    
