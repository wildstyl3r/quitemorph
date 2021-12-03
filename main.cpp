#include "quitemorph.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Quitemorph w;
    w.show();
    return a.exec();
}
