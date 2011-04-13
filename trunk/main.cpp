using namespace std;

#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

#include <tinyxml.h>

#include <SFML/Graphics.hpp>

#define MAX_FRAME 3

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
#define GFX_BOBO 4
#define GFX_COIN 5
#define GFX_DIALOGCORNER 6
#define GFX_DIALOGEXTENSION 7
#define GFX_DRESS0 8
#define GFX_DRESS1 9
#define GFX_DRESS2 10
#define GFX_EMOTEAGGRO 11
#define GFX_EMOTEDEAD 12
#define GFX_EMOTEFEAR 13
#define GFX_FEET0 14
#define GFX_FEET1 15
#define GFX_FEET2 16
#define GFX_FX0 17
#define GFX_HAT 18
#define GFX_HEAD0 19
#define GFX_HEAD1 20
#define GFX_HEAD2 21
#define GFX_INTERFACE 22
#define GFX_MINIHP 23
#define GFX_MOB0 24
#define GFX_PET 25
#define GFX_POTIONL 26
#define GFX_POTIONM 27
#define GFX_PYRO0 28
#define GFX_SKY0 29
#define GFX_STATE 30
#define GFX_WAND0 31
#define GFX_WAND1 32
#define GFX_WAND2 33
#define GFX_WORLD01 34
#define GFX_WORLD02 35
#define GFX_WORLD03 36
#define GFX_WORLDCOLLIDE01 37
#define GFX_WORLDCOLLIDE02 38
#define GFX_WORLDCOLLIDE03 39
#define GFX_WORLDOCCLUDE01 40
#define GFX_WORLDOCCLUDE02 41
#define GFX_WORLDOCCLUDE03 42

void blit(sf::Image* imagesrc, sf::Image* imagedest, int w, int h, int shift);
void white(sf::Image* image, int w, int h, int inshift, int outshift);

sf::Image imageslist[GFX_WORLDOCCLUDE03 + 1];
void prepareimages();
sf::Image getimage(int id);

sf::Font mainfont;
void preparefont();
sf::Font getfont();

#include "dialog.h"
#include "bobo.h"
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
#include "xml.h"

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

    Xml* xml = new Xml();
    xml->readinfos();
    xml->readkeys();

    // fps stuff

    char fpstxt[100];
    sf::String fpsstring;

    fpsstring.SetFont(mainfont);
    fpsstring.SetSize(16);
    fpsstring.SetColor(sf::Color::White);
    fpsstring.SetX(800 - 128);
    fpsstring.SetY(2);

    // our objects

    Bobo* bobo = new Bobo();
    Game* game = new Game(xml->infolevel, xml->infoexp, xml->infomoney, bobo);
    Map* map = new Map();
    Scene* scene = new Scene(xml->infoherox, map);

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
                if (event.Key.Code == xml->keyleft)
                    scene->keyleft();
                if (event.Key.Code == xml->keyright)
                    scene->keyright();
                if (event.Key.Code == xml->keyjump)
                    scene->keyup();
                if (event.Key.Code == xml->keyaction)
                    scene->keyaction(game);
                if (event.Key.Code == xml->keypet)
                    scene->keypetcontrol();
                if (event.Key.Code == sf::Key::Escape)
                    app.Close();
            }
            else if (event.Type == sf::Event::KeyReleased)
            {
                if (event.Key.Code == xml->keyleft)
                    scene->keyleftup();
                if (event.Key.Code == xml->keyright)
                    scene->keyrightup();
                if (event.Key.Code == xml->keyaction)
                    scene->keyactionup();
            }
        }

        game->animate();
        bobo->animate();

        int distance = round(100.0f * app.GetFrameTime());
        if (distance < 300) // frametime init bugfix
            scene->animate(distance, &view, game, map);

        scene->draw(&app, &view);

        app.SetView(app.GetDefaultView());
        game->draw(&app);
        bobo->draw(&app);

        // fps stuff display

        snprintf(fpstxt, 99, "fps (%f)", 1.0f / app.GetFrameTime());
        fpsstring.SetText(fpstxt);
        app.Draw(fpsstring);

        app.SetView(view);
        map->draw(&app, view);

        app.Display();
    }

    xml->infolevel = game->level;
    xml->infoexp = game->exp;
    xml->infomoney = game->money;
    xml->infoherox = scene->hero->x;
    xml->writeinfos();

    xml->writekeys();

    delete bobo;
    delete scene;
    delete map;
    delete game;
    delete xml;

    cout << "Mission sucess !" << endl;
    return EXIT_SUCCESS;
}

void blit(sf::Image* imagesrc, sf::Image* imagedest, int w, int h, int shift)
{
    sf::Color pixel0, pixel1, pixel2;
    for(int x = 0; x < w; x++)
    {
        for(int y = 0; y < h; y++)
        {
            pixel0 = imagesrc->GetPixel(x, y);
            pixel1 = imagedest->GetPixel(x + (w * shift), y);
            pixel2.r = pixel1.r;
            pixel2.g = pixel1.g;
            pixel2.b = pixel1.b;
            pixel2.a = 255;

            if (pixel0.a < 255 && pixel1.a == 255)
                imagedest->SetPixel(x + (w * shift), y, pixel1);
            else if (!pixel1.a)
                imagedest->SetPixel(x + (w * shift), y, pixel0);
            else if (pixel0.a && pixel1.a)
                imagedest->SetPixel(x + (w * shift), y, pixel2);
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
    imageslist[GFX_BOBO].LoadFromFile("gfx/bobo.png");
    imageslist[GFX_COIN].LoadFromFile("gfx/coin.png");
    imageslist[GFX_DIALOGCORNER].LoadFromFile("gfx/dialog_corner.png");
    imageslist[GFX_DIALOGEXTENSION].LoadFromFile("gfx/dialog_extension.png");
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
    imageslist[GFX_WORLD01].LoadFromFile("gfx/world01.png");
    imageslist[GFX_WORLD02].LoadFromFile("gfx/world02.png");
    imageslist[GFX_WORLD03].LoadFromFile("gfx/world03.png");
    imageslist[GFX_WORLDCOLLIDE01].LoadFromFile("gfx/worldcollide01.png");
    imageslist[GFX_WORLDCOLLIDE02].LoadFromFile("gfx/worldcollide02.png");
    imageslist[GFX_WORLDCOLLIDE03].LoadFromFile("gfx/worldcollide03.png");
    imageslist[GFX_WORLDOCCLUDE01].LoadFromFile("gfx/worldocclude01.png");
    imageslist[GFX_WORLDOCCLUDE02].LoadFromFile("gfx/worldocclude02.png");
    imageslist[GFX_WORLDOCCLUDE03].LoadFromFile("gfx/worldocclude03.png");
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
