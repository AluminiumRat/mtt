#include <stdexcept>

#include <QtCore/QFile>

#include <mtt/application/ApplicationShaderLoader.h>

using namespace mtt;

std::string ApplicationShaderLoader::loadText(const std::string& filename) const
{
  std::string text = loadDefault(filename);
  if(text.empty()) text = loadQt(filename);
  if(text.empty()) throw std::runtime_error("ApplicationShaderLoader: Failed to read shader file: " + filename);
  return text;
}

std::string ApplicationShaderLoader::loadQt(
                                    const std::string& filename) const noexcept
{
  QString filenameString = QString::fromUtf8(filename.c_str());
  QFile file(filenameString);
  if (!file.open(QIODevice::ReadOnly))
  {
    filenameString = QString(":/shaders/") + filenameString;
    file.setFileName(filenameString);
    if (!file.open(QIODevice::ReadOnly)) return std::string();
  }

  QByteArray data = file.readAll();
  return std::string(data.data());
}
