class Map
{
    public:
    Map(Gfx* ingfx);
    ~Map();
    void animate();
    void draw(sf::RenderWindow* app, Hero* hero);
    void draw(sf::RenderWindow* app, sf::View view, Pet* pet);
    void draw(sf::RenderWindow* app, vector<Mob*> mobs);
    void draw(sf::RenderWindow* app, vector<Boss*> boss);
    void draw(sf::RenderWindow* app, vector<Loot*> loot);
    void addhero(Hero* hero);
    void addpet(Pet* pet);
    void addmob(Mob* mob);
    void addboss(Boss* boss);
    void addloot(Loot* loot);
    void deletemob(int index);
    void deleteboss();
    void deleteloot(int index);
    void updatehero(Hero* hero);
    void updatepet(Pet* pet);
    void updatemob(int index, Mob* mob);
    void updateboss(Boss* boss);
    void updateloot(int index, Loot* loot);
    bool petleft;
    bool petright;

    private:
    Gfx* gfx;
    Label* herolabel;
    Label* petlabel;
    vector<Label*> moblabels;
    vector<Label*> bosslabels;
    vector<Label*> lootlabels;
    sf::Sprite arrowsprite;
};

Map::Map(Gfx* ingfx)
{
    gfx = ingfx;

    petleft = false;
    petright = false;

    arrowsprite.SetImage(gfx->mapimages[gfx->map_arrow]);
}

Map::~Map()
{
    delete herolabel;
    delete petlabel;
    moblabels.clear();
    bosslabels.clear();
    lootlabels.clear();
}

void Map::draw(sf::RenderWindow* app, Hero* hero)
{
    herolabel->draw(app, hero);
}

void Map::draw(sf::RenderWindow* app, sf::View view, Pet* pet)
{
    if (petleft)
    {
        app->SetView(app->GetDefaultView());
        arrowsprite.FlipX(true);
        arrowsprite.SetX(4);
        arrowsprite.SetY(480 + 16);
        app->Draw(arrowsprite);
        app->SetView(view);
    }
    else if (petright)
    {
        app->SetView(app->GetDefaultView());
        arrowsprite.FlipX(false);
        arrowsprite.SetX(800 - 16 - 4);
        arrowsprite.SetY(480 + 16);
        app->Draw(arrowsprite);
        app->SetView(view);
    }
    else
        petlabel->draw(app, pet);
}

void Map::draw(sf::RenderWindow* app, vector<Mob*> mobs)
{
   for (int i = 0; i < (int)moblabels.size(); i++)
        moblabels[i]->draw(app, mobs[i]);
}

void Map::draw(sf::RenderWindow* app, vector<Boss*> boss)
{
    if (bosslabels.size())
        bosslabels[0]->draw(app, boss[0]);
}

void Map::draw(sf::RenderWindow* app, vector<Loot*> loots)
{
    for (int i = 0; i < (int)lootlabels.size(); i++)
        lootlabels[i]->draw(app, loots[i]);
}

void Map::addhero(Hero* hero)
{
    herolabel = new Label(gfx, hero);
}

void Map::addpet(Pet* pet)
{
    petlabel = new Label(gfx, pet);
}

void Map::addmob(Mob* mob)
{
    moblabels.push_back(new Label(gfx, mob));
}

void Map::addboss(Boss* boss)
{
    bosslabels.push_back(new Label(gfx, boss));
}

void Map::addloot(Loot* loot)
{
    lootlabels.push_back(new Label(gfx, loot));
}

void Map::deletemob(int index)
{
    delete moblabels[index];
    moblabels.erase(moblabels.begin() + index);
}

void Map::deleteboss()
{
    bosslabels.clear();
}

void Map::deleteloot(int index)
{
    delete lootlabels[index];
    lootlabels.erase(lootlabels.begin() + index);
}

void Map::updatehero(Hero* hero)
{
    herolabel->x = hero->x;
}

void Map::updatepet(Pet* pet)
{
    petlabel->x = pet->x;
    petlabel->hp = pet->hp;
    petlabel->maxhp = pet->maxhp;
}

void Map::updatemob(int index, Mob* mob)
{
    moblabels[index]->x = mob->x;
    moblabels[index]->hp = mob->hp;
}

void Map::updateboss(Boss* boss)
{
    bosslabels[0]->x = boss->x;
    bosslabels[0]->hp = boss->hp;
}

void Map::updateloot(int index, Loot* loot)
{
    lootlabels[index]->x = loot->x;
}
