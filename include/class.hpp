#include <SDL2/SDL.h> 

class block
{
public:
    SDL_Rect dest;
    int nbLigne;
    bool visible = false;
    bool move= true;

    
    block(/* args */);
    ~block();
};

block::block(/* args */)
{
}

block::~block()
{
}


class forme
{
   
public:

    int x=0,y=0;

    char choix_forme[10] = {'L','O','T','Z','S','I'};

    block g1,m1,d1,g2,m2,d2,g3,m3,d3;
    
    



    forme(forme &f);
    ~forme();
};

forme::forme(forme &f)
{
    int nb = rand()%6;
    char lettre = choix_forme[nb];
    if (lettre == 'L')
    {
        g1.visible = true;
        g2.visible = true;
        g3.visible = true;
        m3.visible = true;
    }
}

forme::~forme()
{
}
