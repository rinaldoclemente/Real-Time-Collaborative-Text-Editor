//
// Created by giova on 05/10/2019.
//

#include <string>
#include "header_files/dbService.h"
#include <iostream>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QtCore/QVariant>
#include <QtCore/QDateTime>

dbService::DB_RESPONSE dbService::tryLogin(const std::string& user, const std::string& pass) {
    QSqlDatabase db;

    QString username = QString::fromUtf8(user.data(), user.size());
    QString password = QString::fromUtf8(pass.data(), pass.size());

    db = QSqlDatabase::addDatabase("QSQLITE", "MyConnect2");
    db.setDatabaseName("C:/Users/giova/CLionProjects/Real time text editor/ServerModule/Db/texteditor_users.sqlite");

    if(db.open()) {
        QSqlQuery query(QSqlDatabase::database("MyConnect2"));
        query.prepare(QString("SELECT * FROM users WHERE username = :username AND password = :password"));
        query.bindValue(":username", username);
        query.bindValue(":password", password);

        if(query.exec()) {
            if(query.next()) {
                QString usernameFromDb = query.value(0).toString();
                QString passwordFromDb = query.value(1).toString();

                if(usernameFromDb == username && passwordFromDb == password) {
                    std::cout << "Login success" << std::endl;
                    return LOGIN_OK;
                }
            } else {
                std::cout << "Login failed" << std::endl;
                return LOGIN_FAILED;
            }
            db.close();
        } else {
            std::cout << "Error on select" << std::endl;
            db.close();
            return QUERY_ERROR;
        }
    } else {
        QSqlError error = db.lastError();
        std::cout << "Error on db connection. " << error.text().data() << std::endl;
        return DB_ERROR;
    }
}

dbService::DB_RESPONSE dbService::trySignup(const std::string& user, const std::string& pass, const std::string& email) {
    QSqlDatabase db;

    QString username = QString::fromUtf8(user.data(), user.size());
    QString password = QString::fromUtf8(pass.data(), pass.size());
    QString mail = QString::fromUtf8(email.data(), email.size());

    //control email with regex
    QRegExp mailREX(R"(\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\.[A-Z]{2,4}\b)");
    mailREX.setCaseSensitivity(Qt::CaseInsensitive);
    mailREX.setPatternSyntax(QRegExp::RegExp);
    if (!mailREX.exactMatch(mail)) {
        return EMAIL_ERROR;
    }

    db = QSqlDatabase::addDatabase("QSQLITE", "MyConnect");
    db.setDatabaseName("C:/Users/giova/CLionProjects/Real time text editor/ServerModule/Db/texteditor_users.sqlite");

    if (db.open()) {
        QSqlQuery query(QSqlDatabase::database("MyConnect"));
        query.prepare(QString("SELECT * FROM users WHERE username = :username"));
        query.bindValue(":username", username);

        if (query.exec()) {
            if (query.next()) {
                QString usernameFromDb = query.value(0).toString();
                QString passwordFromDb = query.value(1).toString();
                if (usernameFromDb == username)
                    return SIGNUP_FAILED;
            } else { //user can be created
                QSqlQuery query2(QSqlDatabase::database("MyConnect"));
                query2.prepare("INSERT INTO users (username, password, email) VALUES (:username, :password, :email)");
                query2.bindValue(":username", username);
                query2.bindValue(":password", password);
                query2.bindValue(":email", mail);

                if (query2.exec())
                    return SIGNUP_OK;
                else
                    return QUERY_ERROR;
            }
            db.close();
        } else {
            db.close();
            return QUERY_ERROR;
        }
    } else {
        QSqlError error = db.lastError();
        return DB_ERROR;
    }
}

dbService::DB_RESPONSE dbService::tryNewFile(const std::string& user, const std::string& file_name) {
    QSqlDatabase db;

    QString username = QString::fromUtf8(user.data(), user.size());
    QString filename = QString::fromUtf8(file_name.data(), file_name.size());

    db = QSqlDatabase::addDatabase("QSQLITE", "MyConnect3");
    db.setDatabaseName("C:/Users/giova/CLionProjects/Real time text editor/ServerModule/Db/texteditor_users.sqlite");
    if (db.open()) {
        QSqlQuery query(QSqlDatabase::database("MyConnect3"));
        query.prepare(QString("SELECT * FROM files WHERE filename = :filename and  owner = :username "));
        query.bindValue(":username", username);
        query.bindValue(":filename", filename);

        if (query.exec()) {
            if (query.next()) //username already exists
                return NEWFILE_FAILED;

            QString uri = generateURI(10);
            QString timestamp = getTimestamp();
            QSqlQuery query2(QSqlDatabase::database("MyConnect3"));

            query2.prepare("INSERT INTO files (uri, filename, owner, timestamp) VALUES (:uri, :filename, :owner, :timestamp)");
            query2.bindValue(":uri", uri);
            query2.bindValue(":filename", filename);
            query2.bindValue(":owner", username);
            query2.bindValue(":timestamp", timestamp);

            if (query2.exec()) {
                QSqlQuery query3(QSqlDatabase::database("MyConnect3"));

                query3.prepare("INSERT INTO permissions (idfile, iduser, isOwner, isOpen) VALUES (:idfile, :iduser, :isOwner, :isOpen)");
                query3.bindValue(":idfile", uri);
                query3.bindValue(":iduser", username);
                query3.bindValue(":isOwner", true);
                query3.bindValue(":isOpen", true);

                if (query3.exec()) {
                    return NEWFILE_OK;
                } else {
                    return NEWFILE_FAILED;
                }
            } else {
                db.close();
                return QUERY_ERROR;
            }
        } else {
            db.close();
            return QUERY_ERROR;
        }
    } else {
        QSqlError error = db.lastError();
        return DB_ERROR;
    }
}

inline const char* dbService::enumToStr(dbService::DB_RESPONSE db_resp) {
    switch (db_resp) {
        case dbService::LOGIN_OK:       return "LOGIN_OK";
        case dbService::LOGIN_FAILED:   return "LOGIN_FAILED";
        case dbService::SIGNUP_OK:      return "SIGNUP_OK";
        case dbService::SIGNUP_FAILED:  return "SIGNUP_FAILED";
        case dbService::DB_ERROR:       return "DB_ERROR";
        case dbService::QUERY_ERROR:    return "QUERY_ERROR";
        default:                        return "[Unknown db response]";
    }
}

QString dbService::generateURI(int len) {
    std::string str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int n = str.length();
    std::string URI;
    for (int i=1; i<=len; i++)
        URI.push_back(str[rand() % n]);
    return(QString::fromUtf8(URI.data(), URI.size()));
}

QString dbService::getTimestamp() {
    qint64 utcTime = QDateTime::currentMSecsSinceEpoch();
    QDateTime localTime = QDateTime::fromTime_t(utcTime, Qt::LocalTime);
    return localTime.toString("yyyy-MM-dd hh:mm:ss");
}
