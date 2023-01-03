// <one line to give the program's name and a brief idea of what it does.>
// SPDX-FileCopyrightText: 2023 Aleks Rutins <email>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "test.h"

using namespace DiagonalUI;

Test::Test ( const Test& other )
    : m_success ( other.success() )
    , m_hasRun ( other.hasRun() )
    , m_isRunning ( other.isRunning() )
    , m_name ( other.name() )
{

}

Test::Test(QString name)
    : m_success(false)
    , m_hasRun(false)
    , m_isRunning(false)
    , m_name(name) {}

Test::Test()
    : Test("") {}

Test& Test::operator= ( const Test& other )
{
    setName(other.name());
    setHasRun(other.hasRun());
    setIsRunning(other.isRunning());
    setSuccess(other.success());
    return *this;
}

void Test::start() {
    setIsRunning(true);
}

void Test::statusReport(bool success) {
    setIsRunning(false);
    setHasRun(true);
    setSuccess(success);
}

bool Test::success() const
{
    return m_success;
}

void Test::setSuccess ( bool success )
{
    if ( m_success == success ) {
        return;
    }

    m_success = success;
    emit successChanged ( m_success );
}

bool Test::hasRun() const
{
    return m_hasRun;
}

void Test::setHasRun ( bool hasRun )
{
    if ( m_hasRun == hasRun ) {
        return;
    }

    m_hasRun = hasRun;
    emit hasRunChanged ( m_hasRun );
}

bool Test::isRunning() const
{
    return m_isRunning;
}

void Test::setIsRunning ( bool isRunning )
{
    if ( m_isRunning == isRunning ) {
        return;
    }

    m_isRunning = isRunning;
    emit isRunningChanged ( m_isRunning );
}

QString Test::name() const
{
    return m_name;
}

void Test::setName ( const QString& name )
{
    if ( m_name == name ) {
        return;
    }

    m_name = name;
    emit nameChanged ( m_name );
}
