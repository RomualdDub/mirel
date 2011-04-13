class Xml
{
    public:
    void readinfos();
    void writeinfos();
    void readkeys();
    void writekeys();
    int infolevel;
    int infoexp;
    int infomoney;
    int infoherox;
    int keyleft;
    int keyright;
    int keyjump;
    int keyaction;
    int keypet;
};

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

