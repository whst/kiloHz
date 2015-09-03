#ifndef OTHERFUNCTIONS_H
#define OTHERFUNCTIONS_H

#include <qstring.h>

#define ABOUTINFO "千赫兹(KiloHz)基于Qt/C++开发\n"\
    "音频解码使用 Qt 4.8 的 Phonon 库\n\n"\
    "●向开发 Qt SDK 的 Qt Developers 致谢\n"\
    "●向 GNU 及所有为开源做出贡献的志愿者们致谢\n\n"\
    "以下这些人虽然没有参与开发,但仍然十分感谢：\n"\
    "●精英王子[王子亭](沈阳网友,推荐我学习Qt GUI开发,其精英论坛为www.jybox.net)\n"\
    "●李瑞归 (同班同学,对我的项目较关注,算围观者)\n"\
    "●夏湘云 (同班同学,项目围观者.好吧,你总是说这个界面太土,自己却不写代码...)\n"\
    "\t\t以上按照姓氏首字母排列\n\n"\
    "如果您发现有任何Bug或建议,请与项目作者王旭东联系：\n"\
    "E-mail: xukiro@gmail.com 或 腾讯微博 @eesuvon\n"\

QString friendlyFileName(const QString & fileName)
{
    int len = fileName.length();
    int spliter = -1, dot = len;
    for(int i=len-1; i >= 0 && spliter== -1; --i){
        if ( fileName.at(i) == '\\' || fileName.at(i) == '/')
                spliter = i;
    }
    for(int i=len-1; i >= 0 && dot== len; --i){
        if ( fileName.at(i) == '.' )
                dot = i;
    }
    QString newName;
    for(int i=spliter+1; i<dot; ++i){
        newName+=fileName.at(i);
    }
    return newName;
}

#endif //OTHERFUNCTIONS_H
