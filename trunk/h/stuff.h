class Stuff
{
    public:
    Stuff(Gfx* ingfx, Xml* inxml);
    void draw(sf::RenderWindow* app);
    bool visible;

    private:
    Gfx* gfx;
    Xml* xml;
    sf::Sprite stuffsprite;
    sf::String hatstring, hatlevelstring, hatbonusstring, hatlevelinstring, hatbonusinstring;
    sf::String dressstring, dresslevelstring, dressbonusstring, dresslevelinstring, dressbonusinstring;
    sf::String wandstring, wandlevelstring, wandbonusstring, wandlevelinstring, wandbonusinstring;
    std::ostringstream txt;
};

Stuff::Stuff(Gfx* ingfx, Xml* inxml)
{
    gfx = ingfx;
    xml = inxml;

    visible = false;

    stuffsprite.SetImage(gfx->interfaceimages[gfx->interface_stuff]);
    stuffsprite.SetX(208);
    stuffsprite.SetY(108);

    hatstring.SetFont(gfx->font);
    hatstring.SetSize(16);
    hatstring.SetColor(sf::Color::White);
    hatstring.SetX(208 + 124);
    hatstring.SetY(108 + 32);
    hatlevelstring.SetText("Level >");
    hatlevelstring.SetFont(gfx->font);
    hatlevelstring.SetSize(16);
    hatlevelstring.SetColor(sf::Color::Black);
    hatlevelstring.SetX(208 + 124);
    hatlevelstring.SetY(108 + 32 + 16);
    hatlevelinstring.SetFont(gfx->font);
    hatlevelinstring.SetSize(16);
    hatlevelinstring.SetColor(sf::Color::White);
    hatlevelinstring.SetX(hatlevelstring.GetRect().Right + 8);
    hatlevelinstring.SetY(108 + 32 + 16);
    hatbonusstring.SetText("Bonus (Mana) >");
    hatbonusstring.SetFont(gfx->font);
    hatbonusstring.SetSize(16);
    hatbonusstring.SetColor(sf::Color::Black);
    hatbonusstring.SetX(208 + 124);
    hatbonusstring.SetY(108 + 32 + 32);
    hatbonusinstring.SetFont(gfx->font);
    hatbonusinstring.SetSize(16);
    hatbonusinstring.SetColor(sf::Color::White);
    hatbonusinstring.SetX(hatbonusstring.GetRect().Right + 8);
    hatbonusinstring.SetY(108 + 32 + 32);

    dressstring.SetFont(gfx->font);
    dressstring.SetSize(16);
    dressstring.SetColor(sf::Color::White);
    dressstring.SetX(208 + 124);
    dressstring.SetY(108 + 32 + 66);
    dresslevelstring.SetText("Level >");
    dresslevelstring.SetFont(gfx->font);
    dresslevelstring.SetSize(16);
    dresslevelstring.SetColor(sf::Color::Black);
    dresslevelstring.SetX(208 + 124);
    dresslevelstring.SetY(108 + 32 + 66 + 16);
    dresslevelinstring.SetFont(gfx->font);
    dresslevelinstring.SetSize(16);
    dresslevelinstring.SetColor(sf::Color::White);
    dresslevelinstring.SetX(dresslevelstring.GetRect().Right + 8);
    dresslevelinstring.SetY(108 + 32 + 66 + 16);
    dressbonusstring.SetText("Bonus (Life) >");
    dressbonusstring.SetFont(gfx->font);
    dressbonusstring.SetSize(16);
    dressbonusstring.SetColor(sf::Color::Black);
    dressbonusstring.SetX(208 + 124);
    dressbonusstring.SetY(108 + 32 + 66 + 32);
    dressbonusinstring.SetFont(gfx->font);
    dressbonusinstring.SetSize(16);
    dressbonusinstring.SetColor(sf::Color::White);
    dressbonusinstring.SetX(dressbonusstring.GetRect().Right + 8);
    dressbonusinstring.SetY(108 + 32 + 66 + 32);

    wandstring.SetFont(gfx->font);
    wandstring.SetSize(16);
    wandstring.SetColor(sf::Color::White);
    wandstring.SetX(208 + 124);
    wandstring.SetY(108 + 32 + 66 * 2);
    wandlevelstring.SetText("Level >");
    wandlevelstring.SetFont(gfx->font);
    wandlevelstring.SetSize(16);
    wandlevelstring.SetColor(sf::Color::Black);
    wandlevelstring.SetX(208 + 124);
    wandlevelstring.SetY(108 + 32 + 66 * 2 + 16);
    wandlevelinstring.SetFont(gfx->font);
    wandlevelinstring.SetSize(16);
    wandlevelinstring.SetColor(sf::Color::White);
    wandlevelinstring.SetX(wandlevelstring.GetRect().Right + 8);
    wandlevelinstring.SetY(108 + 32 + 66 * 2 + 16);
    wandbonusstring.SetFont(gfx->font);
    wandbonusstring.SetSize(16);
    wandbonusstring.SetColor(sf::Color::Black);
    wandbonusstring.SetX(208 + 124);
    wandbonusstring.SetY(108 + 32 + 66 * 2 + 32);
    wandbonusinstring.SetFont(gfx->font);
    wandbonusinstring.SetSize(16);
    wandbonusinstring.SetColor(sf::Color::White);
    wandbonusinstring.SetY(108 + 32 + 66 * 2 + 32);
}

