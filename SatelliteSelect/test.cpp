// PLC_ERROR CModBusTCP::modbusWRITE(int addr, int nb, const uint16_t *data)
// {
//     if(!_mb) return PLC_ERROR_MODBUSCONTEXT;

//     int res = modbus_write_registers(_mb, addr, nb, data);

//     _logWriteSaver->save(addr, nb, data);

//     return static_cast<PLC_ERROR>(res);
// }


// а вот сам логер

//     class IModbusSaver {
// public:
//     virtual ~IModbusSaver() = default;
//     virtual bool save(int addr, int nb, const uint16_t *data) const = 0;
//     //virtual bool load(std::vector<NORAD_SCHEDULE>& vecNoradSchedule) const = 0;
//     virtual bool clear() = 0;
// };

// // Реализация для сохранения в файл
// class FileModbusSaver : public IModbusSaver {
// public:
//     explicit FileModbusSaver(const std::string& filePath) : filePath_(filePath) {}

//     bool save(int addr, int nb, const uint16_t *data) const override {
//         std::ofstream outFile(filePath_, std::ios::app);
//         if (!outFile.is_open()) {
//             return false;
//         }

//         outFile << "ADDR = " << addr << ", SIZE = " << nb << ";\n";
//         outFile << QDate::currentDate().toString("<dd/MM/yyyy ").toStdString() + QTime::currentTime().toString(" hh:mm:ss.zzz>").toStdString() << ";\n";
//         // save here to low level log;
//         for(int i = 0; i < nb ; ++i) {

//             outFile << i + addr << "," << std::hex << std::setfill('0')
//             << "0x" << std::setw(4) << data[i] << ";\n";

//             outFile << std::dec;
//         }

//         outFile.close();
//         return true;
//     }


//     bool clear() override {

//         // Создаём временный ofstream (автоматически закрывается)
//         std::ofstream(filePath_).close();

//         // Проверяем, что файл доступен и не в состоянии ошибки
//         return !std::ifstream(filePath_).fail();
//     }

// private:
//     std::string filePath_;
// };



// class DatabaseModbusSaver : public IModbusSaver {
// public:
//     explicit DatabaseModbusSaver(const std::string& dbPath) {
//         db_ = QSqlDatabase::addDatabase("QSQLITE", "modbus_connection");
//         db_.setDatabaseName(QString::fromUtf8(dbPath));
//         if (!db_.open()) {
//             qCritical() << "Failed to open database:" << db_.lastError().text();
//             return;
//         }

//         QSqlQuery query(db_);
//         query.prepare(
//             "CREATE TABLE IF NOT EXISTS modbus_data ("
//             "id INTEGER PRIMARY KEY AUTOINCREMENT,"
//             "address INTEGER NOT NULL,"
//             "value_hex TEXT NOT NULL,"
//             "value_dec INTEGER NOT NULL,"
//             "timestamp TEXT NOT NULL)"
//             );
//         if (!query.exec()) {
//             qCritical() << "Failed to create table:" << query.lastError().text();
//         }
//     }

//     ~DatabaseModbusSaver() {
//         if (db_.isOpen()) {
//             db_.close();
//         }
//         QSqlDatabase::removeDatabase("modbus_connection");
//     }

//     bool save(int addr, int nb, const uint16_t* data) const override {
//         if (!db_.isOpen()) {
//             qCritical() << "Database is not open!";
//             return false;
//         }

//         QSqlQuery query(db_);
//         const QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODateWithMs);

//         if (!db_.transaction()) {
//             qCritical() << "Failed to start transaction:" << db_.lastError().text();
//             return false;
//         }

//         query.prepare(
//             "INSERT INTO modbus_data (address, value_hex, value_dec, timestamp) "
//             "VALUES (:address, :value_hex, :value_dec, :timestamp)"
//             );

//         for (int i = 0; i < nb; ++i) {
//             query.bindValue(":address", addr + i);
//             query.bindValue(":value_hex", QString("0x%1").arg(data[i], 4, 16, QChar('0')));
//             query.bindValue(":value_dec", data[i]);
//             query.bindValue(":timestamp", timestamp);

//             if (!query.exec()) {
//                 qCritical() << "Insert failed:" << query.lastError().text();
//                 db_.rollback();
//                 return false;
//             }
//         }

//         if (!db_.commit()) {
//             qCritical() << "Commit failed:" << db_.lastError().text();
//             return false;
//         }

//         return true;
//     }

//     bool clear() override {
//         if (!db_.isOpen()) {
//             qCritical() << "Database is not open!";
//             return false;
//         }
//         /////////////////////// ................ tyt logica o4isheniya DB

//         qDebug() << "Database cleared successfully";
//         return true;
//     }

// private:
//     mutable QSqlDatabase db_;
// };

// class ModbusSaverFactory {
// public:
//     enum class SaverType {
//         File,
//         Database,
//     };

//     // Создаёт указанного типа
//     static std::unique_ptr<IModbusSaver> createSaver(SaverType type, const std::string& connectionParam = "") {
//         switch (type) {
//         case SaverType::File:
//             return std::make_unique<FileModbusSaver>(connectionParam);
//         case SaverType::Database:
//             return std::make_unique<DatabaseModbusSaver>(connectionParam);
//         default:
//             throw std::invalid_argument("Unknown saver type");
//         }
//     }

//     // Альтернативная версия с конфигурацией через строку
//     static std::unique_ptr<IModbusSaver> createSaver(const std::string& config) {
//         // Пример формата: "file:/path/to/file.log" или "db:host=localhost dbname=test"
//         size_t delimiterPos = config.find(':');
//         if (delimiterPos == std::string::npos) {
//             throw std::invalid_argument("Invalid config format");
//         }

//         std::string type = config.substr(0, delimiterPos);
//         std::string param = config.substr(delimiterPos + 1);

//         if (type == "file") {
//             return std::make_unique<FileModbusSaver>(param);
//         } else if (type == "db") {
//             return std::make_unique<DatabaseModbusSaver>(param);
//         } else {
//             throw std::invalid_argument("Unknown saver type: " + type);
//         }
//     }
// };
