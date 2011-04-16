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
#define LABEL_BOSS 3
#define LABEL_LOOT 4

#define MAGIC_SPELL0 0
#define MAGIC_SPELL1 1

#define SYS_CONFIGKEYS 0
#define SYS_RESETKEYS 1
#define SYS_JOKE 2
#define SYS_CLOSEDIALOG 3

#define GFX_ARROW 0
#define GFX_BAREXP 1
#define GFX_BARLIFE 2
#define GFX_BARMANA 3
#define GFX_BOBO0 4
#define GFX_BOBO1 5
#define GFX_BOBO2 6
#define GFX_BOSS0 7
#define GFX_COIN0 8
#define GFX_COIN1 9
#define GFX_COIN2 10
#define GFX_COIN3 11
#define GFX_DIALOGCORNER 12
#define GFX_DIALOGDOT 13
#define GFX_DIALOGEXTENSION 14
#define GFX_DRESS0 15
#define GFX_DRESS1 16
#define GFX_DRESS2 17
#define GFX_EMOTEAGGRO 18
#define GFX_EMOTEDEAD 19
#define GFX_EMOTEFEAR 20
#define GFX_FEET0 21
#define GFX_FEET1 22
#define GFX_FEET2 23
#define GFX_FX0 24
#define GFX_HAT 25
#define GFX_HEAD0 26
#define GFX_HEAD1 27
#define GFX_HEAD2 28
#define GFX_INTERFACE 29
#define GFX_INTERFACESPELL0 30
#define GFX_INTERFACESPELL1 31
#define GFX_MINIHP 32
#define GFX_MOB0 33
#define GFX_POTIONL 34
#define GFX_POTIONM 35
#define GFX_SKY0 36
#define GFX_STATE 37
#define GFX_SPELL0 38
#define GFX_SPELL1 39
#define GFX_WAND0 40
#define GFX_WAND1 41
#define GFX_WAND2 42
#define GFX_WORLD01 43
#define GFX_WORLD02 44
#define GFX_WORLD03 45
#define GFX_WORLDCOLLIDE01 46
#define GFX_WORLDCOLLIDE02 47
#define GFX_WORLDCOLLIDE03 48
#define GFX_WORLDOCCLUDE01 49
#define GFX_WORLDOCCLUDE02 50
#define GFX_WORLDOCCLUDE03 51
#define GFX_ZUZU0 52
#define GFX_ZUZU1 53
#define GFX_ZUZU2 54
#define GFX_ZUZU3 55

void blit(sf::Image* srcimage, sf::Image* destimage);
void white(sf::Image* srcimage, sf::Image* destimage);

sf::Image imageslist[GFX_ZUZU3 + 1];
void prepareimages();
sf::Image getimage(int id);

sf::Font mainfont;
void preparefont();
sf::Font getfont();

#include "system.h"
#include "dialog.h"
#include "bobo.h"
#include "magic.h"
#include "hero.h"
#include "boss.h"
#include "mob.h"
#include "pet.h"
#include "loot.h"
#include "game.h"
#include "world.h"
#include "score.h"
#include "emote.h"
#include "particle.h"
#include "label.h"
#include "map.h"
#include "scene.h"
#include "xml.h"

int main()
{
    // app creation

    sf::RenderWindow app(sf::VideoMode(800, 600, 32), "Mirel the Witch");
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

    System* sys = new System();
    Bobo* bobo = new Bobo(sys);
    Game* game = new Game(xml->infolevel, xml->infoexp, xml->infomoney, xml->infocurrentmagic, bobo);
    Map* map = new Map();
    Scene* scene = new Scene(xml->infoherox, xml->infopetx, xml->infolevel, game, map);

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
                if (sys->configurekeys)
                {
                    if (!sys->keyleft)
                        xml->keyleft = sys->keyleft = event.Key.Code;
                    else if (!sys->keyright)
                        xml->keyright = sys->keyright = event.Key.Code;
                    else if (!sys->keyjump)
                        xml->keyjump = sys->keyjump = event.Key.Code;
                    else if (!sys->keyaction)
                        xml->keyaction = sys->keyaction = event.Key.Code;
                    else if (!sys->keypet)
                        xml->keypet = sys->keypet = event.Key.Code;
                }
                else
                {
                    if (event.Key.Code == xml->keyleft)
                        scene->keyleft();
                    if (event.Key.Code == xml->keyright)
                        scene->keyright();
                    if (event.Key.Code == xml->keyjump)
                        scene->keyup();
                    if (event.Key.Code == xml->keyaction)
                        scene->keyaction();
                    if (event.Key.Code == xml->keypet)
                        scene->keypetcontrol();
                }

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
            else if (event.Type == sf::Event::MouseButtonPressed)
            {
                if (event.MouseButton.Button == sf::Mouse::Left)
                {
                    const sf::Input& input = app.GetInput();
                    if (input.GetMouseX() >= bobo->x && input.GetMouseX() <= bobo->x + 128 && input.GetMouseY() >= bobo->y && input.GetMouseY() <= bobo->y + 128)
                        if (!bobo->ishelp)
                            bobo->help();
                }
            }
        }

        if (sys->resetkeys)
        {
            xml->keyleft = sys->keyleft;
            xml->keyright = sys->keyright;
            xml->keyjump = sys->keyjump;
            xml->keyaction = sys->keyaction;
            xml->keypet = sys->keypet;
        }

        game->animate();
        bobo->animate();

        int distance = round(100.0f * app.GetFrameTime());
        if (distance < 300) // frametime init bugfix
            scene->animate(distance, &view);

        app.SetView(view);
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

        app.SetView(app.GetDefaultView());
        if (game->showdialog)
            game->dialogs[game->dialogindex]->draw(&app);

        app.Display();
    }

    xml->infolevel = game->level;
    xml->infoexp = game->exp;
    xml->infomoney = game->money;
    xml->infoherox = scene->hero->x;
    xml->infopetx = scene->pet->x;
    xml->infocurrentmagic = game->spellindex;
    xml->writeinfos();

    if (sys->keyleft + sys->keyright + sys->keyjump + sys->keyaction + sys->keypet != 0)
    {
        xml->keyleft = sys->keyleft;
        xml->keyright = sys->keyright;
        xml->keyjump = sys->keyjump;
        xml->keyaction = sys->keyaction;
        xml->keypet = sys->keypet;
    }
    xml->writekeys();

    delete bobo;
    delete scene;
    delete map;
    delete game;
    delete xml;

    cout << "Mission sucess !" << endl;
    return EXIT_SUCCESS;
}

