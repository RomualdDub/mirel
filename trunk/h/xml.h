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
    int infolevel;
    int infoexp;
    int infomoney;
    int infoherox;
    int infopetx;
    int infocurrentmagic;
    int infoworld;
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
    vector<Teleport> worldteleport;

    private:
    Gfx* gfx;
};

Xml::Xml(Gfx* ingfx)
{
    gfx = ingfx;
}

Xml::~Xml()
{
    worldteleport.clear();
}

void Xml::readinfos()
{
    TiXmlDocument xml("xml/infos.xml");
    if (!xml.LoadFile())
    {
        // set to default

        infolevel = 1;
        infoexp = 0;
        infomoney = 0;
        infoherox = 200;
        infopetx = 200 - 128;
        infocurrentmagic = gfx->magic_spell0;
        infoworld = 1;
    }
    else
    {
        // read xml attributes

        TiXmlHandle hxml(&xml);
        TiXmlElement* exml = hxml.FirstChildElement().FirstChildElement().Element();

        exml->QueryIntAttribute("value", &infolevel);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("value", &infoexp);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("value", &infomoney);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("value", &infoherox);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("value", &infopetx);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("value", &infocurrentmagic);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("value", &infoworld);
    }
}

void Xml::writeinfos()
{
    TiXmlDocument xml("xml/infos.xml");
    if (!xml.LoadFile())
    {
        // create default xml

        TiXmlDeclaration* dxml = new TiXmlDeclaration("1.0", "", "");
        xml.LinkEndChild(dxml);

        TiXmlElement* exml = new TiXmlElement("infos");
        xml.LinkEndChild(exml);

        TiXmlElement* elevel = new TiXmlElement("info");
        exml->LinkEndChild(elevel);
        elevel->SetAttribute("name", "level");
        elevel->SetAttribute("value", infolevel);

        TiXmlElement* eexp = new TiXmlElement("info");
        exml->LinkEndChild(eexp);
        eexp->SetAttribute("name", "exp");
        eexp->SetAttribute("value", infoexp);

        TiXmlElement* emoney = new TiXmlElement("info");
        exml->LinkEndChild(emoney);
        emoney->SetAttribute("name", "money");
        emoney->SetAttribute("value", infomoney);

        TiXmlElement* eherox = new TiXmlElement("info");
        exml->LinkEndChild(eherox);
        eherox->SetAttribute("name", "herox");
        eherox->SetAttribute("value", infoherox);

        TiXmlElement* epetx = new TiXmlElement("info");
        exml->LinkEndChild(epetx);
        epetx->SetAttribute("name", "petx");
        epetx->SetAttribute("value", infopetx);

        TiXmlElement* ecmagic = new TiXmlElement("info");
        exml->LinkEndChild(ecmagic);
        ecmagic->SetAttribute("name", "currentmagic");
        ecmagic->SetAttribute("value", infocurrentmagic);

        TiXmlElement* eworld = new TiXmlElement("info");
        exml->LinkEndChild(eworld);
        eworld->SetAttribute("name", "worldindex");
        eworld->SetAttribute("value", infoworld);
    }
    else
    {
        // write xml attributes

        TiXmlHandle hxml(&xml);
        TiXmlElement* exml = hxml.FirstChildElement().FirstChildElement().Element();

        exml->SetAttribute("value", infolevel);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("value", infoexp);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("value", infomoney);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("value", infoherox);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("value", infopetx);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("value", infocurrentmagic);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("value", infoworld);
    }

    xml.SaveFile("xml/infos.xml");
}

void Xml::readkeys()
{
    TiXmlDocument xml("xml/keys.xml");
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

        exml->QueryIntAttribute("value", &keyleft);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("value", &keyright);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("value", &keyjump);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("value", &keyaction);
        exml = exml->NextSiblingElement();
        exml->QueryIntAttribute("value", &keypet);
    }
}

