class Game
{
    public:
    Game();
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
    int coinindex;
    int currentframe;
};

Game::Game()
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
    sf::Color color(255, 255, 255);

    levelstring.SetFont(font);
    levelstring.SetSize(32);
    levelstring.SetColor(color);
    levelstring.SetX(42);
    levelstring.SetY(44);

    lifestring.SetFont(font);
    lifestring.SetSize(16);
    lifestring.SetColor(color);
    lifestring.SetX(124);
    lifestring.SetY(24);

    manastring.SetFont(font);
    manastring.SetSize(16);
    manastring.SetColor(color);
    manastring.SetX(124);
    manastring.SetY(66);

    expstring.SetFont(font);
    expstring.SetSize(16);
    expstring.SetColor(color);
    expstring.SetX(124);
    expstring.SetY(108);

    moneystring.SetFont(font);
    moneystring.SetSize(16);
    moneystring.SetColor(color);
    moneystring.SetX(40);
    moneystring.SetY(136);

    level = 1;
    maxlife = 1000 * level;
    life = maxlife;
    maxmana = 100 * level;
    mana = maxmana;
    exp = 0;
    maxexp = 1000 * level;
    money = 0;
    coinindex = 0;

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

    char txt[100];

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

    if (life > maxlife)
        life = maxlife;
    if (life < 0)
        life = 0;
}

void Game::setmana(float val)
{
    mana += val; // val = mana

    if (mana > maxmana)
        mana = maxmana;
    if (mana < 0)
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

