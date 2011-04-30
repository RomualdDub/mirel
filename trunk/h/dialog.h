class Dialog
{
    public:
    Dialog(Gfx* ingfx, string intype, string intext);
    Dialog(Gfx* ingfx, string intype, string message, vector<string> messages, int inx, int iny);
    Dialog(Gfx* ingfx, string intype, string message, vector<string> messages, int inx, int iny, System* insys);
    ~Dialog();
    void animate();
    void draw(sf::RenderWindow* app);
    int x;
    int y;
    int w;
    int h;

    #define XMARGIN 32

    private:
    void philactery();
    Gfx* gfx;
    System* sys;
    string type;
    sf::Vector2<int> mouseclick;
    sf::String dialogstring;
    vector<sf::String> strings;
    sf::Sprite dotsprite;
    sf::Sprite topleftsprite;
    sf::Shape toprectangle;
    sf::Sprite toprightsprite;
    sf::Shape middlerectangle;
    sf::Sprite bottomleftsprite;
    sf::Shape bottomrectangle;
    sf::Sprite bottomrightsprite;
    sf::Sprite extensionsprite;
    sf::Vector2f coinpos;
    vector<sf::Sprite> coinsprites;
    int coinindex;
};

Dialog::Dialog(Gfx* ingfx, string intype, string message)
{
    gfx = ingfx;

    x = 800 - (128 * 2);
    y = 472 - 128;

    dialogstring.SetText(message);
    dialogstring.SetFont(gfx->font);
    dialogstring.SetSize(16);
    dialogstring.SetColor(sf::Color::Black);

    w = dialogstring.GetRect().GetWidth();
    h = dialogstring.GetRect().GetHeight();

    x += 128 - w;
    y -= h;

    dialogstring.SetX(x);
    dialogstring.SetY(y);

    type = intype;

    philactery();
}

Dialog::Dialog(Gfx* ingfx, string intype, string message, vector<string> messages, int inx, int iny)
{
    gfx = ingfx;

    x = inx;
    y = iny;

    dialogstring.SetText(message);
    dialogstring.SetFont(gfx->font);
    dialogstring.SetSize(16);
    dialogstring.SetColor(sf::Color::Red);
    w = dialogstring.GetRect().GetWidth();
    h = dialogstring.GetRect().GetHeight();

    for (int i = 0; i < (int)messages.size(); i++)
    {
        strings.push_back(sf::String(messages[i]));
        strings[i].SetFont(gfx->font);
        strings[i].SetSize(16);
        strings[i].SetColor(sf::Color::Black);

        if (strings[i].GetRect().GetWidth() > w)
            w = strings[i].GetRect().GetWidth();
        h += strings[i].GetRect().GetHeight();
    }

    x -= w / 2;
    y -= h * 2;

    if (x - XMARGIN < 0)
        x = XMARGIN;
    if (y - 32 < 0)
        y = 32;

    dialogstring.SetX(x);
    dialogstring.SetY(y);

    dotsprite.SetImage(gfx->dialogimages[gfx->dialog_dot]);

    for (int i = 0; i < (int)strings.size(); i++)
    {
        strings[i].SetX(x + 16);
        strings[i].SetY(y + 16 + i * 16);

        int coinstringpos = messages[i].find("$");
        if (coinstringpos != -1)
        {
            coinpos = strings[i].GetCharacterPos(coinstringpos);
            messages[i][coinstringpos] = 0;
            strings[i].SetText(messages[i]);
            for (int j = gfx->loot_coin0; j < gfx->loot_coin3; j++)
            {
                coinsprites.push_back(sf::Sprite(gfx->lootimages[gfx->loot_coin0 + j]));
                coinsprites[j].SetScale(0.5f, 0.5f);
                coinsprites[j].SetX((int)coinpos.x + x + 16);
                coinsprites[j].SetY((int)coinpos.y + y + 32);
            }
            coinindex = gfx->loot_coin0;
        }
    }

    type = intype;

    philactery();
    extensionsprite.FlipX(true);
}

