#ifndef DRAWING_H
#define DRAWING_H
#include "headers.h"
class myCurve:public QwtPlotCurve
{
public:
    QVector<QVector<float>> &data;
    QwtPlot* d_plot;
    QwtSymbol *symbol;

    myCurve(QVector<QVector<float>> &dataH,QwtPlot* d_plotH,const QString &title,
            const QColor &color, const QColor &colorSymbol):data(dataH)
    {
        d_plot=d_plotH;
        setTitle(title);
        setPen(color,3);
        setRenderHint
                ( QwtPlotItem::RenderAntialiased, true ); // сглаживание
        symbol = new QwtSymbol( QwtSymbol::Rect,
                                QBrush(colorSymbol), QPen( Qt::black, 2 ), QSize( 7, 7 ) );
        setSymbol( symbol );
    }


    void drawing()
    {

        // Добавить точки на ранее созданную кривую
        QPolygonF points;

        //                points << QPointF( 1.0, 1.0 ) // координаты x, y
        //             << QPointF( 1.5, 2.0 ) << QPointF( 3.0, 2.0 )
        //             << QPointF( 3.5, 3.0 ) << QPointF( 5.0, 3.0 );
        for (int i=0;i<data[1].size();i++)
        {
            points<<QPointF(data[0][i],data[1][i]);
        }


        setSamples( points ); // ассоциировать набор точек с кривой
        attach( d_plot ); // отобразить кривую на графике
    }
};
#endif // DRAWING_H
