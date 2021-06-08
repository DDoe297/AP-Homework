#ifndef THREAD_HPP
#define THREAD_HPP

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QTcpSocket>

class Thread : public QThread
{
    Q_OBJECT
private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
public:
    explicit Thread(qintptr ID, QObject *parent = 0);
    void run(void);
signals:
    void error(QTcpSocket::SocketError socketerror);
public slots:
    void readyRead();
    void disconnected();
};

#endif // THREAD_HPP
