class Map
{
    public:
    ~Map();
    void animate();
    void draw(sf::RenderWindow* app);

    Label* herolabel;
    Label* petlabel;
    vector<Label*> moblabels;
    vector<Label*> lootlabels;
};

Map::~Map()
{
    delete herolabel;
    delete petlabel;
    moblabels.clear();
    lootlabels.clear();
}

void Map::draw(sf::RenderWindow* app)
{
    herolabel->draw(app);

    petlabel->draw(app);

    for (int i = 0; i < (int)moblabels.size(); i++)
        moblabels[i]->draw(app);

    for (int i = 0; i < (int)lootlabels.size(); i++)
        lootlabels[i]->draw(app);
}
