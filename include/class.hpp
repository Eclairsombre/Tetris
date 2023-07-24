#include <SDL2/SDL.h>

class block
{
public:
    SDL_Rect dest;

    int nbLigne;
    int cote_droit, cote_gauche;

    bool move = true;

    SDL_Color color;

    char emplacement[10];

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
    int x = 0, y = 0;
    int indice_tab = 0;

    char choix_forme[10] = {'L', 'O', 'T', 'Z', 'S', 'I'};
    char lettre;
    char liste_emplacement[10][10];

    block liste_block[20];

    SDL_Color color;

    bool vide = false;

    forme();
    ~forme();
};

forme::forme()
{
}

forme::~forme()
{
}
