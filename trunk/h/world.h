class World
{
    public:
    World(Gfx* ingfx, Xml* inxml);
    ~World();
    void animate(int distance);
    void draw(sf::RenderWindow* app);
    vector<int> ground;
    vector<sf::Sprite> occludesprites;

    private:
    Gfx* gfx;
    int skyx;
    vector<sf::Sprite> skysprites;
    vector<sf::Sprite> worldsprites;
    int currentframe;
};

World::World(Gfx* ingfx, Xml* inxml)
{
    gfx = ingfx;

    inxml->readworlds(inxml->infoworldindex);

    gfx->loadworld(inxml->infoworldindex, inxml->worldsplit);
    gfx->loadmob(inxml->worldmobindex);
    gfx->loadboss(inxml->worldbossindex);

    // sky

    skysprites.push_back(sf::Sprite(gfx->worldskyimages[0]));
    skysprites.push_back(sf::Sprite(gfx->worldskyimages[0]));

    skysprites[1].SetX(800);

    skyx = 0;

    // world background

    for (int i = 0; i < (int)gfx->worldbgimages.size(); i++)
    {
        worldsprites.push_back(sf::Sprite(gfx->worldbgimages[i]));
        worldsprites[i].SetX(i * 800);
    }

    // collide

    int index;
    int x2;
    sf::Color pixel;
    for (int x = 0; x < 800 * inxml->worldsplit; x++)
    {
        index = x / 800;
        x2 = x - (800 * index);
        for (int y = 0; y < 472; y++)
        {
            pixel = gfx->worldcollideimages[index].GetPixel(x2, y);
            if (pixel.r == 255 && pixel.g == 255 && pixel.b == 255)
            {
                ground.push_back(y);
                break;
            }
        }
    }

    // occlude

    for (int i = 0; i < (int)gfx->worldoccludeimages.size(); i++)
    {
        occludesprites.push_back(sf::Sprite(gfx->worldoccludeimages[i]));
        occludesprites[i].SetX(i * 800);
    }

    currentframe = 0;
}

World::~World()
{
    ground.clear();
    occludesprites.clear();
    skysprites.clear();
    worldsprites.clear();
}

void World::animate(int distance)
{
    if (currentframe < MAX_FRAME)
        currentframe++;
    else
    {
        currentframe = 0;
        if (skyx >= 800)
            skyx = 0;
        else
            skyx += distance;
    }
}

void World::draw(sf::RenderWindow* app)
{
    for (int i = 0; i < (int)skysprites.size(); i++)
    {
        skysprites[i].SetX(i * 800 + app->GetView().GetRect().Left - skyx);
        app->Draw(skysprites[i]);
    }

    for (int i = 0; i < (int)worldsprites.size(); i++)
        app->Draw(worldsprites[i]);
}
