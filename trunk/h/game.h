class Game
{
    public:
    Game(Gfx* ingfx, Xml* inxml, Bobo* inbobo, System* insys);
    ~Game();
    void animate();
    void draw(sf::RenderWindow* app);
    void levelup(Pet* pet);
    void setlife(float val);
    void setmana(float val);
    void setexp(int val, Pet* pet);
    void setmoney(int val);
    void generatemaxlife();
    void generatemaxmana();
    int getdamage(Mob* mob);
    int getdamage(Boss* boss);
    int getmoney(Loot* loot);
    float getpotionl(Loot* loot);
    float getpotionm(Loot* loot);
    void addgift(Loot* loot);
    int sapik();
    int sahil();
    void makespelldialogs();
    float life;
    float maxlife;
    float mana;
    float maxmana;
    int maxexp;
    vector<Dialog*> spelldialogs;
    vector<Dialog*> icondialogs;
    int dialogindex;
    bool showspelldialog;
    bool showicondialog;
    Inventory* inv;
    Stuff* stuff;

    private:
    sf::Vector2<int> mouseclick;
    std::ostringstream txt;
    sf::Sprite statesprite;
    sf::Sprite barlifesprite;
    sf::Sprite barmanasprite;
    sf::Sprite barexpsprite;
    sf::Sprite interfacesprite;
    vector<sf::Sprite> coinsprites;
    sf::String levelstring;
    sf::String lifestring;
    sf::String manastring;
    sf::String expstring;
    sf::String moneystring;
    vector<sf::Sprite> spellsprites;
    vector<sf::Sprite> iconsprites;
    sf::Sprite spellsprite;
    int lifebonus;
    int manabonus;
    int damagebonus;
    int healbonus;
    Gfx* gfx;
    Xml* xml;
    Bobo* bobo;
    System* sys;
    vector<string> stats;
    int coinindex;
    int currentframe;
};

Game::Game(Gfx* ingfx, Xml* inxml, Bobo* inbobo, System* insys)
{
    gfx = ingfx;
    xml = inxml;
    bobo = inbobo;
    sys = insys;

    if (xml->infolevel + xml->infoexp + xml->infomoney == 1) // very first time player plays
    {
        bobo->say("Hello World !\nMy name is Bobo.\nWelcome to\nMirel the witch !");
        bobo->say("This is a development\nversion...\nSo don't except much !");
        bobo->say("Have fun !");
    }
    else
    {
        bobo->say("Welcome back !");
    }

    mouseclick.x = 0;
    mouseclick.y = 0;

    statesprite.SetImage(gfx->interfaceimages[gfx->interface_state]);

    barlifesprite.SetImage(gfx->interfaceimages[gfx->interface_barlife]);

    barmanasprite.SetImage(gfx->interfaceimages[gfx->interface_barmana]);
    barmanasprite.SetY(42);

    barexpsprite.SetImage(gfx->interfaceimages[gfx->interface_barexp]);
    barexpsprite.SetY(84);

    interfacesprite.SetImage(gfx->interfaceimages[gfx->interface_ui]);
    interfacesprite.SetY(472);

    for (int i = gfx->loot_coin0; i < gfx->loot_coin3; i++)
    {
        coinsprites.push_back(sf::Sprite(gfx->lootimages[gfx->loot_coin0 + i]));
        coinsprites[i].SetY(130);
    }
    coinindex = gfx->loot_coin0;

    levelstring.SetFont(gfx->font);
    levelstring.SetSize(32);
    levelstring.SetColor(sf::Color::White);
    levelstring.SetX(16);
    levelstring.SetY(8);

    lifestring.SetFont(gfx->font);
    lifestring.SetSize(16);
    lifestring.SetColor(sf::Color::White);
    lifestring.SetX(140);
    lifestring.SetY(24);

    manastring.SetFont(gfx->font);
    manastring.SetSize(16);
    manastring.SetColor(sf::Color::White);
    manastring.SetX(140);
    manastring.SetY(66);

    expstring.SetFont(gfx->font);
    expstring.SetSize(16);
    expstring.SetColor(sf::Color::White);
    expstring.SetX(140);
    expstring.SetY(108);

    moneystring.SetFont(gfx->font);
    moneystring.SetSize(16);
    moneystring.SetColor(sf::Color::White);
    moneystring.SetX(40);
    moneystring.SetY(140);

    // inventory & stuff windows init

    inv = new Inventory(gfx, sys);
    stuff = new Stuff(gfx, xml);

    xml->readinventory(&inv->items);
    xml->readstuff();

    manabonus = xml->currentitems[0]->bonus;
    lifebonus = xml->currentitems[1]->bonus;
    if (xml->currentitems[2]->bonus < 0)
    {
        damagebonus = -xml->currentitems[2]->bonus;
        healbonus = 0;
    }
    else if (xml->currentitems[2]->bonus > 0)
    {
        healbonus = xml->currentitems[2]->bonus;
        damagebonus = 0;
    }
    else
    {
        damagebonus = 0;
        healbonus = 0;
    }

    generatemaxlife();
    life = maxlife;
    generatemaxmana();
    mana = maxmana;
    maxexp = 1000 * xml->infolevel;

    // spells bar

    for (int i = 0; i < gfx->interface_spell1 - gfx->interface_spell0 + 1; i++)
    {
        spellsprites.push_back(sf::Sprite(gfx->interfaceimages[gfx->interface_spell0 + i]));
        spellsprites[i].SetX(26 + (i * 76));
        spellsprites[i].SetY(472 + 59);
    }

    spellsprite.SetImage(gfx->interfaceimages[xml->infomagicindex + gfx->interface_spell0]);
    spellsprite.SetX(52);
    spellsprite.SetY(58);

    makespelldialogs();
    showspelldialog = false;

    // icons bar

    for (int i = 0; i < gfx->interface_icon1 - gfx->interface_icon0 + 1; i++)
    {
        iconsprites.push_back(sf::Sprite(gfx->interfaceimages[gfx->interface_icon0 + i]));
        iconsprites[i].SetX(482 + (i * 76));
        iconsprites[i].SetY(472 + 59);
    }

    stats.clear();
    stats.push_back("Show your stuff\nand stats.");
    icondialogs.push_back(new Dialog(gfx, "multi", "Stuff", stats, iconsprites[0].GetPosition().x, iconsprites[0].GetPosition().y));
    stats.clear();
    stats.push_back("Show your collected\nitems.");
    icondialogs.push_back(new Dialog(gfx, "multi", "Inventory", stats, iconsprites[0].GetPosition().x, iconsprites[0].GetPosition().y));

    showicondialog = false;
    dialogindex = 0;

    currentframe = 0;
}

