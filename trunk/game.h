class Game
{
    public:
    Game(Gfx* ingfx, int inlevel, int inexp, int inmoney, int inmagic, Bobo* inbobo);
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
    void makedialogs();
    int level;
    float life;
    float maxlife;
    float mana;
    float maxmana;
    int exp;
    int maxexp;
    int money;
    vector<Dialog*> dialogs;
    int dialogindex;
    bool showdialog;
    int spellindex;

    private:
    Gfx* gfx;
    char txt[100];
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
    sf::Sprite spellsprite;
    Bobo* bobo;
    vector<string> stats;
    int coinindex;
    int currentframe;
};

Game::Game(Gfx* ingfx, int inlevel, int inexp, int inmoney, int inmagic, Bobo* inbobo)
{
    gfx = ingfx;

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

    level = inlevel;
    maxlife = 1000 * level;
    life = maxlife;
    maxmana = 100 * level;
    mana = maxmana;
    exp = inexp;
    maxexp = 1000 * level;
    money = inmoney;
    coinindex = gfx->loot_coin0;

    // spells

    for (int i = 0; i < 2; i++)
    {
        spellsprites.push_back(sf::Sprite(gfx->interfaceimages[i + gfx->interface_spell0]));
        spellsprites[i].SetX(26 + (i * 76));
        spellsprites[i].SetY(472 + 59);
    }
    spellindex = inmagic;

    spellsprite.SetImage(gfx->interfaceimages[spellindex + gfx->interface_spell0]);
    spellsprite.SetX(52);
    spellsprite.SetY(58);

    makedialogs();
    showdialog = false;
    dialogindex = 0;

    // bobo

    bobo = inbobo;

    if (level + exp + money == 1) // very first time player plays
    {
        bobo->say("Hello World !\nMy name is Bobo.\nWelcome to\nMirel the witch !");
        bobo->say("This is a development\nversion...\nSo don't except much !");
        bobo->say("Have fun !");
    }
    else
    {
        bobo->say("Welcome back !");
    }

    currentframe = 0;
}

Game::~Game()
{
    coinsprites.clear();
    spellsprites.clear();
    dialogs.clear();
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
    }
}

void Game::draw(sf::RenderWindow* app)
{
    barlifesprite.SetX(life * 256 / maxlife - 256 + 100);
    app->Draw(barlifesprite);

    barmanasprite.SetX(mana * 256 / maxmana - 256 + 100);
    app->Draw(barmanasprite);

    barexpsprite.SetX(exp * 256 / maxexp - 256 + 100);
    app->Draw(barexpsprite);

    app->Draw(statesprite);

    app->Draw(spellsprite);

    snprintf(txt, 99, "%d", (int)level);
    levelstring.SetText(txt);
    app->Draw(levelstring);

    snprintf(txt, 99, "%d / %d", (int)life, (int)maxlife);
    lifestring.SetText(txt);
    app->Draw(lifestring);

    snprintf(txt, 99, "%d / %d", (int)mana, (int)maxmana);
    manastring.SetText(txt);
    app->Draw(manastring);

    snprintf(txt, 99, "%d / %d", (int)exp, (int)maxexp);
    expstring.SetText(txt);
    app->Draw(expstring);

    app->Draw(coinsprites[coinindex]);

    snprintf(txt, 99, "%d", money);
    moneystring.SetText(txt);
    app->Draw(moneystring);

    app->Draw(interfacesprite);

    for (int i = 0; i < (int)spellsprites.size(); i++)
    {
        const sf::Input& input = app->GetInput();
        if (input.GetMouseX() >= spellsprites[i].GetPosition().x\
            && input.GetMouseX() <= spellsprites[i].GetPosition().x + spellsprites[i].GetSize().x\
            && input.GetMouseY() >= spellsprites[i].GetPosition().y\
            && input.GetMouseY() <= spellsprites[i].GetPosition().y + spellsprites[i].GetSize().y)
            {
                dialogindex = i;
                showdialog = true;

                if (input.IsMouseButtonDown(sf::Mouse::Left))
                {
                    spellindex = i;
                    spellsprite.SetImage(gfx->interfaceimages[spellindex + gfx->interface_spell0]);
                }
            }
        else
            if (i == 0)
                showdialog = false;

        app->Draw(spellsprites[i]);
    }
}

void Game::levelup(Pet* pet)
{
    level++;
    exp -= maxexp;
    maxexp = level * 1000;
    maxlife = level * 1000;
    life = maxlife;
    maxmana = level * 100;
    mana = maxmana;

    pet->hp = level * 2000;
    pet->maxhp = pet->hp;

    makedialogs();

    snprintf(txt, 99, "Congratulations !\nYou have reached\nlevel %i.", level);
    bobo->say(txt);
}

void Game::setexp(int val, Pet* pet)
{
    exp += val * 50 / level; // val = mob level
    if (exp >= maxexp)
        levelup(pet);
}

void Game::setmoney(int val)
{
    money += val; // val = money
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
        return level * sf::Randomizer::Random(32, 64);
}

int Game::sahil()
{
    if (!sf::Randomizer::Random(0, 16))
        return 0;
    else
        return level * sf::Randomizer::Random(250, 500);
}

void Game::makedialogs()
{
    dialogs.clear();

    stats.clear();
    stats.push_back("Type > Damage");
    snprintf(txt, 99, "Value > %i - %i", level * 32, level * 64);
    stats.push_back(txt);
    snprintf(txt, 99, "Mana Cost > %i", level * 5);
    stats.push_back(txt);
    stats.push_back("Tips > Use it to kill\nyour enemies.");
    dialogs.push_back(new Dialog(gfx, "SaPik", stats, spellsprites[0].GetPosition().x, spellsprites[0].GetPosition().y));

    stats.clear();
    stats.push_back("Type > Heal");
    snprintf(txt, 99, "Value > %i - %i", level * 250, level * 500);
    stats.push_back(txt);
    snprintf(txt, 99, "Mana Cost > %i", level * 3);
    stats.push_back(txt);
    stats.push_back("Tips > Use it to heal\nyour pet.");
    dialogs.push_back(new Dialog(gfx, "SaHil", stats, spellsprites[1].GetPosition().x, spellsprites[1].GetPosition().y));
}
