#ifndef QFTREEVIEWCOMBOBOX_H
#define QFTREEVIEWCOMBOBOX_H

#include <QComboBox>
#include <QTreeView>
#include <QWidget>
#include "lib_imexport.h"

/*! \brief a QComboBox with a treeview as dropdown widget
    \ingroup tools_qt

    see <a href="http://www.qtcentre.org/threads/46078-Snippet-QComboBox-showing-tree-models">http://www.qtcentre.org/threads/46078-Snippet-QComboBox-showing-tree-models</a>

 */
class QFLIB_EXPORT QFTreeViewComboBox : public QComboBox {
        Q_OBJECT
    public:
        explicit QFTreeViewComboBox(QWidget *parent = 0);
    virtual void showPopup();

    public slots:
        void selectIndex(const QModelIndex&index);

    private:
        QTreeView* _treeView;
};

#endif // QFTREEVIEWCOMBOBOX_H