Dialog::Dialog(Gfx* ingfx, string intype, string message, vector<string> messages, int inx, int iny, System* insys)
{
    gfx = ingfx;

    x = inx;
    y = iny;

    mouseclick.x = 0;
    mouseclick.y = 0;

    dialogstring.SetText(message);
    dialogstring.SetFont(gfx->font);
    dialogstring.SetSize(16);
    dialogstring.SetColor(sf::Color::Black);
    w = dialogstring.GetRect().GetWidth();
    h = dialogstring.GetRect().GetHeight();

    for (int i = 0; i < (int)messages.size(); i++)
    {
        strings.push_back(sf::String(messages[i]));
        strings[i].SetFont(gfx->font);
        strings[i].SetSize(16);
        strings[i].SetColor(sf::Color::Black);

        if (strings[i].GetRect().GetWidth() > w)
            w = strings[i].GetRect().GetWidth();
        h += strings[i].GetRect().GetHeight();
    }

    x -= w;
    y -= h;

    dialogstring.SetX(x);
    dialogstring.SetY(y);

    dotsprite.SetImage(gfx->dialogimages[gfx->dialog_dot]);

    for (int i = 0; i < (int)strings.size(); i++)
    {
        strings[i].SetX(x + 16);
        strings[i].SetY(y + 16 + i * 16);
    }

    sys = insys;

    type = intype;

    philactery();
}

Dialog::~Dialog()
{
    strings.clear();
    coinsprites.clear();
}

void Dialog::animate()
{
    coinindex++;
    if (coinindex > gfx->loot_coin3 - 1)
        coinindex = 0;
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

    if (input.IsMouseButtonDown(sf::Mouse::Left))
    {
        mouseclick.x = input.GetMouseX();
        mouseclick.y = input.GetMouseY();
    }

    for (int i = 0; i < (int)strings.size(); i++)
    {
        if (coinsprites.size())
            app->Draw(coinsprites[coinindex]);

        if (type != "simple" && type != "multi")
        {
            if (input.GetMouseX() >= strings[i].GetRect().Left && input.GetMouseX() <= strings[i].GetRect().Right && input.GetMouseY() >= strings[i].GetRect().Top && input.GetMouseY() <= strings[i].GetRect().Bottom)
            {
                strings[i].SetColor(sf::Color::Red);

                if (!input.IsMouseButtonDown(sf::Mouse::Left)\
                    && mouseclick.x >= strings[i].GetRect().Left\
                    && mouseclick.x <= strings[i].GetRect().Right\
                    && mouseclick.y >= strings[i].GetRect().Top\
                    && mouseclick.y <= strings[i].GetRect().Bottom)
                {
                    mouseclick.x = 0;
                    mouseclick.y = 0;
                    if (type == "bobo_options")
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
                    else if (type == "item_options")
                    {
                        if (i == SYS_USEITEM)
                            sys->useitem = true;
                        else if (i == SYS_DELETEITEM)
                            sys->deleteitem = true;
                        else if (i == SYS_CLOSEDIALOGITEM)
                            sys->closedialogitem = true;
                    }
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
    topleftsprite.SetImage(gfx->dialogimages[gfx->dialog_corner]);
    topleftsprite.FlipY(true);
    topleftsprite.SetX(x - XMARGIN);
    topleftsprite.SetY(y - 32);

    toprectangle = sf::Shape::Rectangle(x + 32 - XMARGIN, y - 32, x + w - 32 + XMARGIN, y, sf::Color::White);

    toprightsprite.SetImage(gfx->dialogimages[gfx->dialog_corner]);
    toprightsprite.FlipX(true);
    toprightsprite.FlipY(true);
    toprightsprite.SetX(x + w - 32 + XMARGIN);
    toprightsprite.SetY(y - 32);

    middlerectangle = sf::Shape::Rectangle(x - XMARGIN, y, x + w + XMARGIN, y + h, sf::Color::White);

    bottomleftsprite.SetImage(gfx->dialogimages[gfx->dialog_corner]);
    bottomleftsprite.SetX(x - XMARGIN);
    bottomleftsprite.SetY(y + h);

    bottomrectangle = sf::Shape::Rectangle(x + 32 - XMARGIN, y + h, x + w - 32 + XMARGIN, y + h + 32, sf::Color::White);

    bottomrightsprite.SetImage(gfx->dialogimages[gfx->dialog_corner]);
    bottomrightsprite.FlipX(true);
    bottomrightsprite.SetX(x + w - 32 + XMARGIN);
    bottomrightsprite.SetY(y + h);

    extensionsprite.SetImage(gfx->dialogimages[gfx->dialog_extension]);
    extensionsprite.SetX(x + w / 2);
    extensionsprite.SetY(y + h + 32);
}
