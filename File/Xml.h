#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
#include <string>
#include <unordered_map>
#include <fstream>
#include <stack>
#include <list>
#include "FileBase.h"
#include <iostream>
#include "Exception.h"

class XmlNode
{
private:
    typedef std::list<XmlNode *> __List;
    typedef __List::iterator __Iter;
    typedef std::unordered_map<std::string, std::string> __Map;

    std::string __Name;
    std::string __Text;
    __List *__Child;
    __Map *__Attributes;
    XmlNode *__Parent;
public:
    XmlNode(std::string name, std::string text = "") :
        __Text(text), __Parent(nullptr), __Attributes(new __Map), __Name(name), __Child(new __List) {}
    XmlNode(XmlNode *parent, std::string name, std::string text = "") :
        __Parent(parent), __Text(text), __Attributes(new __Map), __Name(name), __Child(new __List)
    {
        __Parent->append_child(this);
    }
    virtual ~XmlNode()
    {
        if(__Parent != nullptr)
            delete __Parent;
        clear();
    }
    std::string get_attribute_text()
    {
        std::string result = "";
        for(__Map::iterator it = __Attributes->begin(); it != __Attributes->end(); it++)
        {
            if(result != "") result += " ";
            result += (*it).first + "=\"" + (*it).second + "\"";
        }
        return result;
    }
    inline bool has_child()
    {
        return __Child->size() > 0;
    }
    inline void remove_text()
    {
        __Text = "";
    }
    inline void append_text(const std::string& text)
    {
        __Text += text;
    }
    inline void append_text(const char& text)
    {
        __Text += text;
    }
    inline void set_text(std::string arg)
    {
        __Text = arg;
    }
    inline std::string get_text()
    {
        return __Text;
    }
    inline void set_parent(XmlNode *parent)
    {
        this->__Parent = parent;
    }
    inline XmlNode* get_parent()
    {
        return __Parent;
    }
    inline std::string node_name()
    {
        return __Name;
    }
    void set_attribute(std::string key, std::string value)
    {
        (*__Attributes)[key] = value;
    }
    std::string get_attribute(std::string key)
    {
        __Map::iterator it = __Attributes->find(key);
        if(it == __Attributes->end()) return "";
        return (*it).second;
    }
    inline void append_child(XmlNode *node)
    {
        __Child->push_back(node);
        node->set_parent(this);
    }
    void remove_child(std::string name)
    {
        for(__Iter it = __Child->begin(); it != __Child->end(); it++)
        {
            if((*it)->node_name() == name)
            {
                __Child->erase(it);
                break;
            }
        }
    }
    void remove_children(std::string name)
    {
        for(__Iter it = __Child->begin(); it != __Child->end(); it++)
        {
            if((*it)->node_name() == name)
            {
                __Iter temp = it;
                it++;
                __Child->erase(temp);
            }
        }
    }
    inline const __List& get_children()
    {
        return *__Child;
    }
    void clear()
    {
        __Child->clear();
        delete __Attributes;
    }
};

class XmlDocument
{
private:
    typedef std::list<XmlNode *> __List;
    typedef __List::iterator __Iter;
    typedef std::unordered_map<std::string, std::string> __Map;
    typedef __Map::iterator __Attr_iter;
    typedef std::unordered_map<std::string, char> __Dictionary;
    typedef unsigned long long __uint64;
    typedef unsigned int __uint32;
    typedef std::stack<XmlNode *> __Stack;

    static constexpr __uint64 START = 0;
    static constexpr __uint64 HEADER = 0x01;
    static constexpr __uint64 NODE_OPEN = 0x02;
    static constexpr __uint64 ATTRIBUTE_NAME = 0x04;
    static constexpr __uint64 NODE_NAME = 0x08;
    static constexpr __uint64 ATTRIBUTE_VALUE = 0x10;
    static constexpr __uint64 NODE_CLOSE = 0x20;
    static constexpr __uint64 NODE_TEXT = 0x40;
    static constexpr __uint64 END = 0x80;
    static constexpr __uint64 SPACE = 0x0100;
    static constexpr __uint64 ATTRIBUTE_VALUE_START = 0x0200;
    static constexpr __uint64 HEADER_END = 0x0400;
    static constexpr __uint64 SPECIAL_CHAR = 0x0800;
    static constexpr __uint64 COMMENT = 0x1000;

