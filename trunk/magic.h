class Magic
{
    public:
    Magic(bool indirection, float inx, float iny);
    void animate(int distance);
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

    magicimage = getimage(GFX_PYRO0);
    magicimage.SetSmooth(false);

    magicsprite.SetImage(magicimage);
    magicsprite.SetX(x);
    magicsprite.SetY(y);

    currentframe = 0;
}

void Magic::animate(int distance)
{
    if (currentframe < MAX_FRAME)
        currentframe++;
    else
    {
        currentframe = 0;

        if (direction == DIRECTION_RIGHT)
            x += 16 * distance;
        else
            x -= 16 * distance;

        angle++;
        y += 16 * distance * sin(angle);

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