Game::~Game()
{
    delete stuff;
    delete inv;
    coinsprites.clear();
    spellsprites.clear();
    iconsprites.clear();
    spelldialogs.clear();
    icondialogs.clear();
    stats.clear();
}

void Game::animate()
{
    if (currentframe < MAX_FRAME)
        currentframe++;
    else
    {
        currentframe = 0;
        coinindex++;
        if (coinindex > gfx->loot_coin3 - 1)
            coinindex = 0;

        if (inv->visible)
            inv->animate();
    }
}

void Game::draw(sf::RenderWindow* app)
{
    // draw interface infos

    barlifesprite.SetX(life * 256 / maxlife - 256 + 100);
    app->Draw(barlifesprite);

    barmanasprite.SetX(mana * 256 / maxmana - 256 + 100);
    app->Draw(barmanasprite);

    barexpsprite.SetX(xml->infoexp * 256 / maxexp - 256 + 100);
    app->Draw(barexpsprite);

    app->Draw(statesprite);

    app->Draw(spellsprite);

    txt << xml->infolevel;
    levelstring.SetText(txt.str());
    txt.str("");
    app->Draw(levelstring);

    txt << (int)life << " / " << (int)maxlife;
    lifestring.SetText(txt.str());
    txt.str("");
    app->Draw(lifestring);

    txt << (int)mana << " / " << (int)maxmana;
    manastring.SetText(txt.str());
    txt.str("");
    app->Draw(manastring);

    txt << xml->infoexp << " / " << (int)maxexp;
    expstring.SetText(txt.str());
    txt.str("");
    app->Draw(expstring);

    app->Draw(coinsprites[coinindex]);

    txt << xml->infomoney;
    moneystring.SetText(txt.str());
    txt.str("");
    app->Draw(moneystring);

    app->Draw(interfacesprite);

    // mouse interface handling

    const sf::Input& input = app->GetInput();

    if (input.IsMouseButtonDown(sf::Mouse::Left))
    {
        mouseclick.x = input.GetMouseX();
        mouseclick.y = input.GetMouseY();
    }

    for (int i = 0; i < (int)spellsprites.size(); i++)
    {
        if (input.GetMouseX() >= spellsprites[i].GetPosition().x\
            && input.GetMouseX() <= spellsprites[i].GetPosition().x + spellsprites[i].GetSize().x\
            && input.GetMouseY() >= spellsprites[i].GetPosition().y\
            && input.GetMouseY() <= spellsprites[i].GetPosition().y + spellsprites[i].GetSize().y)
            {
                dialogindex = i;
                showspelldialog = true;

                if (!sys->sceneether)
                {
                    if (!input.IsMouseButtonDown(sf::Mouse::Left)\
                        && mouseclick.x >= spellsprites[i].GetPosition().x\
                        && mouseclick.x <= spellsprites[i].GetPosition().x + spellsprites[i].GetSize().x\
                        && mouseclick.y >= spellsprites[i].GetPosition().y\
                        && mouseclick.y <= spellsprites[i].GetPosition().y + spellsprites[i].GetSize().y)
                    {
                        mouseclick.x = 0;
                        mouseclick.y = 0;

                        xml->infomagicindex = i;
                        spellsprite.SetImage(gfx->interfaceimages[xml->infomagicindex + gfx->interface_spell0]);
                    }
                }
            }
        else
            if (i == 0)
                showspelldialog = false;

        app->Draw(spellsprites[i]);
    }

    for (int i = 0; i < (int)iconsprites.size(); i++)
    {
        if (input.GetMouseX() >= iconsprites[i].GetPosition().x\
            && input.GetMouseX() <= iconsprites[i].GetPosition().x + iconsprites[i].GetSize().x\
            && input.GetMouseY() >= iconsprites[i].GetPosition().y\
            && input.GetMouseY() <= iconsprites[i].GetPosition().y + iconsprites[i].GetSize().y)
            {
                dialogindex = i;
                showicondialog = true;

                if (!sys->sceneether)
                {
                    if (!input.IsMouseButtonDown(sf::Mouse::Left)\
                        && mouseclick.x >= iconsprites[i].GetPosition().x\
                        && mouseclick.x <= iconsprites[i].GetPosition().x + iconsprites[i].GetSize().x\
                        && mouseclick.y >= iconsprites[i].GetPosition().y\
                        && mouseclick.y <= iconsprites[i].GetPosition().y + iconsprites[i].GetSize().y)
                    {
                        mouseclick.x = 0;
                        mouseclick.y = 0;

                        if (i == gfx->interface_icon0 - gfx->interface_icon0) // stuff
                        {
                            if (stuff->visible)
                                stuff->visible = false;
                            else
                            {
                                xml->readstuff();
                                stuff->visible = true;
                            }
                            inv->visible = false;
                            inv->showoptiondialog = false;
                        }
                        else if (i == gfx->interface_icon1 - gfx->interface_icon0) // inventory
                        {
                            if (inv->visible)
                                inv->visible = false;
                            else
                                inv->visible = true;
                            stuff->visible = false;
                            inv->showoptiondialog = false;
                        }
                    }
                }
            }
        else
            if (i == 0)
                showicondialog = false;

        app->Draw(iconsprites[i]);
    }

    // system commands

    if (sys->useitem)
    {
        if (inv->items[sys->itemindex]->level > xml->infolevel)
        {
            if (!bobo->firsttimelevelitem)
            {
                bobo->firsttimelevelitem = true;
                txt << "You must be level " << inv->items[sys->itemindex]->level << "\nto use this item !";
                bobo->say(txt.str());
                txt.str("");
                inv->showoptiondialog = false;
            }
        }
        else
        {
            if (inv->items[sys->itemindex]->type == "gift")
            {
                int typerand = sf::Randomizer::Random(0, 2);
                Item* tempitem;
                if (inv->items[sys->itemindex]->level == 1) // level 1 surprise
                {
                    if (typerand == 0)
                    {
                        inv->additem(new Item(gfx, "hat", 1));
                        tempitem = xml->readitem("hat", 1);
                    }
                    else if (typerand == 1)
                    {
                        inv->additem(new Item(gfx, "dress", 1));
                        tempitem = xml->readitem("dress", 1);
                    }
                    else if (typerand == 2)
                    {
                        inv->additem(new Item(gfx, "wand", 1));
                        tempitem = xml->readitem("wand", 1);
                    }
                    inv->items.back()->name = tempitem->name;
                    inv->items.back()->setstats(sf::Randomizer::Random(0, 5), tempitem->bonus);
                    inv->items.back()->setprice(tempitem->level);
                }
                else if (inv->items[sys->itemindex]->level == 5) // level 5
                {
                    if (typerand == 0)
                    {
                        inv->additem(new Item(gfx, "hat", 1));
                        tempitem = xml->readitem("hat", 1);
                    }
                    else if (typerand == 1)
                    {
                        inv->additem(new Item(gfx, "dress", 1));
                        tempitem = xml->readitem("dress", 1);
                    }
                    else if (typerand == 2)
                    {
                        inv->additem(new Item(gfx, "wand", 1));
                        tempitem = xml->readitem("wand", 1);
                    }
                    inv->items.back()->name = tempitem->name;
                    inv->items.back()->setstats(sf::Randomizer::Random(5, 10), tempitem->bonus);
                    inv->items.back()->setprice(tempitem->level);
                }
                else if (inv->items[sys->itemindex]->level == 10) // level 10
                {
                    if (typerand == 0)
                    {
                        inv->additem(new Item(gfx, "hat", 2));
                        tempitem = xml->readitem("hat", 2);
                    }
                    else if (typerand == 1)
                    {
                        inv->additem(new Item(gfx, "dress", 2));
                        tempitem = xml->readitem("dress", 2);
                    }
                    else if (typerand == 2)
                    {
                        inv->additem(new Item(gfx, "wand", 2));
                        tempitem = xml->readitem("wand", 2);
                    }
                    inv->items.back()->name = tempitem->name;
                    inv->items.back()->setstats(sf::Randomizer::Random(0, 5), tempitem->bonus);
                    inv->items.back()->setprice(tempitem->level);
                }
                else if (inv->items[sys->itemindex]->level == 15) // level 15
                {
                    if (typerand == 0)
                    {
                        inv->additem(new Item(gfx, "hat", 2));
                        tempitem = xml->readitem("hat", 2);
                    }
                    else if (typerand == 1)
                    {
                        inv->additem(new Item(gfx, "dress", 2));
                        tempitem = xml->readitem("dress", 2);
                    }
                    else if (typerand == 2)
                    {
                        inv->additem(new Item(gfx, "wand", 2));
                        tempitem = xml->readitem("wand", 2);
                    }
                    inv->items.back()->name = tempitem->name;
                    inv->items.back()->setstats(sf::Randomizer::Random(5, 10), tempitem->bonus);
                    inv->items.back()->setprice(tempitem->level);
                }
                delete tempitem;
            }
            else if (inv->items[sys->itemindex]->type == "hat")
            {
                inv->additem(new Item(gfx, "hat", xml->infohatindex));
                inv->items.back()->name = xml->currentitems[0]->name;
                inv->items.back()->setstats(xml->infohatincrementation, xml->currentitems[0]->bonus);
                inv->items.back()->setprice(xml->currentitems[0]->level);

                xml->infohatindex = inv->items[sys->itemindex]->index;
                xml->infohatincrementation = inv->items[sys->itemindex]->incrementation;
                manabonus = inv->items[sys->itemindex]->bonus;
                generatemaxmana();
                if (mana > maxmana)
                    mana = maxmana;
                xml->readstuff();
            }
            else if (inv->items[sys->itemindex]->type == "dress")
            {
                inv->additem(new Item(gfx, "dress", xml->infodressindex));
                inv->items.back()->name = xml->currentitems[1]->name;
                inv->items.back()->setstats(xml->infodressincrementation, xml->currentitems[1]->bonus);
                inv->items.back()->setprice(xml->currentitems[1]->level);

                xml->infodressindex = inv->items[sys->itemindex]->index;
                xml->infodressincrementation = inv->items[sys->itemindex]->incrementation;
                lifebonus = inv->items[sys->itemindex]->bonus;
                generatemaxlife();
                if (life > maxlife)
                    life = maxlife;
                xml->readstuff();
            }
            else if (inv->items[sys->itemindex]->type == "wand")
            {
                inv->additem(new Item(gfx, "wand", xml->infowandindex));
                inv->items.back()->name = xml->currentitems[2]->name;
                inv->items.back()->setstats(xml->infowandincrementation, xml->currentitems[2]->bonus);
                inv->items.back()->setprice(xml->currentitems[2]->level);

                xml->infowandindex = inv->items[sys->itemindex]->index;
                xml->infowandincrementation = inv->items[sys->itemindex]->incrementation;
                if (inv->items[sys->itemindex]->bonus > 0)
                {
                    healbonus = inv->items[sys->itemindex]->bonus;
                    damagebonus = 0;
                }
                else if (inv->items[sys->itemindex]->bonus < 0)
                {
                    damagebonus = -inv->items[sys->itemindex]->bonus;
                    healbonus = 0;
                }
                makespelldialogs(); // update spells infos
                xml->readstuff();
            }
            inv->deleteitem(sys->itemindex);
            sys->useitem = false;
            inv->showoptiondialog = false;
        }
    }
    if (sys->deleteitem)
    {
        inv->deleteitem(sys->itemindex);
        sys->deleteitem = false;
        inv->showoptiondialog = false;
    }
    if (sys->closedialogitem)
    {
        sys->closedialogitem = false;
        inv->showoptiondialog = false;
    }

    // stuff & inventory windows drawing

    if (inv->visible)
        inv->draw(app);
    else if (stuff->visible)
        stuff->draw(app);
}

