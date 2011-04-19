class Inventory
{
    public:
    Inventory(Gfx* ingfx);
    void animate();
    void draw(sf::RenderWindow* app);
    bool visible;

    private:
    Gfx* gfx;
    sf::Sprite invsprite;
    int currentframe;
};

Inventory::Inventory(Gfx* ingfx)
{
    gfx = ingfx;

    visible = false;

    invsprite.SetImage(gfx->interfaceimages[gfx->interface_inv]);
    invsprite.SetX(272);
    invsprite.SetY(108);

    currentframe = 0;
}

void Inventory::animate()
{
    if (currentframe < MAX_FRAME)
        currentframe++;
    else
    {
        currentframe = 0;
    }
}

void Inventory::draw(sf::RenderWindow* app)
{
    app->Draw(invsprite);
}
