using namespace std;

#include <iostream>
#include <fstream>
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

#define LOOT_MONEY 0
#define LOOT_POTIONM 1
#define LOOT_POTIONL 2

#define SYS_CONFIGKEYS 0
#define SYS_RESETKEYS 1
#define SYS_JOKE 2
#define SYS_CLOSEDIALOG 3

#include "gfx.h"
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

    Gfx* gfx = new Gfx();

    Xml* xml = new Xml(gfx);
    xml->readinfos();
    xml->readkeys();

    // fps stuff

    char fpstxt[100];
    sf::String fpsstring;

    fpsstring.SetFont(gfx->font);
    fpsstring.SetSize(16);
    fpsstring.SetColor(sf::Color::White);
    fpsstring.SetX(800 - 128);
    fpsstring.SetY(2);

    // our objects

    System* sys = new System();
    Bobo* bobo = new Bobo(gfx, sys);
    Game* game = new Game(gfx, xml->infolevel, xml->infoexp, xml->infomoney, xml->infocurrentmagic, bobo);
    Map* map = new Map(gfx);
    Scene* scene = new Scene(gfx, xml->infoherox, xml->infopetx, xml->infolevel, game, map);

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

        // map draw

        app.SetView(view);
        map->draw(&app, scene->hero);
        map->draw(&app, view, scene->pet);
        map->draw(&app, scene->mobs);
        map->draw(&app, scene->boss);
        map->draw(&app, scene->loots);

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

    delete gfx;
    delete bobo;
    delete scene;
    delete map;
    delete game;
    delete xml;

    cout << "Mission sucess !" << endl;
    return EXIT_SUCCESS;
}
