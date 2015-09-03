#ifndef OTHERFUNCTIONS_H
#define OTHERFUNCTIONS_H

#include <qstring.h>

#define ABOUTINFO "ǧ����(KiloHz)����Qt/C++����\n"\
    "��Ƶ����ʹ�� Qt 4.8 �� Phonon ��\n\n"\
    "���򿪷� Qt SDK �� Qt Developers ��л\n"\
    "���� GNU ������Ϊ��Դ�������׵�־Ը������л\n\n"\
    "������Щ����Ȼû�в��뿪��,����Ȼʮ�ָ�л��\n"\
    "��Ӣ����[����ͤ](��������,�Ƽ���ѧϰQt GUI����,�侫Ӣ��̳Ϊwww.jybox.net)\n"\
    "������� (ͬ��ͬѧ,���ҵ���Ŀ�Ϲ�ע,��Χ����)\n"\
    "�������� (ͬ��ͬѧ,��ĿΧ����.�ð�,������˵�������̫��,�Լ�ȴ��д����...)\n"\
    "\t\t���ϰ�����������ĸ����\n\n"\
    "������������κ�Bug����,������Ŀ����������ϵ��\n"\
    "E-mail: xukiro@gmail.com �� ��Ѷ΢�� @eesuvon\n"\

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
