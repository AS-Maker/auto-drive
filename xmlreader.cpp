#include "xmlreader.h"
#include <QtXml>
#include <QDebug>


// ********************************************************
OrderList xmlReader::parseFile(const QString &xmlFileName)
{
    OrderList parsedData;

    // Open XML-File
    QFile xmlFile(xmlFileName); // read xml file
    if(!xmlFile.exists() || !xmlFile.open(QIODevice::ReadOnly))
    {
        qCritical()<<"Unable to open *.xml file: "<<xmlFileName;
        return parsedData;
    }

    // Parsing XML file
    QDomDocument doc;
    doc.setContent(xmlFile.readAll()); // create DOM
    xmlFile.close();

    // get "orderList" attributes
    QDomElement galConfig = doc.firstChildElement("orderList");
    parsedData.id = galConfig.firstChildElement("id").text();
    parsedData.description = galConfig.firstChildElement("description").text();

    // iterate over all "order" elements
    QDomNodeList orders = doc.elementsByTagName("order");
    for (int idx = 0; idx < orders.size(); idx++)
    {
        Order currOrder;
        // get current "order" element
        QDomNode currNode = orders.item(idx);
        currOrder.id = currNode.firstChildElement("id").text();
        currOrder.description = currNode.firstChildElement("description").text();
        QString rackStr = currNode.firstChildElement("storageRack").text();
        currOrder.storageRack = Order::storageRackFromStr(rackStr);
        QString lageristStr = currNode.firstChildElement("warehouseClerk").text();
        currOrder.Lagerist = Order::LageristFromString(lageristStr);

        parsedData.orders.append(currOrder);
    }

    return parsedData;
}

// ********************************************************
Order::StorageRack Order::storageRackFromStr(const QString &str)
{
    StorageRack retVal = StorageRack::UNKNOWN_RACK;
    if(!str.compare("Red")) retVal = StorageRack::RACK_RED;
    else if(!str.compare("Green")) retVal = StorageRack::RACK_GREEN;
    else if(!str.compare("Yellow")) retVal = StorageRack::RACK_YELLOW;
    return retVal;
}

// ********************************************************
Order::WarehouseClerk Order::LageristFromString(const QString &str)
{
    WarehouseClerk retVal = WarehouseClerk::UNKNOWN_Lagerist;
    if(!str.compare("ClerkA")) retVal = WarehouseClerk::Lagerist_A;
    else if(!str.compare("ClerkB")) retVal = WarehouseClerk::Lagerist_B;
    return retVal;
}
