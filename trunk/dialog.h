class Dialog
{
    public:
    Dialog(string text);
    void draw(sf::RenderWindow* app);
    int x;
    int y;
    int w;
    int h;

    #define XMARGIN 32

    private:
    sf::Font font;
    sf::String dialogstring;
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

Dialog::Dialog(string text)
{
    x = 800 - (128 * 2);
    y = 472 - 128;

    // text construction

    font = getfont();

    dialogstring.SetText(text);
    dialogstring.SetFont(font);
    dialogstring.SetSize(16);
    dialogstring.SetColor(sf::Color::Black);

    w = dialogstring.GetRect().GetWidth();
    h = dialogstring.GetRect().GetHeight();

    y -= h;

    dialogstring.SetX(x);
    dialogstring.SetY(y);

    // phylactery construction

    cornerimage = getimage(GFX_DIALOGCORNER);
    cornerimage.SetSmooth(false);
    extensionimage = getimage(GFX_DIALOGEXTENSION);
    extensionimage.SetSmooth(false);

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
}
