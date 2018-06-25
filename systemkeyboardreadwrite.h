#ifndef SYSTEMKEYBOARDREADWRITE_H
#define SYSTEMKEYBOARDREADWRITE_H

#pragma once
#include <QObject>
#include <windows.h>

class SystemKeyboardReadWrite : public QObject
{
    Q_OBJECT

public:
    // Returns singleton instance
    static SystemKeyboardReadWrite * instance();

    // Class destructor
    ~SystemKeyboardReadWrite();

    // Returns whether the keyboard hook is connected
    bool connected();

    // Connects / Disconnects the keyboard hook
    bool setConnected(bool state);

signals:
    // Broadcasts a key has been pressed
    void keyPressed(byte *keysDepressed, DWORD keyPressed);

public slots:

private:
    // Number of keys supported
    static const int numberKeys = 256;

    // Pointer to singleton instance
    static SystemKeyboardReadWrite *uniqueInstance;

    // Keyboard hook
    HHOOK keyboardHook;

    // Class constructor
    SystemKeyboardReadWrite();

    // Identifies hook activity
    static LRESULT CALLBACK keyboardProcedure(int nCode, WPARAM wParam, LPARAM lParam);

protected:

};

#endif
