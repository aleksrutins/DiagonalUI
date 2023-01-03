// <one line to give the program's name and a brief idea of what it does.>
// SPDX-FileCopyrightText: 2023 Aleks Rutins <email>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DIAGONALUI_TESTDISPLAY_H
#define DIAGONALUI_TESTDISPLAY_H

#include <QLabel>
#include "test.h"
#include <iostream>
#include <QSizePolicy>

namespace DiagonalUI {

/**
 * @todo write docs
 */
class TestDisplay : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(DiagonalUI::Test data READ data WRITE setData NOTIFY dataChanged)

public:
    TestDisplay() : QLabel() {
        connect(this, SIGNAL(dataChanged(const DiagonalUI::Test&)), this, SLOT(updateDisplay(const DiagonalUI::Test&)));
        auto policy = sizePolicy();
        policy.setVerticalPolicy(QSizePolicy::Maximum);
        setSizePolicy(policy);
    }

    TestDisplay(QString name) : m_data(name) {}
    /**
     * @return the data
     */
    DiagonalUI::Test data() const;

    inline bool operator==(Test test) { return this->m_data == test; }
    inline bool operator==(QString name) { return this->m_data == name; }

public Q_SLOTS:
    /**
     * Sets the data.
     *
     * @param data the new data
     */
    void setData(const DiagonalUI::Test& data);

    void updateDisplay(const DiagonalUI::Test& newData) {
        setText(newData.name());
        auto newColor =
              newData.showProgress()? Qt::black
            : newData.showSuccess()? Qt::green
            : newData.showFailure()? Qt::red
            : Qt::gray;
        QPalette palette = this->palette();
        palette.setColor(foregroundRole(), newColor);
        setPalette(palette);
    }

Q_SIGNALS:
    void dataChanged(const DiagonalUI::Test& data);

private:
    DiagonalUI::Test m_data;
};

}

#endif // DIAGONALUI_TESTDISPLAY_H
