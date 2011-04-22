class Xml
{
    public:
    Xml(Gfx* ingfx);
    ~Xml();
    void readinfos();
    void writeinfos();
    void readkeys();
    void writekeys();
    void readworlds(int worldindex);
    Item* readitem(string type, int index);
    void readstuff();
    void readinventory(vector<Item*>* items);
    void writeinventory(vector<Item*> items);
    int infolevel;
    int infoexp;
    int infomoney;
    int infoherox;
    int infopetx;
    int infomagicindex;
    int infoworldindex;
    int infohatindex;
    int infohatincrementation;
    int infodressindex;
    int infodressincrementation;
    int infowandindex;
    int infowandincrementation;
    int keyleft;
    int keyright;
    int keyjump;
    int keyaction;
    int keypet;
    int worldsplit;
    int worldmobindex;
    string mobname;
    int moblevel;
    int worldmobposx;
    int worldbossindex;
    string bossname;
    int bosslevel;
    int worldbossposx;
    vector<Teleport> worldteleports;
    vector<Item*> currentitems;

    private:
    Gfx* gfx;
};

Xml::Xml(Gfx* ingfx)
{
    gfx = ingfx;
}

Xml::~Xml()
{
    worldteleports.clear();
    currentitems.clear();
}

void Xml::readinfos()
{
    TiXmlDocument xml("xml/_infos.xml");
    if (!xml.LoadFile())
    {
        // set to default

        infolevel = 1;
        infoexp = 0;
        infomoney = 0;
        infoherox = 200;
        infopetx = 200 - 128;
        infomagicindex = gfx->magic_spell0;
        infoworldindex = 1;
        infohatindex = 1;
        infohatincrementation = 0;
        infodressindex = 1;
        infodressincrementation = 0;
        infowandindex = 1;
        infowandincrementation = 0;
    }
    else
    {
        // read xml attributes

        TiXmlHandle hxml(&xml);
        TiXmlElement* exml = hxml.FirstChildElement().FirstChildElement().Element();

        exml->QueryIntAttribute("level", &infolevel);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("exp", &infoexp);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("money", &infomoney);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("herox", &infoherox);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("petx", &infopetx);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("magicindex", &infomagicindex);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("worldindex", &infoworldindex);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("hatindex", &infohatindex);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("hatincrementation", &infohatincrementation);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("dressindex", &infodressindex);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("dressincrementation", &infodressincrementation);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("wandindex", &infowandindex);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("wandincrementation", &infowandincrementation);
    }
}

void Xml::writeinfos()
{
    TiXmlDocument xml("xml/_infos.xml");
    if (!xml.LoadFile())
    {
        // create default xml

        TiXmlDeclaration* dxml = new TiXmlDeclaration("1.0", "", "");
        xml.LinkEndChild(dxml);

        TiXmlElement* exml = new TiXmlElement("infos");
        xml.LinkEndChild(exml);

        TiXmlElement* elevel = new TiXmlElement("info");
        exml->LinkEndChild(elevel);
        elevel->SetAttribute("level", infolevel);

        TiXmlElement* eexp = new TiXmlElement("info");
        exml->LinkEndChild(eexp);
        eexp->SetAttribute("exp", infoexp);

        TiXmlElement* emoney = new TiXmlElement("info");
        exml->LinkEndChild(emoney);
        emoney->SetAttribute("money", infomoney);

        TiXmlElement* eherox = new TiXmlElement("info");
        exml->LinkEndChild(eherox);
        eherox->SetAttribute("herox", infoherox);

        TiXmlElement* epetx = new TiXmlElement("info");
        exml->LinkEndChild(epetx);
        epetx->SetAttribute("petx", infopetx);

        TiXmlElement* ecmagic = new TiXmlElement("info");
        exml->LinkEndChild(ecmagic);
        ecmagic->SetAttribute("magicindex", infomagicindex);

        TiXmlElement* eworld = new TiXmlElement("info");
        exml->LinkEndChild(eworld);
        eworld->SetAttribute("worldindex", infoworldindex);

        TiXmlElement* ehat = new TiXmlElement("info");
        exml->LinkEndChild(ehat);
        ehat->SetAttribute("hatindex", infohatindex);

        TiXmlElement* ehati = new TiXmlElement("info");
        exml->LinkEndChild(ehati);
        ehati->SetAttribute("hatincrementation", infohatincrementation);

        TiXmlElement* edress = new TiXmlElement("info");
        exml->LinkEndChild(edress);
        edress->SetAttribute("dressindex", infodressindex);

        TiXmlElement* edressi = new TiXmlElement("info");
        exml->LinkEndChild(edressi);
        edressi->SetAttribute("dressincrementation", infodressincrementation);

        TiXmlElement* ewand = new TiXmlElement("info");
        exml->LinkEndChild(ewand);
        ewand->SetAttribute("wandindex", infowandindex);

        TiXmlElement* ewandi = new TiXmlElement("info");
        exml->LinkEndChild(ewandi);
        ewandi->SetAttribute("wandincrementation", infowandincrementation);
    }
    else
    {
        // write xml attributes

        TiXmlHandle hxml(&xml);
        TiXmlElement* exml = hxml.FirstChildElement().FirstChildElement().Element();

        exml->SetAttribute("level", infolevel);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("exp", infoexp);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("money", infomoney);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("herox", infoherox);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("petx", infopetx);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("magicindex", infomagicindex);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("worldindex", infoworldindex);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("hatindex", infohatindex);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("hatincrementation", infohatincrementation);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("dressindex", infodressindex);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("dressincrementation", infodressincrementation);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("wandindex", infowandindex);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("wandincrementation", infowandincrementation);
    }

    xml.SaveFile("xml/_infos.xml");
}

