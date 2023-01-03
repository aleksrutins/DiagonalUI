// <one line to give the program's name and a brief idea of what it does.>
// SPDX-FileCopyrightText: 2023 Aleks Rutins <email>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DIAGONALUI_TEST_H
#define DIAGONALUI_TEST_H

#include <qobject.h>

namespace DiagonalUI
{

/**
 * @todo write docs
 */
class Test : public QObject
{
    Q_OBJECT
    Q_PROPERTY ( bool success READ success WRITE setSuccess NOTIFY successChanged )
    Q_PROPERTY ( bool hasRun READ hasRun WRITE setHasRun NOTIFY hasRunChanged )
    Q_PROPERTY ( bool isRunning READ isRunning WRITE setIsRunning NOTIFY isRunningChanged )
    Q_PROPERTY ( QString name READ name WRITE setName NOTIFY nameChanged )

public:
    /**
     * Copy constructor
     *
     * @param other TODO
     */
    Test ( const Test& other );

    Test ( QString name );

    Test ();

    Test &operator=(const Test &other);

    void start();
    void statusReport(bool success);

    /**
     * @return the success
     */
    bool success() const;

    /**
     * @return the hasRun
     */
    bool hasRun() const;

    /**
     * @return the isRunning
     */
    bool isRunning() const;

    /**
     * @return the name
     */
    QString name() const;

    inline bool showSuccess() const {return hasRun() && success();}
    inline bool showFailure() const {return hasRun() && !success();}
    inline bool showProgress() const {return isRunning();}

    inline bool operator==(Test other) const {return m_name == other.name();}
    inline bool operator==(QString name) const {return m_name == name;}

public Q_SLOTS:
    /**
     * Sets the success.
     *
     * @param success the new success
     */
    void setSuccess ( bool success );

    /**
     * Sets the hasRun.
     *
     * @param hasRun the new hasRun
     */
    void setHasRun ( bool hasRun );

    /**
     * Sets the isRunning.
     *
     * @param isRunning the new isRunning
     */
    void setIsRunning ( bool isRunning );

    /**
     * Sets the name.
     *
     * @param name the new name
     */
    void setName ( const QString& name );

Q_SIGNALS:
    void successChanged ( bool success );

    void hasRunChanged ( bool hasRun );

    void isRunningChanged ( bool isRunning );

    void nameChanged ( const QString& name );

private:
    bool m_success;
    bool m_hasRun;
    bool m_isRunning;
    QString m_name;
};

}

Q_DECLARE_METATYPE(DiagonalUI::Test)
#endif // DIAGONALUI_TEST_H
