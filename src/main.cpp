// Include
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
using namespace std;
#include <string.h>
#include "../include/class.hpp"
#include <cstdio>
#include <SDL2/SDL_mixer.h>

// Fonctions
bool check_collision(SDL_Rect &A, SDL_Rect &B);
int check_ligne(int nb_block[50]);
void sup_case(int i, int &indice, block tab_block[1000]);
void draw_forme(forme &f, SDL_Renderer *rend);
void init_forme(forme &f, SDL_Color tab_color[15]);
void actualiser_forme(forme &f, forme &prochain, int vitesse, int &indice, int nb_block[50], block tab_block[1000], SDL_Color tab_color[15], bool &close, int score, char time[100]);
void check_colission_plateau(forme &f);
void check_colision_block(forme &f, block tab_block[1000], int indice, bool &bas);
void rotation(forme &f);
void test_vide(forme &f);
bool simple_test_colision(forme &f, block tab_block[1000], int indice);
void timer(int &heure, int &minute, int &seconde, TTF_Font *dogica, SDL_Color blanc, SDL_Renderer *rend, SDL_Rect t_timer, char time[100]);
bool verifierEtCreerFichier(const char *nomFichier);
bool ecrireDansFichier(const char *nomFichier, const char *texte);
int game();
int menu();

// Main
int main(int argc, char *argv[])
{
    menu();
    printf("test");
    return 0;
}