void Xml::readkeys()
{
    TiXmlDocument xml("xml/_keys.xml");
    if (!xml.LoadFile())
    {
        // set to default (AZERTY keyboard configuration)

        keyleft = sf::Key::Q;
        keyright = sf::Key::D;
        keyjump = sf::Key::Z;
        keyaction = sf::Key::Space;
        keypet = sf::Key::A;
    }
    else
    {
        // read xml attributes

        TiXmlHandle hxml(&xml);
        TiXmlElement* exml = hxml.FirstChildElement().FirstChildElement().Element();

        exml->QueryIntAttribute("left", &keyleft);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("right", &keyright);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("jump", &keyjump);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("action", &keyaction);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("pet", &keypet);
    }
}

void Xml::writekeys()
{
    TiXmlDocument xml("xml/_keys.xml");
    if (!xml.LoadFile())
    {
        // create default xml

        TiXmlDeclaration* dxml = new TiXmlDeclaration("1.0", "", "");
        xml.LinkEndChild(dxml);

        TiXmlElement* exml = new TiXmlElement("keys");
        xml.LinkEndChild(exml);

        TiXmlElement* eleft = new TiXmlElement("key");
        exml->LinkEndChild(eleft);
        eleft->SetAttribute("left", keyleft);

        TiXmlElement* eright = new TiXmlElement("key");
        exml->LinkEndChild(eright);
        eright->SetAttribute("right", keyright);

        TiXmlElement* ejump = new TiXmlElement("key");
        exml->LinkEndChild(ejump);
        ejump->SetAttribute("jump", keyjump);

        TiXmlElement* eaction = new TiXmlElement("key");
        exml->LinkEndChild(eaction);
        eaction->SetAttribute("action", keyaction);

        TiXmlElement* epet = new TiXmlElement("key");
        exml->LinkEndChild(epet);
        epet->SetAttribute("pet", keypet);
    }
    else
    {
        // write xml attributes

        TiXmlHandle hxml(&xml);
        TiXmlElement* exml = hxml.FirstChildElement().FirstChildElement().Element();

        exml->SetAttribute("left", keyleft);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("right", keyright);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("jump", keyjump);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("action", keyaction);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("pet", keypet);
    }

    xml.SaveFile("xml/_keys.xml");
}

void Xml::readworlds(int worldindex)
{
    std::ostringstream txt;
    int tindex;
    worldteleports.clear();

    TiXmlDocument xml("xml/worlds.xml");
    xml.LoadFile();

    TiXmlHandle hxml(&xml);
    txt << "worlds" << worldindex - 1;
    TiXmlElement* exml = hxml.FirstChildElement(txt.str()).FirstChildElement().Element();
    txt.str("");

    exml->QueryIntAttribute("split", &worldsplit);
    exml = exml->NextSiblingElement();
    exml->QueryIntAttribute("mob", &worldmobindex);
    exml->QueryIntAttribute("posx", &worldmobposx);
    exml = exml->NextSiblingElement();
    exml->QueryIntAttribute("boss", &worldbossindex);
    exml->QueryIntAttribute("posx", &worldbossposx);
    exml = exml->NextSiblingElement();
    exml->QueryIntAttribute("teleport", &tindex);

    // teleports

    for (int i = 0; i < tindex; i++)
    {
        worldteleports.push_back(Teleport(gfx));
        txt << "pos" << i;
        exml->QueryIntAttribute(txt.str(), &worldteleports[i].x);
        txt.str("");
        txt << "worlddest" << i;
        exml->QueryIntAttribute(txt.str(), &worldteleports[i].worlddest);
        txt.str("");
        txt << "worldxdest" << i;
        exml->QueryIntAttribute(txt.str(), &worldteleports[i].worldxdest);
        txt.str("");
    }

    // mobs

    TiXmlDocument mxml("xml/mobs.xml");
    mxml.LoadFile();

    TiXmlHandle mhxml(&mxml);
    txt << "mobs" << worldmobindex - 1;
    TiXmlElement* mexml = mhxml.FirstChildElement(txt.str()).FirstChildElement().Element();
    txt.str("");

    mexml->QueryStringAttribute("name", &mobname);
    mexml = mexml->NextSiblingElement();
    mexml->QueryIntAttribute("level", &moblevel);

    // bosses

    TiXmlDocument bxml("xml/bosses.xml");
    bxml.LoadFile();

    TiXmlHandle bhxml(&bxml);
    txt << "bosses" << worldbossindex - 1;
    TiXmlElement* bexml = bhxml.FirstChildElement(txt.str()).FirstChildElement().Element();
    txt.str("");

    bexml->QueryStringAttribute("name", &bossname);
    bexml = bexml->NextSiblingElement();
    bexml->QueryIntAttribute("level", &bosslevel);
}

