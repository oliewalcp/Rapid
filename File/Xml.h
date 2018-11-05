#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
#include <string>
#include <unordered_map>
#include <fstream>
#include "../Container/Stack.h"
#include "../Container/DoubleLinkedList.h"
#include "FileBase.h"

class XmlNode
{
private:
    typedef DoubleLinkedList<XmlNode *> __List;
    typedef __List::iterator __Iter;
    typedef std::unordered_map<std::string, std::string> __Map;

    std::string __name;
    std::string __text;
    __List __child;
    XmlNode *__parent;
    __Map *__attributes;
public:

    XmlNode(std::string name, std::string text = "") : __text(text), __parent(nullptr), __attributes(new __Map), __name(name) {}
    XmlNode(XmlNode *parent, std::string text = "") : __parent(parent), __text(text), __attributes(new __Map) {}
    ~XmlNode()
    {
        if(__parent != nullptr)
            delete __parent;
        delete __attributes;
    }
    void set_attribute(std::string key, std::string value)
    {
        (*__attributes)[key] = value;
    }
    std::string get_attribute(std::string key)
    {
        __Map::iterator it = __attributes->find(key);
        if(it == __attributes->end()) return "";
        return (*it).second;
    }
    inline void set_text(std::string arg)
    {
        __text = arg;
    }
    inline std::string get_text()
    {
        return __text;
    }
    inline void set_parent(XmlNode *parent)
    {
        this->__parent = parent;
    }
    inline XmlNode* get_parent()
    {
        return __parent;
    }
    inline void append_child(XmlNode *node)
    {
        __child.push_back(node);
        node->set_parent(this);
    }
    inline std::string get_name()
    {
        return __name;
    }
    inline const __List& get_children()
    {
        return __child;
    }
    void remove_child(std::string name)
    {
        for(__Iter it = __child.begin(); it != __child.end(); it++)
        {
            if((*it)->get_name() == name)
            {
                __child.erase(it);
                break;
            }
        }
    }
    void remove_children(std::string name)
    {
        for(__Iter it = __child.begin(); it != __child.end(); it++)
        {
            if((*it)->get_name() == name)
            {
                __Iter temp = it;
                it++;
                __child.erase(temp);
            }
        }
    }
};

class XmlDocument
{
private:
    typedef DoubleLinkedList<XmlNode *> __List;
    typedef __List::iterator __Iter;
    typedef std::unordered_map<std::string, std::string> __Map;
    typedef unsigned long long __uint64;

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
    __List *__root;
    __Map *__header;
    __Map *__char_dictionary;

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
        return __is_name_start(ch) || (ch >= '0' && ch <= '9');
    }
    inline bool __is_space(const char ch)
    {
        return (ch == '\t' || ch == '\n' || ch == ' ' || ch == '\r');
    }
public:
    XmlDocument(std::string filename = "") : __root(nullptr), __header(new __Map), __file_name(nullptr)
    {
        if(filename != "")
            parse(filename);
        __char_dictionary = new __Map();
        (*__char_dictionary)["lt"] = "<";
        (*__char_dictionary)["gt"] = ">";
        (*__char_dictionary)["amp"] = "&";
        (*__char_dictionary)["quot"] = "\"";
        (*__char_dictionary)["apos"] = "'";
    }
    ~XmlDocument()
    {
        if(__file_name != nullptr)
            delete __file_name;
        if(__root != nullptr)
            delete __root;
        delete __header;
    }
    inline void set_attribute(std::string key, std::string value)
    {
        (*__header)[key] = value;
    }
    inline XmlNode* get_root_node()
    {
        return __root->front();
    }
    std::string get_attribute(std::string key)
    {
        __Map::iterator it = __header->find(key);
        if(it == __header->end()) return "";
        return (*it).second;
    }
    void append_child(XmlNode *root)
    {
        __root->push_back(root);
    }
    void parse(std::string filename);
    void save();
    void save_as(std::string filename);
    void remove_child(XmlNode *node);
};

