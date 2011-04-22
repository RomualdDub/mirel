class Inventory
{
    public:
    Inventory(Gfx* ingfx, System* insys);
    ~Inventory();
    void animate();
    void draw(sf::RenderWindow* app);
    void additem(Item* item);
    void deleteitem(int itemindex);
    void makeinfodialogs();
    bool visible;
    vector<Item*> items;
    vector<Dialog*> infodialogs;
    vector<Dialog*> optiondialogs;
    int dialogindex;
    bool showinfodialog;
    bool showoptiondialog;

    private:
    Gfx* gfx;
    System* sys;
    sf::Vector2<int> mouseclick;
    std::ostringstream txt;
    vector<string> messages;
    vector<string> options;
    int pageindex;
    int maxpages;
    bool preparedialogs;
    sf::Sprite invsprite;
};

Inventory::Inventory(Gfx* ingfx, System* insys)
{
    gfx = ingfx;
    sys = insys;

    visible = false;

    mouseclick.x = 0;
    mouseclick.y = 0;

    preparedialogs = true;
    showinfodialog = false;
    showoptiondialog = false;
    dialogindex = 0;

    options.push_back("Use item.");
    options.push_back("Delete item.");
    options.push_back("Nothing...");

    invsprite.SetImage(gfx->interfaceimages[gfx->interface_inv]);
    invsprite.SetX(272);
    invsprite.SetY(108);

    pageindex = 0;
}

Inventory::~Inventory()
{
    infodialogs.clear();
    optiondialogs.clear();
}

void Inventory::animate()
{
    if (showinfodialog)
        infodialogs[dialogindex]->animate();
}

void Inventory::draw(sf::RenderWindow* app)
{
    app->Draw(invsprite);

    const sf::Input& input = app->GetInput();
    if (input.IsMouseButtonDown(sf::Mouse::Left))
    {
        mouseclick.x = input.GetMouseX();
        mouseclick.y = input.GetMouseY();
    }

    // pages handling

    maxpages = (int)((float)(items.size() - 1) / 6);

    if (!input.IsMouseButtonDown(sf::Mouse::Left)\
        && mouseclick.x >= (272 + 109)\
        && mouseclick.x <= (272 + 122)\
        && mouseclick.y >= (108 + 224)\
        && mouseclick.y <= (108 + 238)) // left page
    {
        mouseclick.x = 0;
        mouseclick.y = 0;

        if (pageindex > 0)
        {
            pageindex--;
            preparedialogs = true;
        }
    }
    else if (!input.IsMouseButtonDown(sf::Mouse::Left)\
        && mouseclick.x >= (272 + 133)\
        && mouseclick.x <= (272 + 146)\
        && mouseclick.y >= (108 + 224)\
        && mouseclick.y <= (108 + 238)) // right page
    {
        mouseclick.x = 0;
        mouseclick.y = 0;

        if (pageindex < maxpages)
        {
            pageindex++;
            preparedialogs = true;
        }
    }

    // display sprites

    for (int i = 0; i < 9; i++)
    {
        if (i + (pageindex * 9) > (int)items.size() - 1)
            break;

        if (i == 0 || i == 3 || i == 6)
            items[i + (pageindex * 9)]->x = 272 + 23;
        else if (i == 1 || i == 4 || i == 7)
            items[i + (pageindex * 9)]->x = 272 + 97;
        else if (i == 2 || i == 5 || i == 8)
            items[i + (pageindex * 9)]->x = 272 + 169;

        if (i >= 0 && i <= 2)
            items[i + (pageindex * 9)]->y = 108 + 33;
        else if (i >= 3 && i <= 5)
            items[i + (pageindex * 9)]->y = 108 + 99;
        else if (i >= 6 && i <= 8)
            items[i + (pageindex * 9)]->y = 108 + 164;

        if (!showoptiondialog)
        {
            if (input.GetMouseX() >= items[i + (pageindex * 9)]->x\
                && input.GetMouseX() <= items[i + (pageindex * 9)]->x + 58\
                && input.GetMouseY() >= items[i + (pageindex * 9)]->y\
                && input.GetMouseY() <= items[i + (pageindex * 9)]->y + 63)
                {
                    dialogindex = i;
                    showinfodialog = true;

                    if (!input.IsMouseButtonDown(sf::Mouse::Left)\
                        && mouseclick.x >= items[i + (pageindex * 9)]->x\
                        && mouseclick.x <= items[i + (pageindex * 9)]->x + 58\
                        && mouseclick.y >= items[i + (pageindex * 9)]->y\
                        && mouseclick.y <= items[i + (pageindex * 9)]->y + 63)
                    {
                        mouseclick.x = 0;
                        mouseclick.y = 0;

                        showinfodialog = false;
                        showoptiondialog = true;
                        optiondialogs.push_back(new Dialog(gfx, "item_options", "What will you do ?", options, items[i + (pageindex * 9)]->x, items[i + (pageindex * 9)]->y, sys));
                        sys->itemindex = i + (pageindex * 9);
                    }
                }
            else
                if (i == 0)
                    showinfodialog = false;
        }

        items[i + (pageindex * 9)]->draw(app);
    }

    if (preparedialogs && items.size() && visible)
        makeinfodialogs();
}

