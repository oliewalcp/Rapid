#ifndef TREETOOL_H
#define TREETOOL_H

#include <functional>
#include "Core/BinaryTree.h"
#include "Core/Stack.h"
#include <iostream>

#ifdef QT_LIB
#include "Core/RedBlackTree.h"
#include "Core/Preoperation.h"
#include <QApplication>
#include <QWidget>
#include <QScrollArea>
#include <QPainter>
#include <QKeyEvent>
#include <cmath>

namespace rapid
{

template<typename NodeType>
struct StackDataNode
{
    const NodeType *treeNode; // 结点
    long long layer; // 结点所在层数
};

extern QApplication *app;

template<typename NodeType>
using GetDataFunctionType = std::function<std::string(const NodeType *)>;
template<typename NodeType>
using GetColorFunctionType = std::function<unsigned long(const NodeType *)>;

template<typename DataType,
         typename TreeType,
         typename NodeType = BTreeNode<DataType>,
         typename OrdinaryType = BinaryTree<DataType>>
class PaintArea : public QWidget
{
public:
    using StackNode = StackDataNode<NodeType>;

    static constexpr qreal Pi = M_PI;
    static constexpr qreal WIDTH = 1024; // 窗口宽度
    static constexpr qreal HEIGHT = 768; // 窗口高度
    static constexpr qreal PAINT_WIDTH = 2500; // 绘图宽度
    static constexpr qreal PAINT_HEIGHT = 800; // 绘图高度
private:
    OrdinaryType *tree;//树
    GetDataFunctionType<NodeType> func;//结点数据获取函数
    GetColorFunctionType<NodeType> color_func;//结点颜色获取函数

    QPen black_pen;
    QPen red_pen;
    QPen normal_pen;
public:
    PaintArea(const OrdinaryType &tree,
              GetDataFunctionType<NodeType> &&f,
              GetColorFunctionType<NodeType> &&cf,
              QWidget *parent)
        : QWidget(parent), func(f), color_func(cf)
    {
        this->tree = new OrdinaryType(tree);
        setGeometry(0, 0, static_cast<int>(PAINT_WIDTH), static_cast<int>(PAINT_HEIGHT));

        black_pen.setWidth(2); //线宽
        black_pen.setColor(Qt::black); //划线颜色
        black_pen.setStyle(Qt::SolidLine);//线的类型，实线、虚线等
        black_pen.setCapStyle(Qt::FlatCap);//线端点样式
        black_pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式

        red_pen.setWidth(2); //线宽
        red_pen.setColor(Qt::red); //划线颜色
        red_pen.setStyle(Qt::SolidLine);//线的类型，实线、虚线等
        red_pen.setCapStyle(Qt::FlatCap);//线端点样式
        red_pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式

        normal_pen.setWidth(1); //线宽
        normal_pen.setColor(Qt::blue); //划线颜色
        normal_pen.setStyle(Qt::SolidLine);//线的类型，实线、虚线等
        normal_pen.setCapStyle(Qt::FlatCap);//线端点样式
        normal_pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式

    }

    ~PaintArea()
    {
        delete tree;
    }

