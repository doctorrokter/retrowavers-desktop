#include "trackscontroller.h"
#include "common.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QDir>
#include <QFile>

TracksController::TracksController() {}

TracksController::TracksController(TracksService* tracksService, QObject* parent) : QObject(parent), m_pTracksService(tracksService), m_index(-1), m_favIndex(0), m_playerMode(Playlist) {
    m_pNetwork = new QNetworkAccessManager(this);

    foreach(Track* track, m_pTracksService->getFavouriteTracksList()) {
        if (track->getLocalPath().compare("") == 0) {
            download(track);
        }
    }
}

TracksController::~TracksController() {
    m_pNetwork->deleteLater();
}

bool TracksController::play(const QVariantMap& track) {
    m_index = 0;
    m_favIndex = 0;
    if ((m_pTracksService->count() == 0 && m_playerMode == Playlist) || (m_pTracksService->getFavouriteTracksList().size() == 0 && m_playerMode == Favourite)) {
        return false;
    }

    Track* pTrack = nullptr;

    if (track.contains("id")) {
        if (m_playerMode == Playlist) {
            for (int i = 0; i < m_pTracksService->count(); i++) {
                QString id = track.value("id").toString();
                if (m_pTracksService->getTracksList().at(i)->getId().compare(id) == 0) {
                    m_index = i;
                    pTrack = m_pTracksService->findById(id);
                }
            }
        } else {
            for (int i = 0; i < m_pTracksService->getFavouriteTracksList().size(); i++) {
                QString id = track.value("id").toString();
                if (m_pTracksService->getFavouriteTracksList().at(i)->getId().compare(id) == 0) {
                    m_favIndex = i;
                    pTrack = m_pTracksService->findFavouriteById(id);
                }
            }
        }
    } else {
        if (m_playerMode == Playlist) {
            pTrack = m_pTracksService->getTracksList().at(m_index);
        } else {
            pTrack = m_pTracksService->getFavouriteTracksList().at(m_favIndex);
        }
    }

    if (pTrack != nullptr) {
        m_pTracksService->setActive(pTrack);
        emit played(pTrack->toMap());
        emit indexChanged(m_index);
    }

    return true;
}

bool TracksController::next() {
    if (m_playerMode == Playlist) {
        if (m_pTracksService->count() == 0) {
            return false;
        }

        if (m_index < (m_pTracksService->count() - 1)) {
            play(m_pTracksService->getTracks().at(m_index + 1).toMap());
            return true;
        }
    } else {
        if (m_favIndex < (m_pTracksService->getFavouriteTracksList().size() - 1)) {
            play(m_pTracksService->getFavouriteTracks().at(m_favIndex + 1).toMap());
        } else {
            play(m_pTracksService->getFavouriteTracks().at(0).toMap());
        }
        return true;
    }
    return false;
}

bool TracksController::prev() {
    if (m_playerMode == Playlist) {
        if (m_pTracksService->count() == 0) {
            return false;
        }

        if (m_index != 0) {
            play(m_pTracksService->getTracks().at(m_index - 1).toMap());
        }
        return true;
    } else {
        if (m_favIndex != 0) {
            play(m_pTracksService->getFavouriteTracks().at(m_favIndex - 1).toMap());
        }
        return true;
    }
    return false;
}

void TracksController::like() {
    Track* track = m_pTracksService->getActive();
    if (track != nullptr && !track->isFavourite()) {
        track->setFavourite(true);
        m_pTracksService->addFavourite(track);
        download(track);
        emit liked(track->getId());
    }
}

void TracksController::removeFavourite(const QVariantMap& track) {
    QString id = track.value("id").toString();
    if (m_pTracksService->removeFavourite(id)) {
        emit favouriteTrackRemoved(id);
    }
}

int TracksController::getPlayerMode() const { return m_playerMode; }
void TracksController::setPlayerMode(const int& playerMode) {
    if (m_playerMode != playerMode) {
        m_playerMode = playerMode;
        emit playerModeChanged(m_playerMode);
    }
}

void TracksController::download(Track* track) {
    QNetworkRequest req;

    QUrl url(track->getStreamUrl());
    req.setUrl(url);

    QNetworkReply* reply = m_pNetwork->get(req);
    reply->setProperty("id", track->getId());
    bool res = QObject::connect(reply, SIGNAL(finished()), this, SLOT(onDownload()));
    Q_ASSERT(res);
    res = QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onDownloadError(QNetworkReply::NetworkError)));
    Q_ASSERT(res);
    res = QObject::connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));
    Q_ASSERT(res);
    Q_UNUSED(res);
}

void TracksController::onDownload() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QString id = reply->property("id").toString();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();

        QString tracksDir = QDir::currentPath() + TRACKS;
        QDir dir(tracksDir);
        if (!dir.exists()) {
            dir.mkpath(tracksDir);
        }

        Track* track = m_pTracksService->findFavouriteById(id);
        QString filepath = tracksDir + "/" + track->getFilename();
        QFile file(filepath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(data);
            file.close();
            track->setLocalPath(filepath);
            qDebug() << "===>>> TracksController#onDownload track saved to: " << filepath << endl;
        } else {
            qDebug() << file.errorString() << endl;
        }
    }

    reply->deleteLater();
}

void TracksController::onDownloadError(QNetworkReply::NetworkError e) {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    qDebug() << "TracksController#onError: " << e << endl;
    qDebug() << "TracksController#onError: " << reply->errorString() << endl;
    reply->deleteLater();
}

void TracksController::onDownloadProgress(qint64 sent, qint64 total) {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QString trackId = reply->property("id").toString();
    emit downloadProgress(trackId, sent, total);
}

const int& TracksController::getIndex() const {
    return m_index;
}
