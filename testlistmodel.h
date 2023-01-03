// <one line to give the program's name and a brief idea of what it does.>
// SPDX-FileCopyrightText: 2023 Aleks Rutins <email>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef TESTLISTMODEL_H
#define TESTLISTMODEL_H

#include "test.h"
#include <qabstractitemmodel.h>

/**
 * @todo write docs
 */
class TestListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY ( QList<DiagonalUI::Test> tests READ tests WRITE setTests NOTIFY testsChanged )

public:
    /**
     * Default constructor
     */
    TestListModel();

    /**
     * @todo write docs
     *
     * @param index TODO
     * @param role TODO
     * @return TODO
     */
    virtual QVariant data ( const QModelIndex& index, int role ) const = 0;

    /**
     * @todo write docs
     *
     * @param parent TODO
     * @return TODO
     */
    virtual int rowCount ( const QModelIndex& parent ) const = 0;

    /**
     * @return the tests
     */
    QList<DiagonalUI::Test> tests() const;

public Q_SLOTS:
    /**
     * Sets the tests.
     *
     * @param tests the new tests
     */
    void setTests ( QList<DiagonalUI::Test> tests );

Q_SIGNALS:
    void testsChanged ( QList<DiagonalUI::Test> tests );

private:
    QList<DiagonalUI::Test> m_tests;
};

#endif // TESTLISTMODEL_H