int menu()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Erreur lors de l'initialisation de SDL: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        return 1;
    }
    if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
    {
        SDL_Log("Erreur initialisation SDL_mixer : %s", Mix_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Window *win = SDL_CreateWindow("TETRIS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 500, 0);
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);

    Mix_Music *musique;
    musique = Mix_LoadMUS("music/tetris_theme.mp3");
    Mix_PlayMusic(musique, -1);
    Mix_VolumeMusic(10);
    bool close = false;

    SDL_Rect bouton_play;
    bouton_play.h = 100;
    bouton_play.w = 200;
    bouton_play.x = 50;
    bouton_play.y = 100;

    SDL_Rect bouton_score;
    bouton_score.h = 100;
    bouton_score.w = 200;
    bouton_score.x = 50;
    bouton_score.y = 225;

    SDL_Rect bouton_about;
    bouton_about.h = 100;
    bouton_about.w = 200;
    bouton_about.x = 50;
    bouton_about.y = 350;

    SDL_Event event;
    int mouseX;
    int mouseY;
    while (!close)
    {

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderClear(rend);

        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
        SDL_RenderDrawRect(rend, &bouton_play);

        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
        SDL_RenderDrawRect(rend, &bouton_score);

        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
        SDL_RenderDrawRect(rend, &bouton_about);

        while (SDL_PollEvent(&event))
        {

            switch (event.type)
            {
            case SDL_QUIT:
                close = true;
                SDL_DestroyRenderer(rend);
                SDL_DestroyWindow(win);

                Mix_FreeMusic(musique);
                Mix_CloseAudio();
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouseX = event.button.x;
                mouseY = event.button.y;
                if ((mouseX >= 50 && mouseX <= 250) && (mouseY >= 100 && mouseY <= 200))
                {

                    close = true;
                    SDL_DestroyRenderer(rend);
                    SDL_DestroyWindow(win);
                    Mix_FreeMusic(musique);
                    Mix_CloseAudio();
                    game();
                }
                break;
            default:
                break;
            }
        }
        SDL_RenderPresent(rend);
        SDL_Delay(1000 / 60);
    }

    SDL_Quit();
    return 0;
}

bool check_collision(SDL_Rect &A, SDL_Rect &B)
{
    // Les cotes des rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    // Calcul les cotes du rectangle A
    leftA = A.x + 3;
    rightA = A.x - 3 + A.w;
    topA = A.y + 3;
    bottomA = A.y + A.h - 3;

    // Calcul les cotes du rectangle B
    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;

    if (bottomA <= topB)
    {
        return false;
    }

    if (topA >= bottomB)
    {
        return false;
    }

    if (rightA <= leftB)
    {
        return false;
    }

    if (leftA >= rightB)
    {
        return false;
    }

    // Si conditions collision detectee
    return true;
}

int check_ligne(int nb_block[50])
{
    for (int i = 1; i < 30; ++i)
    {
        if (nb_block[i] >= 10 && i + 1 != 2)
        {
            cout << i + 1 << ' ';
            return i + 1;
        }
    }
    return -1;
}

void sup_case(int i, int &indice, block tab_block[1000])
{
    for (int y = i; y <= indice; y++)
    {
        tab_block[y] = tab_block[y + 1];
    }
}

void draw_forme(forme &f, SDL_Renderer *rend)
{

    for (int i = 0; i <= f.indice_tab; i++)
    {
        SDL_SetRenderDrawColor(rend, f.color.r, f.color.g, f.color.b, 255);
        SDL_RenderFillRect(rend, &f.liste_block[i].dest);

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderDrawRect(rend, &f.liste_block[i].dest);
    }
}

void init_forme(forme &f, SDL_Color tab_color[15])
{

    f.lettre = f.choix_forme[rand() % 6];
    f.color = tab_color[rand() % 5];
    block g1, m1, d1, g2, m2, d2, g3, m3, d3;
    if (f.lettre == 'L')
    {

        g1.dest.w = 30;
        g1.dest.h = 30;
        g1.dest.x = 370;
        g1.dest.y = 120;

        g2.dest.w = 30;
        g2.dest.h = 30;
        g2.dest.x = 370;
        g2.dest.y = 150;

        g3.dest.w = 30;
        g3.dest.h = 30;
        g3.dest.x = 370;
        g3.dest.y = 180;

        m3.dest.w = 30;
        m3.dest.h = 30;
        m3.dest.x = 400;
        m3.dest.y = 180;

        f.liste_block[0] = g1;
        f.liste_block[1] = g2;
        f.liste_block[2] = g3;
        f.liste_block[3] = m3;

        strcpy(f.liste_block[0].emplacement, "g1");
        strcpy(f.liste_block[1].emplacement, "g2");
        strcpy(f.liste_block[2].emplacement, "g3");
        strcpy(f.liste_block[3].emplacement, "m3");

        strcpy(f.liste_emplacement[0], "g1");
        strcpy(f.liste_emplacement[1], "g2");
        strcpy(f.liste_emplacement[2], "g3");
        strcpy(f.liste_emplacement[3], "m3");

        f.liste_block[0].cote_droit = 3;
        f.liste_block[0].cote_gauche = 0;
        f.liste_block[1].cote_droit = 3;
        f.liste_block[1].cote_gauche = 0;
        f.liste_block[2].cote_droit = 3;
        f.liste_block[2].cote_gauche = 0;
        f.liste_block[3].cote_droit = 2;
        f.liste_block[3].cote_gauche = 1;

        f.indice_tab = 3;
    }
    else if (f.lettre == 'O')
    {

        g1.dest.w = 30;
        g1.dest.h = 30;
        g1.dest.x = 370;
        g1.dest.y = 120;

        m1.dest.w = 30;
        m1.dest.h = 30;
        m1.dest.x = 400;
        m1.dest.y = 120;

        g2.dest.w = 30;
        g2.dest.h = 30;
        g2.dest.x = 370;
        g2.dest.y = 150;

        m2.dest.w = 30;
        m2.dest.h = 30;
        m2.dest.x = 400;
        m2.dest.y = 150;

        f.liste_block[0] = g1;
        f.liste_block[1] = g2;
        f.liste_block[2] = m1;
        f.liste_block[3] = m2;

        strcpy(f.liste_block[0].emplacement, "g1");
        strcpy(f.liste_block[1].emplacement, "g2");
        strcpy(f.liste_block[2].emplacement, "m1");
        strcpy(f.liste_block[3].emplacement, "m2");

        strcpy(f.liste_emplacement[0], "g1");
        strcpy(f.liste_emplacement[1], "g2");
        strcpy(f.liste_emplacement[2], "m1");
        strcpy(f.liste_emplacement[3], "m2");

        f.liste_block[0].cote_droit = 3;
        f.liste_block[0].cote_gauche = 0;
        f.liste_block[1].cote_droit = 3;
        f.liste_block[1].cote_gauche = 0;
        f.liste_block[2].cote_droit = 2;
        f.liste_block[2].cote_gauche = 1;
        f.liste_block[3].cote_droit = 2;
        f.liste_block[3].cote_gauche = 1;

        f.indice_tab = 3;
    }
    else if (f.lettre == 'T')
    {

        g1.dest.w = 30;
        g1.dest.h = 30;
        g1.dest.x = 370;
        g1.dest.y = 120;

        m1.dest.w = 30;
        m1.dest.h = 30;
        m1.dest.x = 400;
        m1.dest.y = 120;

        d1.dest.w = 30;
        d1.dest.h = 30;
        d1.dest.x = 430;
        d1.dest.y = 120;

        m2.dest.w = 30;
        m2.dest.h = 30;
        m2.dest.x = 400;
        m2.dest.y = 150;

        f.liste_block[0] = g1;
        f.liste_block[1] = d1;
        f.liste_block[2] = m1;
        f.liste_block[3] = m2;

        strcpy(f.liste_block[0].emplacement, "g1");
        strcpy(f.liste_block[1].emplacement, "d1");
        strcpy(f.liste_block[2].emplacement, "m1");
        strcpy(f.liste_block[3].emplacement, "m2");

        strcpy(f.liste_emplacement[0], "g1");
        strcpy(f.liste_emplacement[1], "d1");
        strcpy(f.liste_emplacement[2], "m1");
        strcpy(f.liste_emplacement[3], "m2");

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
        g1.dest.y = 120;

        g2.dest.w = 30;
        g2.dest.h = 30;
        g2.dest.x = 370;
        g2.dest.y = 150;

        m3.dest.w = 30;
        m3.dest.h = 30;
        m3.dest.x = 400;
        m3.dest.y = 180;

        m2.dest.w = 30;
        m2.dest.h = 30;
        m2.dest.x = 400;
        m2.dest.y = 150;

        f.liste_block[0] = g1;
        f.liste_block[1] = g2;
        f.liste_block[2] = m3;
        f.liste_block[3] = m2;

        strcpy(f.liste_block[0].emplacement, "g1");
        strcpy(f.liste_block[1].emplacement, "g2");
        strcpy(f.liste_block[2].emplacement, "m3");
        strcpy(f.liste_block[3].emplacement, "m2");

        strcpy(f.liste_emplacement[0], "g1");
        strcpy(f.liste_emplacement[1], "g2");
        strcpy(f.liste_emplacement[2], "m3");
        strcpy(f.liste_emplacement[3], "m2");

        f.liste_block[0].cote_droit = 3;
        f.liste_block[0].cote_gauche = 0;
        f.liste_block[1].cote_droit = 3;
        f.liste_block[1].cote_gauche = 0;
        f.liste_block[2].cote_droit = 2;
        f.liste_block[2].cote_gauche = 1;
        f.liste_block[3].cote_droit = 2;
        f.liste_block[3].cote_gauche = 1;

        f.indice_tab = 3;
    }
    else if (f.lettre == 'S')
    {

        d1.dest.w = 30;
        d1.dest.h = 30;
        d1.dest.x = 430;
        d1.dest.y = 120;

        d2.dest.w = 30;
        d2.dest.h = 30;
        d2.dest.x = 430;
        d2.dest.y = 150;

        m3.dest.w = 30;
        m3.dest.h = 30;
        m3.dest.x = 400;
        m3.dest.y = 180;

        m2.dest.w = 30;
        m2.dest.h = 30;
        m2.dest.x = 400;
        m2.dest.y = 150;

        f.liste_block[0] = d2;
        f.liste_block[1] = d1;
        f.liste_block[2] = m3;
        f.liste_block[3] = m2;

        strcpy(f.liste_block[0].emplacement, "d2");
        strcpy(f.liste_block[1].emplacement, "d1");
        strcpy(f.liste_block[2].emplacement, "m3");
        strcpy(f.liste_block[3].emplacement, "m2");

        strcpy(f.liste_emplacement[0], "d2");
        strcpy(f.liste_emplacement[1], "d1");
        strcpy(f.liste_emplacement[2], "m3");
        strcpy(f.liste_emplacement[3], "m2");

        f.liste_block[0].cote_droit = 1;
        f.liste_block[0].cote_gauche = 2;
        f.liste_block[1].cote_droit = 1;
        f.liste_block[1].cote_gauche = 2;
        f.liste_block[2].cote_droit = 2;
        f.liste_block[2].cote_gauche = 1;
        f.liste_block[3].cote_droit = 2;
        f.liste_block[3].cote_gauche = 1;

        f.indice_tab = 3;
    }
    else if (f.lettre == 'I')
    {

        m1.dest.w = 30;
        m1.dest.h = 30;
        m1.dest.x = 400;
        m1.dest.y = 120;

        m2.dest.w = 30;
        m2.dest.h = 30;
        m2.dest.x = 400;
        m2.dest.y = 150;

        m3.dest.w = 30;
        m3.dest.h = 30;
        m3.dest.x = 400;
        m3.dest.y = 180;

        f.liste_block[0] = m1;
        f.liste_block[1] = m2;
        f.liste_block[2] = m3;

        strcpy(f.liste_block[0].emplacement, "m1");
        strcpy(f.liste_block[1].emplacement, "m2");
        strcpy(f.liste_block[2].emplacement, "m3");

        strcpy(f.liste_emplacement[0], "m1");
        strcpy(f.liste_emplacement[1], "m2");
        strcpy(f.liste_emplacement[2], "m3");

        f.liste_block[0].cote_droit = 2;
        f.liste_block[0].cote_gauche = 1;
        f.liste_block[1].cote_droit = 2;
        f.liste_block[1].cote_gauche = 1;
        f.liste_block[2].cote_droit = 2;
        f.liste_block[2].cote_gauche = 1;

        f.indice_tab = 2;
    }
}

void actualiser_forme(forme &f, forme &prochain, int vitesse, int &indice, int nb_block[50], block tab_block[1000], SDL_Color tab_color[15], bool &close, int score, char time[100])
{
    bool new_forme = false;
    for (int i = 0; i <= f.indice_tab; i++)
    {
        if (f.liste_block[i].move == true)
        {
            f.liste_block[i].dest.y += vitesse;
        }
        else
        {
            if (f.liste_block[i].dest.y / 30 <= 6)
            {
                close = true;
            }

            else
            {
                f.liste_block[i].nbLigne = f.liste_block[i].dest.y / 30;
                f.liste_block[i].dest.y = 30 * f.liste_block[i].nbLigne;
                nb_block[f.liste_block[i].dest.y / 30] += 1;
                f.liste_block[i].color = f.color;
                tab_block[indice] = f.liste_block[i];
                indice += 1;

                new_forme = true;
            }
        }
    }

    if (new_forme == true)
    {
        for (int i = 0; i <= prochain.indice_tab; i++)
        {
            prochain.liste_block[i].dest.x -= 285;
            prochain.liste_block[i].dest.y -= 35;
        }
        forme g;

        init_forme(g, tab_color);

        f = prochain;
        prochain = g;
        for (int i = 0; i <= prochain.indice_tab; i++)
        {
            prochain.liste_block[i].dest.x += 285;
            prochain.liste_block[i].dest.y += 35;
        }
    }
    if (close == true)
    {
        cout << " 1";
        if (verifierEtCreerFichier("files/score.txt") == true)
        {

            char temp[100];
            sprintf(temp, "%d ", score);
            printf(temp);
            strcat(temp, time);

            ecrireDansFichier("files/score.txt", temp);
        }
    }
}

void check_colission_plateau(forme &f)
{

    for (int i = 0; i <= f.indice_tab; i++)
    {

        if (f.liste_block[i].dest.x + 30 * f.liste_block[i].cote_droit > 551)
        {
            f.liste_block[i].dest.x = 549 - 30 * f.liste_block[i].cote_droit;
        }

        if (f.liste_block[i].dest.x - 30 * f.liste_block[i].cote_gauche <= 253)
        {
            f.liste_block[i].dest.x = 250 + 30 * f.liste_block[i].cote_gauche;
        }

        if (f.liste_block[i].dest.y + f.liste_block[i].dest.h > 810)
        {
            f.liste_block[i].dest.y = 810 - f.liste_block[i].dest.h;

            for (int y = 0; y <= f.indice_tab; y++)
            {
                f.liste_block[y].move = false;
            }
        }

        if (f.liste_block[i].dest.y < 0)
        {
            f.liste_block[i].dest.y = 0;
        }
    }
}

bool simple_test_colision(forme &f, block tab_block[1000], int indice)
{
    for (int i = 0; i <= f.indice_tab; i++)
    {
        for (int y = 0; y <= indice; y++)
        {
            if (check_collision(f.liste_block[i].dest, tab_block[y].dest) == true)
            {
                return true;
            }
        }
    }
    return false;
}

void check_colision_block(forme &f, block tab_block[1000], int indice, bool &bas)
{
    bool test = true;
    for (int i = 0; i <= f.indice_tab; i++)
    {
        for (int y = 0; y <= indice; y++)
        {
            if (check_collision(f.liste_block[i].dest, tab_block[y].dest) == true)
            {
                test = false;
            }
        }
    }
    if (test == false)
    {
        for (int i = 0; i <= f.indice_tab; i++)
        {
            f.liste_block[i].move = false;
        }
    }
}

void rotation(forme &f)
{

    if (f.lettre != 'O')
    {
        for (int i = 0; i <= f.indice_tab; i++)
        {

            if (strcmp(f.liste_block[i].emplacement, "g0") == true)
            {

                f.liste_block[i].dest.x += 60;
                strcpy(f.liste_block[i].emplacement, "d1");
                strcpy(f.liste_emplacement[i], "d1");
                f.liste_block[i].cote_gauche = 2;
                f.liste_block[i].cote_droit = 1;
            }

            else if (strcmp(f.liste_block[i].emplacement, "m0") == true)
            {

                f.liste_block[i].dest.x += 30;
                f.liste_block[i].dest.y += 30;
                strcpy(f.liste_block[i].emplacement, "d2");
                strcpy(f.liste_emplacement[i], "d2");
                f.liste_block[i].cote_gauche = 2;
                f.liste_block[i].cote_droit = 1;
            }

            else if (strcmp(f.liste_block[i].emplacement, "d0") == true)
            {

                f.liste_block[i].dest.y += 60;
                strcpy(f.liste_block[i].emplacement, "d3");
                strcpy(f.liste_emplacement[i], "d3");
                f.liste_block[i].cote_gauche = 2;
                f.liste_block[i].cote_droit = 1;
            }

            else if (strcmp(f.liste_block[i].emplacement, "g1") == true)
            {

                f.liste_block[i].dest.x += 30;
                f.liste_block[i].dest.y -= 30;
                strcpy(f.liste_block[i].emplacement, "m1");
                strcpy(f.liste_emplacement[i], "m1");

                f.liste_block[i].cote_gauche = 1;
                f.liste_block[i].cote_droit = 2;
            }

            else if (strcmp(f.liste_block[i].emplacement, "d1") == true)
            {

                f.liste_block[i].dest.x -= 30;
                f.liste_block[i].dest.y += 30;
                strcpy(f.liste_block[i].emplacement, "m3");
                strcpy(f.liste_emplacement[i], "m3");
                f.liste_block[i].cote_gauche = 1;
                f.liste_block[i].cote_droit = 2;
            }

            else if (strcmp(f.liste_block[i].emplacement, "g2") == true)

            {

                f.liste_block[i].dest.y -= 60;
                strcpy(f.liste_block[i].emplacement, "g1");
                strcpy(f.liste_emplacement[i], "g1");
                f.liste_block[i].cote_gauche = 0;
                f.liste_block[i].cote_droit = 3;
            }

            else if (strcmp(f.liste_block[i].emplacement, "m2") == true)
            {

                f.liste_block[i].dest.x -= 30;
                f.liste_block[i].dest.y -= 30;
                strcpy(f.liste_block[i].emplacement, "g2");
                strcpy(f.liste_emplacement[i], "g2");
                f.liste_block[i].cote_gauche = 0;
                f.liste_block[i].cote_droit = 3;
            }

            else if (strcmp(f.liste_block[i].emplacement, "d2") == true)
            {

                f.liste_block[i].dest.x -= 60;
                strcpy(f.liste_block[i].emplacement, "g3");
                strcpy(f.liste_emplacement[i], "g3");
                f.liste_block[i].cote_gauche = 0;
                f.liste_block[i].cote_droit = 3;
            }
            else if (strcmp(f.liste_block[i].emplacement, "m1") == true)
            {

                strcpy(f.liste_block[i].emplacement, "m2");
                strcpy(f.liste_emplacement[i], "m2");
                f.liste_block[i].cote_gauche = 1;
                f.liste_block[i].cote_droit = 2;
            }
        }
        f.vide = false;
    }
}

void test_vide(forme &f)
{
    bool droite = false, gauche = false;
    if (f.vide == false)
    {
        for (int i = 0; i <= f.indice_tab; i++)
        {
            if (strcmp(f.liste_emplacement[i], "g0") == true || strcmp(f.liste_emplacement[i], "g1") == true || strcmp(f.liste_emplacement[i], "g2") == true)
            {
                gauche = true;
            }
            else if (strcmp(f.liste_emplacement[i], "d0") == true || strcmp(f.liste_emplacement[i], "d1") == true || strcmp(f.liste_emplacement[i], "d2") == true)
            {
                droite = true;
            }
        }

        if (droite == false)
        {
            for (int i = 0; i <= f.indice_tab; i++)
            {
                f.liste_block[i].cote_droit -= 1;
            }
            f.vide = true;
        }
        if (gauche == false)
        {
            for (int i = 0; i <= f.indice_tab; i++)
            {
                f.liste_block[i].cote_gauche -= 1;
            }
            f.vide = true;
        }
    }
}

void timer(int &heure, int &minute, int &seconde, TTF_Font *dogica, SDL_Color blanc, SDL_Renderer *rend, SDL_Rect t_timer, char time[100])
{
    seconde = (SDL_GetTicks() / 1000) % 60;
    minute = ((SDL_GetTicks() / 1000) / 60) % 60;
    heure = (SDL_GetTicks() / 1000) / 3600;

    if (heure < 10)
    {
        if (minute < 10)
        {
            if (seconde < 10)
            {
                sprintf(time, "Timer : 0%d : 0%d : 0%d", heure, minute, seconde);
            }
            else
            {
                sprintf(time, "Timer : 0%d : 0%d : %d", heure, minute, seconde);
            }
        }
        else
        {
            if (seconde < 10)
            {
                sprintf(time, "Timer : 0%d : %d : 0%d", heure, minute, seconde);
            }
            else
            {
                sprintf(time, "Timer : 0%d : %d : %d", heure, minute, seconde);
            }
        }
    }
    else
    {
        if (minute < 10)
        {
            if (seconde < 10)
            {
                sprintf(time, "Timer : %d : 0%d : 0%d", heure, minute, seconde);
            }
            else
            {
                sprintf(time, "Timer : %d : 0%d : %d", heure, minute, seconde);
            }
        }
        else
        {
            if (seconde < 10)
            {
                sprintf(time, "Timer : %d : %d : 0%d", heure, minute, seconde);
            }
            else
            {
                sprintf(time, "Timer : %d : %d : %d", heure, minute, seconde);
            }
        }
    }

    SDL_Surface *texte_timer = TTF_RenderText_Blended(dogica, time, blanc);
    SDL_Texture *pTextureTxtTimer = SDL_CreateTextureFromSurface(rend, texte_timer);
    SDL_FreeSurface(texte_timer);
    SDL_RenderCopy(rend, pTextureTxtTimer, nullptr, &t_timer);
}

bool verifierEtCreerFichier(const char *nomFichier)
{
    FILE *fichier = fopen(nomFichier, "r");

    if (fichier != nullptr)
    {
        fclose(fichier);
        return true; // Le fichier existe déjà
    }
    else
    {
        fichier = fopen(nomFichier, "w");

        if (fichier != nullptr)
        {
            fclose(fichier);
            return true; // Le fichier a été créé
        }
        else
        {
            return false; // Erreur lors de la création du fichier
        }
    }
}

bool ecrireDansFichier(const char *nomFichier, const char *texte)
{
    FILE *fichier = std::fopen(nomFichier, "a");

    if (fichier != nullptr)
    {
        std::fprintf(fichier, "%s\n", texte);
        std::fclose(fichier);
        return true; // Écriture réussie
    }
    else
    {
        return false; // Erreur lors de l'ouverture du fichier
    }
}

int game()
{

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Erreur lors de l'initialisation de SDL: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        return 1;
    }
    if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
    {
        SDL_Log("Erreur initialisation SDL_mixer : %s", Mix_GetError());
        SDL_Quit();
        return -1;
    }

    srand(time(NULL));

    SDL_Window *win = SDL_CreateWindow("TETRIS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);

    SDL_Color blanc = {255, 255, 255};
    TTF_Font *dogica = TTF_OpenFont("font/dogica.ttf", 16);
    if (dogica == NULL)
    {
        fprintf(stderr, "Impossible de charger \"dogica.ttf\"");
        exit(EXIT_FAILURE);
    }
    SDL_Surface *texte_prochain = TTF_RenderText_Blended(dogica, "NEXT", blanc);
    int txtW = 0;
    int txtH = 0;
    SDL_Texture *pTextureTxt = SDL_CreateTextureFromSurface(rend, texte_prochain);
    SDL_QueryTexture(pTextureTxt, NULL, NULL, &txtW, &txtH);
    SDL_Rect t_pro;
    t_pro.x = 670;
    t_pro.y = 120;
    t_pro.w = txtW;
    t_pro.h = txtH;

    SDL_Rect plateau;
    plateau.h = 712;
    plateau.w = 303;
    plateau.x = 248;
    plateau.y = 100;

    SDL_Rect rect_prochain;
    rect_prochain.h = 100;
    rect_prochain.w = 100;
    rect_prochain.x = 650;
    rect_prochain.y = 150;

    int nb_block[50] = {0};

    block tab_block[1000];
    int indice = 0;

    int vitesse = 1;

    bool close = false, bas = true;
    int speed = 300;

    forme f;
    forme prochain;

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

    SDL_Color purple = SDL_Color();
    yellow.r = 76;
    yellow.g = 0;
    yellow.b = 153;
    yellow.a = 255;

    SDL_Color tab_color[15] = {blue, red, green, orange, yellow, purple};

    init_forme(f, tab_color);

    init_forme(prochain, tab_color);

    for (int i = 0; i <= prochain.indice_tab; i++)
    {
        prochain.liste_block[i].dest.x += 285;
        prochain.liste_block[i].dest.y += 35;
    }

    int minute = 0, seconde = 0, heure = 0;
    char time[100];
    sprintf(time, "Timer : %d : %d : %d", heure, minute, seconde);
    SDL_Surface *texte_timer = TTF_RenderText_Blended(dogica, time, blanc);
    int txtTimerW = 0;
    int txtTimerH = 0;
    SDL_Texture *pTextureTxtTimer = SDL_CreateTextureFromSurface(rend, texte_timer);
    SDL_FreeSurface(texte_timer);
    SDL_QueryTexture(pTextureTxtTimer, NULL, NULL, &txtTimerW, &txtTimerH);
    SDL_Rect t_timer;
    t_timer.x = 260;
    t_timer.y = 50;
    t_timer.w = txtTimerW;
    t_timer.h = txtTimerH;

    int score = 0;
    char sco[100];
    sprintf(sco, "Score: %d ", score);
    SDL_Surface *texte_score = TTF_RenderText_Blended(dogica, sco, blanc);
    int txtScoreW = 0;
    int txtScoreH = 0;
    SDL_Texture *pTextureTxtScore = SDL_CreateTextureFromSurface(rend, texte_score);
    SDL_FreeSurface(texte_score);
    SDL_QueryTexture(pTextureTxtScore, NULL, NULL, &txtScoreW, &txtScoreH);
    SDL_Rect t_score;
    t_score.x = 600;
    t_score.y = 305;
    t_score.w = txtScoreW;
    t_score.h = txtScoreH;

    SDL_Rect rect_score;
    rect_score.x = 595;
    rect_score.y = 290;
    rect_score.w = 200;
    rect_score.h = 50;

    Mix_Music *musique;
    musique = Mix_LoadMUS("music/tetris_theme.mp3");
    Mix_PlayMusic(musique, -1);
    Mix_VolumeMusic(10);

    while (!close)
    {

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                close = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:

                    rotation(f);
                    if (simple_test_colision(f, tab_block, indice) == true)
                    {
                        rotation(f);
                        rotation(f);
                        rotation(f);
                    }

                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:

                    for (int i = 0; i <= f.indice_tab; i++)
                    {
                        f.liste_block[i].dest.x -= 30;
                    }
                    if (simple_test_colision(f, tab_block, indice) == true)
                    {
                        for (int i = 0; i <= f.indice_tab; i++)
                        {
                            f.liste_block[i].dest.x += 30;
                        }
                    }

                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    score += 1;

                    for (int i = 0; i <= f.indice_tab; i++)
                    {
                        f.liste_block[i].dest.y += 30;
                    }
                    if (simple_test_colision(f, tab_block, indice) == true)
                    {
                        for (int i = 0; i <= f.indice_tab; i++)
                        {
                            f.liste_block[i].dest.y -= 30;
                        }
                    }

                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    for (int i = 0; i <= f.indice_tab; i++)
                    {
                        f.liste_block[i].dest.x += 30;
                    }
                    if (simple_test_colision(f, tab_block, indice) == true)
                    {
                        for (int i = 0; i <= f.indice_tab; i++)
                        {
                            f.liste_block[i].dest.x -= 30;
                        }
                    }

                    break;

                default:
                    break;
                }
                break;
            }
        }

        check_colission_plateau(f);
        actualiser_forme(f, prochain, vitesse, indice, nb_block, tab_block, tab_color, close, score, time);

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderClear(rend);

        SDL_RenderCopy(rend, pTextureTxt, nullptr, &t_pro);

        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
        SDL_RenderDrawRect(rend, &plateau);

        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
        SDL_RenderDrawRect(rend, &rect_prochain);
        SDL_RenderDrawRect(rend, &rect_score);

        timer(heure, minute, seconde, dogica, blanc, rend, t_timer, time);

        sprintf(sco, "Score: %d ", score);
        SDL_Surface *texte_score = TTF_RenderText_Blended(dogica, sco, blanc);

        SDL_Texture *pTextureTxtScore = SDL_CreateTextureFromSurface(rend, texte_score);
        SDL_FreeSurface(texte_score);
        SDL_RenderCopy(rend, pTextureTxtScore, nullptr, &t_score);

        draw_forme(prochain, rend);

        draw_forme(f, rend);

        check_colision_block(f, tab_block, indice, bas);

        test_vide(f);

        for (int y = 0; y <= indice; y++)
        {

            SDL_SetRenderDrawColor(rend, tab_block[y].color.r, tab_block[y].color.g, tab_block[y].color.b, 255);
            SDL_RenderFillRect(rend, &tab_block[y].dest);

            SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
            SDL_RenderDrawRect(rend, &tab_block[y].dest);
        }

        int ligne = check_ligne(nb_block);

        if (ligne > 2)
        {
            score += 40;

            for (int i = 0; i <= indice; i++)
            {

                if (tab_block[i].nbLigne + 1 < ligne)
                {
                    nb_block[tab_block[i].nbLigne] -= 1;
                    tab_block[i].nbLigne += 1;

                    nb_block[tab_block[i].nbLigne] += 1;
                    if ((tab_block[i].dest.y) % 30 == 0)
                    {
                        tab_block[i].dest.y += 30;
                    }
                    else
                    {

                        tab_block[i].dest.y = 30 * tab_block[i].nbLigne;
                    }
                }

                else if (tab_block[i].nbLigne + 1 == ligne)
                {
                    sup_case(i, indice, tab_block);
                    nb_block[ligne - 1] -= 1;
                    i -= 1;
                    indice -= 1;
                }
            }
        }

        SDL_RenderPresent(rend);
        SDL_Delay(1000 / 60);
    }
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    TTF_CloseFont(dogica);
    Mix_FreeMusic(musique);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    menu();
}