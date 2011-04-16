class Dialog
{
    public:
    Dialog(string intext);
    Dialog(string message, vector<string> messages, int inx, int iny);
    Dialog(string message, vector<string> messages, System* insys);
    ~Dialog();
    void draw(sf::RenderWindow* app);
    int x;
    int y;
    int w;
    int h;

    #define DIALOGSTANDARD 0
    #define DIALOGSPELL 1
    #define DIALOGOPTIONS 2

    #define XMARGIN 32

    private:
    void philactery();
    System* sys;
    int type;
    sf::Font font;
    sf::String dialogstring;
    vector<sf::String> strings;
    sf::Image dotimage;
    sf::Sprite dotsprite;
    sf::Image cornerimage;
    sf::Image extensionimage;
    sf::Image spaceimage;
    sf::Sprite topleftsprite;
    sf::Shape toprectangle;
    sf::Sprite toprightsprite;
    sf::Shape middlerectangle;
    sf::Sprite bottomleftsprite;
    sf::Shape bottomrectangle;
    sf::Sprite bottomrightsprite;
    sf::Sprite extensionsprite;
};

Dialog::Dialog(string message)
{
    x = 800 - (128 * 2);
    y = 472 - 128;

    font = getfont();

    dialogstring.SetText(message);
    dialogstring.SetFont(font);
    dialogstring.SetSize(16);
    dialogstring.SetColor(sf::Color::Black);

    w = dialogstring.GetRect().GetWidth();
    h = dialogstring.GetRect().GetHeight();

    x += 128 - w;
    y -= h;

    dialogstring.SetX(x);
    dialogstring.SetY(y);

    type = DIALOGSTANDARD;

    philactery();
}

Dialog::Dialog(string message, vector<string> messages, int inx, int iny)
{
    x = inx;
    y = iny - 128;

    font = getfont();

    dialogstring.SetText(message);
    dialogstring.SetFont(font);
    dialogstring.SetSize(16);
    dialogstring.SetColor(sf::Color::Red);

    w = dialogstring.GetRect().GetWidth();
    h = dialogstring.GetRect().GetHeight();

    x += 128 - w;
    y -= h;

    dialogstring.SetX(x);
    dialogstring.SetY(y);

    dotimage = getimage(GFX_DIALOGDOT);

    dotsprite.SetImage(dotimage);

    for (int i = 0; i < (int)messages.size(); i++)
    {
        strings.push_back(sf::String(messages[i]));
        strings[i].SetFont(font);
        strings[i].SetSize(16);
        strings[i].SetColor(sf::Color::Black);
        strings[i].SetX(x + 16);
        strings[i].SetY(y + h);

        if (strings[i].GetRect().GetWidth() > w)
            w = strings[i].GetRect().GetWidth();
        h += strings[i].GetRect().GetHeight();
    }

    type = DIALOGSPELL;

    philactery();
    extensionsprite.FlipX(true);
}

Dialog::Dialog(string message, vector<string> messages, System* insys)
{
    x = 800 - (128 * 2);
    y = 472 - 128;

    font = getfont();

    dialogstring.SetText(message);
    dialogstring.SetFont(font);
    dialogstring.SetSize(16);
    dialogstring.SetColor(sf::Color::Black);

    w = dialogstring.GetRect().GetWidth();
    h = dialogstring.GetRect().GetHeight();

    x += 128 - w;
    y -= h;

    dialogstring.SetX(x);
    dialogstring.SetY(y);

    dotimage = getimage(GFX_DIALOGDOT);

    dotsprite.SetImage(dotimage);

    for (int i = 0; i < (int)messages.size(); i++)
    {
        strings.push_back(sf::String(messages[i]));
        strings[i].SetFont(font);
        strings[i].SetSize(16);
        strings[i].SetColor(sf::Color::Black);
        strings[i].SetX(x + 16);
        strings[i].SetY(y + h);

        if (strings[i].GetRect().GetWidth() > w)
            w = strings[i].GetRect().GetWidth();
        h += strings[i].GetRect().GetHeight();
    }

    sys = insys;

    type = DIALOGOPTIONS;

    philactery();
}

