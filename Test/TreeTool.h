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

extern QApplication app;

template<typename NodeType>
using GetDataFunctionType = std::function<std::string(const NodeType *)>;
template<typename NodeType>
using GetColorFunctionType = std::function<unsigned long(const NodeType *)>;

template<typename DataType,
         typename TreeType,
         typename NodeType,
         typename OrdinaryType>
class MainWindow;

template<typename DataType,
         typename TreeType,
         typename NodeType = BTreeNode<DataType>,
         typename OrdinaryType = BinaryTree<DataType>>
class PaintArea : public QWidget
{
public:
    static constexpr qreal Pi = M_PI;
    static constexpr qreal WIDTH = 1024; // 窗口宽度
    static constexpr qreal HEIGHT = 768; // 窗口高度
    static constexpr qreal PAINT_WIDTH = 2500; // 绘图宽度
    static constexpr qreal PAINT_HEIGHT = 800; // 绘图高度
private:
    QWidget *app;
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
        app = parent->parentWidget();
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
        if CONSTEXPR (!IsSame<TreeType, BinaryTree<DataType>>::value)
        {
            static size_type index = 0;
            static size_type era_index = 0;
            static constexpr int num[] = {50, 45, 40, 48, 39, 43, 47, 49, 38,
                                          42, 44, 46, 100, 90, 200, 60, 95, 199,
                                          202, 80, 94, 96, 201, 300};
            static constexpr int eranum[] = {48, 100};
            static TreeType save_tree;
            static bool insert_tree = false;
            static bool insert_tree_assist = false;
            if(!insert_tree)
            {
                if(tree->size() > 0)
                    insert_tree_assist = true;
            }
            if(insert_tree_assist) return;
            insert_tree = true;
            if(event->key() == Qt::Key_Return && index < get_array_length(num))
            {
                save_tree.insert(num[index++]);
                QString title = tr("current insert: ");
                title += QString::fromStdString(std::to_string(num[index - 1]));
                title += tr(" - next insert: ");
                title += QString::fromStdString(std::to_string(num[index]));
                app->setWindowTitle(title);
                std::cout << "current insert: " << num[index - 1] << " - next insert: " << num[index] << std::endl;
            }
            if(index >= get_array_length(num) &&
                    event->key() == Qt::Key_Backspace &&
                    era_index < get_array_length(eranum))
            {
                save_tree.erase(save_tree.find(eranum[era_index++]));
            }
            *tree = save_tree.to_ordinary_tree();
            repaint();
        }
    }
    /* 画一个节点
     * param[painter]:画图工具
     * param[node]:树结点
     * param[pos]:结点圆心位置
     * param[full_num]:该结点所在层满结点时的结点数
     * param[radius]:半径
     */
    void print_node(QPainter &painter, NodeType *node, QPointF pos,
                    int full_num, double layer_height, double radius)
    {
        static QPen p[] = {red_pen, black_pen};

        painter.setPen(p[color_func(node)]);
        //圆心位置和大小
        QRectF node_pos(-radius, -radius, 2 * radius, 2 * radius);
        //移动坐标原点
        painter.save();
        painter.translate(pos);
        //画圆
        painter.drawEllipse(node_pos);
        //画文本
        QString node_name = QString().fromStdString(func(node));
        painter.drawText(node_pos, Qt::AlignCenter, node_name);

        NodeType *left_node = OrdinaryType::left_child(node);
        NodeType *right_node = OrdinaryType::right_child(node);

        full_num *= 2;//下一层的结点数
        double horizontal_offset = width() / 2.0 / full_num;//子结点的水平偏移量

        double deg = std::atan(horizontal_offset / layer_height);// 返回的是弧度

        double temp1 = radius * std::sin(deg), temp2 = radius * std::cos(deg);

        if(left_node != nullptr)
        {
            QPointF child_pos(-horizontal_offset, layer_height);

            double left_down_x = -temp1;
            double left_down_y = temp2;

            double right_top_x = child_pos.x() + temp1;
            double right_top_y = child_pos.y() - temp2;

            painter.setPen(normal_pen);
            painter.drawLine(QPointF(left_down_x, left_down_y),
                             QPointF(right_top_x, right_top_y));

            print_node(painter, left_node, child_pos,
                       full_num, layer_height, radius);
        }
        if(right_node != nullptr)
        {
            QPointF child_pos(horizontal_offset, layer_height);

            double right_down_x = temp1;
            double right_down_y = temp2;

            double left_top_x = child_pos.x() - temp1;
            double left_top_y = child_pos.y() - temp2;

            painter.setPen(normal_pen);
            painter.drawLine(QPointF(right_down_x, right_down_y),
                             QPointF(left_top_x, left_top_y));

            print_node(painter, right_node, child_pos,
                       full_num, layer_height, radius);
        }
        painter.restore();
    }
    void paint_tree()
    {
        int tree_depth = tree->depth();//树的深度

        int node_num = static_cast<int>(std::pow(2, tree_depth) - 1);//满二叉树的节点数
        double node_radius = width() / (node_num + 1) / 2.0;//结点半径
        double layer_height = height() / tree_depth;//每一层的高度

        QPainter painter(this);
        //设置字体
        QFont font;
        font.setPointSize(8);
        font.setBold(true);
        painter.setFont(font);

        print_node(painter, tree->root(), QPointF(width() / 2, node_radius), 1, layer_height, node_radius);
    }

    void paintEvent([[maybe_unused]] QPaintEvent *event)
    {
        if(tree->size() == 0) return;
        paint_tree();
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
