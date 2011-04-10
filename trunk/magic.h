class Magic
{
    public:
    Magic(bool indirection, float inx, float iny);
    void animate();
    void draw(sf::RenderWindow* app);
    float x;
    float y;
    bool direction;
    int time;

    private:
    sf::Image magicimage;
    sf::Sprite magicsprite;
    float angle;
    int currentframe;
    int maxframe;
};

Magic::Magic(bool indirection, float inx, float iny)
{
    direction = indirection;
    if (direction == DIRECTION_RIGHT)
        x = inx + 64;
    else
        x = inx - 64;

    y = iny - 64;

    angle = 0;
    time = 0;

    magicimage.LoadFromFile("gfx/pyro0.png");
    magicimage.SetSmooth(false);

    magicsprite.SetImage(magicimage);
    magicsprite.SetX(x);
    magicsprite.SetY(y);

    currentframe = 0;
    maxframe = 7;
}

void Magic::animate()
{
    if (currentframe < maxframe)
        currentframe++;
    else
    {
        currentframe = 0;

        if (direction == DIRECTION_RIGHT)
            x += 16;
        else
            x -= 16;

        angle++;
        y += 16 * sin(angle);

        time++;
    }
}

void Magic::draw(sf::RenderWindow* app)
{
    if (direction == DIRECTION_LEFT)
        magicsprite.FlipX(true);
    else
        magicsprite.FlipX(false);

    magicsprite.SetX(x);
    magicsprite.SetY(y);
    app->Draw(magicsprite);
}
