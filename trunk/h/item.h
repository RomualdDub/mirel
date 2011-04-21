class Item
{
    public:
    Item(Gfx* ingfx, string intype, int inindex);
    void draw(sf::RenderWindow* app);
    void setstats(int inincrementation, int inbonus);
    void setprice(int inlevel);
    string name;
    string type;
    int index;
    int level;
    int incrementation;
    int bonus;
    int price;
    int x;
    int y;

    private:
    Gfx* gfx;
    sf::Sprite itemsprite;
};

Item::Item(Gfx* ingfx, string intype, int inindex)
{
    gfx = ingfx;

    type = intype;
    index = inindex;

    incrementation = 0;

    itemsprite.SetImage(*gfx->loaditem(type, index));
}

void Item::draw(sf::RenderWindow* app)
{
    itemsprite.SetX(x);
    itemsprite.SetY(y);
    app->Draw(itemsprite);
}

void Item::setstats(int inincrementation, int inbonus)
{
    incrementation = inincrementation;
    bonus = inbonus;
    if (incrementation)
        bonus += bonus / 10 * incrementation;
}

void Item::setprice(int inlevel)
{
    level = inlevel;
    price = level * (8 * (incrementation + 1));
}