void XmlDocument::parse(std::string filename)
{
    __file_name = new std::string(filename);
    std::string file_content = FileBaes::open(filename.data());
//    std::ifstream file(filename);
//    if(!file) throw Exception("fail to open file");
    Stack<std::string> *node = new Stack<std::string>;
    __uint64 current_signal = START;
    std::string key = "", value = "", special = "";
    std::string *temp = &key;
    const char* content = file_content.data();
    for(__uint64 index = 0; index < file_content.length(); index++)
    {
        ch = content[index];
        if(current_signal == START)
        {
            if(__is_space(ch)) continue;
            if(ch == '<') current_signal = NODE_OPEN;
        }
        else if(current_signal & COMMENT > 0)
        {
            if(ch == '-' && content[++index] == '-' && content[++index] == '>')
                current_signal ^= COMMENT;
        }
        else if(current_signal == NODE_OPEN)
        {
            if(ch == '?') current_signal = HEADER;
            else if(ch == '>') current_signal = START;
            else if(__is_name_start(ch))
            {
                current_signal = NODE_NAME;
            }
            else if(ch == '!' && content[++index] == '-' && content[++index] == '-') current_signal = COMMENT;
            else throw Exception("a grammar mistake occurs");
        }
        else if(current_signal & HEADER > 0)
        {
            if(current_signal & ATTRIBUTE_NAME > 0)
            {
                if(__is_space(ch) && (*temp) != "") current_signal |= SPACE;
                else if(__is_name(ch))
                {
                    if(current_signal & SPACE > 0)
                        throw Exception("a grammar mistake occurs");
                    (*temp) += ch;
                }
                else if(ch == '=')
                {
                    temp = &value;
                    current_signal = ATTRIBUTE_VALUE | HEADER;
                }
            }
            else if(current_signal & ATTRIBUTE_VALUE > 0)
            {
                if(ch == '\"')
                {
                    if(current_signal & ATTRIBUTE_VALUE_START == 0) current_signal |= ATTRIBUTE_VALUE_START;
                    else current_signal ^= ATTRIBUTE_VALUE_START | ATTRIBUTE_VALUE;
                }
                else if(current_signal & ATTRIBUTE_VALUE_START > 0 && ch == '&')
                    current_signal |= SPECIAL_CHAR;
                else if(current_signal & SPECIAL_CHAR > 0)
                {
                    if(ch == ';')
                    {
                        current_signal ^= SPECIAL_CHAR;
                        auto it = __char_dictionary->find(special);
                        if(it == __char_dictionary->end()) throw Exception("illegal character");
                        (*temp) += (*it).second;
                        special = "";
                    }
                    else if(__is_word(ch)) special += ch;
                }
                else if(__is_space(ch) && current_signal & ATTRIBUTE_VALUE_START == 0) current_signal |= SPACE;
                else if(__is_name(ch))
                {
                    if(current_signal & SPACE > 0)
                        throw Exception("a grammar mistake occurs");
                    (*temp) += ch;
                }
            }
            else if(ch == '?') current_signal |= HEADER_END;
            else if(current_signal & HEADER_END > 0 && ch == '>')
            {
                current_signal = START;
            }
            else if(__is_space(ch)) continue;
            else if(__is_name_start(ch))
            {
                current_signal |= ATTRIBUTE_NAME;
                (*temp) += ch;
            }
        }
        else if(current_signal & NODE_NAME)
        {

        }
    }
    if(node->size() > 0 || (current_signal != END && current_signal != HEADER)) throw Exception(node->top() + " is not matched");
    delete node;
}

void XmlDocument::save()
{
    if(__file_name == nullptr) throw Exception("file name is empty");
}

void XmlDocument::save_as(std::string filename)
{
    __file_name = new std::string(filename);
    save();
}

void XmlDocument::remove_child(XmlNode *node)
{
    for(__Iter it = __root->begin(); it != __root->end(); it++)
    {
        if((*it) == node)
        {
            __root->erase(it);
            break;
        }
    }
}

#endif // XMLDOCUMENT_H
