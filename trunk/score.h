class Score
{
    public:
    Score(Gfx* gfx, int type, float val, int inx, int iny);
    void animate(int distance);
    void draw(sf::RenderWindow* app);
    int x;
    int y;
    int basey;
    float angle;

    private:
    bool zoom;
    float size;
    float maxsize;
    char txt[100];
    sf::Font font;
    sf::String scorestring0;
    sf::String scorestring1;
    int currentframe;
};

Score::Score(Gfx* gfx, int type, float val, int inx, int iny)
{
    x = inx;
    y = iny - 32 - sf::Randomizer::Random(32, 128);
    basey = y;

    angle = sf::Randomizer::Random(1, 4);

    zoom = true;
    size = 1.0f;
    maxsize = sf::Randomizer::Random(1.0f, 1.5f);

    sf::Color color0;
    sf::Color color1;

    if (type == SCORE_MONEY)
    {
        color0.r = 109;
        color0.g = 97;
        color0.b = 0;
        color1.r = 255;
        color1.g = 255;
        color1.b = sf::Randomizer::Random(0, 128);
    }
    else if (type == SCORE_DAMAGE)
    {
        color0.r = 77;
        color0.g = 97;
        color0.b = 109;
        color1.r = sf::Randomizer::Random(128, 255);
        color1.g = sf::Randomizer::Random(128, 255);
        color1.b = 255;
    }
    else if (type == SCORE_LIFE)
    {
        color0.r = 165;
        color0.g = 0;
        color0.b = 0;
        color1.r = 255;
        color1.g = 0;
        color1.b = sf::Randomizer::Random(0, 128);
    }
    else if (type == SCORE_MANA)
    {
        color0.r = 0;
        color0.g = 0;
        color0.b = 128;
        color1.r = 0;
        color1.g = sf::Randomizer::Random(0, 128);
        color1.b = 255;
    }

    if (val)
        snprintf(txt, 99, "%i", (int)val);
    else
        snprintf(txt, 99, "Miss");

    scorestring0.SetText(txt);
    scorestring0.SetFont(gfx->font);
    scorestring0.SetSize(32);
    scorestring0.SetColor(color0);

    scorestring1.SetText(txt);
    scorestring1.SetFont(gfx->font);
    scorestring1.SetSize(32);
    scorestring1.SetColor(color1);

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
