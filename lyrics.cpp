#include "lyrics.h"

QList<QString> Lyrics::getListLyricsText() const
{
    return listLyricsText;
}

QList<int> Lyrics::getListLyricsTime() const
{
    return listLyricsTime;
}

Lyrics::Lyrics(QString lyricsPath)
{
    this->lyricsPath = lyricsPath;
}
Lyrics::Lyrics()
{
}

bool Lyrics::readLyricsFile(QString lyricsPath)
{
    QFile file(lyricsPath);
    qDebug()<<"lyricsPath = "<<lyricsPath;
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        listLyricsText.clear();
        listLyricsTime.clear();
        return false;
    }
    QString line="";
    while((line=file.readLine())>0){
        qDebug()<<line;
        analysisLyricsFile(line);
    }

    return true;
}

bool Lyrics::analysisLyricsFile(QString line)
{
    if(line == NULL || line.isEmpty()){
        qDebug()<<"this line is empty!";
        return false;
    }
    QRegularExpression regularExpression("\\[(\\d+)?:(\\d+)?(\\.\\d+)?\\](.*)?");
    int index = 0;
    QRegularExpressionMatch match;
    match = regularExpression.match(line, index);
    if(match.hasMatch()) {
        int totalTime;
        totalTime = match.captured(1).toInt() * 60000 + match.captured(2).toInt() * 1000;                    /*  计算该时间点毫秒数            */
        QString currentText =QString::fromStdString(match.captured(4).toStdString());      /*   获取歌词文本*/
        qDebug()<<totalTime;
        qDebug()<<currentText;
        listLyricsText.push_back(currentText);
        listLyricsTime.push_back(totalTime);
        return true;
    }
    return false;
}

