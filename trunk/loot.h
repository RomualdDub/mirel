class Loot
{
    public:
    Loot(int inlevel, float inx, float iny);
    void animate(int distance);
    void draw(sf::RenderWindow* app);
    bool collide(Hero* hero);
    bool visible;
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
};

Loot::Loot(int inlevel, float inx, float iny)
{
    index = 0;

    visible = false;

    level = inlevel;

    x = inx;
    y = iny - 96 - sf::Randomizer::Random(32, 64);

    angle = 0;

    type = sf::Randomizer::Random(LOOT_MONEY, LOOT_POTIONL);

    if (type == LOOT_MONEY)
    {
        lootimage = getimage(GFX_COIN);

        lootrect.Left = index * 32;
        lootrect.Right = lootrect.Left + 32;
        lootrect.Bottom = 0;
        lootrect.Top = 32;

        lootsprite.SetSubRect(lootrect);
    }
    else if (type == LOOT_POTIONM)
        lootimage = getimage(GFX_POTIONM);
    else if (type == LOOT_POTIONL)
        lootimage = getimage(GFX_POTIONL);

    lootimage.SetSmooth(false);
    lootsprite.SetImage(lootimage);

    currentframe = 0;
}

void Loot::animate(int distance)
{
    if (currentframe < MAX_FRAME)
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
        y += 5 * distance * sin(angle);
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
    if (hero->x >= x - 32 && hero->x - 64 <= x && hero->y >= y - 32 && hero->y - 96 <= y)
        return true;
    else
        return false;
}
