using namespace std;

#include <stdio.h>
#include <vector>

#include <SFML/Graphics.hpp>

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

#define LABEL_HERO 0
#define LABEL_PET 1
#define LABEL_MOB 2
#define LABEL_LOOT 3

void blit(sf::Image* imagesrc, sf::Image* imagedest, int w, int h, int shift);
void white(sf::Image* image, int w, int h, int inshift, int outshift);

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
#include "label.h"
#include "map.h"
#include "scene.h"

int main()
{
    // app creation

    sf::RenderWindow app(sf::VideoMode(800, 600, 32), "Mirel the witch");
    app.SetFramerateLimit(140);

    sf::View view(sf::FloatRect(0, 0, 800, 600));
    app.SetView(view);

    // program main loop init

    Game* game = new Game();
    Map* map = new Map();
    Scene* scene = new Scene(map);

    // program main loop

    while (app.IsOpened())
    {
        sf::Event event;
        while (app.GetEvent(event))
        {
            if (event.Type == sf::Event::Closed)
                app.Close();
            if (event.Type == sf::Event::KeyPressed)
            {
                if (event.Key.Code == sf::Key::Q)
                    scene->keyleft();
                if (event.Key.Code == sf::Key::D)
                    scene->keyright();
                if (event.Key.Code == sf::Key::Z)
                    scene->keyup();
                if (event.Key.Code == sf::Key::Space)
                    scene->keyaction(game);
                if (event.Key.Code == sf::Key::A)
                    scene->keypetcontrol();
                if (event.Key.Code == sf::Key::Escape)
                    app.Close();
            }
            else if (event.Type == sf::Event::KeyReleased)
            {
                if (event.Key.Code == sf::Key::Q)
                    scene->keyleftup();
                if (event.Key.Code == sf::Key::D)
                    scene->keyrightup();
                if (event.Key.Code == sf::Key::Space)
                    scene->keyactionup();
            }
        }

        game->animate();
        scene->animate(game, map);

        scene->draw(&app, &view);
        app.SetView(app.GetDefaultView());
        game->draw(&app);
        app.SetView(view);
        map->draw(&app);

        app.Display();
    }

    delete game;
    delete scene;
    delete map;

    printf("Mission sucess !!!\n");
    return EXIT_SUCCESS;
}

void blit(sf::Image* imagesrc, sf::Image* imagedest, int w, int h, int shift)
{
    sf::Color color0, color1, color2;
    for(int x = 0; x < w; x++)
    {
        for(int y = 0; y < h; y++)
        {
            color0 = imagesrc->GetPixel(x, y);
            color1 = imagedest->GetPixel(x + (w * shift), y);
            color2.r = color1.r;
            color2.g = color1.g;
            color2.b = color1.b;
            color2.a = 255;

            if (color0.a < 255 && color1.a == 255)
                imagedest->SetPixel(x + (w * shift), y, color1);
            else if (!color1.a)
                imagedest->SetPixel(x + (w * shift), y, color0);
            else if (color0.a && color1.a)
                imagedest->SetPixel(x + (w * shift), y, color2);
        }
    }
}

void white(sf::Image* image, int w, int h, int inshift, int outshift)
{
    sf::Color pixel;
    for(int x = 0; x < w; x++)
    {
        for(int y = 0; y < h; y++)
        {
            pixel = image->GetPixel(x + (w * inshift), y);
            pixel.r = 255;
            pixel.g = 255;
            pixel.b = 255;

            image->SetPixel(x + (w * outshift), y, pixel);
        }
    }
}
