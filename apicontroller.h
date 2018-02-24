#ifndef APICONTROLLER_H
#define APICONTROLLER_H

#include <QObject>
#include <QVariantList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "tracksservice.h"

class ApiController : public QObject {
    Q_OBJECT
public:
    explicit ApiController(TracksService* tracksService, QObject *parent = nullptr);
    virtual ~ApiController();

    Q_INVOKABLE void load();

signals:
    void loaded(const QVariantList& songs);

private slots:
    void onLoad();
    void onLoadError(QNetworkReply::NetworkError e);


private:
    QNetworkAccessManager m_network;
    int m_cursor;
    TracksService* m_pTracksService;
};

#endif // APICONTROLLER_H
