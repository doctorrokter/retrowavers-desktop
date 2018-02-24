#include "tracksservice.h"
#include <QDir>
#include <QFile>
#include "common.h"
#include <QDebug>
#include <QVariant>
#include <QVariantList>
#include <QJsonDocument>
#include <QDebug>

TracksService::TracksService(QObject *parent) : QObject(parent), m_active(nullptr) {
    QFile file(QDir::currentPath() + FAVORITE_TRACKS);
    if (file.exists()) {
        QJsonDocument jda;
        QVariantList list = jda.fromBinaryData(file.readAll()).toVariant().toList();
        foreach(QVariant var, list) {
            Track* track = new Track();
            track->fromMap(var.toMap());
            m_favouriteTracks.append(track);
            qDebug() << track->toMap() << endl;
        }
        qDebug() << "Favourite tracks: " << m_favouriteTracks.size() << endl;
    }
}

TracksService::~TracksService() {
    saveFavourite();
    m_active->deleteLater();
    foreach(Track* track, m_tracks) {
        track->deleteLater();
    }
    foreach(Track* track, m_favouriteTracks) {
        track->deleteLater();
    }
}

QVariantList TracksService::getTracks() const {
    QVariantList tracks;
    foreach(Track* track, m_tracks) {
        tracks.append(track->toMap());
    }
    return tracks;
}

void TracksService::setTracks(const QVariantList& tracks) {
    foreach(QVariant var, tracks) {
        Track* track = new Track();
        track->fromMap(var.toMap());
        m_tracks.append(track);
    }
    emit tracksChanged(tracks);
}

void TracksService::appendTracks(const QList<Track*>& tracks) {
    m_tracks.append(tracks);
    QVariantList tracksMaps;
    foreach(Track* track, tracks) {
        tracksMaps.append(track->toMap());
    }
    emit tracksChanged(tracksMaps);
}

void TracksService::addFavourite(Track* track) {
    bool exists = false;
    foreach(Track* t, m_favouriteTracks) {
        exists = t == track;
    }

    if (!exists) {
        m_favouriteTracks.append(track);
        emit favouriteTracksChanged(getFavouriteTracks());
        saveFavourite();
    }
}

bool TracksService::removeFavourite(const QString& id) {
    Track* track = findFavouriteById(id);
    if (track != nullptr) {
        if (m_favouriteTracks.removeOne(track)) {
            QFile file(track->getLocalPath());
            if (file.exists()) {
                qDebug() << " Removing file: " << track->getLocalPath() << endl;
                file.remove();
            }

            if (!m_tracks.contains(track)) {
                track->deleteLater();
            }

            saveFavourite();
            return true;
        }
        return false;
    }
    return false;
}

Track* TracksService::findById(const QString& id) {
    for (int i = 0; i < m_tracks.size(); i++) {
        Track* track = m_tracks.at(i);
        if (track->getId().compare(id) == 0) {
            return track;
        }
    }
    return nullptr;
}

Track* TracksService::findFavouriteById(const QString& id) {
    for (int i = 0; i < m_favouriteTracks.size(); i++) {
        Track* track = m_favouriteTracks.at(i);
        if (track->getId().compare(id) == 0) {
            return track;
        }
    }
    return nullptr;
}

Track* TracksService::getActive() const {
    return m_active;
}

void TracksService::setActive(Track* track) {
    if (m_active != track) {
        m_active = track;
        emit activeChanged(m_active);
    }
}

QVariantList TracksService::getFavouriteTracks() const {
    QVariantList tracks;
    foreach(Track* track, m_favouriteTracks) {
        tracks.append(track->toMap());
    }
    return tracks;
}

int TracksService::count() const {
    return m_tracks.size();
}

void TracksService::setImagePath(const QString& id, const QString& imagePath) {
    foreach(Track* track, m_tracks) {
        if (track->getId().compare(id) == 0) {
            track->setImagePath("file://" + imagePath);
            emit imageChanged(id, imagePath);
        }
    }
}

void TracksService::setBlurImagePath(const QString& id, const QString& imagePath) {
    foreach(Track* track, m_tracks) {
        if (track->getId().compare(id) == 0) {
            track->setBImagePath("file://" + imagePath);
            emit blurImageChanged(id, track->getBImagePath());
        }
    }

    foreach(Track* track, m_favouriteTracks) {
        if (track->getId().compare(id) == 0) {
            track->setBImagePath("file://" + imagePath);
            emit blurImageChanged(id, track->getBImagePath());
        }
    }
}

QList<Track*>& TracksService::getTracksList() {
    return m_tracks;
}

QList<Track*>& TracksService::getFavouriteTracksList() {
    return m_favouriteTracks;
}

void TracksService::saveFavourite() {
    QVariantList list;
    foreach(Track* track, m_favouriteTracks) {
        list.append(track->toMap());
    }

    QJsonDocument jda;
    QFile file(QDir::currentPath() + FAVORITE_TRACKS);
    if (file.open(QIODevice::WriteOnly)) {
        jda.fromVariant(list);
        file.write(jda.toBinaryData());
        qDebug() << "Save favourite tracks: " << QDir::currentPath() + FAVORITE_TRACKS << endl;
    }
}
