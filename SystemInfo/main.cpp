#include <QCoreApplication>
#include <QDebug>
#include <QProcess>

typedef struct {
  int exitCode;
  QString standardOutput;
  QString standardError;
} ProcessResult;

void printProcessResult(const ProcessResult &result) {
  qDebug() << "exitCode:" << result.exitCode;

  if (!result.standardOutput.isEmpty()) {
    qDebug() << "standardOutput:" << result.standardOutput;
  } else if (!result.standardError.isEmpty()) {
    qDebug() << "standardError:" << result.standardError;
  }
}

ProcessResult runCommand(const QStringList &args) {
  QProcess process;
  process.start("C:/Windows/System32/wbem/WMIC.exe", args);
  process.waitForFinished();
  return {
      process.exitCode(),
      process.readAllStandardOutput().simplified(),
      process.readAllStandardError().simplified(),
  };
}

double getCpuLoadPercent() {
  QString value;
  QStringList args;

  args << "cpu"
       << "get"
       << "LoadPercentage"
       << "/value";

  auto result = runCommand(args);

  QRegExp regex("=(\\d+)");
  int pos = regex.indexIn(result.standardOutput);
  if (pos > -1) {
    value = regex.cap(1);
    qDebug() << QString("CPU load percent: %1%").arg(value);
  }

  return value.toDouble() / 100;
}

double getMemoryUsedPercent() {
  QString valueTotalPhysicalMemory;
  QString valueFreePhysicalMemory;

  // -------------------------------------

  QStringList argsTotalPhysicalMemory;

  argsTotalPhysicalMemory << "ComputerSystem"
                          << "get"
                          << "TotalPhysicalMemory"
                          << "/value";

  auto resultTotalPhysicalMemory = runCommand(argsTotalPhysicalMemory);

  QRegExp regexTotalPhysicalMemory("=(\\d+)");
  int posTotalPhysicalMemory = regexTotalPhysicalMemory.indexIn(resultTotalPhysicalMemory.standardOutput);
  if (posTotalPhysicalMemory > -1) {
    valueTotalPhysicalMemory = regexTotalPhysicalMemory.cap(1);
    qDebug() << QString("Total physical memory: %1").arg(valueTotalPhysicalMemory);
  }

  // -------------------------------------

  QStringList argsFreePhysicalMemory;

  argsFreePhysicalMemory
      << "OS"
      << "get"
      << "FreePhysicalMemory"
      << "/value";

  auto resultFreePhysicalMemory = runCommand(argsFreePhysicalMemory);

  QRegExp regexFreePhysicalMemory("=(\\d+)");
  int posFreePhysicalMemory = regexFreePhysicalMemory.indexIn(resultFreePhysicalMemory.standardOutput);
  if (posFreePhysicalMemory > -1) {
    valueFreePhysicalMemory = regexFreePhysicalMemory.cap(1);
    qDebug() << QString("Free physical memory: %1").arg(valueFreePhysicalMemory);
  }

  qDebug() << valueFreePhysicalMemory << "->" << valueFreePhysicalMemory.toDouble() / 1024 / 1024 << "GB";
  qDebug() << valueTotalPhysicalMemory << "->" << valueTotalPhysicalMemory.toDouble() / 1024 / 1024 / 1024 << "GB";

  auto usedPercent = 1 - valueFreePhysicalMemory.toDouble() / valueTotalPhysicalMemory.toDouble() * 1024;

  qDebug() << QString("Memory Used Percent: %1%").arg(usedPercent * 100);

  return usedPercent;
}

double getDiskUsage() {
  QStringList valueCapacityList;
  QStringList valueFreeSpaceList;

  // --------------------

  QStringList argsCapacity;

  argsCapacity << "volume"
               << "get"
               << "Capacity"
               << "/value";

  auto resultCapacity = runCommand(argsCapacity);

  QRegExp regexCapacity("=(\\d+)");
  int posCapacity = regexCapacity.indexIn(resultCapacity.standardOutput);
  while (posCapacity > -1) {
    valueCapacityList.append(regexCapacity.cap(1));
    posCapacity = regexCapacity.indexIn(resultCapacity.standardOutput, posCapacity + regexCapacity.matchedLength());
  }
  qDebug() << "Capacity: " << valueCapacityList;

  // ----------------------------------------

  QStringList argsFreeSpace;

  argsFreeSpace << "volume"
                << "get"
                << "FreeSpace"
                << "/value";

  auto resultFreeSpace = runCommand(argsFreeSpace);

  QRegExp regexFreeSpace("=(\\d+)");
  int posFreeSpace = regexFreeSpace.indexIn(resultFreeSpace.standardOutput);
  while (posFreeSpace > -1) {
    valueFreeSpaceList.append(regexFreeSpace.cap(1));
    posFreeSpace = regexFreeSpace.indexIn(resultFreeSpace.standardOutput, posFreeSpace + regexFreeSpace.matchedLength());
  }
  qDebug() << "FreeSpace: " << valueFreeSpaceList;

  auto sumCapacity = 0.0;
  for (auto &valueCapacity : valueCapacityList) {
    sumCapacity += valueCapacity.toDouble();
  }

  auto sumFreeSpace = 0.0;
  for (auto &valueFreeSpace : valueFreeSpaceList) {
    sumFreeSpace += valueFreeSpace.toDouble();
  }

  qDebug() << sumCapacity / 1024 / 1024 / 1024 << "GB";
  qDebug() << sumFreeSpace / 1024 / 1024 / 1024 << "GB";

  auto usedPercent = 1 - sumFreeSpace / sumCapacity;

  qDebug() << QString("Disk Usage Percent: %1%").arg(usedPercent * 100);

  return usedPercent;
}

QString getIpAddress() {
  QString value;
  QStringList args;

  args << "NicConfig"
       << "where"
       << "IPEnabled=true"
       << "get"
       << "IPAddress"
       << "/value";

  auto result = runCommand(args);

  //  printProcessResult(result);

  QRegExp regex(R"((\d+\.\d+\.\d+\.\d+))");
  int pos = regex.indexIn(result.standardOutput);
  if (pos > -1) {
    value = regex.cap(1);
    qDebug() << QString("IP address: %1").arg(value);
  }

  return value;
}

int main(int argc, char *argv[]) {
  qDebug() << getCpuLoadPercent();
  qDebug() << getMemoryUsedPercent();
  qDebug() << getDiskUsage();
  qDebug() << getIpAddress();

  return 0;
}
