class Bobo
{
    public:
    Bobo(Gfx* ingfx, System* insys);
    ~Bobo();
    void animate();
    void draw(sf::RenderWindow* app);
    void say(string message);
    void help();
    bool ishelp;
    bool firsttimelife;
    bool firsttimemana;
    int x;
    int y;

    #define DIALOGTIME 77

    private:
    Gfx* gfx;
    int index;
    System* sys;
    vector<sf::Sprite> bobosprites;
    vector<Dialog*> dialogs;
    vector<string> options;
    ifstream fjokes;
    vector<string> jokes;
    int time;
    int currentframe;
};

Bobo::Bobo(Gfx* ingfx, System* insys)
{
    gfx = ingfx;

    ishelp = false;
    firsttimelife = false;
    firsttimemana = false;

    index = gfx->bobo_normal;

    x = 800 - 128;
    y = 472 - 128 + 16;

    options.push_back("Configure keys.");
    options.push_back("Reset default keys.");
    options.push_back("Tell me something !");
    options.push_back("Nothing...");

    for (int i = 0; i < (int)gfx->boboimages.size(); i++)
    {
        bobosprites.push_back(sf::Sprite(gfx->boboimages[i]));
        bobosprites[i].SetX(x);
        bobosprites[i].SetY(y);
    }

    sys = insys;

    // jokes gathering and formating

    ifstream fjokes("data/jokes.txt", ios::in);
    string fline;
    while (getline(fjokes, fline))
        jokes.push_back(fline);
    fjokes.close();

    for (int i = 0; i < (int)jokes.size(); i++)
    {
        while ((int)jokes[i].find("\\") != -1)
            jokes[i][jokes[i].find("\\")] = '\n';
    }

    time = 0;

    currentframe = 0;
}

Bobo::~Bobo()
{
    bobosprites.clear();
    dialogs.clear();
    options.clear();
    jokes.clear();
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
                index = gfx->bobo_idle;
            else
                index = gfx->bobo_normal;
        }
        else // bobo talks
        {
            if (!sf::Randomizer::Random(0, 4))
                index = gfx->bobo_talk;
            else
                index = gfx->bobo_normal;

            if (sys->closedialog)
            {
                sys->closedialog = false;
                ishelp = false;
                dialogs.clear();
            }

            if (sys->sayajoke)
            {
                sys->sayajoke = false;
                ishelp = false;
                dialogs.clear();
                time = 0;
                int jindex = sf::Randomizer::Random(0, (int)jokes.size());
                if (jindex < (int)jokes.size())
                    say(jokes[jindex]);
                else
                    say("I don't want to...");
            }

            if (sys->configurekeys)
            {
                if (!sys->keyleft)
                {
                    dialogs.clear();
                    dialogs.push_back(new Dialog(gfx, "Press 'Left' key..."));
                }
                else if (!sys->keyright)
                {
                    dialogs.clear();
                    dialogs.push_back(new Dialog(gfx, "Press 'Right' key..."));
                }
                else if (!sys->keyjump)
                {
                    dialogs.clear();
                    dialogs.push_back(new Dialog(gfx, "Press 'Jump' key..."));
                }
                else if (!sys->keyaction)
                {
                    dialogs.clear();
                    dialogs.push_back(new Dialog(gfx, "Press 'Action' key..."));
                }
                else if (!sys->keypet)
                {
                    dialogs.clear();
                    dialogs.push_back(new Dialog(gfx, "Press 'Pet control' key..."));
                }
                else
                {
                    sys->configurekeys = false;
                    ishelp = false;
                    dialogs.clear();
                }
            }

            if (sys->resetkeys)
            {
                sys->resetkeys = false;
                ishelp = false;
                dialogs.clear();
            }

            if (!ishelp)
            {
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
}

void Bobo::draw(sf::RenderWindow* app)
{
    app->Draw(bobosprites[index]);

    if (dialogs.size())
        dialogs[0]->draw(app);
}

void Bobo::say(string message)
{
    if (!ishelp)
        dialogs.push_back(new Dialog(gfx, message));
}

void Bobo::help()
{
    ishelp = true;
    dialogs.clear();
    dialogs.push_back(new Dialog(gfx, "How can I help you ?", options, sys));
}
