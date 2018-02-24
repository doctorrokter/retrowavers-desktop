#include "apicontroller.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>
#include <QJsonDocument>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>
#include <QByteArray>
#include "common.h"
#include "track.h"

ApiController::ApiController(TracksService* tracksService, QObject *parent) : QObject(parent), m_cursor(1), m_pTracksService(tracksService) {}

ApiController::~ApiController() {}

void ApiController::load() {
    QNetworkRequest req;

    QUrl url(QString(API_ENDPOINT).append("/tracks"));
    QUrlQuery query;
    query.addQueryItem("cursor", QString::number(m_cursor));
    query.addQueryItem("limit", QString::number(25));
    url.setQuery(query);

    req.setUrl(url);

    qDebug() << "===>>> ApiController#load " << url << endl;

    QNetworkReply* reply = m_network.get(req);
    bool res = QObject::connect(reply, SIGNAL(finished()), this, SLOT(onLoad()));
    Q_ASSERT(res);
    res = QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onLoadError(QNetworkReply::NetworkError)));
    Q_UNUSED(res);
}

void ApiController::onLoad() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());

    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument json = QJsonDocument::fromJson(reply->readAll());

        QVariantMap response = json.toVariant().toMap();
        QVariantMap body = response.value("body").toMap();
        m_cursor = body.value("cursor").toInt();

        QVariantList tracks = body.value("tracks").toList();
        QVariantList updatedTracks;
        QList<Track*> tracksList;

        foreach(QVariant var, tracks) {
            QVariantMap trMap = var.toMap();
            Track* track = m_pTracksService->findFavouriteById(trMap.value("id").toString());
            if (track == nullptr) {
                QString imageUrl = trMap.value("artworkUrl").toString();
                trMap["artworkUrl"] = QString(ROOT_IMAGE_ENDPOINT).append(imageUrl);
                trMap["bArtworkUrl"] = QString(IMAGE_PROCESSOR_URL).append("/blur/65/retrowave.ru").append(imageUrl);
                trMap["streamUrl"] = ROOT_ENDPOINT + trMap.value("streamUrl").toString();
                trMap["favourite"] = false;

                QString filename = trMap.value("streamUrl").toString().split("/").last();
                trMap["filename"] = filename;

                updatedTracks.append(trMap);

                track = new Track(this);
                track->fromMap(trMap);
            } else {
                qDebug() << "===>>> ApiController#onLoad track already favourite: " << track->getTitle() << endl;
                qDebug() << track->toMap() << endl;
            }
            tracksList.append(track);
        }

        m_pTracksService->appendTracks(tracksList);
//        foreach(Track* track, tracksList) {
//            loadImage(track->getId(), track->getArtworkUrl());
//            loadBlurImage(track->getId(), track->getBArtworkUrl());
//        }

        emit loaded(updatedTracks);
    }

    reply->deleteLater();
}

void ApiController::onLoadError(QNetworkReply::NetworkError e) {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    qDebug() << "===>>> ApiController#onLoadError: " << e << endl;
    qDebug() << "===>>> ApiController#onLoadError: " << reply->errorString() << endl;
    reply->deleteLater();
}
