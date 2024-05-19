#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QDebug>
#include <QSettings>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_G), this, SLOT(buttonGenerateGCode()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this, SLOT(buttonCopyGCode()));

    connect(
        ui->buttonGenerateGCode, &QPushButton::clicked,
        this, &MainWindow::buttonGenerateGCode
    );
    connect(
        ui->buttonCopyGCode, &QPushButton::clicked,
        this, &MainWindow::buttonCopyGCode
    );
    connect(
        ui->zSafeDistance, &QDoubleSpinBox::valueChanged,
        this, &MainWindow::zSafeDistanceChanged
    );
    connect(
        ui->toolDiameter, &QDoubleSpinBox::valueChanged,
        this, &MainWindow::toolDiameterChanged
        );
    connect(
        ui->perStepDepthOfCut, &QDoubleSpinBox::valueChanged,
        this, &MainWindow::perStepDepthOfCutChanged
        );
    connect(
        ui->totalDepth, &QDoubleSpinBox::valueChanged,
        this, &MainWindow::totalDepthChanged
        );
    connect(
        ui->freeMovementSpeed, &QSpinBox::valueChanged,
        this, &MainWindow::freeMovementSpeedChanged
        );
    connect(
        ui->cuttingFeedrate, &QSpinBox::valueChanged,
        this, &MainWindow::cuttingFeedrateChanged
        );
    connect(
        ui->cuttingRPM, &QSpinBox::valueChanged,
        this, &MainWindow::cuttingRPMChanged
    );
    connect(
        ui->plungeFeedRate, &QSpinBox::valueChanged,
        this, &MainWindow::plungeFeedRateChanged
    );
    loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buttonGenerateGCode(){

    scanExcellon();
    generateGCode();
}

void MainWindow::buttonCopyGCode(){
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(ui->millingCode->toPlainText());
    ui->statusbar->showMessage("Copied!");
}
const QString settingsFileName = "exctomill.ini";
void MainWindow::loadSettings(){

    //QSettings settings(settingsFileName, QSettings::NativeFormat);
    QSettings settings;
    QString postProcessor = settings.value("postProcessor","GRBL").toString();
    double zSafeDistance = settings.value("zSafeDistance", 5.0).toDouble();
    double toolDiameter = settings.value("toolDiameter", 2.0).toDouble();
    double perStepDepthOfCut = settings.value("perStepDepthOfCut", 0.5).toDouble();
    double totalDepth = settings.value("totalDepth", -2.5).toDouble();
    int freeMovementSpeed = settings.value("freeMovementSpeed", 500).toInt();
    int cuttingFeedrate = settings.value("cuttingFeedrate", 500).toInt();
    int cuttingRPM = settings.value("cuttingRPM", 12000).toInt();
    int plungeFeedRate = settings.value("plungeFeedRate", 50).toInt();

    ui->postProcessor->setCurrentText(postProcessor);
    ui->zSafeDistance->setValue(zSafeDistance);
    ui->toolDiameter->setValue(toolDiameter);
    ui->perStepDepthOfCut->setValue(perStepDepthOfCut);
    ui->totalDepth->setValue(totalDepth);
    ui->freeMovementSpeed->setValue(freeMovementSpeed);
    ui->cuttingFeedrate->setValue(cuttingFeedrate);
    ui->cuttingRPM->setValue(cuttingRPM);
    ui->plungeFeedRate->setValue(plungeFeedRate);

    settings.sync();
}

void MainWindow::zSafeDistanceChanged(double newVal)
{
        saveSettings();
}

void MainWindow::toolDiameterChanged(double newVal)
{
    saveSettings();
}

void MainWindow::perStepDepthOfCutChanged(double newVal)
{
    saveSettings();
}

void MainWindow::totalDepthChanged(double newVal)
{
    saveSettings();
}

void MainWindow::freeMovementSpeedChanged(int newVal)
{
    saveSettings();
}

void MainWindow::cuttingFeedrateChanged(int newVal)
{
    saveSettings();
}

void MainWindow::cuttingRPMChanged(int newVal)
{
    saveSettings();
}

void MainWindow::plungeFeedRateChanged(int newVal){
    saveSettings();
}


void MainWindow::saveSettings()
{
    //QSettings settings(settingsFileName, QSettings::NativeFormat);
    QSettings settings;
    settings.setValue("postProcessor", ui->postProcessor->currentText());
    settings.setValue("zSafeDistance", ui->zSafeDistance->value());
    settings.setValue("toolDiameter", ui->toolDiameter->value());
    settings.setValue("perStepDepthOfCut", ui->perStepDepthOfCut->value());
    settings.setValue("totalDepth", ui->totalDepth->value());
    settings.setValue("freeMovementSpeed", ui->freeMovementSpeed->value());
    settings.setValue("cuttingFeedrate", ui->cuttingFeedrate->value());
    settings.setValue("cuttingRPM", ui->cuttingRPM->value());
    settings.setValue("plungeFeedRate", ui->plungeFeedRate->value());
    settings.sync();
}






