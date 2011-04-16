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
    Label* herolabel;
    Label* petlabel;
    vector<Label*> moblabels;
    vector<Label*> bosslabels;
    vector<Label*> lootlabels;
    sf::Image arrowimage;
    sf::Sprite arrowsprite;
};

Map::Map()
{
    petleft = false;
    petright = false;

    arrowimage = getimage(GFX_ARROW);

    arrowsprite.SetImage(arrowimage);
}

Map::~Map()
{
    delete herolabel;
    delete petlabel;
    moblabels.clear();
    bosslabels.clear();
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

    if (bosslabels.size())
        bosslabels[0]->draw(app);

    for (int i = 0; i < (int)lootlabels.size(); i++)
        lootlabels[i]->draw(app);
}

void Map::addhero(Hero* hero)
{
    herolabel = new Label(hero->x, LABEL_HERO, NULL, NULL, NULL, NULL);
}

void Map::addpet(Pet* pet)
{
    petlabel = new Label(pet->x, LABEL_PET, NULL, pet->hp, &pet->petsprites[3], pet->name);
}

void Map::addmob(Mob* mob)
{
    moblabels.push_back(new Label(mob->x, LABEL_MOB, mob->level, mob->hp, &mob->mobsprites[0], mob->name));
}

void Map::addboss(Boss* boss)
{
    bosslabels.push_back(new Label(boss->x, LABEL_BOSS, boss->level, boss->hp, &boss->bosssprites[0], boss->name));
}

void Map::addloot(Loot* loot)
{
    lootlabels.push_back(new Label(loot->x, LABEL_LOOT, NULL, NULL, &loot->lootsprites[0], NULL));
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
