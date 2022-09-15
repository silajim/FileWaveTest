#ifndef ROCKPAPERSCISSORSLIBTEST_H
#define ROCKPAPERSCISSORSLIBTEST_H

#include <QObject>
#include <QTest>

class rockpaperscissorsLibTest : public QObject
{
    Q_OBJECT
public:
    explicit rockpaperscissorsLibTest(QObject *parent = nullptr);

signals:

private slots:
    void addLoginUser();
    void testGame();
    void testDataSerialization();
    void testDeleteGame();

};

#endif // ROCKPAPERSCISSORSLIBTEST_H
