#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QDebug>
#include <QSettings>



void MainWindow::scanExcellon(){
    QString excellonCode = ui->excellonCode->toPlainText();
    QTextStream stream(&excellonCode);
    QString codeLine;
    int currentToolIndex;
    bool inCode = false;
    double factorToMm = 1.0;
    assignations.clear();
    tools.clear();
    while (stream.readLineInto(&codeLine)) {
        if(codeLine.startsWith(";")){
            //comment
            continue;
        }
        if(codeLine.startsWith("%")){
            //pause
            continue;
        }
        if(codeLine.startsWith("M48")){
            inCode = true;
            qDebug()<<"Start of code";
        }
        if(codeLine.startsWith("M30")){
            inCode = false;
            qDebug()<<"End of code";
        }
        if(inCode){
            if(codeLine.startsWith("T")&&codeLine.contains("C")){
                //new tool definition
                //qDebug()<<"New tool!";
                int indexOfC = codeLine.indexOf("C");
                int indexOfF = codeLine.indexOf("F");
                int endOfToolName = indexOfC;
                if(indexOfF>0){
                    endOfToolName = indexOfF;
                }
                QString toolDiameterName = codeLine.mid(1, endOfToolName-1);
                QString  toolDiameterStr = codeLine.mid(indexOfC+1, -1);
                int toolIndex = toolDiameterName.toInt(nullptr, 10);
                double toolDiameter = toolDiameterStr.toDouble();
                toolDiameter*=factorToMm;
                tools[toolIndex] = toolDiameter;
                qDebug()<<"New tool, diameter: "<<toolDiameter<<" mm";
            }else if (codeLine.startsWith("T")&&!codeLine.contains("C")){
                //change of tool
                currentToolIndex = codeLine.mid(1).toInt(nullptr,10);
                assignations[currentToolIndex].holeSize = tools[currentToolIndex];
            }else if(codeLine.startsWith("INCH")){
                //new tool definition
                factorToMm = 25.4;
                qDebug()<<"inch system!";
            }else if(codeLine.startsWith("METRIC")){
                //new tool definition
                factorToMm = 1.0;
                qDebug()<<"metric system!";
            }
            else if(codeLine.startsWith("X")){
                //new hole coordinate
                int indexOfXCoord = 1;
                int indexOfYCoord = codeLine.indexOf("Y");
                QString xString = codeLine.mid(indexOfXCoord, indexOfYCoord-indexOfXCoord);
                QString yString = codeLine.mid(indexOfYCoord+1, -1);
                assignations[currentToolIndex].coordinates.push_back(QPointF(
                    xString.toDouble()*factorToMm,
                    yString.toDouble()*factorToMm
                ));
                qDebug()<<"metric system!";
            }
        }
    }
    return;
}