    std::string *__file_name;
    __Dictionary *__char_dictionary;
    __List *__Child;
    __Map *__Attributes;

    inline bool __is_word(const char& ch)
    {
        return (ch <= 'z' && ch >= 'a') || (ch <= 'Z' && ch >= 'A');
    }
    inline bool __is_name_start(const char& ch)
    {
        return __is_word(ch) || ch == '_';
    }
    inline bool __is_name(const char& ch)
    {
        return __is_name_start(ch) || (ch >= '0' && ch <= '9') || ch == '.' || ch == '-';
    }
    inline bool __is_space(const char& ch)
    {
        return (ch == '\t' || ch == '\n' || ch == ' ' || ch == '\r');
    }
    inline bool __is_special_char(const char& ch)
    {
        return ch == '<' || ch == '>' || ch == '"' || ch == '\'';
    }
    void __write_space(std::ofstream &file, int num)
    {
        for(int i = 0; i < num; i++)
            file << " ";
    }
    void __write_child_info(std::ofstream &file, XmlNode *node, int layer, int indent_width);
public:
    XmlDocument(std::string filename = "") : __Child(new __List), __Attributes(new __Map), __file_name(nullptr)
    {
        __char_dictionary = new __Dictionary();
        (*__char_dictionary)["&lt;"] = '<';
        (*__char_dictionary)["&gt;"] = '>';
        (*__char_dictionary)["&amp;"] = '&';
        (*__char_dictionary)["&quot;"] = '"';
        (*__char_dictionary)["&apos;"] = '\'';
        if(filename != "")
            parse(filename);
    }
    virtual ~XmlDocument()
    {
        if(__file_name != nullptr)
            delete __file_name;
        this->clear();
        delete __char_dictionary;
    }
    inline void set_attribute(std::string key, std::string value)
    {
        (*__Attributes)[key] = value;
    }
    inline XmlNode* get_root_node()
    {
        if(__Child->size() == 0) return nullptr;
        return __Child->front();
    }
    std::string get_attribute(std::string key)
    {
        __Map::iterator it = __Attributes->find(key);
        if(it == __Attributes->end()) return "";
        return (*it).second;
    }
    void append_child(XmlNode *root)
    {
        __Child->push_back(root);
        root->set_parent(nullptr);
    }
    void clear()
    {
        __Child->clear();
        delete __Attributes;
    }
    void parse(std::string filename);
    void save(int indent);
    void save_as(std::string filename, int indent);
    void remove_child(XmlNode *node);
};

void XmlDocument::__write_child_info(std::ofstream &file, XmlNode *node, int layer, int indent_width)
{
    __write_space(file, layer * indent_width);
    file << "<" + node->node_name();
    std::string attr = node->get_attribute_text();
    if(attr != "") file << " " << attr << " ";
    file << ">";
    __List children = node->get_children();
    if(children.size() > 0)
    {
        file << std::endl;
        for(XmlNode *child : children)
        {
            __write_child_info(file, child, layer + 1, indent_width);
        }
        __write_space(file, layer * indent_width);
    }
    else
    {
        file << node->get_text();
    }
    file << "</" << node->node_name() << ">" << std::endl;
}