    void keyPress(QKeyEvent *event)
    {
        static size_type index = 0;
        static constexpr int num[] = {50, 45, 40, 48, 39, 43, 47, 49, 38,
                                      42, 44, 46, 100, 90, 200, 60, 95, 199,
                                      202, 80, 94, 96, 201, 300};
        static TreeType save_tree;
        if(event->key() == Qt::Key_Return && index < get_array_length(num))
        {
            save_tree.insert(num[index++]);
        }
        *tree = save_tree.to_ordinary_tree();
        repaint();
    }
    /* 画一个节点
     * param[painter]:画图工具
     * param[node]:树结点
     * param[pos]:结点圆心位置
     * param[full_num]:该结点所在层满结点时的结点数
     * param[radius]:半径
     */
    void print_node(QPainter &painter, NodeType *node, QPointF pos,
                    int full_num, int layer_height, int radius)
    {
        static QPen p[] = {red_pen, black_pen};

        painter.setPen(p[color_func(node)]);
        //圆心位置和大小
        QRectF node_pos(-radius, -radius, 2 * radius, 2 * radius);
        //移动坐标原点
        painter.translate(pos);
        painter.save();
        //画圆
        painter.drawEllipse(node_pos);
        //画文本
        QString node_name = QString().fromStdString(func(node));
        painter.drawText(node_pos, Qt::AlignCenter, node_name);

        painter.setPen(normal_pen);

        NodeType *left_node = OrdinaryType::left_child(node);
        NodeType *right_node = OrdinaryType::right_child(node);

        full_num *= 2;
        int horizontal_offset = width() / 2 / full_num;//子结点的水平偏移量

        if(left_node != nullptr)
        {
            QPointF child_pos(pos.x() - horizontal_offset, pos.y() + layer_height);
            print_node(painter, left_node, child_pos,
                       full_num, layer_height, radius);
        }
        if(right_node != nullptr)
        {
            QPointF child_pos(pos.x() + horizontal_offset, pos.y() + layer_height);
            print_node(painter, right_node, child_pos,
                       full_num, layer_height, radius);
        }
        painter.restore();
    }
    void paint_tree()
    {
        int tree_depth = tree->depth();//树的深度
        int node_num = static_cast<int>(std::pow(2, tree_depth) - 1);//满二叉树的节点数
        int node_radius = width() / node_num;//结点半径
        int layer_height = height() / tree_depth;//每一层的高度
        QPainter painter(this);
        print_node(painter, tree->root(), QPointF(width() / 2, node_radius), 1, layer_height, node_radius);
    }

