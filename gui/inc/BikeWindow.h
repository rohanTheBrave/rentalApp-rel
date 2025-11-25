#ifndef BIKEWINDOW_H_
#define BIKEWINDOW_H_

#include <QWidget>
#include "Bike.h"

class QPushButton;
class QTextEdit;
class QListWidget;
class QFrame;
class QLabel;
class SpecForm;
class QPixmap;
class QFileDialog;
class QComboBox;

class BikeWindow : public QWidget
{
    Q_OBJECT
public:
    explicit BikeWindow(QWidget* parent = 0);
    QString getBrandPlainText() const;
    QString getModelPlainText() const;

    QListWidget* getBikeList() const { return m_outputList; }
    SpecForm*   getSpecForm() const { return m_specForm; }
    bool checkIfAnySelected() const;

signals:
    void riseWarning(const QString&);
private slots:
    void switchSpec(bool);

private:
    SpecForm    *m_specForm;
    QTextEdit   *m_brandText;
    QTextEdit   *m_modelText;
    QPushButton *m_addButton;
    QPushButton *m_removeButton;
    QPushButton *m_backButton;
    QPushButton *m_specButton;
    QFrame      *m_line;
    QListWidget *m_outputList;
};

class SpecForm : public QWidget
{
    Q_OBJECT
public:
    explicit SpecForm(QWidget* parent = 0);
    const Bike::SpecDesc& getSpecDescription() const { return specDescription; }
    const Bike::Type getBikeType() const { return bikeType; }
signals:
    void sendSpecDescription();
private slots:
    void updateSpecDescription();
    void chooseFile();
private:
    QPushButton *m_applyButton;
    QPushButton *m_backButton;
    QTextEdit   *m_brakeText;
    QTextEdit   *m_driveText;
    QTextEdit   *m_wheelText;
    QTextEdit   *m_saddleText;
    QTextEdit   *m_handlebarText;
    QTextEdit   *m_fsuspText;
    QTextEdit   *m_rsuspText;
    QLabel      *m_brakeLabel;
    QLabel      *m_driveLabel;
    QLabel      *m_wheelLabel;
    QLabel      *m_saddleLabel;
    QLabel      *m_handlebarLabel;
    QLabel      *m_fsuspLabel;
    QLabel      *m_rsuspLabel;
    QPushButton *m_uploadPictureButton;
    QFileDialog *m_openFile;
    QComboBox   *m_bikeTypeComboBox;
    
    Bike::SpecDesc specDescription;
    Bike::Type bikeType;
};

#endif