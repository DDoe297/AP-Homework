#include "adressmanager.hpp"
AdressManager *AdressManager::instance = nullptr;
AdressManager::AdressManager(QObject *parent) : QObject(parent) {
  Iterator = Adresses.cbegin();
  importFromFile("./data.phonebook");
}

AdressManager *AdressManager::getInstance() {
  if (instance == nullptr) {
    instance = new AdressManager();
  }
  return instance;
}

void AdressManager::addAdress(QString name, QString adress) {
  if (Adresses.contains(name)) {
    emit nameAlreadyExist();
    return;
  }
  Adresses.insert(Adresses.cend(), name, adress);
  Iterator = Adresses.cend();
  Iterator--;
  exportToFile("./data.phonebook");
}

void AdressManager::editAdress(QString adress) {
  Adresses[Iterator.key()] = adress;
  exportToFile("./data.phonebook");
}

void AdressManager::findAdress(QString name) {
  if (!Adresses.contains(name)) {
    emit nameDosentExist();
    return;
  }
  Iterator = Adresses.find(name);
}

void AdressManager::removeAdress(void) {
  if (Adresses.cbegin() != Adresses.cend()) {
    QMap<QString, QString>::const_iterator toRemove = Iterator;
    nextAdress();
    Adresses.remove(toRemove.key());
  }
  exportToFile("./data.phonebook");
}

void AdressManager::nextAdress() {
  if (Adresses.cbegin() != Adresses.cend()) {
    QMap<QString, QString>::const_iterator current = Iterator;
    if (current == Adresses.cbegin()) {
      Iterator = Adresses.cend();
    }
    Iterator--;
  }
}

void AdressManager::previousAdress() {
  if (Adresses.cbegin() != Adresses.cend()) {
    QMap<QString, QString>::const_iterator current = Iterator;
    if (++current == Adresses.cend()) {
      Iterator = Adresses.cbegin();
      return;
    }
    Iterator++;
  }
}

QPair<QString, QString> AdressManager::getCurrent() {
  if (Adresses.cbegin() != Adresses.cend()) {
    QPair<QString, QString> name_adress;
    name_adress.first = Iterator.key();
    name_adress.second = Iterator.value();
    return name_adress;
  } else {
    QPair<QString, QString> name_adress;
    name_adress.first = "";
    name_adress.second = "";
    return name_adress;
  }
}

void AdressManager::exportToFile(QString fileName) {
  QFile file(fileName);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    return;
  }
  QTextStream out = QTextStream(&file);
  QMap<QString, QString>::const_iterator current = Adresses.cbegin();
  while (current != Adresses.cend()) {
    out << "{" << current.key() << ":" << current.value() << "}\n";
    current++;
  }
  file.close();
}

void AdressManager::importFromFile(QString fileName) {
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return;
  }
  QTextStream in = QTextStream(&file);
  Adresses.clear();
  QRegularExpression re("{([\\w\\d\\s]*):([\\w\\d\\s]*)}");
  re.setPatternOptions(QRegularExpression::MultilineOption);
  QString text = in.readAll();
  QRegularExpressionMatchIterator reIterator = re.globalMatch(text);
  while (reIterator.hasNext()) {
    QRegularExpressionMatch match = reIterator.next();
    QString name = match.captured(1);
    QString adress = match.captured(2);
    addAdress(name, adress);
  }
  previousAdress();
  file.close();
}
