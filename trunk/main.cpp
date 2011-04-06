#include <cstdlib>
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

#include <GL/gl.h>
#include <GL/glu.h>

#define DIRECTION_LEFT 0
#define DIRECTION_RIGHT 1

#define SCORE_MONEY 0
#define SCORE_DAMAGE 1
#define SCORE_LIFE 2
#define SCORE_MANA 3

#define EMOTE_AGGRO 0
#define EMOTE_FEAR 1
#define EMOTE_DEAD 2

#define LOOT_MONEY 0
#define LOOT_POTIONM 1
#define LOOT_POTIONL 2

Uint32 getpixel(SDL_Surface* surface, int x, int y);
void putpixel(SDL_Surface* surface, int x, int y, Uint32 pixel);
void white(SDL_Surface* surfdest, SDL_Surface* surfsrc);
void blit(SDL_Surface* surfdest, SDL_Surface* surfsrc);
GLuint glloadtexture(SDL_Surface* surface);

#include "magic.h"
#include "hero.h"
#include "mob.h"
#include "loot.h"
#include "game.h"
#include "world.h"
#include "score.h"
#include "emote.h"
#include "particle.h"
#include "pet.h"
#include "scene.h"

using namespace std;

int main (int argc, char** argv)
{
    // rand init

    srand(time(NULL));

    // SDL init

    SDL_Init(SDL_INIT_VIDEO);

    // font init

    TTF_Init();

    // screen init

    SDL_SetVideoMode(800, 600, 32, SDL_OPENGL);
    SDL_WM_SetCaption("Mirel the Witch", NULL);

    // OpenGL init

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // program main loop init

    Game* game = new Game();
    Scene* scene = new Scene();

    // program main loop

    bool done = false;
    while (!done)
    {
        // message processing loop

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    done = true;
                    break;

                case SDL_KEYDOWN:
                {
                    if (event.key.keysym.sym == SDLK_q)
                        scene->keyleft();
                    if (event.key.keysym.sym == SDLK_d)
                        scene->keyright();
                    if (event.key.keysym.sym == SDLK_z)
                        scene->keyup();
                    if (event.key.keysym.sym == SDLK_SPACE)
                        scene->keyaction(game);
                    if (event.key.keysym.sym == SDLK_LCTRL)
                        scene->keycontrol();
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    break;
                }
                case SDL_KEYUP:
                {
                    if (event.key.keysym.sym == SDLK_q)
                        scene->keyleftup();
                    if (event.key.keysym.sym == SDLK_d)
                        scene->keyrightup();
                    if (event.key.keysym.sym == SDLK_SPACE)
                        scene->keyactionup();
                }
            }
        }

        game->animate();
        scene->animate(game);

        scene->draw();
        game->draw();

        SDL_Delay(7);

        glFlush();
        SDL_GL_SwapBuffers();
    }

    delete game;
    delete scene;

    TTF_Quit();

    printf("Mission sucess !!!\n");
    return 0;
}

Uint32 getpixel(SDL_Surface* surface, int x, int y)
{
    Uint32 *pixels = (Uint32 *)surface->pixels;
    return pixels[ ( y * surface->w ) + x ];
}

void putpixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
    Uint32 *pixels = (Uint32 *)surface->pixels;
    pixels[(y * surface->w) + x] = pixel;
}

void white(SDL_Surface* surfdest, SDL_Surface* surfsrc)
{
    Uint8 r, g, b, a;
    for(int x = 0; x < surfsrc->w; x++)
    {
        for(int y = 0; y < surfsrc->h; y++)
        {
            Uint32 pixel = getpixel(surfsrc, x, y);
            SDL_GetRGBA(pixel, surfsrc->format, &r, &g, &b, &a);
            r = 0xff;
            g = 0xff;
            b = 0xff;
            pixel = SDL_MapRGBA(surfsrc->format, r, g, b, a);
            putpixel(surfdest, x, y, pixel);
        }
    }
}

void blit(SDL_Surface* surfdest, SDL_Surface* surfsrc)
{
    Uint8 r, g, b, a;
    for(int x = 0; x < surfsrc->w; x++)
    {
        for(int y = 0; y < surfsrc->h; y++)
        {
            Uint32 pixel = getpixel(surfsrc, x, y);
            SDL_GetRGBA(pixel, surfsrc->format, &r, &g, &b, &a);
            if ((a >= 0x000000AA) && (a <= 0x000000FF))
            {
                putpixel(surfdest, x, y, pixel);
            }
        }
    }
}

GLuint glloadtexture(SDL_Surface* surface)
{
    GLuint glID;

    SDL_Surface* flipped = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);

    for (int current_line = 0; current_line < surface->h; current_line++)
        memcpy(&((unsigned char*)flipped->pixels)[current_line * surface->pitch],
               &((unsigned char*)surface->pixels)[(surface->h - 1 - current_line) * surface->pitch], surface->pitch);

    glGenTextures(1, &glID);
    glBindTexture(GL_TEXTURE_2D, glID);

    glTexImage2D(GL_TEXTURE_2D, 0, 4, flipped->w, flipped->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, flipped->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(flipped);

    return glID;
}
