/*
Copyright (c) 2008-2014 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center

    last modification: $LastChangedDate$  (revision $Rev$)

    This file is part of QuickFit 3 (http://www.dkfz.de/Macromol/quickfit).

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "qffccsfilesetsmodel.h"
#include "qffccsfitevaluation_item.h"

QFFCCSFileSetsModel::QFFCCSFileSetsModel(QFFCCSFitEvaluationItem *parent) :
    QAbstractListModel(parent)
{
    FCCSfit=parent;
    qRegisterMetaType<QList<QFRawDataRecord*> >("QList<QFRawDataRecord*>");
    rebuildModel();
}

QList<QFRawDataRecord *> QFFCCSFileSetsModel::getFileSetForIndex(const QModelIndex &idx)
{
    return getFileSetForRow(idx.row());
}

QList<QFRawDataRecord *> QFFCCSFileSetsModel::getFileSetForRow(int row) const
{
    if (row>=0) {
        if (row<FCCSfit->getFittedFiles().size()) {
            return FCCSfit->getFittedFiles().at(row);
        } else if (row-FCCSfit->getFittedFiles().size()<FCCSfit->getGuessedFiles().size()){
            return FCCSfit->getGuessedFiles().at(row-FCCSfit->getFittedFiles().size());
        }
    }
    return QList<QFRawDataRecord *>();
}

int QFFCCSFileSetsModel::rowCount(const QModelIndex &parent) const
{
    return fittedSets+guessedSets;
}

Qt::ItemFlags QFFCCSFileSetsModel::flags(const QModelIndex &/*index*/) const
{
    return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
}

QVariant QFFCCSFileSetsModel::data(const QModelIndex &index, int role) const
{
    if (role==Qt::DisplayRole) {
        return names.value(index.row(), tr("???"));
    } else if (role==Qt::TextColorRole) {
        if (index.row()<fittedSets) return QColor("darkblue");
        else return QColor("darkgrey");
    } else if (role==Qt::ToolTipRole) {
        if (index.row()<fittedSets) return tr("there are fit results for this set of files");
        else return tr("this set of files was guessed depending on your last fit");
    }
    return QVariant();
}

void QFFCCSFileSetsModel::rebuildModel()
{
    names.clear();
    if (FCCSfit) {
        fittedSets=FCCSfit->getFittedFiles().size();
        guessedSets=FCCSfit->getGuessedFiles().size();
        for (int r=0; r<fittedSets+guessedSets; r++) {
            QList<QFRawDataRecord *> set=getFileSetForRow(r);
            if (set.size()>0) {
                QString name="";
                QString groupName=set[0]->getGroupName();
                bool sameGroup=true;
                QString folderName=set[0]->getFolder();
                bool sameFolder=true;
                QString roles="";
                QString n="";
                for (int i=0; i<set.size(); i++) {
                    if (set[i]) {
                        if (set[i]->getGroupName()!=groupName) sameGroup=false;
                        if (set[i]->getFolder()!=folderName) sameFolder=false;
                        if (roles.size()>0) roles+="+";
                        roles+=set[i]->getRole();
                        if (n.size()>0) n+="+";
                        n+=set[i]->getName();
                    }
                }

                if (sameGroup && !groupName.isEmpty()) name=groupName;
                else if (sameFolder && !folderName.isEmpty()) name=folderName;
                else name=n;

                if (!roles.isEmpty()) name+=QString(" - ")+roles;

                if (name.isEmpty()) name=tr("???");
                names<<name;
            } else {
                names<<tr("???");
            }
        }
    } else {
        fittedSets=0;
        guessedSets=0;
    }
    reset();
}