    void paintEvent([[maybe_unused]] QPaintEvent *event)
    {
        if(tree->size() == 0) return;
        paint_tree();
//        QPainter painter(this);

//        qreal W = width();
//        qreal H = height();

//        int treeHeight = 7/*tree->depth()*/;//树高度
//        qreal R = W / (2 * std::pow(2, treeHeight) + 2); // 结点半径
//        int layerHeight = static_cast<int>((H - 4 * R) / (treeHeight + 1));//层高

//        QRectF node(QPointF(-R, -R), QPointF(R, R));
//        Stack<StackNode> stack;    // 存放右孩子节点
//        StackNode qNode;

//        Stack<QPointF> points;     // 存放右孩子节点相对于当前坐标系统原点的位置
//        QPointF point;

//        qreal Hoffset;// 水平偏移量
//        int curLayer = 1; //当前层数
//        int curAngle; // 当前角度

//        painter.setRenderHint(QPainter::Antialiasing);
//        painter.setRenderHint(QPainter::TextAntialiasing);
//        //设置字体
//        QFont font;
//        font.setPointSize(8);
//        font.setBold(true);
//        painter.setFont(font);

//        //设置画笔
//        QPen black_pen;
//        black_pen.setWidth(2); //线宽
//        black_pen.setColor(Qt::black); //划线颜色
//        black_pen.setStyle(Qt::SolidLine);//线的类型，实线、虚线等
//        black_pen.setCapStyle(Qt::FlatCap);//线端点样式
//        black_pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式

//        QPen red_pen;
//        red_pen.setWidth(2); //线宽
//        red_pen.setColor(Qt::red); //划线颜色
//        red_pen.setStyle(Qt::SolidLine);//线的类型，实线、虚线等
//        red_pen.setCapStyle(Qt::FlatCap);//线端点样式
//        red_pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式

//        QPen normal_pen;
//        normal_pen.setWidth(1); //线宽
//        normal_pen.setColor(Qt::blue); //划线颜色
//        normal_pen.setStyle(Qt::SolidLine);//线的类型，实线、虚线等
//        normal_pen.setCapStyle(Qt::FlatCap);//线端点样式
//        normal_pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式

//        QPen p[] = {red_pen, black_pen};

//        painter.setPen(black_pen);

//        // 将坐标系统的原点（下文简称原点）移动到初始位置
//        painter.translate(W / 2, 2 * R);

//        const NodeType *t = tree->root();

//        bool not_end_loop = true;

//        while(not_end_loop)
//        {
//            painter.setPen(p[color_func(t)]);
//            painter.drawEllipse(node);
//            QString node_name = QString().fromStdString(func(t));
//            painter.drawText(node, Qt::AlignCenter, node_name);
//            painter.setPen(normal_pen);
//            // 设置孩子节点相对于父节点的水平偏移量
//            Hoffset = std::pow(2, (treeHeight - curLayer)) * R;
//            double deg = std::atan(Hoffset / layerHeight);// 返回的是弧度
//            curAngle = 180 / Pi * deg; // 将弧度转化为角度

//            if (OrdinaryType::right_child(t) != nullptr)
//            {
//                // 坐标轴逆时针旋转
//                painter.rotate(-curAngle);

//                //绘制图形路径
//                painter.drawLine(0, static_cast<int>(R), 0, static_cast<int>(layerHeight / std::cos(deg) - R));

//                // 旋转复原
//                painter.rotate(curAngle);

//                // 右孩子节点压栈
//                qNode.treeNode = OrdinaryType::right_child(t);
//                qNode.layer = curLayer + 1;
//                stack.push(qNode);

//                // 右孩子相对于当前坐标系统原点的位置压栈
//                points.push(QPointF(QPointF(0, 0) + QPointF(Hoffset, layerHeight)));
//                painter.save();
//            }
//            else if(stack.size() == 0)
//            {
//                not_end_loop = false;
//            }
//            if (OrdinaryType::left_child(t) != nullptr)
//            {
//                // 坐标轴顺时针旋转
//                painter.rotate(curAngle);
//                // 绘制边
//                painter.drawLine(0, static_cast<int>(R), 0, static_cast<int>(layerHeight / std::cos(deg) - R));
//                // 旋转复原
//                painter.rotate(-curAngle);
//                // 原点移动到左孩子节点的位置
//                painter.translate(QPointF(QPointF(0, 0) + QPointF(-Hoffset, layerHeight)));
//                t = OrdinaryType::left_child(t);
//                // 层次加1
//                curLayer++;
//            }
//            else
//            {
//                // 获取到右节点的层次状态
//                qNode = stack.top();
//                stack.pop();
//                t = qNode.treeNode;
//                curLayer = qNode.layer;

//                // 原点移动到右孩子节点的位置
//                point = points.top();
//                points.pop();
//                painter.restore();
//                painter.translate(point);
//            }
//        }
//        painter.resetTransform();
    }
};

template<typename DataType,
         typename TreeType,
         typename NodeType = BTreeNode<DataType>,
         typename OrdinaryType = BinaryTree<DataType>>
class MainWindow : public QWidget
{
private:
    using PrintAreaType = PaintArea<DataType, TreeType, NodeType, OrdinaryType>;

    PrintAreaType *_M_area;
    QScrollArea *_M_sarea;
public:
    MainWindow(const OrdinaryType &tree,
               GetDataFunctionType<NodeType> &&f,
               GetColorFunctionType<NodeType> &&cf)
    {
        setGeometry(100, 100,
                    static_cast<int>(PrintAreaType::WIDTH),
                    static_cast<int>(PrintAreaType::HEIGHT));
        _M_sarea = new QScrollArea(this);
        _M_area = new PrintAreaType(tree, rapid::move(f), rapid::move(cf), _M_sarea);
        _M_sarea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        _M_sarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        _M_sarea->setGeometry(0, 0,
                             static_cast<int>(PrintAreaType::WIDTH),
                             static_cast<int>(PrintAreaType::HEIGHT));
        _M_sarea->setWidget(_M_area);
    }
    ~MainWindow()
    {
        delete _M_area;
        delete _M_sarea;
    }
    void keyPressEvent(QKeyEvent *event)
    {
        _M_area->keyPress(event);
        QWidget::keyPressEvent(event);
    }
    void paintEvent([[maybe_unused]] QPaintEvent *event)
    {
        _M_sarea->setGeometry(0, 0, width(), height());
    }
};

void test_TreeTool_main();

} // namespace

#endif // QT_LIB

#endif // TREETOOL_H
