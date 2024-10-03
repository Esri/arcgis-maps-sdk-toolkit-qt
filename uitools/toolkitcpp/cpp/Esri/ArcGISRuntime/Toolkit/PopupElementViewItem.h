#ifndef POPUPELEMENTVIEWITEM_H
#define POPUPELEMENTVIEWITEM_H

// Qt headers
#include <QObject>

class PopupElementViewItem : public QObject
{
    Q_OBJECT
public:
    explicit PopupElementViewItem(QObject *parent = nullptr);
};

#endif // POPUPELEMENTVIEWITEM_H