void Inventory::additem(Item* item)
{
    items.push_back(item);
    preparedialogs = true;
}

void Inventory::deleteitem(int itemindex)
{
    delete items[itemindex];
    items.erase(items.begin() + itemindex);
    preparedialogs = true;
}

void Inventory::makeinfodialogs()
{
    preparedialogs = false;
    infodialogs.clear();

    for (int i = 0; i < 9; i++)
    {
        if (i + (pageindex * 9) > (int)items.size() - 1)
            break;

        if (items[i + (pageindex * 9)]->type == "gift")
        {
            messages.clear();
            txt << "Level > " << items[i + (pageindex * 9)]->level;
            messages.push_back(txt.str());
            txt.str("");
            txt << "Price > " << items[i + (pageindex * 9)]->price << "$";
            messages.push_back(txt.str());
            txt.str("");
            messages.push_back("Tips > Open me !");
            infodialogs.push_back(new Dialog(gfx, "multi", "Surprise gift", messages, items[i + (pageindex * 9)]->x, items[i + (pageindex * 9)]->y));
        }
        else if (items[i + (pageindex * 9)]->type == "hat" || items[i + (pageindex * 9)]->type == "dress" || items[i + (pageindex * 9)]->type == "wand")
        {
            messages.clear();
            txt << "Level > " << items[i + (pageindex * 9)]->level;
            messages.push_back(txt.str());
            txt.str("");
            txt << "Price > " << items[i + (pageindex * 9)]->price << "$";
            messages.push_back(txt.str());
            txt.str("");

            if (items[i + (pageindex * 9)]->type == "hat")
                txt << "Bonus (Mana) > +" << items[i + (pageindex * 9)]->bonus;
            else if (items[i + (pageindex * 9)]->type == "dress")
                txt << "Bonus (Life) > +" << items[i + (pageindex * 9)]->bonus;
            else if (items[i + (pageindex * 9)]->type == "wand" && items[i + (pageindex * 9)]->bonus > 0)
                txt << "Bonus (Heal) > +" << items[i + (pageindex * 9)]->bonus;
            else if (items[i + (pageindex * 9)]->type == "wand" && items[i + (pageindex * 9)]->bonus < 0)
                txt << "Bonus (Damage) > " << -items[i + (pageindex * 9)]->bonus;
            else if (items[i + (pageindex * 9)]->type == "wand" && items[i + (pageindex * 9)]->bonus == 0)
                txt << "Bonus > +" << items[i + (pageindex * 9)]->bonus;

            messages.push_back(txt.str());
            txt.str("");

            if (items[i + (pageindex * 9)]->incrementation)
            {
                txt << items[i + (pageindex * 9)]->name << " + " << items[i + (pageindex * 9)]->incrementation;
                infodialogs.push_back(new Dialog(gfx, "multi", txt.str(), messages, items[i + (pageindex * 9)]->x, items[i + (pageindex * 9)]->y));
                txt.str("");
            }
            else
                infodialogs.push_back(new Dialog(gfx, "multi", items[i + (pageindex * 9)]->name, messages, items[i + (pageindex * 9)]->x, items[i + (pageindex * 9)]->y));
        }
    }
}
