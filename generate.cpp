#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QDebug>
#include <QSettings>
template<typename T> QString str(T val){
    return QString::number(val);
}

void MainWindow::generateGCode(){
    double toolDiameter = ui->toolDiameter->value();
    double zSafeDistance = ui->zSafeDistance->value();
    double freeMovementSpeed = ui->freeMovementSpeed->value();
    double perStepDepthOfCut = ui->perStepDepthOfCut->value();
    double plungeFeedRate = ui->plungeFeedRate->value();
    double totalDepth = ui->totalDepth->value();
    double cuttingFeedrate = ui->cuttingFeedrate->value();
    double cuttingRPM = ui->cuttingRPM->value();

    QString gcode = "( File created using Excellon to mill )\n"
            "( by Raul Huertas )\n"
            "(Expects the z0 coordinate at the top surface of)\n"
            "(the material)\n";
    "(tool diameter to use: "+QString::number(toolDiameter )+"mm)\n";
    gcode += "G21\n";
    gcode += "G90\n";
    gcode += "G17\n";
    gcode += "G94\n";
    gcode += "G01 F"+str(freeMovementSpeed)+"\n";
    gcode += "M5\n";
    gcode += "G00 X0 Y0\n";
    gcode += "G00 Z"+QString::number(zSafeDistance)+" F"+str(freeMovementSpeed)+"\n";


    gcode += "M0\n";
    gcode += "G00 Z"+QString::number(zSafeDistance)+" F"+str(freeMovementSpeed)+"\n";
    gcode += "M03 S"+str(cuttingRPM)+".0 \n";
    gcode += "G01 F"+str(freeMovementSpeed)+"\n";


    //for(int t = 0; t< tools.size();t++){
    for (const auto& [toolIndex, holeDiamater] : tools.asKeyValueRange()) {
        //for each tool
        const auto& assignation = assignations[toolIndex];
        if(holeDiamater<=toolDiameter){
            continue;
        }
        double toolMovementRadius = (holeDiamater-toolDiameter)/2.0;

        gcode += "\n (Hole diameter: "+QString::number(assignation.holeSize)+")\n";
        for(int h = 0; h<assignation.coordinates.size(); h++){
            const auto& coordinate = assignation.coordinates[h];
            gcode += "G90\n";
            gcode += "G01 Z"+str(zSafeDistance)+" F"+str(freeMovementSpeed)+"\n";
            gcode += "G01 X"+str(coordinate.x())+" Y"+str(coordinate.y())+"\n";
            gcode += "G01 Z"+str(perStepDepthOfCut)+" F"+str(freeMovementSpeed)+"\n";
            gcode += "G91\n";
            gcode += "G01 X"+str(-toolMovementRadius)+" Y"+str(0.0)+" F"+str(freeMovementSpeed)+"\n";
            gcode += "G01 Z"+str(-2*perStepDepthOfCut)+" F"+str(plungeFeedRate)+"\n";
            gcode += "G02 X0 Y0 I"+str(toolMovementRadius)+" F"+str(cuttingFeedrate)+"\n";
            double currentDepth = perStepDepthOfCut;
            while(currentDepth<-totalDepth){
                double zMove = std::min((-totalDepth-currentDepth), perStepDepthOfCut);
                gcode += "G01 Z"+str(-perStepDepthOfCut)+" F"+str(plungeFeedRate)+"\n";
                gcode += "G02  X0 Y0  I"+str(toolMovementRadius)+" F"+str(cuttingFeedrate)+"\n";
                currentDepth +=zMove;
            }

            //go back to safe Z
            gcode += "G90\n";
            gcode += "G01 Z"+str(zSafeDistance)+" F"+str(freeMovementSpeed)+"\n";

        }

    }

    gcode += "G90\n";
    gcode += "G01 Z"+QString::number(zSafeDistance)+" F"+str(freeMovementSpeed)+"\n";
    gcode += "M30\n";
    gcode += "M05\n";
    ui->millingCode->clear();
    ui->millingCode->setPlainText(gcode);
    ui->statusbar->showMessage("Generated!");
}
