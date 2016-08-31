#pragma once

#include <QVariantMap>
#include <QFile>
#include <QSaveFile>
#include <QJsonDocument>
#include <QJsonParseError>

#include <QDebug>

class JsonTools
{
public:
    template <typename T = QVariantMap>
    static T fromFile(const QString& path)
    {
        T result;

        QFile file(path);

        if (file.open(QFile::ReadOnly))
        {
            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);

            if (parseError.error == QJsonParseError::NoError)
                result = doc.toVariant().value<T>();
            else
                qWarning() << QString("can't parse %1: %2").arg(file.fileName(), parseError.errorString()).toUtf8().constData();
        }
        else
            qWarning() << QString("can't open %1 for reading: %2").arg(file.fileName(), file.errorString()).toUtf8().constData();

        return result;
    }

    static bool toFile(const QVariant& data, const QString& path)
    {
        bool result = false;

        QSaveFile file(path);

        if (file.open(QFile::WriteOnly))
        {
            file.write(QJsonDocument::fromVariant(data).toJson());
            result = file.commit();

            if (!result)
                qWarning() << QString("can't commit %1: %2").arg(file.fileName(), file.errorString()).toUtf8().constData();
        }
        else
            qWarning() << QString("can't open %1 for writing: %2").arg(file.fileName(), file.errorString()).toUtf8().constData();

        return result;
    }
};