void blit(sf::Image* srcimage, sf::Image* destimage)
{
    sf::Color pixel0, pixel1, pixel2;
    for(int x = 0; x < (int)srcimage->GetWidth(); x++)
    {
        for(int y = 0; y < (int)srcimage->GetHeight(); y++)
        {
            pixel0 = srcimage->GetPixel(x, y);
            pixel1 = destimage->GetPixel(x, y);
            pixel2.r = pixel1.r;
            pixel2.g = pixel1.g;
            pixel2.b = pixel1.b;
            pixel2.a = 255;

            if (pixel0.a < 255 && pixel1.a == 255)
                destimage->SetPixel(x, y, pixel1);
            else if (!pixel1.a)
                destimage->SetPixel(x, y, pixel0);
            else if (pixel0.a && pixel1.a)
                destimage->SetPixel(x, y, pixel2);
        }
    }
}

void white(sf::Image* srcimage, sf::Image* destimage)
{
    sf::Color pixel;
    for(int x = 0; x < (int)srcimage->GetWidth(); x++)
    {
        for(int y = 0; y < (int)srcimage->GetHeight(); y++)
        {
            pixel = srcimage->GetPixel(x, y);
            pixel.r = 255;
            pixel.g = 255;
            pixel.b = 255;

            destimage->SetPixel(x, y, pixel);
        }
    }
}

void prepareimages()
{
    imageslist[GFX_ARROW].LoadFromFile("gfx/arrow.png");
    imageslist[GFX_BAREXP].LoadFromFile("gfx/barexp.png");
    imageslist[GFX_BARLIFE].LoadFromFile("gfx/barlife.png");
    imageslist[GFX_BARMANA].LoadFromFile("gfx/barmana.png");
    imageslist[GFX_BOBO0].LoadFromFile("gfx/bobo0.png");
    imageslist[GFX_BOBO1].LoadFromFile("gfx/bobo1.png");
    imageslist[GFX_BOBO2].LoadFromFile("gfx/bobo2.png");
    imageslist[GFX_BOSS0].LoadFromFile("gfx/boss0.png");
    imageslist[GFX_COIN0].LoadFromFile("gfx/coin0.png");
    imageslist[GFX_COIN1].LoadFromFile("gfx/coin1.png");
    imageslist[GFX_COIN2].LoadFromFile("gfx/coin2.png");
    imageslist[GFX_COIN3].LoadFromFile("gfx/coin3.png");
    imageslist[GFX_DIALOGCORNER].LoadFromFile("gfx/dialog_corner.png");
    imageslist[GFX_DIALOGDOT].LoadFromFile("gfx/dialog_dot.png");
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
    imageslist[GFX_INTERFACESPELL0].LoadFromFile("gfx/interface_spell0.png");
    imageslist[GFX_INTERFACESPELL1].LoadFromFile("gfx/interface_spell1.png");
    imageslist[GFX_MINIHP].LoadFromFile("gfx/minihp.png");
    imageslist[GFX_MOB0].LoadFromFile("gfx/mob0.png");
    imageslist[GFX_POTIONL].LoadFromFile("gfx/potionl.png");
    imageslist[GFX_POTIONM].LoadFromFile("gfx/potionm.png");
    imageslist[GFX_SKY0].LoadFromFile("gfx/sky0.png");
    imageslist[GFX_STATE].LoadFromFile("gfx/state.png");
    imageslist[GFX_SPELL0].LoadFromFile("gfx/spell0.png");
    imageslist[GFX_SPELL1].LoadFromFile("gfx/spell1.png");
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
    imageslist[GFX_ZUZU0].LoadFromFile("gfx/zuzu0.png");
    imageslist[GFX_ZUZU1].LoadFromFile("gfx/zuzu1.png");
    imageslist[GFX_ZUZU2].LoadFromFile("gfx/zuzu2.png");
    imageslist[GFX_ZUZU3].LoadFromFile("gfx/zuzu3.png");

    for (int i = 0; i < GFX_ZUZU3; i++)
        imageslist[i].SetSmooth(false);
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
