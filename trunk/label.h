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
};

Label::Label(int inx, int intype, int inlevel, int inmaxhp, sf::Sprite* sprite, char* inname)
{
    x = inx;
    y = 480;

    type = intype;

    if (type == LABEL_HERO)
    {
        iconimage = getimage(GFX_FX0);
        iconsprite.SetImage(iconimage);
    }
    else if (type == LABEL_PET)
    {
        iconsprite.SetImage(*sprite->GetImage());

        font = getfont();

        snprintf(name, 99, "%s", inname);
        labelstring.SetText(name);
        labelstring.SetFont(font);
        labelstring.SetSize(16);
        labelstring.SetColor(sf::Color::White);
    }
    else if (type == LABEL_MOB || type == LABEL_BOSS)
    {
        iconsprite.SetImage(*sprite->GetImage());

        font = getfont();

        snprintf(name, 99, "%s (lvl %i)", inname, inlevel);
        labelstring.SetText(name);
        labelstring.SetFont(font);
        labelstring.SetSize(16);
        labelstring.SetColor(sf::Color::White);
    }
    else if (type == LABEL_LOOT)
        iconsprite.SetImage(*sprite->GetImage());

    if (type == LABEL_PET || type == LABEL_MOB || type == LABEL_BOSS)
    {
        hpimage = getimage(GFX_MINIHP);
        hpsprite.SetImage(hpimage);

        maxhp = inmaxhp;
        hp = inmaxhp;
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
    else if (type == LABEL_BOSS)
        iconsprite.SetScale(0.125f, 0.125f);
    app->Draw(iconsprite);

    if (type == LABEL_PET || type == LABEL_MOB || type == LABEL_BOSS)
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

        if (hprecord > 0.0)
        {
            hpsprite.SetX(x);
            hpsprite.SetY(y + 16);
            hpsprite.SetScaleX(hprecord);
            app->Draw(hpsprite);
        }
    }
}
