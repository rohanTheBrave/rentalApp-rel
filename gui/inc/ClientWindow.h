#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QWidget>
#include <QString>
#include <vector>

class QPushButton;
class QTextEdit;
class QListWidget;
class QFrame;
class ClientWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ClientWindow(QWidget *parent = 0);

    QString getNamePlainText() const;
    QString getSurnamePlainText() const;

    QListWidget* getClientList() const { return m_outputList; }
    
signals:

private slots:

private:
    QTextEdit   *m_nameText;
    QTextEdit   *m_surnameText;
    QPushButton *m_addButton;
    QPushButton *m_removeButton;
    QPushButton *m_backButton;
    QFrame      *m_line;
    QListWidget *m_outputList;
};

#endif // CLIENTWINDOW_H