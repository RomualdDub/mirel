class Item
{
    public:
    Item(Gfx* ingfx, string type, int itemindex);
    void draw(sf::RenderWindow* app);
    string name;
    int level;
    int bonus;
    int x;
    int y;

    private:
    Gfx* gfx;
    sf::Sprite itemsprite;
};

Item::Item(Gfx* ingfx, string type, int itemindex)
{
    gfx = ingfx;

    x = 0;
    y = 0;

    itemsprite.SetImage(*gfx->loaditem(type, itemindex));
}

void Item::draw(sf::RenderWindow* app)
{
    itemsprite.SetX(x);
    itemsprite.SetY(y);
    app->Draw(itemsprite);
}