Item* Xml::readitem(string type, int index)
{
    std::ostringstream txt;
    Item* item;

    if (type == "gift")
    {
        item = new Item(gfx, type, index);
        item->name = "Suprise Gift";
    }
    else if (type == "hat" || type == "dress" || type == "wand")
    {
        TiXmlDocument xml("xml/items.xml");
        xml.LoadFile();

        TiXmlHandle hxml(&xml);
        txt << type << index - 1;
        TiXmlElement* exml = hxml.FirstChildElement(txt.str()).FirstChildElement().Element();
        txt.str("");

        item = new Item(gfx, type, index);
        exml->QueryStringAttribute("name", &item->name);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("level", &item->level);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("bonus", &item->bonus);
    }

    return item;
}

void Xml::readstuff()
{
    currentitems.clear();

    currentitems.push_back(readitem("hat", infohatindex));
    currentitems[0]->setstats(infohatincrementation, currentitems[0]->bonus);
    currentitems.push_back(readitem("dress", infodressindex));
    currentitems[1]->setstats(infodressincrementation, currentitems[1]->bonus);
    currentitems.push_back(readitem("wand", infowandindex));
    currentitems[2]->setstats(infowandincrementation, currentitems[2]->bonus);
}

void Xml::readinventory(vector<Item*>* items)
{
    std::ostringstream txt;
    int itemcount;
    string itemtype;
    int itemindex;
    int itemlevel;
    int itemincrementation;
    int itembonus;

    TiXmlDocument xml("xml/_inventory.xml");
    if (xml.LoadFile())
    {
        TiXmlHandle hxml(&xml);
        TiXmlElement* exml = hxml.FirstChildElement("count").Element();
        exml->QueryIntAttribute("value", &itemcount);

        for (int i = 0; i < itemcount; i++)
        {
            txt << "item" << i;
            exml = hxml.FirstChildElement(txt.str()).Element();
            txt.str("");
            exml->QueryStringAttribute("type", &itemtype);
            exml->QueryIntAttribute("index", &itemindex);
            items->push_back(readitem(itemtype, itemindex));

            if (itemtype == "gift")
            {
                exml->QueryIntAttribute("level", &itemlevel); // level
                itemincrementation = 0; // incrementation
                itembonus = 0; // bonus
            }
            else if (itemtype == "hat" || itemtype == "dress" || itemtype == "wand")
            {
                itemlevel = (*items)[(int)items->size() - 1]->level; // level
                exml->QueryIntAttribute("incrementation", &itemincrementation); // incrementation
                itembonus = (*items)[(int)items->size() - 1]->bonus; // bonus
            }
            (*items)[(int)items->size() - 1]->setstats(itemincrementation, itembonus); // incrementation + bonus
            (*items)[(int)items->size() - 1]->setprice(itemlevel); // price + level
        }
    }
}

void Xml::writeinventory(vector<Item*> items)
{
    std::ostringstream txt;
    TiXmlDocument xml("xml/_inventory.xml");

    // create default xml

    TiXmlDeclaration* dxml = new TiXmlDeclaration("1.0", "", "");
    xml.LinkEndChild(dxml);

    TiXmlElement* exml = new TiXmlElement("count");
    xml.LinkEndChild(exml);
    exml->SetAttribute("value", (int)items.size());

    for (int i = 0; i < (int)items.size(); i++)
    {
        txt << "item" << i;
        TiXmlElement* exml = new TiXmlElement(txt.str());
        txt.str("");
        xml.LinkEndChild(exml);
        exml->SetAttribute("type", items[i]->type);
        exml->SetAttribute("index", items[i]->index);

        if (items[i]->type == "gift")
            exml->SetAttribute("level", items[i]->level);
        else if (items[i]->type == "hat" || items[i]->type == "dress" || items[i]->type == "wand")
            exml->SetAttribute("incrementation", items[i]->incrementation);
    }
    xml.SaveFile("xml/_inventory.xml");
}
