class Loot
{
    public:
    Loot(Gfx* ingfx, Xml* inxml, string intype, int inlevel, float inx, float iny);
    ~Loot();
    void animate(int distance);
    void draw(sf::RenderWindow* app);
    bool collide(Hero* hero);
    bool visible;
    float x;
    float y;
    int level;
    string type;
    vector<sf::Sprite> lootsprites;

    private:
    Gfx* gfx;
    Xml* xml;
    float angle;
    int index;
    int currentframe;
};

Loot::Loot(Gfx* ingfx, Xml* inxml, string intype, int inlevel, float inx, float iny)
{
    gfx = ingfx;
    xml = inxml;

    index = 0;

    visible = false;

    level = inlevel;

    x = inx;
    y = iny - 96 - sf::Randomizer::Random(32, 64);

    angle = 0;

    type = intype;

    if (type == "coin")
        for (int i = gfx->loot_coin0; i < gfx->loot_coin3; i++)
            lootsprites.push_back(sf::Sprite(gfx->lootimages[i]));
    else if (type == "potionl")
        lootsprites.push_back(sf::Sprite(gfx->lootimages[gfx->loot_potionl]));
    else if (type == "potionm")
        lootsprites.push_back(sf::Sprite(gfx->lootimages[gfx->loot_potionm]));
    else if (type == "gift")
        lootsprites.push_back(sf::Sprite(gfx->lootimages[gfx->loot_gift]));

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

        if (type == "coin")
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
    if (xml->infoherox + 64 >= x && xml->infoherox <= x + 32 && hero->y + 128 >= y && hero->y <= y + 32)
        return true;
    else
        return false;
}