void Game::levelup(Pet* pet)
{
    xml->infolevel++;
    xml->infoexp -= maxexp;
    maxexp = xml->infolevel * 1000;
    generatemaxlife();
    life = maxlife;
    generatemaxmana();
    mana = maxmana;

    pet->hp = xml->infolevel * 2000;
    pet->maxhp = pet->hp;

    makespelldialogs();

    txt << "Congratulations !\nYou have reached\nlevel " << xml->infolevel << ".";
    bobo->say(txt.str());
    txt.str("");
}

void Game::setexp(int val, Pet* pet)
{
    xml->infoexp += val * 50 / xml->infolevel; // val = mob level
    if (xml->infoexp >= maxexp)
        levelup(pet);
}

void Game::setmoney(int val)
{
    xml->infomoney += val; // val = money
    if (xml->infomoney < 0)
        xml->infomoney = 0;
}

void Game::setlife(float val)
{
    life += val; // val = life

    if (!bobo->firsttimelife && (int)life <= 0)
    {
        bobo->firsttimelife = true;
        bobo->say("You ran out of life !\nBe careful !");
    }

    if (life > maxlife)
        life = maxlife;
    else if ((int)life < 0)
    {
        sys->ether_v = true;
        life = maxlife;
        mana = maxmana;
    }
}

