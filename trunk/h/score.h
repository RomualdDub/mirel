class Score
{
    public:
    Score(Gfx* ingfx, Xml* inxml, string type, float inval, int inx, int iny);
    void animate(int distance);
    void draw(sf::RenderWindow* app);
    bool collide(Hero* hero);
    int x;
    int y;
    int basey;
    float angle;
    float val;

    private:
    Xml* xml;
    bool zoom;
    float size;
    float maxsize;
    sf::Font font;
    sf::String scorestring0;
    sf::String scorestring1;
    int currentframe;
};

Score::Score(Gfx* ingfx, Xml* inxml, string type, float inval, int inx, int iny)
{
    xml = inxml;

    x = inx;
    y = iny - 32 - sf::Randomizer::Random(32, 128);
    basey = y;
    val = inval;

    angle = sf::Randomizer::Random(1, 4);

    zoom = true;
    size = 1.0f;
    maxsize = sf::Randomizer::Random(1.0f, 1.5f);

    sf::Color color0;
    sf::Color color1;

    if (type == "money")
    {
        color0.r = 109;
        color0.g = 97;
        color0.b = 0;
        color1.r = 255;
        color1.g = 255;
        color1.b = sf::Randomizer::Random(0, 128);
    }
    else if (type == "damage")
    {
        color0.r = 77;
        color0.g = 97;
        color0.b = 109;
        color1.r = sf::Randomizer::Random(128, 255);
        color1.g = sf::Randomizer::Random(128, 255);
        color1.b = 255;
    }
    else if (type == "life")
    {
        color0.r = 165;
        color0.g = 0;
        color0.b = 0;
        color1.r = 255;
        color1.g = 0;
        color1.b = sf::Randomizer::Random(0, 128);
    }
    else if (type == "mana")
    {
        color0.r = 0;
        color0.g = 0;
        color0.b = 128;
        color1.r = 0;
        color1.g = sf::Randomizer::Random(0, 128);
        color1.b = 255;
    }

    std::ostringstream txt;

    if (val)
        txt << (int)val;
    else
        txt << "Miss";

    scorestring0.SetText(txt.str());
    scorestring0.SetFont(ingfx->font);
    scorestring0.SetSize(32);
    scorestring0.SetColor(color0);

    scorestring1.SetText(txt.str());
    scorestring1.SetFont(ingfx->font);
    scorestring1.SetSize(32);
    scorestring1.SetColor(color1);

    txt.str("");

    currentframe = 0;
}

void Score::animate(int distance)
{
    if (currentframe < MAX_FRAME)
        currentframe++;
    else
    {
        currentframe = 0;

        if (zoom)
        {
            size += 0.05 * distance;
            if (size >= maxsize)
                zoom = false;
        }
        else
        {
            size -= 0.05 * distance;
            if (size <= -maxsize)
                zoom = true;
        }

        y -= angle;
    }
}

void Score::draw(sf::RenderWindow* app)
{
    scorestring0.SetScale(size, size);
    scorestring0.SetX(x - 4);
    scorestring0.SetY(y - 4);
    app->Draw(scorestring0);

    scorestring1.SetScale(size, size);
    scorestring1.SetX(x);
    scorestring1.SetY(y);
    app->Draw(scorestring1);
}

bool Score::collide(Hero* hero)
{
    if (hero->xe + 32 >= x && hero->xe <= x + 32 && hero->ye + 64 >= y && hero->ye <= y + 32)
        return true;
    else
        return false;
}
