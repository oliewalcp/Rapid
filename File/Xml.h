#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
#include <string>
#include "../Container/Stack.h"
#include "../Container/DoubleLinkedList.h"

class XmlNode
{
private:
    std::string text;
    DoubleLinkedList<std::string> child;
    XmlNode *parent;
public:
    void add_child(std::string name)
    {
        child.push_back(name);
    }
    void remove_child(std::string name)
    {
        for(DoubleLinkedList<std::string>::iterator it = child.begin(); it != child.end(); it++)
        {
            if((*it) == name)
            {
                child.erase(it);
                break;
            }
        }
    }
    void remove_children(std::initializer_list<std::string> names)
    {

    }
};

class XmlDocument
{
private:

public:

};

#endif // XMLDOCUMENT_H