void Stuff::draw(sf::RenderWindow* app)
{
    app->Draw(stuffsprite);

    for (int i = 0; i < (int)xml->currentitems.size(); i++)
    {
        xml->currentitems[i]->x = 208 + 39;
        xml->currentitems[i]->y = 108 + 32 + i * 66;
        xml->currentitems[i]->draw(app);

        if (i == 0) // hat
        {
            if (xml->currentitems[i]->incrementation)
            {
                txt << xml->currentitems[i]->name << " +" << xml->currentitems[i]->incrementation;
                hatstring.SetText(txt.str());
                txt.str("");
            }
            else
                hatstring.SetText(xml->currentitems[i]->name);

            txt << xml->currentitems[i]->level;
            hatlevelinstring.SetText(txt.str());
            txt.str("");
            txt << "+" << xml->currentitems[i]->bonus;
            hatbonusinstring.SetText(txt.str());
            txt.str("");

            app->Draw(hatstring);
            app->Draw(hatlevelstring);
            app->Draw(hatbonusstring);
            app->Draw(hatlevelinstring);
            app->Draw(hatbonusinstring);
        }
        else if (i == 1) // dress
        {
            if (xml->currentitems[i]->incrementation)
            {
                txt << xml->currentitems[i]->name << " +" << xml->currentitems[i]->incrementation;
                dressstring.SetText(txt.str());
                txt.str("");
            }
            else
                dressstring.SetText(xml->currentitems[i]->name);

            txt << xml->currentitems[i]->level;
            dresslevelinstring.SetText(txt.str());
            txt.str("");
            txt << "+" << xml->currentitems[i]->bonus;
            dressbonusinstring.SetText(txt.str());
            txt.str("");

            app->Draw(dressstring);
            app->Draw(dresslevelstring);
            app->Draw(dressbonusstring);
            app->Draw(dresslevelinstring);
            app->Draw(dressbonusinstring);
        }
        else if (i == 2) // wand
        {
            if (xml->currentitems[i]->incrementation)
            {
                txt << xml->currentitems[i]->name << " +" << xml->currentitems[i]->incrementation;
                wandstring.SetText(txt.str());
                txt.str("");
            }
            else
                wandstring.SetText(xml->currentitems[i]->name);

            txt << xml->currentitems[i]->level;
            wandlevelinstring.SetText(txt.str());
            txt.str("");
            if (xml->currentitems[i]->bonus < 0) // dps wand
            {
                txt << "Bonus (Damage) >";
                wandbonusstring.SetText(txt.str());
                txt.str("");
                txt << "+" << -xml->currentitems[i]->bonus;
                wandbonusinstring.SetText(txt.str());
                txt.str("");
            }
            else if (xml->currentitems[i]->bonus > 0) // heal wand
            {
                txt << "Bonus (Heal) >";
                wandbonusstring.SetText(txt.str());
                txt.str("");
                txt << "+" << xml->currentitems[i]->bonus;
                wandbonusinstring.SetText(txt.str());
                txt.str("");
            }
            else // beginner's wand
            {
                txt << "Bonus >";
                wandbonusstring.SetText(txt.str());
                txt.str("");
                txt << "+" << xml->currentitems[i]->bonus;
                wandbonusinstring.SetText(txt.str());
                txt.str("");
            }
            wandbonusinstring.SetX(wandbonusstring.GetRect().Right + 8);

            app->Draw(wandstring);
            app->Draw(wandlevelstring);
            app->Draw(wandbonusstring);
            app->Draw(wandlevelinstring);
            app->Draw(wandbonusinstring);
        }
    }
}
