using namespace std;

#include <iostream>
#include <stdio.h>
#include <vector>

#include <SFML/Graphics.hpp>

#define MAX_FRAME 4

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

#define GFX_ARROW 0
#define GFX_BAREXP 1
#define GFX_BARLIFE 2
#define GFX_BARMANA 3
#define GFX_COIN 4
#define GFX_DRESS0 5
#define GFX_DRESS1 6
#define GFX_DRESS2 7
#define GFX_EMOTEAGGRO 8
#define GFX_EMOTEDEAD 9
#define GFX_EMOTEFEAR 10
#define GFX_FEET0 11
#define GFX_FEET1 12
#define GFX_FEET2 13
#define GFX_FX0 14
#define GFX_HAT 15
#define GFX_HEAD0 16
#define GFX_HEAD1 17
#define GFX_HEAD2 18
#define GFX_INTERFACE 19
#define GFX_MINIHP 20
#define GFX_MOB0 21
#define GFX_PET 22
#define GFX_POTIONL 23
#define GFX_POTIONM 24
#define GFX_PYRO0 25
#define GFX_SKY0 26
#define GFX_STATE 27
#define GFX_WAND0 28
#define GFX_WAND1 29
#define GFX_WAND2 30
#define GFX_WORLD0 31
#define GFX_WORLD0COLLISION 32
#define GFX_WORLD0OCCLUDE 33

sf::Image imageslist[GFX_WORLD0OCCLUDE + 1];
sf::Font mainfont;

void blit(sf::Image* imagesrc, sf::Image* imagedest, int w, int h, int shift);
void white(sf::Image* image, int w, int h, int inshift, int outshift);
void prepareimages();
sf::Image getimage(int id);
void preparefont();
sf::Font getfont();

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

    sf::RenderWindow app(sf::VideoMode(800, 600, 32), "Mirel the Witch (Development version)");
    app.SetFramerateLimit(60);

    sf::View view(sf::FloatRect(0, 0, 800, 600));
    app.SetView(view);

    // program main loop init

    prepareimages();
    preparefont();

    // fps stuff

    char fpstxt[100];
    sf::Color fpscolor(255, 255, 255);
    sf::String fpsstring;

    fpsstring.SetFont(mainfont);
    fpsstring.SetSize(16);
    fpsstring.SetColor(fpscolor);
    fpsstring.SetX(32);
    fpsstring.SetY(568);

    // our objects

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
        scene->animate(round(100.0f * app.GetFrameTime()), &view, game, map);

        scene->draw(&app, &view);
        app.SetView(app.GetDefaultView());
        game->draw(&app);

        // fps stuff display

        snprintf(fpstxt, 99, "fps (%f)", 1.0f / app.GetFrameTime());
        fpsstring.SetText(fpstxt);
        app.Draw(fpsstring);

        app.SetView(view);
        map->draw(&app, view);

        app.Display();
    }

    delete game;
    delete scene;
    delete map;

    cout << "Mission sucess !" << endl;
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

void prepareimages()
{
    imageslist[GFX_ARROW].LoadFromFile("gfx/arrow.png");
    imageslist[GFX_BAREXP].LoadFromFile("gfx/barexp.png");
    imageslist[GFX_BARLIFE].LoadFromFile("gfx/barlife.png");
    imageslist[GFX_BARMANA].LoadFromFile("gfx/barmana.png");
    imageslist[GFX_COIN].LoadFromFile("gfx/coin.png");
    imageslist[GFX_DRESS0].LoadFromFile("gfx/dress0.png");
    imageslist[GFX_DRESS1].LoadFromFile("gfx/dress1.png");
    imageslist[GFX_DRESS2].LoadFromFile("gfx/dress2.png");
    imageslist[GFX_EMOTEAGGRO].LoadFromFile("gfx/emote_aggro.png");
    imageslist[GFX_EMOTEDEAD].LoadFromFile("gfx/emote_dead.png");
    imageslist[GFX_EMOTEFEAR].LoadFromFile("gfx/emote_fear.png");
    imageslist[GFX_FEET0].LoadFromFile("gfx/feet0.png");
    imageslist[GFX_FEET1].LoadFromFile("gfx/feet1.png");
    imageslist[GFX_FEET2].LoadFromFile("gfx/feet2.png");
    imageslist[GFX_FX0].LoadFromFile("gfx/fx0.png");
    imageslist[GFX_HAT].LoadFromFile("gfx/hat.png");
    imageslist[GFX_HEAD0].LoadFromFile("gfx/head0.png");
    imageslist[GFX_HEAD1].LoadFromFile("gfx/head1.png");
    imageslist[GFX_HEAD2].LoadFromFile("gfx/head2.png");
    imageslist[GFX_INTERFACE].LoadFromFile("gfx/interface.png");
    imageslist[GFX_MINIHP].LoadFromFile("gfx/minihp.png");
    imageslist[GFX_MOB0].LoadFromFile("gfx/mob0.png");
    imageslist[GFX_PET].LoadFromFile("gfx/pet.png");
    imageslist[GFX_POTIONL].LoadFromFile("gfx/potionl.png");
    imageslist[GFX_POTIONM].LoadFromFile("gfx/potionm.png");
    imageslist[GFX_PYRO0].LoadFromFile("gfx/pyro0.png");
    imageslist[GFX_SKY0].LoadFromFile("gfx/sky0.png");
    imageslist[GFX_STATE].LoadFromFile("gfx/state.png");
    imageslist[GFX_WAND0].LoadFromFile("gfx/wand0.png");
    imageslist[GFX_WAND1].LoadFromFile("gfx/wand1.png");
    imageslist[GFX_WAND2].LoadFromFile("gfx/wand2.png");
    imageslist[GFX_WORLD0].LoadFromFile("gfx/world0.png");
    imageslist[GFX_WORLD0COLLISION].LoadFromFile("gfx/world00.png");
    imageslist[GFX_WORLD0OCCLUDE].LoadFromFile("gfx/world000.png");
}

sf::Image getimage(int id)
{
    return imageslist[id];
}

void preparefont()
{
    mainfont.LoadFromFile("font/Plakkaat.ttf");
}

sf::Font getfont()
{
    return mainfont;
}
