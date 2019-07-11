#pragma once

#include <QString>
#include <functional>

// onSubmit arguments are (ssid, password)
void showPasswordDialog(
    const QString &ssid,
    std::function<void(const QString &, const QString &)> onSubmit);
