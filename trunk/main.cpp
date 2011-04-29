using namespace std;

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>

#include "tinyxml/tinyxml.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#define MAX_FRAME 3

#define DIRECTION_LEFT 0
#define DIRECTION_RIGHT 1

#define SYS_CONFIGKEYS 0
#define SYS_RESETKEYS 1
#define SYS_JOKE 2
#define SYS_CLOSEDIALOG 3

#define SYS_USEITEM 0
#define SYS_DELETEITEM 1
#define SYS_CLOSEDIALOGITEM 2

#include "h/gfx.h"
#include "h/snd.h"
#include "h/particle.h"
#include "h/teleport.h"
#include "h/item.h"
#include "h/xml.h"
#include "h/system.h"
#include "h/dialog.h"
#include "h/bobo.h"
#include "h/inventory.h"
#include "h/stuff.h"
#include "h/magic.h"
#include "h/hero.h"
#include "h/boss.h"
#include "h/mob.h"
#include "h/loot.h"
#include "h/pet.h"
#include "h/game.h"
#include "h/world.h"
#include "h/score.h"
#include "h/emote.h"
#include "h/label.h"
#include "h/map.h"
#include "h/scene.h"
#include "h/ether.h"

int main()
{
    // app creation

    sf::Image appicon;
    appicon.LoadFromFile("data/icon.png");

    sf::RenderWindow app(sf::VideoMode(800, 600, 32), "Mirel the Witch", sf::Style::None | sf::Style::Close);
    app.SetFramerateLimit(60);

    app.SetIcon(appicon.GetWidth(), appicon.GetHeight(), appicon.GetPixelsPtr());

    sf::View view(sf::FloatRect(0, 0, 800, 600));
    app.SetView(view);

    // program main loop init

    Gfx* gfx = new Gfx();
    Snd* snd = new Snd();

    Xml* xml = new Xml(gfx);
    xml->readinfos();
    xml->readkeys();
    xml->readworlds(xml->infoworldindex);

    // our objects

    System* sys = new System();
    Bobo* bobo = new Bobo(gfx, sys);
    Game* game = new Game(gfx, xml, bobo, sys);
    Map* map = new Map(gfx, xml);
    Scene* scene = new Scene(gfx, snd, bobo, xml, game, map, sys);
    Ether* ether;

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
                    if (!sys->sceneether)
                        scene->keypress(event.Key.Code); // key pressed
                    else
                        ether->keypress(event.Key.Code);
                }

                if (event.Key.Code == sf::Key::Escape)
                    app.Close();
            }
            else if (event.Type == sf::Event::KeyReleased)
            {
                if (!sys->sceneether)
                    scene->keyrelease(event.Key.Code); // key released
                else
                    ether->keyrelease(event.Key.Code);
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

        if (sys->ether_v)
        {
            sys->sceneether = true;
            sys->ether_v = false;
            delete scene;
            delete map;
            ether = new Ether(gfx, snd, xml, game, "v");
        }
        else if (sys->ether_hl)
        {
            sys->sceneether = true;
            sys->ether_hl = false;
            delete scene;
            delete map;
            ether = new Ether(gfx, snd, xml, game, "hl");
        }
        else if (sys->ether_hr)
        {
            sys->sceneether = true;
            sys->ether_hr = false;
            delete scene;
            delete map;
            ether = new Ether(gfx, snd, xml, game, "hr");
        }
        if (sys->sceneether)
        {
            if (ether->type == "v")
            {
                if (ether->time >= 333)
                {
                    sys->sceneether = false;
                    delete ether;
                    map = new Map(gfx, xml);
                    scene = new Scene(gfx, snd, bobo, xml, game, map, sys);
                }
            }
            else
            {
                if (ether->time >= 33)
                {
                    sys->sceneether = false;
                    delete ether;
                    map = new Map(gfx, xml);
                    scene = new Scene(gfx, snd, bobo, xml, game, map, sys);
                }
            }
        }

        game->animate();
        bobo->animate();

        int distance = round(100.0f * app.GetFrameTime());
        if (distance > 1 && distance < 10) // frametime bugfix
        {
            if (sys->sceneether)
                ether->animate(distance);
            else
                scene->animate(distance, &view);
        }

        if (sys->sceneether)
        {
            app.SetView(app.GetDefaultView());
            ether->draw(&app);
        }
        else
        {
            app.SetView(view);
            scene->draw(&app, &view);
        }

        app.SetView(app.GetDefaultView());
        game->draw(&app);
        bobo->draw(&app);

        // map draw

        if (!sys->sceneether)
        {
            app.SetView(view);
            map->draw(&app, scene->hero);
            map->draw(&app, view, scene->pets.front());
            map->draw(&app, scene->mobs);
            map->draw(&app, scene->boss);
            map->draw(&app, scene->loots);
        }

        // interface dialogs draw

        app.SetView(app.GetDefaultView());
        if (game->showspelldialog)
            game->spelldialogs[game->dialogindex]->draw(&app);
        else if (game->showicondialog)
            game->icondialogs[game->dialogindex]->draw(&app);

        if (game->inv->showinfodialog)
            game->inv->infodialogs[game->inv->dialogindex]->draw(&app);
        else if (game->inv->showoptiondialog)
            game->inv->optiondialogs[0]->draw(&app);

        app.Display();
    }

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

    xml->writeinventory(game->inv->items);

    delete gfx;
    delete snd;
    delete bobo;
    if (!sys->sceneether)
    {
        delete scene;
        delete map;
    }
    else
        delete ether;
    delete game;
    delete xml;

    cout << "Mission sucess !" << endl;
    return EXIT_SUCCESS;
}
