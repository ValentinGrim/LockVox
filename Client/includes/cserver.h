#ifndef CSERVER_H
#define CSERVER_H

#include <QtNetwork>
#include <QDebug>
#include <QRegExp>
#include <QUuid>
#include <QtXml>

#include "AbstractServer.h"
#include "clientlist.h"
#include "cclient.h"
#include "channellist.h"
#include "cchannel.h"
#include "Client/cpacket.h"
#include "uiworker.h"


class CClient;
class CChannel;

class CServer : public AbstractServer
{
    Q_OBJECT
    public:

        CServer();
        CServer(ClientList* clients, ChannelList * channels);

        //m_socket
        QTcpSocket * get_socket();
        void set_socket(QTcpSocket* soc);

        //m_clientsList
        ClientList *getClientsList() const;
        void setClientsList(ClientList *clientsList);

        //m_channelsList
        ChannelList *getChannelsList();
        void setChannelsList(ChannelList *channelsList);

        //m_self
        CClient * get_self(){return m_self;}
        void set_self(CClient *c);





        //Process
        void processIncomingData(QByteArray data);         //Process incoming data



        //Server action - To develop
        void changeChannel(int id);
        void quitChannel(int id);
        void joinChannel(int id);

        //Serialization | Deserialization
        QByteArray Serialize();                                             //Serialize client and channels on the same json document
        void Deserialize(QByteArray in);

        QByteArray SerializeChannels();                                     //Serialize channels into json document then byte array
        QByteArray SerializeClients();                                      //Serialize clients into json document then byte array

        void DeserializeChannels(QByteArray in);                            //Deserialize channels from byte array
        void DeserializeClient(QByteArray in);                              //Deserialize clients from byte array

        void deserializeChannel(QJsonArray & json_array);
        void deserializeClients(QJsonArray & json_array);

        CChannel * deserializeToChannel(QJsonObject json_obj);              //Deserialize channels from json object
        CClient * deserializeToClient(QJsonObject json_obj);                //Deserialize clients from json object



signals:
        void changeState(QString);
        void on_Authentification(int newValue);
        void selfChanged(CClient*);

        //UI

    public slots:
        void RequestServer(int type, int action, CClient * client, CChannel * chan);
        bool Login(QString mail, QString passwd);               //Requests the server to authenticate
        bool Register(QString username, QString mail, QString password,QString password_confirm);
        bool sendMessage(QString msg);
        void connectServer(QString ip);


    private slots:

        void onReceiveData();
        void onCurrentIndexChanged(int);
        void sendToServer(QByteArray ba);
        void sendToServer();

    private:
        //Client mode
        QTcpSocket * m_socket;
        CClient* m_self;
        ClientList * m_clientsList;
        ChannelList * m_channelsList;

        QString m_name;
        bool m_state;

        //Logging
        QFile log_file;
        QTextStream log;

};

#endif // CSERVER_H