class Game
{
    public:
    Game(int inlevel, int inexp, int inmoney, Bobo* inbobo);
    void animate();
    void draw(sf::RenderWindow* app);
    void levelup();
    void setlife(float val);
    void setmana(float val);
    void setexp(int val);
    void setmoney(int val);
    int getindamage(Mob* mob);
    int getoutdamage();
    int getmoney(Loot* loot);
    float getpotionl(Loot* loot);
    float getpotionm(Loot* loot);
    int level;
    float life;
    float maxlife;
    float mana;
    float maxmana;
    int exp;
    int maxexp;
    int money;

    private:
    char txt[100];
    sf::Sprite statesprite;
    sf::Sprite barlifesprite;
    sf::Sprite barmanasprite;
    sf::Sprite barexpsprite;
    sf::Sprite interfacesprite;
    sf::Sprite coinsprite;
    sf::String levelstring;
    sf::String lifestring;
    sf::String manastring;
    sf::String expstring;
    sf::String moneystring;
    sf::Image stateimage;
    sf::Image barlifeimage;
    sf::Image barmanaimage;
    sf::Image barexpimage;
    sf::Image interfaceimage;
    sf::Image coinimage;
    sf::Font font;
    sf::Rect<int> coinrect;
    Bobo* bobo;
    int coinindex;
    int currentframe;
};

Game::Game(int inlevel, int inexp, int inmoney, Bobo* inbobo)
{
    stateimage = getimage(GFX_STATE);
    stateimage.SetSmooth(false);
    statesprite.SetImage(stateimage);

    barlifeimage = getimage(GFX_BARLIFE);
    barlifeimage.SetSmooth(false);
    barlifesprite.SetImage(barlifeimage);

    barmanaimage = getimage(GFX_BARMANA);
    barmanaimage.SetSmooth(false);
    barmanasprite.SetImage(barmanaimage);
    barmanasprite.SetY(42);

    barexpimage = getimage(GFX_BAREXP);
    barexpimage.SetSmooth(false);
    barexpsprite.SetImage(barexpimage);
    barexpsprite.SetY(84);

    interfaceimage = getimage(GFX_INTERFACE);
    interfaceimage.SetSmooth(false);
    interfacesprite.SetImage(interfaceimage);
    interfacesprite.SetY(472);

    coinrect.Left = 0;
    coinrect.Right = 32;
    coinrect.Bottom = 0;
    coinrect.Top = 32;

    coinimage = getimage(GFX_COIN);
    coinimage.SetSmooth(false);
    coinsprite.SetImage(coinimage);
    coinsprite.SetY(160);

    font = getfont();

    levelstring.SetFont(font);
    levelstring.SetSize(32);
    levelstring.SetColor(sf::Color::White);
    levelstring.SetX(42);
    levelstring.SetY(44);

    lifestring.SetFont(font);
    lifestring.SetSize(16);
    lifestring.SetColor(sf::Color::White);
    lifestring.SetX(124);
    lifestring.SetY(24);

    manastring.SetFont(font);
    manastring.SetSize(16);
    manastring.SetColor(sf::Color::White);
    manastring.SetX(124);
    manastring.SetY(66);

    expstring.SetFont(font);
    expstring.SetSize(16);
    expstring.SetColor(sf::Color::White);
    expstring.SetX(124);
    expstring.SetY(108);

    moneystring.SetFont(font);
    moneystring.SetSize(16);
    moneystring.SetColor(sf::Color::White);
    moneystring.SetX(40);
    moneystring.SetY(136);

    level = inlevel;
    maxlife = 1000 * level;
    life = maxlife;
    maxmana = 100 * level;
    mana = maxmana;
    exp = inexp;
    maxexp = 1000 * level;
    money = inmoney;
    coinindex = 0;

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

void Game::animate()
{
    if (currentframe < MAX_FRAME)
        currentframe++;
    else
    {
        currentframe = 0;
        coinindex++;
        if (coinindex > 3)
            coinindex = 0;
    }
}

void Game::draw(sf::RenderWindow* app)
{
    barlifesprite.SetX(life * 256 / maxlife - 256 + 55);
    app->Draw(barlifesprite);

    barmanasprite.SetX(mana * 256 / maxmana - 256 + 55);
    app->Draw(barmanasprite);

    barexpsprite.SetX(exp * 256 / maxexp - 256 + 55);
    app->Draw(barexpsprite);

    app->Draw(statesprite);

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

    coinrect.Left = coinindex * 32;
    coinrect.Right = coinrect.Left + 32;
    coinsprite.SetSubRect(coinrect);
    app->Draw(coinsprite);

    snprintf(txt, 99, "%d", money);
    moneystring.SetText(txt);
    app->Draw(moneystring);

    app->Draw(interfacesprite);
}

void Game::levelup()
{
    level++;
    exp -= maxexp;
    maxexp = 1000 * level;
    maxlife = 1000 * level;
    life = maxlife;
    maxmana = 100 * level;
    mana = maxmana;

    snprintf(txt, 99, "Congratulations !\nYou have reached\nlevel %i.", level);
    bobo->say(txt);
}

void Game::setexp(int val)
{
    exp += val * 50 / level; // val = mob level
    if (exp >= maxexp)
        levelup();
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

int Game::getindamage(Mob* mob)
{
    if (!sf::Randomizer::Random(0, 16))
        return 0;
    else
        return mob->level * sf::Randomizer::Random(50, 100);
}

int Game::getoutdamage()
{
    if (!sf::Randomizer::Random(0, 16))
        return 0;
    else
        return level * sf::Randomizer::Random(32, 64);
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

