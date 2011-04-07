class Map
{
    public:
    Map(Scene* scene);
    ~Map();
    void animate(Scene* scene);
    void draw(Scene* scene);

    private:
    Label* herolabel;
    Label* petlabel;
    vector<Label*> moblabels;
    vector<Label*> lootlabels;
};

Map::Map(Scene* scene)
{
    herolabel = new Label(scene->hero->x, LABEL_HERO, NULL, NULL, NULL, NULL);

    petlabel = new Label(scene->pet->x, LABEL_PET, scene->pet->level, scene->pet->hp, scene->pet->texture[2], scene->pet->name);

    for (int i = 0; i < (int)scene->mobs.size(); i++)
        moblabels.push_back(new Label(scene->mobs[i]->x, LABEL_MOB, scene->mobs[i]->level, scene->mobs[i]->hp, scene->mobs[i]->texture[0], scene->mobs[i]->name));
}

Map::~Map()
{
    delete herolabel;
    delete petlabel;
    moblabels.clear();
    lootlabels.clear();
}

void Map::animate(Scene* scene)
{
    // hero

    herolabel->x = scene->hero->x;

    // pet

    petlabel->x = scene->pet->x;
    petlabel->hp = scene->pet->hp;

    // mobs

    for (int i = 0; i < (int)scene->mobs.size(); i++)
    {
        moblabels[i]->x = scene->mobs[i]->x;
        moblabels[i]->hp = scene->mobs[i]->hp;
    }

    // loots

    if ((int)scene->loots.size() != (int)lootlabels.size())
    {
        lootlabels.clear();
        for (int i = 0; i < (int)scene->loots.size(); i++)
        {
            if (scene->loots[i]->type == LOOT_MONEY)
                lootlabels.push_back(new Label(scene->loots[i]->x, LABEL_LOOT, NULL, NULL, scene->loots[i]->cointexture[0], NULL));
            else if (scene->loots[i]->type == LOOT_POTIONM)
                lootlabels.push_back(new Label(scene->loots[i]->x, LABEL_LOOT, NULL, NULL, scene->loots[i]->potionmtexture, NULL));
            else if (scene->loots[i]->type == LOOT_POTIONL)
                lootlabels.push_back(new Label(scene->loots[i]->x, LABEL_LOOT, NULL, NULL, scene->loots[i]->potionltexture, NULL));
        }
    }
    else
        for (int i = 0; i < (int)lootlabels.size(); i++)
            lootlabels[i]->x = scene->loots[i]->x;
}

void Map::draw(Scene* scene)
{
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    if (scene->hero->x <= 400)
        gluOrtho2D(0, 800, 600, 0);
    else if (scene->hero->x > 400 && scene->hero->x <= 2400 - 400)
        gluOrtho2D(scene->hero->x - 400, scene->hero->x + 400, 600, 0);
    else if (scene->hero->x > 2400 - 400)
        gluOrtho2D(2400 - 800, 2400, 600, 0);

    herolabel->draw();

    petlabel->draw();

    for (int i = 0; i < (int)moblabels.size(); i++)
        moblabels[i]->draw();

    for (int i = 0; i < (int)lootlabels.size(); i++)
        lootlabels[i]->draw();
}
