// <one line to give the program's name and a brief idea of what it does.>
// SPDX-FileCopyrightText: 2023 Aleks Rutins <email>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "testlistmodel.h"

TestListModel::TestListModel()
{
}

QVariant TestListModel::data ( const QModelIndex& index, int role ) const
{
    if(!index.isValid()) return QVariant();

    if(index.row() >= m_tests.size()) return QVariant();

    if(role == Qt::DisplayRole)
        return QVariant::fromValue(m_tests.at(index.row()));
    else
        return QVariant();
}

int TestListModel::rowCount ( const QModelIndex& parent ) const
{
    return m_tests.size();
}

QList<DiagonalUI::Test> TestListModel::tests() const
{
    return m_tests;
}

void TestListModel::setTests ( QList<DiagonalUI::Test> tests )
{
    if ( m_tests == tests ) {
        return;
    }

    m_tests = tests;
    emit testsChanged ( m_tests );
}
