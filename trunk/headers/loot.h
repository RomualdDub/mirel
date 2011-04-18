class Loot
{
    public:
    Loot(Gfx* ingfx, int inlevel, float inx, float iny);
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
    Gfx* gfx;
    float angle;
    int index;
    int currentframe;
};

Loot::Loot(Gfx* ingfx, int inlevel, float inx, float iny)
{
    gfx = ingfx;

    index = 0;

    visible = false;

    level = inlevel;

    x = inx;
    y = iny - 96 - sf::Randomizer::Random(32, 64);

    angle = 0;

    type = sf::Randomizer::Random(LOOT_MONEY, LOOT_POTIONL);

    if (type == LOOT_MONEY)
    {
        for (int i = gfx->loot_coin0; i < gfx->loot_coin3; i++)
            lootsprites.push_back(sf::Sprite(gfx->lootimages[i]));
    }
    else if (type == LOOT_POTIONL)
        lootsprites.push_back(sf::Sprite(gfx->lootimages[gfx->loot_potionl]));
    else if (type == LOOT_POTIONM)
        lootsprites.push_back(sf::Sprite(gfx->lootimages[gfx->loot_potionm]));

    currentframe = 0;
}

Loot::~Loot()
{
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
            if (index > gfx->loot_coin3 - 1)
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
