
#include "c_config.h"
//CConfig         mGlobbalConfig;
#include <ctime>
#include <iostream>
double CConfig::shipHeadingDeg=5;
double CConfig::shipSpeed=0;
double CConfig::antennaAziDeg=0;
QHash<QString, QString> CConfig::mHashData = CConfig::readFile();
volatile long long int CConfig::time_now_ms = 0;
void CConfig::setValue(QString key, double value)
{
    QString strValue = QString::number(value);
    mHashData.insert(key, strValue);
    //SaveToFile();
}

void CConfig::setValue(QString key, QString value)
{
    mHashData.insert(key, value);
    //SaveToFile();
}

double CConfig::getDouble(QString key,double defaultValue )
{
    if(mHashData.find(key)!=mHashData.end())
    return mHashData.value(key).toDouble();
    else
    {
        setValue(key,defaultValue);
        return defaultValue;
    }
}
int CConfig::getInt(QString key, int defaultValue )
{
    if(mHashData.find(key)!=mHashData.end())
    return mHashData.value(key).toInt();
    else
    {
        setValue(key,defaultValue);
        return defaultValue;
    }
}
QString CConfig::getString(QString key,QString defaultValue )
{
    if(mHashData.find(key)!=mHashData.end())
    return mHashData.value(key);
    else
    {
        setValue(key,defaultValue);
        return defaultValue;
    }
}

CConfig::CConfig(void)
{
    //hashData.;
    //shipHeadingDeg = 0;
    readFile();
}

CConfig::~CConfig(void)
{
}

void CConfig::SaveToFile()
{
    QHash<QString, QString>::const_iterator it = mHashData.constBegin();
    QXmlStreamAttributes attr;
    while (it != mHashData.constEnd()) {
        attr.append(it.key(),it.value());
        ++it;
    }
    if (QFile::exists(HR_CONFIG_FILE_BACKUP_2))
    {
        QFile::remove(HR_CONFIG_FILE_BACKUP_2);
    }
    if (QFile::exists(HR_CONFIG_FILE_BACKUP_1))
    {
        QFile::rename(HR_CONFIG_FILE_BACKUP_1,HR_CONFIG_FILE_BACKUP_2);
    }
    if (QFile::exists(HR_CONFIG_FILE))
    {
        QFile::rename(HR_CONFIG_FILE,HR_CONFIG_FILE_BACKUP_1);
    }
    QXmlStreamWriter writer;
    QFile xmlFile(HR_CONFIG_FILE);
    xmlFile.open(QIODevice::WriteOnly);
    writer.setDevice(&xmlFile);
    writer.writeEmptyElement(XML_ELEM_NAME);
    writer.writeAttributes(attr);
    writer.writeEndElement();
    xmlFile.close();

}

void CConfig::setDefault()
{
    if (QFile::exists(HR_CONFIG_FILE))
    {
        QFile::remove(HR_CONFIG_FILE);
    }

    QFile::copy(HR_CONFIG_FILE_DF, HR_CONFIG_FILE);

}

QHash<QString, QString> CConfig::readFile() {

   return readFile(HR_CONFIG_FILE);
}
void CConfig::ReportError(const char* error)
{

    freopen(HR_ERROR_FILE, "a", stderr );
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S    ",timeinfo);
    std::cerr << buffer;
    std::cerr << error;
    std::cerr << std::endl;
}
QHash<QString, QString> CConfig::readFile(QString fileName)
{
    QFile xmlFile(fileName);
    xmlFile.open(QIODevice::ReadOnly);

    QXmlStreamReader xml;
    xml.setDevice(&xmlFile);
    int nElement = 0;
    QHash<QString, QString> hashData;
    while (xml.readNextStartElement())
    {

        if(xml.name()==XML_ELEM_NAME)
        {

           for (uint i=0;i<xml.attributes().size();i++)
           {
               nElement++;
               QXmlStreamAttribute attr = xml.attributes().at(i);
               hashData.insert( attr.name().toString(),
                                attr.value().toString());
           }
        }
        if (xml.tokenType() == QXmlStreamReader::Invalid)
            xml.readNext();
        // readNextStartElement() leaves the stream in
        // an invalid state at the end. A single readNext()
        // will advance us to EndDocument.
        if (xml.hasError()) {
            if(fileName==HR_CONFIG_FILE) readFile(HR_CONFIG_FILE_BACKUP_1);
            else if(fileName==HR_CONFIG_FILE_BACKUP_1)readFile(HR_CONFIG_FILE_BACKUP_2);
            else
            {
                ReportError("Config load failed");
            }
        }
    }
    if(!nElement)
    {
        if(fileName==HR_CONFIG_FILE) readFile(HR_CONFIG_FILE_BACKUP_1);
        else if(fileName==HR_CONFIG_FILE_BACKUP_1)readFile(HR_CONFIG_FILE_BACKUP_2);
        else
        {
            ReportError("Config load failed");
        }
    }
    xmlFile.close();
    return hashData;
}
