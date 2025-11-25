#ifndef RENTALWINDOW_H
#define RENTALWINDOW_H

#include <QWidget>
#include "Bike.h"

class QLabel;
class QPushButton;
class QTextEdit;

class RentBikeWindow;
class RentalWindow : public QWidget
{
    Q_OBJECT
public:
    explicit RentalWindow(QWidget* parent);

    void showBike(const Bike&);
    void reset();
    RentBikeWindow* getRentBikeWindow() const { return m_rentBikeWindow; }
    int getShowedBikeId() const { return m_showedBikeId; }
signals:
    void sendReturnBikeId(int);
private slots:
    void showRentBikeGui(bool);
    void returnBike(bool);
private:
    QLabel*         m_typeLabel;
    QLabel*         m_availableLabel;
    QLabel*         m_brandLabel;
    QLabel*         m_modelLabel;
    QLabel*         m_brakeLabel;
    QLabel*         m_driveLabel;
    QLabel*         m_wheelLabel;
    QLabel*         m_saddleLabel;
    QLabel*         m_handlerbarLabel;
    QLabel*         m_fsuspLabel;
    QLabel*         m_rsuspLabel;
    QLabel*         m_pictureLabel;
    QPushButton*    m_backButton;
    QPushButton*    m_nextBikeButton;
    QPushButton*    m_prevBikeButton;
    QPushButton*    m_rentBikeButton;
    QPushButton*    m_returnBikeButton;

    RentBikeWindow* m_rentBikeWindow;
    int             m_showedBikeId{};
};

class RentBikeWindow : public QWidget
{
    Q_OBJECT
public:
    explicit RentBikeWindow(QWidget* parent);
signals:
    void sentClientAndBikeId(int,int);
private:
    QTextEdit*      m_clientIdText;
    QPushButton*    m_applyButton;
private slots:
    void receivedClientId(bool);
};

#endif