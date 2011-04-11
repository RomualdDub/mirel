class Map
{
    public:
    Map();
    ~Map();
    void animate();
    void draw(sf::RenderWindow* app, sf::View view);
    void addhero(Hero* hero);
    void addpet(Pet* pet);
    void addmob(Mob* mob);
    void addloot(Loot* loot);
    void deletemob(int index);
    void deleteloot(int index);
    void updatehero(Hero* hero);
    void updatepet(Pet* pet);
    void updatemob(int index, Mob* mob);
    void updateloot(int index, Loot* loot);
    bool petleft;
    bool petright;

    private:
    Label* herolabel;
    Label* petlabel;
    vector<Label*> moblabels;
    vector<Label*> lootlabels;
    sf::Image arrowimage;
    sf::Sprite arrowsprite;
};

Map::Map()
{
    petleft = false;
    petright = false;

    arrowimage = getimage(GFX_ARROW);
    arrowimage.SetSmooth(false);

    arrowsprite.SetImage(arrowimage);
}

Map::~Map()
{
    delete herolabel;
    delete petlabel;
    moblabels.clear();
    lootlabels.clear();
}

void Map::draw(sf::RenderWindow* app, sf::View view)
{
    herolabel->draw(app);

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
        petlabel->draw(app);

    for (int i = 0; i < (int)moblabels.size(); i++)
        moblabels[i]->draw(app);

    for (int i = 0; i < (int)lootlabels.size(); i++)
        lootlabels[i]->draw(app);
}

void Map::addhero(Hero* hero)
{
    herolabel = new Label(hero->x, LABEL_HERO, NULL, NULL, NULL, NULL);
}

void Map::addpet(Pet* pet)
{
    petlabel = new Label(pet->x, LABEL_PET, pet->level, pet->hp, &pet->petsprite, pet->name);
}

void Map::addmob(Mob* mob)
{
    moblabels.push_back(new Label(mob->x, LABEL_MOB, mob->level, mob->hp, &mob->mobsprite, mob->name));
}

void Map::addloot(Loot* loot)
{
    lootlabels.push_back(new Label(loot->x, LABEL_LOOT, NULL, NULL, &loot->lootsprite, NULL));
}

void Map::deletemob(int index)
{
    delete moblabels[index];
    moblabels.erase(moblabels.begin() + index);
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
}

void Map::updatemob(int index, Mob* mob)
{
    moblabels[index]->x = mob->x;
    moblabels[index]->hp = mob->hp;
}

void Map::updateloot(int index, Loot* loot)
{
    lootlabels[index]->x = loot->x;
}
