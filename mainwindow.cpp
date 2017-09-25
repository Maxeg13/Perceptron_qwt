#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawing.h"
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QDebug>
#include "headers.h"
#include "perceptron.h"
#include <vector>
using namespace std;

vector<vector<float>> dataTest;
vector<vector<float>> dataL;
myCurve *curveLearn, *curveTest;
QwtPlot *d_plot;
int freq_cnt;
QTimer *timer;
int mode;
perceptron* perc;
int PN=3;

class MyQwtPlotPicker:public QwtPlotPicker
{
public:
    int ind;
    QPointF MouseP;
    MyQwtPlotPicker(int A1, int A2,
                    RubberBand RB, DisplayMode DM, QWidget * QW):
        QwtPlotPicker(A1, A2,RB, DM, QW) // ассоциация с осями
    {};
    void widgetMouseMoveEvent(QMouseEvent *e) //QwtPicker override
    {
        //        qDebug()<<invTransform(e->pos());
        MouseP=invTransform(e->pos());
        dataL[0][ind]=MouseP.x();
        dataL[1][ind]=MouseP.y();
        //        curveTest->drawing();
        curveLearn->drawing();
        //        qDebug()<<ind;
    }

    void widgetMousePressEvent(QMouseEvent *e)
    {

        timer->stop();
        //        MouseP=invTransform(e->pos());
        MouseP=invTransform(e->pos());
        float min=90000,minH;
        for(int j=0;j<dataL[0].size();j++)
            if(min>(minH=((MouseP.x()-dataL[0][j])*(MouseP.x()-dataL[0][j])
                          +(MouseP.y()-dataL[1][j])*(MouseP.y()-dataL[1][j]))))
            {min=minH;ind=j;}
        //        curveTest->drawing();
        curveLearn->drawing();
        //        qDebug()<<minH;
    }
    void widgetMouseReleaseEvent(QMouseEvent *e)
    {
        timer->start(42);
    }
};

