class Bobo
{
    public:
    Bobo(System* insys);
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

    #define BOBONORMAL 0
    #define BOBOIDLE 1
    #define BOBOTALK 2

    private:
    int index;
    System* sys;
    vector<sf::Image> boboimages;
    vector<sf::Sprite> bobosprites;
    vector<Dialog*> dialogs;
    vector<string> options;
    int time;
    int currentframe;
};

Bobo::Bobo(System* insys)
{
    ishelp = false;
    firsttimelife = false;
    firsttimemana = false;

    index = BOBONORMAL;

    x = 800 - 128;
    y = 472 - 128 + 16;

    options.push_back("Configure keys.");
    options.push_back("Reset default keys.");
    options.push_back("Tell me something !");
    options.push_back("Nothing...");

    boboimages.push_back(sf::Image(getimage(GFX_BOBO0)));
    boboimages.push_back(sf::Image(getimage(GFX_BOBO1)));
    boboimages.push_back(sf::Image(getimage(GFX_BOBO2)));

    for (int i = 0; i < (int)boboimages.size(); i++)
    {
        bobosprites.push_back(sf::Sprite(boboimages[i]));
        bobosprites[i].SetX(x);
        bobosprites[i].SetY(y);
    }

    sys = insys;

    time = 0;

    currentframe = 0;
}

Bobo::~Bobo()
{
    boboimages.clear();
    bobosprites.clear();
    dialogs.clear();
    options.clear();
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
                int joke = sf::Randomizer::Random(0, 5);
                if (joke == 0)
                    say("You are beautiful !");
                else if (joke == 1)
                    say("Every system tends to\nthe perfection.\nOr not...");
                else if (joke == 2)
                    say("The meaning of the Universe\nand everything is 42.");
                else if (joke == 3)
                    say("ALT+F3 and ALT+F4 are on a boat...");
                else if (joke == 4)
                    say("Don't be bad...");
                else
                    say("I don't want to...");
            }

            if (sys->configurekeys)
            {
                if (!sys->keyleft)
                {
                    dialogs.clear();
                    dialogs.push_back(new Dialog("Press 'Left' key..."));
                }
                else if (!sys->keyright)
                {
                    dialogs.clear();
                    dialogs.push_back(new Dialog("Press 'Right' key..."));
                }
                else if (!sys->keyjump)
                {
                    dialogs.clear();
                    dialogs.push_back(new Dialog("Press 'Jump' key..."));
                }
                else if (!sys->keyaction)
                {
                    dialogs.clear();
                    dialogs.push_back(new Dialog("Press 'Action' key..."));
                }
                else if (!sys->keypet)
                {
                    dialogs.clear();
                    dialogs.push_back(new Dialog("Press 'Pet control' key..."));
                }
                else
                {
                    dialogs.clear();
                    sys->configurekeys = false;
                    ishelp = false;
                }
            }

            if (sys->resetkeys)
            {
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
        dialogs.push_back(new Dialog(message));
}

void Bobo::help()
{
    ishelp = true;
    dialogs.clear();
    dialogs.push_back(new Dialog("How can I help you ?", options, sys));
}