Dialog::~Dialog()
{
    strings.clear();
}

void Dialog::draw(sf::RenderWindow* app)
{
    app->Draw(topleftsprite);
    app->Draw(toprectangle);
    app->Draw(toprightsprite);
    app->Draw(middlerectangle);
    app->Draw(bottomleftsprite);
    app->Draw(bottomrectangle);
    app->Draw(bottomrightsprite);
    app->Draw(extensionsprite);
    app->Draw(dialogstring);

    const sf::Input& input = app->GetInput();

    for (int i = 0; i < (int)strings.size(); i++)
    {
        if (type == DIALOGOPTIONS)
        {
            if (input.GetMouseX() >= strings[i].GetRect().Left && input.GetMouseX() <= strings[i].GetRect().Right && input.GetMouseY() >= strings[i].GetRect().Top && input.GetMouseY() <= strings[i].GetRect().Bottom)
            {
                strings[i].SetColor(sf::Color::Red);

                if (input.IsMouseButtonDown(sf::Mouse::Left))
                {
                    if (i == SYS_CONFIGKEYS)
                    {
                        sys->configurekeys = true;
                        sys->keyleft = 0;
                        sys->keyright = 0;
                        sys->keyjump = 0;
                        sys->keyaction = 0;
                        sys->keypet = 0;
                    }
                    else if (i == SYS_RESETKEYS)
                    {
                        sys->resetkeys = true;
                        sys->keyleft = sf::Key::Q;
                        sys->keyright = sf::Key::D;
                        sys->keyjump = sf::Key::Z;
                        sys->keyaction = sf::Key::Space;
                        sys->keypet = sf::Key::A;
                    }
                    else if (i == SYS_JOKE)
                        sys->sayajoke = true;
                    else if (i == SYS_CLOSEDIALOG)
                        sys->closedialog = true;
                }
            }
            else
                strings[i].SetColor(sf::Color::Black);
        }

        dotsprite.SetX(x);
        dotsprite.SetY(strings[i].GetRect().Top + 4);

        app->Draw(dotsprite);
        app->Draw(strings[i]);
    }
}

void Dialog::philactery()
{
    cornerimage = getimage(GFX_DIALOGCORNER);
    extensionimage = getimage(GFX_DIALOGEXTENSION);

    topleftsprite.SetImage(cornerimage);
    topleftsprite.FlipY(true);
    topleftsprite.SetX(x - XMARGIN);
    topleftsprite.SetY(y - 32);

    toprectangle = sf::Shape::Rectangle(x + 32 - XMARGIN, y - 32, x + w - 32 + XMARGIN, y, sf::Color::White);

    toprightsprite.SetImage(cornerimage);
    toprightsprite.FlipX(true);
    toprightsprite.FlipY(true);
    toprightsprite.SetX(x + w - 32 + XMARGIN);
    toprightsprite.SetY(y - 32);

    middlerectangle = sf::Shape::Rectangle(x - XMARGIN, y, x + w + XMARGIN, y + h, sf::Color::White);

    bottomleftsprite.SetImage(cornerimage);
    bottomleftsprite.SetX(x - XMARGIN);
    bottomleftsprite.SetY(y + h);

    bottomrectangle = sf::Shape::Rectangle(x + 32 - XMARGIN, y + h, x + w - 32 + XMARGIN, y + h + 32, sf::Color::White);

    bottomrightsprite.SetImage(cornerimage);
    bottomrightsprite.FlipX(true);
    bottomrightsprite.SetX(x + w - 32 + XMARGIN);
    bottomrightsprite.SetY(y + h);

    extensionsprite.SetImage(extensionimage);
    extensionsprite.SetX(x);
    extensionsprite.SetY(y + h + 32);
}
