class Loot
{
    public:
    Loot(int inlevel, float inx, float iny);
    void animate();
    void draw(sf::RenderWindow* app);
    bool collide(Hero* hero);
    float x;
    float y;
    int level;
    int type;
    sf::Sprite lootsprite;

    private:
    float angle;
    sf::Image lootimage;
    sf::Rect<int> lootrect;
    int index;
    int currentframe;
    int maxframe;
};

Loot::Loot(int inlevel, float inx, float iny)
{
    index = 0;

    level = inlevel;

    x = inx;
    y = iny - 128 - sf::Randomizer::Random(32, 64);

    angle = 0;

    type = sf::Randomizer::Random(LOOT_MONEY, LOOT_POTIONL);

    if (type == LOOT_MONEY)
    {
        lootimage.LoadFromFile("gfx/coin.png");

        lootrect.Left = index * 32;
        lootrect.Right = lootrect.Left + 32;
        lootrect.Bottom = 0;
        lootrect.Top = 32;

        lootsprite.SetSubRect(lootrect);
    }
    else if (type == LOOT_POTIONM)
        lootimage.LoadFromFile("gfx/potionm.png");
    else if (type == LOOT_POTIONL)
        lootimage.LoadFromFile("gfx/potionl.png");

    lootimage.SetSmooth(false);
    lootsprite.SetImage(lootimage);

    currentframe = 0;
    maxframe = 7;
}

void Loot::animate()
{
    if (currentframe < maxframe)
        currentframe++;
    else
    {
        currentframe = 0;

        if (type == LOOT_MONEY)
        {
            index++;
            if (index > 3)
                index = 0;
        }

        angle++;
        y += 5 * sin(angle);
    }
}

void Loot::draw(sf::RenderWindow* app)
{
    if (type == LOOT_MONEY)
    {
        lootrect.Left = index * 32;
        lootrect.Right = lootrect.Left + 32;
        lootsprite.SetSubRect(lootrect);
    }

    lootsprite.SetX(x);
    lootsprite.SetY(y);
    app->Draw(lootsprite);
}

bool Loot::collide(Hero* hero)
{
    if (hero->x >= x - 24 && hero->x - 64 <= x && hero->y >= y - 24 && hero->y - 128 <= y)
        return true;
    else
        return false;
}