void Game::setmana(float val)
{
    mana += val; // val = mana

    if (!bobo->firsttimemana && (int)mana <= 0)
    {
        bobo->firsttimemana = true;
        bobo->say("You ran out of mana !");
    }

    if (mana > maxmana)
        mana = maxmana;
    else if ((int)mana < 0)
        mana = 0;
}

void Game::generatemaxlife()
{
    maxlife = xml->infolevel * 1000 + lifebonus;
}

void Game::generatemaxmana()
{
    maxmana = xml->infolevel * 100 + manabonus;
}

int Game::getdamage(Mob* mob)
{
    if (!sf::Randomizer::Random(0, 16))
        return 0;
    else
        return mob->level * sf::Randomizer::Random(50, 100);
}

int Game::getdamage(Boss* boss)
{
    if (!sf::Randomizer::Random(0, 16))
        return 0;
    else
        return boss->level * sf::Randomizer::Random(50, 100);
}

int Game::getmoney(Loot* loot)
{
    return loot->level *= sf::Randomizer::Random(4, 8);
}

float Game::getpotionl(Loot* loot)
{
    return loot->level *= sf::Randomizer::Random(250, 500);
}

float Game::getpotionm(Loot* loot)
{
    return loot->level *= sf::Randomizer::Random(25, 50);
}

