#ifndef HELPFULLFUNCSTOCONTAINTERS
#define HELPFULLFUNCSTOCONTAINTERS

#include <QVariantList>

template <typename T>
QList<T> variantListToTypeList(const QVariantList &list)
{
    QList<T> listType;
    foreach (const QVariant &element, list) {
        listType.push_back(qvariant_cast<T>(element));
    }
    return listType;
}

template <typename T>
QVariantList TypeListToVariantList(const QList<T> &list)
{
    QVariantList listType;
    foreach (const T &element, list) {
        listType.push_back(qvariant_cast<QVariant>(element));
    }
    return listType;
}


#endif // HELPFULLFUNCSTOCONTAINTERS

