#include "password-dialog.hpp"

#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QVBoxLayout>

#include "make-widget.hpp"

void showPasswordDialog(
    const QString &ssid,
    std::function<void(const QString &, const QString &)> onSubmit) {

  auto *dialog = new QDialog();
  dialog->setAttribute(Qt::WA_DeleteOnClose);
  QLineEdit *edit{};

  dialog->setLayout(makeLayout<QVBoxLayout>({
      // password input
      make([&](QLineEdit *w) { edit = w; }),

      // ok & cancel buttons
      make(
          [&](QDialogButtonBox *w) {
            QObject::connect(
                w, &QDialogButtonBox::accepted,
                [dialog, edit, onSubmit = std::move(onSubmit), ssid]() {
                  onSubmit(ssid, edit->text());
                  dialog->close();
                });
          },
          QDialogButtonBox::Ok /*| QDialogButtonBox::Cancel*/),
  }));

  dialog->show();
}
