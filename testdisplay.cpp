// <one line to give the program's name and a brief idea of what it does.>
// SPDX-FileCopyrightText: 2023 Aleks Rutins <email>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "testdisplay.h"

using namespace DiagonalUI;

DiagonalUI::Test TestDisplay::data() const
{
    return m_data;
}

void TestDisplay::setData(const DiagonalUI::Test& data)
{
    if (m_data == data) {
        return;
    }

    m_data = data;
    emit dataChanged(m_data);
    updateDisplay(m_data);
}