void Game::addgift(Loot* loot)
{
    inv->additem(new Item(gfx, "gift", 1));
    inv->items.back()->setstats(0, 0);
    inv->items.back()->setprice(loot->level);
}

int Game::sapik()
{
    if (!sf::Randomizer::Random(0, 16))
        return 0;
    else
        return xml->infolevel * sf::Randomizer::Random(32, 64) + damagebonus;
}

int Game::sahil()
{
    if (!sf::Randomizer::Random(0, 16))
        return 0;
    else
        return xml->infolevel * sf::Randomizer::Random(250, 500) + healbonus;
}

void Game::makespelldialogs()
{
    spelldialogs.clear();

    stats.clear();
    stats.push_back("Type > Damage");
    txt << "Value > " << xml->infolevel * 32 + damagebonus << " - " << xml->infolevel * 64 + damagebonus;
    stats.push_back(txt.str());
    txt.str("");
    txt << "Mana Cost > " << xml->infolevel * 5;
    stats.push_back(txt.str());
    txt.str("");
    stats.push_back("Tips > Use it to kill\nyour enemies.");
    spelldialogs.push_back(new Dialog(gfx, "multi", "SaPik", stats, spellsprites[0].GetPosition().x, spellsprites[0].GetPosition().y));

    stats.clear();
    stats.push_back("Type > Heal");
    txt << "Value > " << xml->infolevel * 250 + healbonus << " - " << xml->infolevel * 500 + healbonus;
    stats.push_back(txt.str());
    txt.str("");
    txt << "Mana Cost > " << xml->infolevel * 3;
    stats.push_back(txt.str());
    txt.str("");
    stats.push_back("Tips > Use it to heal\nyour pet.");
    spelldialogs.push_back(new Dialog(gfx, "multi", "SaHil", stats, spellsprites[1].GetPosition().x, spellsprites[1].GetPosition().y));
}