MyQwtPlotPicker *d_picker;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);


    vector<int> constr;
    constr.push_back(1);
    constr.push_back(8);
    constr.push_back(8);
    constr.push_back(1);


    perc =new perceptron(constr);
    //    delete[] perc->lr[2];
    //    perc->lr[2]=new layer(1,2,(perc->lr[1]));


    d_plot = new QwtPlot( this );
    //________________________

    QGridLayout* MW=new QGridLayout();
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(MW);
    this->resize(QSize(600,450));
    MW->addWidget(ui->pushButton_3,1,1);
    MW->addWidget(ui->pushButton,1,2);
    MW->addWidget(ui->pushButton_2,1,3);
    MW->addWidget(ui->pushButton_4,1,4);

    MW->addWidget(d_plot,2,1,2,4);
    setCentralWidget(centralWidget);
    //        setMenuWidget(MW);


    //d_plot->autoRefresh();
    d_plot->setAutoReplot();
    //_______232
    d_plot->setAxisScale(QwtPlot::yLeft,-1.5,1.5);
    // настройка функций
    d_picker =
            new MyQwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft, // ассоциация с осями
                QwtPlotPicker::CrossRubberBand, // стиль перпендикулярных линий
                QwtPicker::ActiveOnly, // включение/выключение
                d_plot->canvas() ); // ассоциация с полем
    // Цвет перпендикулярных линий
    d_picker->setRubberBandPen( QColor( Qt::red ) );

    // цвет координат положения указателя
    d_picker->setTrackerPen( QColor( Qt::black ) );

    // непосредственное включение вышеописанных функций
    d_picker->setStateMachine( new QwtPickerDragPointMachine() );

    // Включить возможность приближения/удаления графика
    // #include <qwt_plot_magnifier.h>
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier(d_plot->canvas());
    // клавиша, активирующая приближение/удаление
    magnifier->setMouseButton(Qt::MidButton);
    // Включить возможность перемещения по графику
    // #include <qwt_plot_panner.h>
    QwtPlotPanner *d_panner = new QwtPlotPanner( d_plot->canvas() );
    // клавиша, активирующая перемещение
    d_panner->setMouseButton( Qt::RightButton );
    // Включить отображение координат курсора и двух перпендикулярных
    // линий в месте его отображения
    // #include <qwt_plot_picker.h>
    d_plot->setTitle( "My perceptron demonstration" ); // заголовок
    d_plot->setCanvasBackground( Qt::white ); // цвет фона

    // Параметры осей координат
    d_plot->setAxisTitle(QwtPlot::yLeft, "Y");
    d_plot->setAxisTitle(QwtPlot::xBottom, "X");
    d_plot->insertLegend( new QwtLegend() );


    // Включить сетку
    // #include <qwt_plot_grid.h>
    QwtPlotGrid *grid = new QwtPlotGrid(); //

    grid->setMajorPen(QPen( Qt::gray, 2 )); // цвет линий и толщина
    grid->attach( d_plot ); // добавить сетку к полю графика



    int testSize=40;
    dataTest.resize(2);
    dataTest[0].resize(testSize);
    dataTest[1].resize(testSize);
    float k=0.5;
    for(int i=0;i<dataTest[0].size();i++)
    {
        dataTest[0][i]=((i-testSize/2)/200.)*k;
        //        dataTest[1][i]=cos(i/4.);
        perc->refresh(&dataTest[0][i]);
        dataTest[1][i]= perc->lr[PN-1]->n[0].state;

    }
    curveTest=new myCurve(dataTest,d_plot,"reaction",Qt::black,Qt::black);
    curveTest->drawing();



    dataL.resize(2);
    int learnSize=15;
    dataL[0].resize(learnSize);
    dataL[1].resize(learnSize);
    for(int i=0;i<dataL[0].size();i++)
    {
        dataL[0][i]=((i-learnSize/2)/100.)*k;
        dataL[1][i]=(sin(i/2.)+1)*.4;
    }
    curveLearn=new myCurve(dataL,d_plot,"Target",QColor(0,0,0,0),Qt::red);
    curveLearn->drawing();


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(test()));
    timer->start();
}



void MainWindow::test()
{

    for(int i=0;i<dataTest[0].size();i++)
    {
        perc->refresh(&dataTest[0][i]);
        //dataTest[1][i]= perc->lr[PN-1]->n[0].state;
        dataTest[1][i]=*(perc->out)[0];

    }

    for(int j=0; j<400; j++)
    {
        freq_cnt++;
        switch(mode)
        {

        case 0:

            for(int i=0;i<dataL[0].size();i++)
                perc->learn1(&dataL[0][i],&dataL[1][i]);
            break;
        case 1:
            for(int i=0;i<dataL[0].size();i++)
                perc->learn1(&dataL[0][d_picker->ind],&dataL[1][d_picker->ind]);


        }
    }

    qDebug()<<freq_cnt;
    freq_cnt=0;
    curveTest->drawing();

    //    qDebug()<<i;
    //    qDebug()<<perc->lr[2]->w[0][0];
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()//8
{
    delete[] perc;
    vector<int> constr;
    constr.push_back(1);
    constr.push_back(8);
    constr.push_back(8);
    constr.push_back(1);
    perc =new perceptron(constr);

    qDebug()<<perc->lr[1]->w[0][0];
}

void MainWindow::on_pushButton_2_clicked()//12
{
    delete[] perc;
    vector<int> constr;
    constr.push_back(1);
    constr.push_back(2);
    constr.push_back(12);
    constr.push_back(1);
    perc =new perceptron(constr);
}



void MainWindow::on_pushButton_3_clicked()//2
{
    delete[] perc;
    vector<int> constr;
    constr.push_back(1);
    constr.push_back(3);
    constr.push_back(1);
    perc =new perceptron(constr);
}

void MainWindow::on_pushButton_4_clicked()
{
    mode=(mode==0)?1:0;
}
