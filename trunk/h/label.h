class Label
{
    public:
    Label(Gfx* ingfx, Hero* hero);
    Label(Gfx* ingfx, Pet* pet);
    Label(Gfx* ingfx, Mob* mob);
    Label(Gfx* ingfx, Boss* boss);
    Label(Gfx* ingfx, Loot* loot);
    void draw(sf::RenderWindow* app, Hero* hero);
    void draw(sf::RenderWindow* app, Pet* pet);
    void draw(sf::RenderWindow* app, Mob* mob);
    void draw(sf::RenderWindow* app, Boss* boss);
    void draw(sf::RenderWindow* app, Loot* loot);
    int x;
    int hp;
    int maxhp;

    private:
    Gfx* gfx;
    int type;
    int y;
    string name;
    sf::String labelstring;
    sf::Sprite iconsprite;
    sf::Sprite hpsprite;
};

Label::Label(Gfx* ingfx, Hero* hero)
{
    gfx = ingfx;

    x = hero->x;
    y = 480 + 16;

    iconsprite.SetImage(gfx->fximages[gfx->fx_0]);
}

Label::Label(Gfx* ingfx, Pet* pet)
{
    gfx = ingfx;

    x = pet->x;
    y = 480;

    iconsprite.SetImage(*pet->petsprites[0].GetImage());
    iconsprite.SetScale(0.25f, 0.25f);

    name = pet->name;
    labelstring.SetText(name);
    labelstring.SetFont(gfx->font);
    labelstring.SetSize(16);
    labelstring.SetColor(sf::Color::White);

    hpsprite.SetImage(gfx->mapimages[gfx->map_minihp]);

    maxhp = pet->maxhp;
    hp = pet->hp;
}

Label::Label(Gfx* ingfx, Mob* mob)
{
    gfx = ingfx;

    x = mob->x;
    y = 480;

    iconsprite.SetImage(*mob->mobsprites[0].GetImage());
    iconsprite.SetScale(0.25f, 0.25f);

    std::ostringstream txt;
    txt << mob->name << " (" << mob->level << ")";
    name = txt.str();
    txt.str("");
    labelstring.SetText(name);
    labelstring.SetFont(gfx->font);
    labelstring.SetSize(16);
    labelstring.SetColor(sf::Color::White);

    hpsprite.SetImage(gfx->mapimages[gfx->map_minihp]);

    maxhp = mob->getmaxhp();
    hp = mob->hp;
}

Label::Label(Gfx* ingfx, Boss* boss)
{
    gfx = ingfx;

    x = boss->x;
    y = 480;

    iconsprite.SetImage(*boss->bosssprites[0].GetImage());
    iconsprite.SetScale(0.125f, 0.125f);

    std::ostringstream txt;
    txt << boss->name << " (" << boss->level << ")";
    name = txt.str();
    txt.str("");
    labelstring.SetText(name);
    labelstring.SetFont(gfx->font);
    labelstring.SetSize(16);
    labelstring.SetColor(sf::Color::White);

    hpsprite.SetImage(gfx->mapimages[gfx->map_minihp]);

    maxhp = boss->getmaxhp();
    hp = boss->hp;
}

Label::Label(Gfx* ingfx, Loot* loot)
{
    gfx = ingfx;

    x = loot->x;
    y = 480;

    iconsprite.SetImage(*loot->lootsprites[0].GetImage());
    iconsprite.SetScale(0.5f, 0.5f);
}

void Label::draw(sf::RenderWindow* app, Hero* hero)
{
    iconsprite.SetX(x);
    iconsprite.SetY(y);
    app->Draw(iconsprite);
}

void Label::draw(sf::RenderWindow* app, Pet* pet)
{
    if (pet->attack)
        iconsprite.SetImage(*pet->petsprites[3].GetImage());
    else
        iconsprite.SetImage(*pet->petsprites[0].GetImage());
    if (pet->direction == DIRECTION_LEFT)
        iconsprite.FlipX(true);
    else if (pet->direction == DIRECTION_RIGHT)
        iconsprite.FlipX(false);
    iconsprite.SetX(x);
    iconsprite.SetY(y);
    app->Draw(iconsprite);

    labelstring.SetX(x + 16);
    labelstring.SetY(y - 2);
    app->Draw(labelstring);

    float hprecord = (float)pet->hp / (float)pet->maxhp;

    if (hprecord == 1)
    {
        sf::Color color(0, 255, 0);
        hpsprite.SetColor(color);
    }
    else if (hprecord > 0.375 && hprecord < 1)
    {
        sf::Color color(255, 255, 0);
        hpsprite.SetColor(color);
    }
    else if (hprecord <= 0.375)
    {
        sf::Color color(255, 0, 0);
        hpsprite.SetColor(color);
    }

    if (hprecord > 0.0)
    {
        hpsprite.SetX(x);
        hpsprite.SetY(y + 16);
        hpsprite.SetScaleX(hprecord);
        app->Draw(hpsprite);
    }
}

void Label::draw(sf::RenderWindow* app, Mob* mob)
{
    if (mob->direction == DIRECTION_LEFT)
        iconsprite.FlipX(true);
    else if (mob->direction == DIRECTION_RIGHT)
        iconsprite.FlipX(false);
    iconsprite.SetX(x);
    iconsprite.SetY(y);
    app->Draw(iconsprite);

    labelstring.SetX(x + 16);
    labelstring.SetY(y - 2);
    app->Draw(labelstring);

    float hprecord = (float)mob->hp / (float)mob->getmaxhp();

    if (hprecord == 1)
    {
        sf::Color color(0, 255, 0);
        hpsprite.SetColor(color);
    }
    else if (hprecord > 0.375 && hprecord < 1)
    {
        sf::Color color(255, 255, 0);
        hpsprite.SetColor(color);
    }
    else if (hprecord <= 0.375)
    {
        sf::Color color(255, 0, 0);
        hpsprite.SetColor(color);
    }

    if (hprecord > 0.0)
    {
        hpsprite.SetX(x);
        hpsprite.SetY(y + 16);
        hpsprite.SetScaleX(hprecord);
        app->Draw(hpsprite);
    }
}

void Label::draw(sf::RenderWindow* app, Boss* boss)
{
    if (boss->direction == DIRECTION_LEFT)
        iconsprite.FlipX(true);
    else if (boss->direction == DIRECTION_RIGHT)
        iconsprite.FlipX(false);
    iconsprite.SetX(x);
    iconsprite.SetY(y);
    app->Draw(iconsprite);

    labelstring.SetX(x + 16);
    labelstring.SetY(y - 2);
    app->Draw(labelstring);

    float hprecord = (float)boss->hp / (float)boss->getmaxhp();

    if (hprecord == 1)
    {
        sf::Color color(0, 255, 0);
        hpsprite.SetColor(color);
    }
    else if (hprecord > 0.375 && hprecord < 1)
    {
        sf::Color color(255, 255, 0);
        hpsprite.SetColor(color);
    }
    else if (hprecord <= 0.375)
    {
        sf::Color color(255, 0, 0);
        hpsprite.SetColor(color);
    }

    if (hprecord > 0.0)
    {
        hpsprite.SetX(x);
        hpsprite.SetY(y + 16);
        hpsprite.SetScaleX(hprecord);
        app->Draw(hpsprite);
    }
}

void Label::draw(sf::RenderWindow* app, Loot* loot)
{
    iconsprite.SetX(x);
    iconsprite.SetY(y);
    app->Draw(iconsprite);
}
