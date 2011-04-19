class Game
{
    public:
    Game(Gfx* ingfx, Xml* inxml, Bobo* inbobo);
    ~Game();
    void animate();
    void draw(sf::RenderWindow* app);
    void levelup(Pet* pet);
    void setlife(float val);
    void setmana(float val);
    void setexp(int val, Pet* pet);
    void setmoney(int val);
    int getdamage(Mob* mob);
    int getdamage(Boss* boss);
    int getmoney(Loot* loot);
    float getpotionl(Loot* loot);
    float getpotionm(Loot* loot);
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
    Gfx* gfx;
    Xml* xml;
    Bobo* bobo;
    Inventory* inv;
    Stuff* stuff;
    vector<string> stats;
    int coinindex;
    int currentframe;
};

Game::Game(Gfx* ingfx, Xml* inxml, Bobo* inbobo)
{
    gfx = ingfx;
    xml = inxml;
    bobo = inbobo;

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

    maxlife = 1000 * xml->infolevel;
    life = maxlife;
    maxmana = 100 * xml->infolevel;
    mana = maxmana;
    maxexp = 1000 * xml->infolevel;
    coinindex = gfx->loot_coin0;

    // windows

    inv = new Inventory(gfx);
    stuff = new Stuff(gfx, xml);

    // spells

    for (int i = 0; i < gfx->interface_spell1 - gfx->interface_spell0 + 1; i++)
    {
        spellsprites.push_back(sf::Sprite(gfx->interfaceimages[gfx->interface_spell0 + i]));
        spellsprites[i].SetX(26 + (i * 76));
        spellsprites[i].SetY(472 + 59);
    }

    spellsprite.SetImage(gfx->interfaceimages[xml->infocurrentmagic + gfx->interface_spell0]);
    spellsprite.SetX(52);
    spellsprite.SetY(58);

    makespelldialogs();
    showspelldialog = false;

    // icons

    for (int i = 0; i < gfx->interface_icon1 - gfx->interface_icon0 + 1; i++)
    {
        iconsprites.push_back(sf::Sprite(gfx->interfaceimages[gfx->interface_icon0 + i]));
        iconsprites[i].SetX(482 + (i * 76));
        iconsprites[i].SetY(472 + 59);
    }

    stats.clear();
    stats.push_back("Show your stuff\nand stats.");
    icondialogs.push_back(new Dialog(gfx, "Stuff", stats, iconsprites[0].GetPosition().x, iconsprites[0].GetPosition().y));
    stats.clear();
    stats.push_back("Show your collected\nitems.");
    icondialogs.push_back(new Dialog(gfx, "Inventory", stats, iconsprites[0].GetPosition().x, iconsprites[0].GetPosition().y));

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

                if (!input.IsMouseButtonDown(sf::Mouse::Left)\
                    && mouseclick.x >= spellsprites[i].GetPosition().x\
                    && mouseclick.x <= spellsprites[i].GetPosition().x + spellsprites[i].GetSize().x\
                    && mouseclick.y >= spellsprites[i].GetPosition().y\
                    && mouseclick.y <= spellsprites[i].GetPosition().y + spellsprites[i].GetSize().y)
                {
                    mouseclick.x = 0;
                    mouseclick.y = 0;

                    xml->infocurrentmagic = i;
                    spellsprite.SetImage(gfx->interfaceimages[xml->infocurrentmagic + gfx->interface_spell0]);
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
                            xml->readitems();
                            stuff->visible = true;
                        }
                        inv->visible = false;
                    }
                    else if (i == gfx->interface_icon1 - gfx->interface_icon0) // inventory
                    {
                        if (inv->visible)
                            inv->visible = false;
                        else
                            inv->visible = true;
                        stuff->visible = false;
                    }
                }
            }
        else
            if (i == 0)
                showicondialog = false;

        app->Draw(iconsprites[i]);
    }

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
    maxlife = xml->infolevel * 1000;
    life = maxlife;
    maxmana = xml->infolevel * 100;
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
    else if (life <= 0)
        life = 0;
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
    else if (mana <= 0)
        mana = 0;
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

int Game::sapik()
{
    if (!sf::Randomizer::Random(0, 16))
        return 0;
    else
        return xml->infolevel * sf::Randomizer::Random(32, 64);
}

int Game::sahil()
{
    if (!sf::Randomizer::Random(0, 16))
        return 0;
    else
        return xml->infolevel * sf::Randomizer::Random(250, 500);
}

void Game::makespelldialogs()
{
    spelldialogs.clear();

    stats.clear();
    stats.push_back("Type > Damage");
    txt << "Value > " << xml->infolevel * 32 << " - " << xml->infolevel * 64;
    stats.push_back(txt.str());
    txt.str("");
    txt << "Mana Cost > " << xml->infolevel * 5;
    stats.push_back(txt.str());
    txt.str("");
    stats.push_back("Tips > Use it to kill\nyour enemies.");
    spelldialogs.push_back(new Dialog(gfx, "SaPik", stats, spellsprites[0].GetPosition().x, spellsprites[0].GetPosition().y));

    stats.clear();
    stats.push_back("Type > Heal");
    txt << "Value > " << xml->infolevel * 250 << " - " << xml->infolevel * 500;
    stats.push_back(txt.str());
    txt.str("");
    txt << "Mana Cost > " << xml->infolevel * 3;
    stats.push_back(txt.str());
    txt.str("");
    stats.push_back("Tips > Use it to heal\nyour pet.");
    spelldialogs.push_back(new Dialog(gfx, "SaHil", stats, spellsprites[1].GetPosition().x, spellsprites[1].GetPosition().y));
}
