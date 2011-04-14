class World
{
    public:
    World();
    ~World();
    void animate(int distance);
    void draw(sf::RenderWindow* app);

    private:
    int skyx;
    sf::Image skyimage;
    vector<sf::Sprite> skysprites;
    vector<sf::Image> worldimages;
    vector<sf::Sprite> worldsprites;
    int currentframe;
};

World::World()
{
    skyimage = getimage(GFX_SKY0);

    skysprites.push_back(sf::Sprite(skyimage));
    skysprites.push_back(sf::Sprite(skyimage));

    skysprites[1].SetX(800);

    worldimages.push_back(sf::Image(getimage(GFX_WORLD01)));
    worldimages.push_back(sf::Image(getimage(GFX_WORLD02)));
    worldimages.push_back(sf::Image(getimage(GFX_WORLD03)));

    for (int i = 0; i < (int)worldimages.size(); i++)
        worldsprites.push_back(sf::Sprite(worldimages[i]));

    worldsprites[1].SetX(800);
    worldsprites[2].SetX(800 * 2);

    skyx = 0;

    currentframe = 0;
}

World::~World()
{
    skysprites.clear();
    worldimages.clear();
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
