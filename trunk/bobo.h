class Bobo
{
    public:
    Bobo();
    ~Bobo();
    void animate();
    void draw(sf::RenderWindow* app);
    void say(string message);
    bool firsttimelife;
    bool firsttimemana;
    float x;
    float y;

    #define DIALOGTIME 77

    #define BOBONORMAL 0
    #define BOBOIDLE 1
    #define BOBOTALK 2

    private:
    int index;
    sf::Image boboimage;
    sf::Sprite bobosprite;
    sf::Rect<int> boborect;
    vector<Dialog*> dialogs;
    int time;
    int currentframe;
};

Bobo::Bobo()
{
    firsttimelife = false;
    firsttimemana = false;

    index = BOBONORMAL;

    x = 800 - 128;
    y = 472 + 16;

    boboimage = getimage(GFX_BOBO);
    boboimage.SetSmooth(false);

    boborect.Left = index * 128;
    boborect.Right = boborect.Left + 128;
    boborect.Bottom = 0;
    boborect.Top = 128;
    bobosprite.SetImage(boboimage);
    bobosprite.SetSubRect(boborect);

    time = 0;

    currentframe = 0;
}

Bobo::~Bobo()
{
    dialogs.clear();
}

void Bobo::animate()
{
    if (currentframe < MAX_FRAME)
        currentframe++;
    else
    {
        currentframe = 0;

        if (!dialogs.size()) // bobo is idle
        {
            if (!sf::Randomizer::Random(0, 10))
                index = BOBOIDLE;
            else
                index = BOBONORMAL;
        }
        else // bobo talks
        {
            if (!sf::Randomizer::Random(0, 4))
                index = BOBOTALK;
            else
                index = BOBONORMAL;

            time++;
            if (time >= DIALOGTIME)
            {
                time = 0;
                delete dialogs[0];
                dialogs.erase(dialogs.begin());
            }
        }
    }
}

void Bobo::draw(sf::RenderWindow* app)
{
    boborect.Left = index * 128;
    boborect.Right = boborect.Left + 128;
    bobosprite.SetSubRect(boborect);
    bobosprite.SetX(x);
    bobosprite.SetY(y);
    app->Draw(bobosprite);

    if (dialogs.size())
        dialogs[0]->draw(app);
}

void Bobo::say(string message)
{
    dialogs.push_back(new Dialog(message));
}