void XmlDocument::parse(std::string filename)
{
    __file_name = new std::string(filename);
    __Stack *node = new __Stack;
    __uint32 file_length;
    const char* content = FileBase::open(filename.data(), file_length, std::ios_base::in);
    __uint64 current_signal = START;
    std::string key = "", value = "", special = "";
    std::string *temp = &key;
    for(__uint64 index = 0; index < file_length; index++)
    {
        char ch = content[index];
        if((current_signal & NODE_TEXT) > 0 && (current_signal & (NODE_NAME | HEADER | NODE_OPEN | COMMENT | NODE_CLOSE)) == 0)
        {
            if(ch == '&')
            {
                special = ch;
                current_signal |= SPECIAL_CHAR;
            }
            else if((current_signal & SPECIAL_CHAR) > 0)
            {
                if(ch == ';')
                {
                    special += ch;
                    current_signal ^= SPECIAL_CHAR;
                    auto it = __char_dictionary->find(special);
                    if(it == __char_dictionary->end()) throw Exception("illegal character");
                    (*temp) += (*it).second;
                    special = "";
                }
                else if(__is_word(ch)) special += ch;
                else throw Exception("illegal character");
            }
            else if(ch == '<')
            {
                current_signal ^= NODE_OPEN;
            }
            else if(__is_special_char(ch))
            {
                throw Exception("illegal character");
            }
            else node->top()->append_text(ch);
        }
        else if(current_signal == START)
        {
            if(__is_space(ch)) continue;
            if(ch == '<') current_signal = NODE_OPEN;
        }
        else if((current_signal & COMMENT) > 0)
        {
            if(ch == '-')
            {
                if(index + 2 >= file_content.length())
                    throw Exception("there is no end of file");
                else if(content[++index] == '-' && content[index + 1] == '>')
                    current_signal ^= COMMENT;
            }
        }
        else if((current_signal & NODE_OPEN) > 0)
        {
            if(__is_name_start(ch))
            {
                special = "";
                temp = &special;
                (*temp) += ch;
                current_signal ^= NODE_NAME | NODE_OPEN;
            }
            else if(ch == '?' && content[++index] == 'x' && content[++index] == 'm' && content[++index] == 'l')
                current_signal ^= HEADER | NODE_OPEN;
            else if(ch == '!' && content[++index] == '-' && content[++index] == '-') current_signal ^= COMMENT | NODE_OPEN;
            else if(ch == '<' && content[++index] == '!' && content[++index] == '-' && content[++index] == '-')
                current_signal ^= COMMENT;
            else if(ch == '/')  current_signal ^= NODE_CLOSE | NODE_OPEN;
            else throw Exception("a grammar mistake occurs");
        }
        else if((current_signal & ATTRIBUTE_NAME) > 0)
        {
            if(__is_space(ch)) current_signal |= SPACE;
            else if(__is_name(ch))
            {
                if((current_signal & SPACE) > 0)
                    throw Exception("a grammar mistake occurs");
                (*temp) += ch;
            }
            else if(ch == '=')
            {
                value = "";
                temp = &value;
                current_signal ^= ATTRIBUTE_VALUE | ATTRIBUTE_NAME;
            }
            else throw Exception("a grammar mistake occurs");
        }
        else if((current_signal & ATTRIBUTE_VALUE) > 0)
        {
            if(ch == '\"')
            {
                if((current_signal & ATTRIBUTE_VALUE_START) == 0) current_signal |= ATTRIBUTE_VALUE_START;
                else
                {
                    if((current_signal & HEADER) > 0) set_attribute(key, value);
                    else node->top()->set_attribute(key, value);
                    current_signal ^= ATTRIBUTE_VALUE_START | ATTRIBUTE_VALUE;
                }
            }
            else if((current_signal & ATTRIBUTE_VALUE_START) > 0 && ch == '&')
            {
                current_signal |= SPECIAL_CHAR;
                special = ch;
            }
            else if((current_signal & SPECIAL_CHAR) > 0)
            {
                if(ch == ';')
                {
                    current_signal ^= SPECIAL_CHAR;
                    special += ch;
                    auto it = __char_dictionary->find(special);
                    if(it == __char_dictionary->end()) throw Exception("illegal character");
                    (*temp) += (*it).second;
                    special = "";
                }
                else if(__is_word(ch)) special += ch;
                else throw Exception("illegal character");
            }
            else if(__is_space(ch) && (current_signal & ATTRIBUTE_VALUE_START) == 0) current_signal |= SPACE;
            else (*temp) += ch;
        }
        else if((current_signal & HEADER) > 0)
        {
            if(ch == '?' && content[++index] == '>') current_signal ^= HEADER;
            else if(__is_space(ch)) continue;
            else if(__is_name_start(ch))
            {
                key = "";
                temp = &key;
                current_signal |= ATTRIBUTE_NAME;
                (*temp) += ch;
            }
            else throw Exception("illegal character");
        }
        else if((current_signal & NODE_NAME) > 0)
        {
            if(ch == '>')
            {
                if((current_signal & NODE_CLOSE) > 0)
                {
                    if(node->top()->node_name() != (*temp))
                        throw Exception(node->top()->node_name() + " is not matched");
                    std::cout << "node : " << node->top()->node_name() << " is popped" << std::endl;
                    node->pop();
                    std::cout << "now the node : " << (node->size() > 0 ? node->top()->node_name() : "null") << std::endl;
                    current_signal ^= NODE_CLOSE;
                }
                else if((current_signal & SPACE) == 0)
                {
                    XmlNode *new_node = nullptr;
                    if(node->size() > 0)
                    {
                        new_node = new XmlNode(node->top(), *temp);
                        node->push(new_node);
                    }
                    else
                    {
                        new_node = new XmlNode(*temp);
                        node->push(new_node);
                        __Child->push_back(new_node);
                    }
                    std::cout << "node : " << *temp << " push" << std::endl;
                    std::cout << "is's parent is : " << (node->top()->get_parent() != nullptr ? node->top()->get_parent()->node_name() : "null") << std::endl;
                }
                current_signal |= NODE_TEXT;
                current_signal ^= NODE_NAME;
            }
            else if(ch == '/' && content[++index] == '>')
            {
                if(current_signal & SPACE == 0)
                {
                    if(node->top()->node_name() != *temp)
                        throw Exception(node->top()->node_name() + " is not matched");
                    std::cout << "node : " << node->top()->node_name() << " is popped" << std::endl;
                    node->pop();
                    std::cout << "now the node : " << (node->size() > 0 ? node->top()->node_name() : "null") << std::endl;
                }
                current_signal ^= NODE_NAME;
            }
            else if((current_signal & SPACE) > 0)
            {
                if(__is_space(ch)) continue;
                if(__is_name_start(ch))
                {
                    current_signal ^= ATTRIBUTE_NAME | SPACE;
                    key = "";
                    temp = &key;
                    (*temp) += ch;
                }
                else throw Exception("a grammar mistake occurs");
            }
            else if(__is_name(ch)) (*temp) += ch;
            else if(__is_space(ch))
            {
                if((current_signal & NODE_CLOSE) == 0)
                {
                    XmlNode *new_node = nullptr;
                    if(node->size() > 0)
                    {
                        new_node = new XmlNode(node->top(), *temp);
                        node->push(new_node);
                    }
                    else
                    {
                        new_node = new XmlNode(*temp);
                        node->push(new_node);
                        __Child->push_back(new_node);
                    }
                    std::cout << "node : " << *temp << " push" << std::endl;
                    std::cout << "is's parent is : " << (node->top()->get_parent() != nullptr ? node->top()->get_parent()->node_name() : "null") << std::endl;
                    current_signal |= SPACE;
                }
                else current_signal ^= NODE_NAME;
            }
            else throw Exception("illegal character");
        }
        else if((current_signal & NODE_CLOSE) > 0)
        {
            if(__is_space(ch)) continue;
            else if(__is_name_start(ch))
            {
                special = "";
                temp = &special;
                (*temp) += ch;
                current_signal ^= NODE_NAME;
            }
            else throw Exception("a grammar mistake occurs");
        }
        else throw Exception("a grammar mistake occurs");
    }
    if(node->size() > 0) throw Exception(node->top()->node_name() + " is not matched");
    delete node;
}

void XmlDocument::save(int indent)
{
    if(__file_name == nullptr || (*__file_name) == "") throw Exception("file name is empty");
    std::ofstream file(*__file_name);
    if(__Attributes->size() > 0)
    {
        __Attr_iter it_end = __Attributes->end();
        file << "<?xml ";
        for(__Attr_iter it = __Attributes->begin(); it != it_end; it++)
        {
            file << (*it).first << "=\"" << (*it).second << "\" ";
        }
        file << "?>" << std::endl;
    }
    XmlNode *temp = get_root_node();
    if(temp != nullptr)
    {
        __write_child_info(file, temp, 0, indent);
    }
    file.close();
}

void XmlDocument::save_as(std::string filename, int indent)
{
    if(__file_name == nullptr) __file_name = new std::string(filename);
    else *__file_name = filename;
    save(indent);
}

void XmlDocument::remove_child(XmlNode *node)
{
    for(__Iter it = __Child->begin(); it != __Child->end(); it++)
    {
        if((*it) == node)
        {
            __Child->erase(it);
            break;
        }
    }
}

#endif // XMLDOCUMENT_H
