class Label
{
    public:
    Label(int inx, int intype, int inlevel, int inmaxhp, sf::Sprite* sprite, char* inname);
    void draw(sf::RenderWindow* app);
    int x;
    int hp;

    private:
    int type;
    int maxhp;
    int y;
    char name[100];
    sf::Font font;
    sf::String labelstring;
    sf::Sprite iconsprite;
    sf::Sprite hpsprite;
    sf::Image iconimage;
    sf::Image hpimage;
    sf::Rect<int> iconrect;
};

Label::Label(int inx, int intype, int inlevel, int inmaxhp, sf::Sprite* sprite, char* inname)
{
    x = inx;
    y = 480;

    type = intype;

    maxhp = inmaxhp;
    hp = inmaxhp;

    if (type == LABEL_HERO)
    {
        iconimage = getimage(GFX_FX0);
        iconimage.SetSmooth(false);
        iconsprite.SetImage(iconimage);
    }
    else if (type == LABEL_PET)
    {
        iconsprite.SetImage(*sprite->GetImage());

        iconrect.Left = 3 * 64;
        iconrect.Right = iconrect.Left + 64;
        iconrect.Top = 0;
        iconrect.Bottom = 64;

        iconsprite.SetSubRect(iconrect);

        font = getfont();

        snprintf(name, 99, "%s", inname);
        labelstring.SetText(name);
        labelstring.SetFont(font);
        labelstring.SetSize(16);
        labelstring.SetColor(sf::Color::White);
    }
    else if (type == LABEL_MOB)
    {
        iconsprite.SetImage(*sprite->GetImage());

        iconrect.Left = 0 * 64;
        iconrect.Right = iconrect.Left + 64;
        iconrect.Top = 0;
        iconrect.Bottom = 64;

        iconsprite.SetSubRect(iconrect);

        font = getfont();

        snprintf(name, 99, "%s (lvl %i)", inname, inlevel);
        labelstring.SetText(name);
        labelstring.SetFont(font);
        labelstring.SetSize(16);
        labelstring.SetColor(sf::Color::White);
    }
    else if (type == LABEL_LOOT)
    {
        iconsprite.SetImage(*sprite->GetImage());
        if (iconsprite.GetSize().x > 32) // coin detected
        {
            iconrect.Left = 0 * 32;
            iconrect.Right = iconrect.Left + 32;
            iconrect.Top = 0;
            iconrect.Bottom = 32;

            iconsprite.SetSubRect(iconrect);
        }
    }

    if (type == LABEL_PET || type == LABEL_MOB)
    {
        hpimage = getimage(GFX_MINIHP);
        hpimage.SetSmooth(false);
        hpsprite.SetImage(hpimage);
    }
}

void Label::draw(sf::RenderWindow* app)
{
    iconsprite.SetX(x);
    if (type == LABEL_HERO)
        iconsprite.SetY(y + 16);
    else
        iconsprite.SetY(y);
    if (type == LABEL_PET || type == LABEL_MOB)
        iconsprite.SetScale(0.25f, 0.25f);
    else if (type == LABEL_LOOT)
        iconsprite.SetScale(0.5f, 0.5f);
    app->Draw(iconsprite);

    if (type == LABEL_PET || type == LABEL_MOB)
    {
        labelstring.SetX(x + 16);
        labelstring.SetY(y - 2);
        app->Draw(labelstring);

        float hprecord = (float)hp / (float)maxhp;

        if (hprecord == 1)
        {
            sf::Color color(0, 255, 0);
            hpsprite.SetColor(color);
        }
        else if (hprecord > 0.375 && hprecord < 1)
        {
            sf::Color color(255, 255, 0);
            hpsprite.SetColor(color);
        }
        else if (hprecord <= 0.375)
        {
            sf::Color color(255, 0, 0);
            hpsprite.SetColor(color);
        }

        hpsprite.SetX(x);
        hpsprite.SetY(y + 16);
        hpsprite.SetScaleX(hprecord);
        app->Draw(hpsprite);
    }
}
