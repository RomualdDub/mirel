class World
{
    public:
    World();
    void animate();
    void draw(sf::RenderWindow* app);

    private:
    GLuint skyx;
    sf::Image skytempimage;
    sf::Image skyimage;
    sf::Image worldimage;
    sf::Sprite skysprite;
    sf::Sprite worldsprite;
    sf::Rect<int> skyrect;
    int currentframe;
    int maxframe;
};

World::World()
{
    skytempimage.LoadFromFile("gfx/sky0.png");

    sf::Color color(0, 0, 0, 0);
    skyimage.Create(800 * 2, 600, color);
    skyimage.SetSmooth(false);

    skyimage.Copy(skytempimage, 0, 0, skyrect, false);
    skyimage.Copy(skytempimage, 800, 0, skyrect, false);

    skyrect.Left = 0;
    skyrect.Right = 800;
    skyrect.Bottom = 600;
    skyrect.Top = 0;
    skysprite.SetImage(skyimage);
    skysprite.SetSubRect(skyrect);

    worldimage.LoadFromFile("gfx/world0.png");
    worldimage.SetSmooth(false);
    worldsprite.SetImage(worldimage);

    skyx = 0;

    currentframe = 0;
    maxframe = 7;
}

void World::animate()
{
    if (currentframe < maxframe)
        currentframe++;
    else
    {
        currentframe = 0;
        if (skyx == 800)
            skyx = 0;
        else
            skyx++;
    }
}

void World::draw(sf::RenderWindow* app)
{
    skyrect.Left = skyx;
    skyrect.Right = skyrect.Left + 800;
    skysprite.SetSubRect(skyrect);

    sf::View view = app->GetView();
    sf::FloatRect viewrect = view.GetRect();
    skysprite.SetX(viewrect.Left);

    app->Draw(skysprite);
    app->Draw(worldsprite);
}
