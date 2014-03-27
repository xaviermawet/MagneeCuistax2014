#include "DataBaseManager.hpp"

bool DataBaseManager::restorePreviousDataBase(void)
{
    QSettings settings;

    if(settings.contains(DATABASE_KEYWORD))
    {
        QString dbFilePath = settings.value(DATABASE_KEYWORD).toString();

        if (QFile::exists(dbFilePath))
            return DataBaseManager::openDataBase(dbFilePath);
    }

    return false;
}

bool DataBaseManager::createDataBase(const QString &dataBaseFilePath)
{
    if (!DataBaseManager::installDataBase(dataBaseFilePath))
        return false;

    // Save the file path of the latest database opened to prevent restoring
    QSettings settings;
    settings.setValue(DATABASE_KEYWORD, dataBaseFilePath);

    return true;
}

bool DataBaseManager::createDataBase(QDir const& destDir, QString const& dbName)
{
    return DataBaseManager::createDataBase(destDir.filePath(dbName));
}

bool DataBaseManager::openExistingDataBase(const QString& dataBaseFilePath)
{
    if (!QFile::exists(dataBaseFilePath) ||
            !DataBaseManager::openDataBase(dataBaseFilePath))
        return false;

    // Save the file path of the latest database opened to prevent restoring
    QSettings settings;
    settings.setValue(DATABASE_KEYWORD, dataBaseFilePath);

    return true;
}

bool DataBaseManager::openExistingDataBase(const QDir &destDir,
                                           const QString &dbName)
{
    return DataBaseManager::openExistingDataBase(destDir.filePath(dbName));
}

QSqlQuery DataBaseManager::execQuery(const QString &queryString,
                                     const QVariantList &values, bool forwardOnly)
{
    QSqlQuery query(queryString);

    // bind values
    foreach (QVariant value, values)
        query.addBindValue(value);

    query.setForwardOnly(forwardOnly);

    if (!query.exec())
        throw NException(QObject::tr("La requete a échouée : ")
                         + query.lastQuery() + " " + query.lastError().text());

    return query; // Implicit sharing
}

void DataBaseManager::execTransaction(QSqlQuery &query)
{
    QSqlDriver* sqlDriver = QSqlDatabase::database().driver();

    sqlDriver->beginTransaction();

    if(!query.exec())
    {
        sqlDriver->rollbackTransaction();
        throw NException(QObject::tr("la requête a échouée : ")
                         + query.lastQuery() + query.lastError().text());
    }

    // Try to commit transaction
    if(!sqlDriver->commitTransaction())
        throw NException(QObject::tr("la validation des données à échouée"));
}

void DataBaseManager::execBatch(QSqlQuery &query,
                                QSqlQuery::BatchExecutionMode mode)
{
    QSqlDriver* sqlDriver = QSqlDatabase::database().driver();

    sqlDriver->beginTransaction();

    if(!query.execBatch(mode))
    {
        sqlDriver->rollbackTransaction();
        throw NException(QObject::tr("la requête a échouée : ")
                         + query.lastQuery() + query.lastError().text());
    }

    // Try to commit transaction
    if(!sqlDriver->commitTransaction())
        throw NException(QObject::tr("la validation des données à échouée"));
}

bool DataBaseManager::openDataBase(const QString& dataBaseFilePath)
{
    if(QSqlDatabase::database().isValid() &&
            QSqlDatabase::database().isOpen())
    {
        QString dbConnectionName =
                QSqlDatabase::database().connectionName();

        QSqlDatabase::database().close();
        QSqlDatabase::removeDatabase(dbConnectionName);
    }

    /* ---------------------------------------------------------- *
     *                       Open database                        *
     * ---------------------------------------------------------- */
    // SQLite version 3 or above
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dataBaseFilePath);

    if (!db.open())
        throw NException(QObject::tr("Impossible d'ouvrir la base ") +
                         dataBaseFilePath + db.lastError().text());

    db.exec("PRAGMA foreign_keys = ON");
    db.exec("PRAGMA journal_mode = MEMORY");
    db.exec("PRAGMA synchronous  = OFF");

    return true;
}

bool DataBaseManager::installDataBase(QString const& DBFilePath)
{
    DataBaseManager::openDataBase(DBFilePath);

    /* ---------------------------------------------------------- *
     *                        Create tables                       *
     * ---------------------------------------------------------- */
    QSqlDatabase db = QSqlDatabase::database();

    // Use a script to create tables
    QFile schemaFile(":/sql/schemaTables"); // Resource file

    // get all table creation script
    schemaFile.open(QFile::ReadOnly);
    QStringList schemaTableList = QString(
           schemaFile.readAll()).split(";", QString::SkipEmptyParts);

    db.driver()->beginTransaction();

    // Create all table from script
    foreach(const QString schemaTable, schemaTableList)
        if(!schemaTable.trimmed().isEmpty())
            db.exec(schemaTable);

    bool commitSucced = db.driver()->commitTransaction();

    schemaFile.close();

    return commitSucced;
}
