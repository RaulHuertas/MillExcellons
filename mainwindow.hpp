#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <vector>
#include "drillassignations.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QMap<int, DrillAssignations> assignations;
    QMap<int, double> tools;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void buttonGenerateGCode();
    void buttonCopyGCode();

private slots:
    void zSafeDistanceChanged(double zSafeDistance);
    void toolDiameterChanged(double );
    void perStepDepthOfCutChanged(double);
    void totalDepthChanged(double);
    void freeMovementSpeedChanged(int);
    void cuttingFeedrateChanged(int);
    void cuttingRPMChanged(int);
    void plungeFeedRateChanged(int);
private:
    Ui::MainWindow *ui;

    void scanExcellon();
    void generateGCode();

    void loadSettings();
    void saveSettings();

};
#endif // MAINWINDOW_HPP