void Xml::writekeys()
{
    TiXmlDocument xml("xml/keys.xml");
    if (!xml.LoadFile())
    {
        // create default xml

        TiXmlDeclaration* dxml = new TiXmlDeclaration("1.0", "", "");
        xml.LinkEndChild(dxml);

        TiXmlElement* exml = new TiXmlElement("keys");
        xml.LinkEndChild(exml);

        TiXmlElement* eleft = new TiXmlElement("key");
        exml->LinkEndChild(eleft);
        eleft->SetAttribute("name", "left");
        eleft->SetAttribute("value", keyleft);

        TiXmlElement* eright = new TiXmlElement("key");
        exml->LinkEndChild(eright);
        eright->SetAttribute("name", "right");
        eright->SetAttribute("value", keyright);

        TiXmlElement* ejump = new TiXmlElement("key");
        exml->LinkEndChild(ejump);
        ejump->SetAttribute("name", "jump");
        ejump->SetAttribute("value", keyjump);

        TiXmlElement* eaction = new TiXmlElement("key");
        exml->LinkEndChild(eaction);
        eaction->SetAttribute("name", "action");
        eaction->SetAttribute("value", keyaction);

        TiXmlElement* epet = new TiXmlElement("key");
        exml->LinkEndChild(epet);
        epet->SetAttribute("name", "pet");
        epet->SetAttribute("value", keypet);
    }
    else
    {
        // write xml attributes

        TiXmlHandle hxml(&xml);
        TiXmlElement* exml = hxml.FirstChildElement().FirstChildElement().Element();

        exml->SetAttribute("value", keyleft);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("value", keyright);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("value", keyjump);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("value", keyaction);
        exml = exml->NextSiblingElement();
        exml->SetAttribute("value", keypet);
    }

    xml.SaveFile("xml/keys.xml");
}

void Xml::readworlds(int worldindex)
{
    std::ostringstream txt;
    int tindex;
    worldteleport.clear();

    TiXmlDocument xml("xml/worlds.xml");
    xml.LoadFile();

    TiXmlHandle hxml(&xml);
    txt << "worlds" << worldindex - 1;
    TiXmlElement* exml = hxml.FirstChildElement(txt.str()).FirstChildElement().Element();
    txt.str("");

    exml->QueryIntAttribute("value", &worldsplit);
    exml = exml->NextSiblingElement();
    exml->QueryIntAttribute("value", &worldmobindex);
    exml->QueryIntAttribute("posx", &worldmobposx);
    exml = exml->NextSiblingElement();
    exml->QueryIntAttribute("value", &worldbossindex);
    exml->QueryIntAttribute("posx", &worldbossposx);
    exml = exml->NextSiblingElement();
    exml->QueryIntAttribute("value", &tindex);

    // teleports

    char ttxt[100];
    for (int i = 0; i < tindex; i++)
    {
        worldteleport.push_back(Teleport(gfx));
        snprintf(ttxt, 99, "pos%i", i);
        exml->QueryIntAttribute(ttxt, &worldteleport[i].x);
        snprintf(ttxt, 99, "worlddest%i", i);
        exml->QueryIntAttribute(ttxt, &worldteleport[i].worlddest);
        snprintf(ttxt, 99, "worldxdest%i", i);
        exml->QueryIntAttribute(ttxt, &worldteleport[i].worldxdest);
    }

    // mobs

    TiXmlDocument mxml("xml/mobs.xml");
    mxml.LoadFile();

    TiXmlHandle mhxml(&mxml);
    txt << "mobs" << worldmobindex - 1;
    TiXmlElement* mexml = mhxml.FirstChildElement(txt.str()).FirstChildElement().Element();
    txt.str("");

    mexml->QueryStringAttribute("value", &mobname);
    mexml = mexml->NextSiblingElement();
    mexml->QueryIntAttribute("value", &moblevel);

    // bosses

    TiXmlDocument bxml("xml/bosses.xml");
    bxml.LoadFile();

    TiXmlHandle bhxml(&bxml);
    txt << "bosses" << worldbossindex - 1;
    TiXmlElement* bexml = bhxml.FirstChildElement(txt.str()).FirstChildElement().Element();
    txt.str("");

    bexml->QueryStringAttribute("value", &bossname);
    bexml = bexml->NextSiblingElement();
    bexml->QueryIntAttribute("value", &bosslevel);
}
