//!
//! \file xmmlreader.h
//! \author Team-3
//! \date 04-Februar -2018

//! \brief Klasse, in der die xml datei gelesen wird
//!
//!
#ifndef XMLREADER_H
#define XMLREADER_H

#include <QString>
#include <QList>

#define UNKNOWN_STR "UNKNOWN"   //!< Used when the actual value is still unknown

//!\brief Struct which holds information about a logistic order.
//!
//! The struct stores the information in which storage rack a
//! specific article can be found and to which warehouse Lagerist
//! it has to be delivered.
struct Order
{
    //! \brief Enum which defines the possible rack colors
    enum StorageRack
    {
        UNKNOWN_RACK = -1,
        RACK_RED,
        RACK_GREEN,
        RACK_YELLOW
    };

    //! \brief Enum to distinguish warehouse Lagerists
    enum WarehouseClerk
    {
        UNKNOWN_Lagerist = -1,
        Lagerist_A,
        Lagerist_B
    };

    QString id = UNKNOWN_STR;               //!< ID of this order
    QString description = UNKNOWN_STR;      //!< human readable description of the article
    StorageRack storageRack = UNKNOWN_RACK; //!< storage rack with the article
    WarehouseClerk Lagerist = UNKNOWN_Lagerist;   //!< warehouse Lagerist which handles the article





    //! \brief Converts QString to the corresponding StorageRack
    //! \param [in] str QString which corresponds to a StorageRack
    //! \return Corresponding enum. StorageRack::UNKNOWN_RACK if
    //!         no correspondence is found.
    static StorageRack storageRackFromStr(const QString& str);

    //! \brief Converts QString to the corresponding WarehouseClerk
    //! \param [in] str QString which corresponds to a WarehouseClerk
    //! \return Corresponding enum. WarehouseClerk::UNKNOWN_Lagerist if
    //!         no correspondence is found.
    static WarehouseClerk LageristFromString(const QString& str);
};

//!\brief Struct which holds a list of logistic orders
struct OrderList
{
    QString id = UNKNOWN_STR;           //!< ID of this order
    QString description = UNKNOWN_STR;  //!< human readable description of the article
    QList<Order> orders;                //!< list with all orders


};



//! \brief Helper class to parse xml file which holds information about logistic orders
class xmlReader
{
private:
    //!
    //! \brief xmlReader constructor
    //!
    xmlReader();

    //!
    //! \brief xmlReader makes a deep copy of xmlreader
    //! \param obj
    //!

    xmlReader (const xmlReader &obj);

public:
    //! \brief Parse xml file defined by a file path
    //!
    //! If an error occures during the parsing (e.g. the file does not exist),
    //! a struct filled with default values is returned.
    //! \param [in] path to xml file
    //! \return Struct with the parsed order informations. Filled with default
    //!         values if an error occured during parsing.
    static OrderList parseFile(const QString& xmlFileName);
};



#endif // XMLREADER_H
