class Loot
{
    public:
    Loot(int inlevel, float inx, float iny);
    ~Loot();
    void animate(int distance);
    void draw(sf::RenderWindow* app);
    bool collide(Hero* hero);
    bool visible;
    float x;
    float y;
    int level;
    int type;
    vector<sf::Sprite> lootsprites;

    private:
    float angle;
    vector<sf::Image> lootimages;
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
        lootimages.push_back(sf::Image(getimage(GFX_COIN0)));
        lootimages.push_back(sf::Image(getimage(GFX_COIN1)));
        lootimages.push_back(sf::Image(getimage(GFX_COIN2)));
        lootimages.push_back(sf::Image(getimage(GFX_COIN3)));
    }
    else if (type == LOOT_POTIONM)
        lootimages.push_back(sf::Image(getimage(GFX_POTIONM)));
    else if (type == LOOT_POTIONL)
        lootimages.push_back(sf::Image(getimage(GFX_POTIONL)));

    for (int i = 0; i < (int)lootimages.size(); i++)
        lootsprites.push_back(sf::Sprite(lootimages[i]));

    currentframe = 0;
}

Loot::~Loot()
{
    lootimages.clear();
    lootsprites.clear();
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
    lootsprites[index].SetX(x);
    lootsprites[index].SetY(y);
    app->Draw(lootsprites[index]);
}

bool Loot::collide(Hero* hero)
{
    if (hero->x + 64 >= x && hero->x <= x + 32 && hero->y + 128 >= y && hero->y <= y + 32)
        return true;
    else
        return false;
}
