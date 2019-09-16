#ifndef TREETOOL_H
#define TREETOOL_H

#include <functional>
#include "Core/BinaryTree.h"
#include "Core/Stack.h"
#include <iostream>

#ifdef QT_LIB
#include <QApplication>
#include <QWidget>
#include <QScrollArea>
#include <QPainter>
#include <cmath>

namespace rapid
{

template<typename DataType>
struct StackDataNode
{
    const BTreeNode<DataType> *treeNode; // 结点
    long long layer; // 结点所在层数
};

extern QApplication *app;

template<typename T>
using GetDataFunctionType = std::function<std::string(const BTreeNode<T> *)>;

template<typename DataType>
class PaintArea : public QWidget
{
public:
    using StackNode = StackDataNode<DataType>;

    static constexpr qreal Pi = M_PI;
    static constexpr qreal WIDTH = 1024; // 窗口宽度
    static constexpr qreal HEIGHT = 768; // 窗口高度

    const BinaryTree<DataType> *tree;//树
    GetDataFunctionType<DataType> func;//结点数据获取函数

public:
    PaintArea(const BinaryTree<DataType> &tree, GetDataFunctionType<DataType> f, QWidget *parent)
        : tree(new BinaryTree<DataType>(tree)), func(f), QWidget(parent)
    {
        setGeometry(0, 0, static_cast<int>(WIDTH), static_cast<int>(HEIGHT));
    }

    ~PaintArea()
    {
        delete tree;
    }

    void paintEvent([[maybe_unused]] QPaintEvent *event)
    {
        QPainter painter(this);

        qreal W = width();
        [[maybe_unused]] qreal H = height();

        int treeHeight = tree->depth();//树高度
        qreal R = WIDTH / (2 * std::pow(2, treeHeight) + 2); // 结点半径
        int layerHeight = static_cast<int>((HEIGHT - 4 * R) / (treeHeight - 1));//层高

        QRectF node(QPointF(-R, -R), QPointF(R, R));
        Stack<StackNode> stack;    // 存放右孩子节点
        StackNode qNode;

        Stack<QPointF> points;     // 存放右孩子节点相对于当前坐标系统原点的位置
        QPointF point;

        qreal Hoffset;                  // 水平偏移量
        int curLayer = 1; //当前层数
        int curAngle;                   // 当前角度

        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);
        //设置字体
        QFont font;
        font.setPointSize(9);
        font.setBold(true);
        painter.setFont(font);

        //设置画笔
        QPen black_pen;
        black_pen.setWidth(2); //线宽
        black_pen.setColor(Qt::black); //划线颜色
        black_pen.setStyle(Qt::SolidLine);//线的类型，实线、虚线等
        black_pen.setCapStyle(Qt::FlatCap);//线端点样式
        black_pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式

        QPen red_pen;
        red_pen.setWidth(2); //线宽
        red_pen.setColor(Qt::red); //划线颜色
        red_pen.setStyle(Qt::SolidLine);//线的类型，实线、虚线等
        red_pen.setCapStyle(Qt::FlatCap);//线端点样式
        red_pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式

        painter.setPen(black_pen);

        // 将坐标系统的原点（下文简称原点）移动到初始位置
        painter.translate(W / 2, 2 * R);

        const BTreeNode<DataType> *t = tree->root();

        bool first_time = true;

        while(stack.size() > 0 || first_time)
        {
            first_time = false;
            painter.drawEllipse(node);
            QString node_name = QString().fromStdString(func(t));
            painter.drawText(node, Qt::AlignCenter, node_name);

            // 设置孩子节点相对于父节点的水平偏移量
            Hoffset = std::pow(2, (treeHeight - curLayer)) * R;
            double deg = std::atan(Hoffset / layerHeight);// 返回的是弧度
            curAngle = 180 / Pi * deg; // 将弧度转化为角度

            if (t->_M_right != nullptr)
            {
                // 坐标轴逆时针旋转
                painter.rotate(-curAngle);

                //绘制图形路径
                painter.drawLine(0, static_cast<int>(R), 0, static_cast<int>(layerHeight / std::cos(deg) - R));

                // 旋转复原
                painter.rotate(curAngle);

                // 右孩子节点压栈
                qNode.treeNode = t->_M_right;
                qNode.layer = curLayer + 1;
                stack.push(qNode);

                // 右孩子相对于当前坐标系统原点的位置压栈
                points.push(QPointF(QPointF(0, 0) + QPointF(Hoffset, layerHeight)));
                painter.save();
            }
            if (t->_M_left != nullptr)
            {
                // 坐标轴顺时针旋转
                painter.rotate(curAngle);
                // 绘制边
                painter.drawLine(0, static_cast<int>(R), 0, static_cast<int>(layerHeight / std::cos(deg) - R));
                // 旋转复原
                painter.rotate(-curAngle);
                // 原点移动到左孩子节点的位置
                painter.translate(QPointF(QPointF(0, 0) + QPointF(-Hoffset, layerHeight)));
                t = t->_M_left;
                // 层次加1
                curLayer++;
            }
            else
            {
                // 获取到右节点的层次状态
                qNode = stack.top();
                stack.pop();
                t = qNode.treeNode;
                curLayer = qNode.layer;

                // 原点移动到右孩子节点的位置
                point = points.top();
                painter.restore();
                points.pop();
                painter.translate(point);
            }
        }
        painter.resetTransform();
    }
};

template<typename DataType>
class MainWindow : public QWidget
{
private:
    PaintArea<DataType> *_M_area;
    QScrollArea *_M_sarea;
public:
    MainWindow(const BinaryTree<DataType> &tree, GetDataFunctionType<DataType> f)
    {
        setGeometry(100, 100,
                    static_cast<int>(PaintArea<DataType>::WIDTH),
                    static_cast<int>(PaintArea<DataType>::HEIGHT));
        _M_sarea = new QScrollArea(this);
        _M_area = new PaintArea<DataType>(tree, f, _M_sarea);
        _M_sarea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        _M_sarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        _M_sarea->setGeometry(0, 0,
                             static_cast<int>(PaintArea<DataType>::WIDTH),
                             static_cast<int>(PaintArea<DataType>::HEIGHT));
        _M_sarea->setWidget(_M_area);
    }
    ~MainWindow()
    {
        delete _M_area;
        delete _M_sarea;
    }
    void paintEvent([[maybe_unused]] QPaintEvent *event)
    {
        _M_sarea->setGeometry(0, 0, width(), height());
    }
};

} // namespace

#endif // QT_VERSION

#endif // TREETOOL_H
