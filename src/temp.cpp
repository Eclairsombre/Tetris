

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
using namespace std;

int main(int argc, char *argv[])
{
    cout << "Texte avec SDL2_ttf" << endl;

    SDL_Window *pFenetre = 0;
    SDL_Renderer *pRendu = 0;
    SDL_Rect destRect;

    SDL_Init(SDL_INIT_VIDEO);
    pFenetre = SDL_CreateWindow(
        "Texte avec SDL2_ttf", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 300,
        300, SDL_WINDOW_SHOWN);

    if (TTF_Init() == -1) // Initialisation de SDL2_ttf
    {
        cout << "erreur d'initialisation de la SDL2_ttf" << endl;
    }

    TTF_Font *pFont = TTF_OpenFont("/home/alex/Bureau/project SDL/font/dogica.ttf", 25);
    SDL_Color couleur = {255, 255, 0}; // du jaune ici
    SDL_Surface *pSurfTxt = TTF_RenderText_Blended(pFont, "Bonjour !", couleur);
    TTF_CloseFont(pFont); // On libère la mémoire utilisée par la police
    pRendu = SDL_CreateRenderer(pFenetre, -1, SDL_RENDERER_ACCELERATED);
    // Transfert de l'image de la surface à une texture
    SDL_Texture *pTextureTxt = SDL_CreateTextureFromSurface(pRendu, pSurfTxt);
    // Deux variables qui vont recevoir les dimensions du texte
    int txtW = 0;
    int txtH = 0;
    SDL_QueryTexture(pTextureTxt, NULL, NULL, &txtW, &txtH);
    SDL_FreeSurface(pSurfTxt); // La mémoire de surface est libérée
    // Positionnement du texte
    destRect.x = 50;
    destRect.y = 50;
    // On utilise les données fournies par SDL_QueryTexture
    destRect.w = txtW;
    destRect.h = txtH;
    // On applique le texte dans la fenêtre
    SDL_RenderCopy(pRendu, pTextureTxt, nullptr, &destRect);
    SDL_DestroyTexture(pTextureTxt); // La mémoire de texture est libérée

    // Actualisation de la fenêtre
    SDL_RenderPresent(pRendu);

    int attendre = 1;
    SDL_Event evenement;

    // Boucle d'evenements
    while (attendre)
    {
        SDL_WaitEvent(&evenement);
        switch (evenement.type)
        {
        case SDL_QUIT:
            attendre = 0;
            break;
        }
    }

    SDL_DestroyRenderer(pRendu);
    SDL_DestroyWindow(pFenetre);
    SDL_Quit();

    return EXIT_SUCCESS;
}
