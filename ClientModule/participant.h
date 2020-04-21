//
// Created by giova on 10/10/2019.
//

#ifndef SERVERMODULE_PARTICIPANT_H
#define SERVERMODULE_PARTICIPANT_H

#include <vector>
#include "message.h"
#include "symbol.h"
//#include "msgInfo.h"
#include "symbolInfo.h"

class participant {

private:
    int _siteId;
    std::string currentFile;
    std::string username;
    std::string color;
    std::vector<symbol> _symbols;
    int _counter = 0;
    std::vector<int> generatePos(int index);
    std::vector<int> generatePosBetween(std::vector<int> pos1, std::vector<int> pos2, std::vector<int> newPos = {});
    int comparePosdx(std::vector<int> curSymPos, std::vector<int> newSymPos, int posIndex);
    int comparePos(std::vector<int> curSymPos, std::vector<int> newSymPos, int posIndex);
    enum formatType {MAKE_BOLD=0, MAKE_ITALIC=1, MAKE_UNDERLINE=2, UNMAKE_BOLD=3, UNMAKE_ITALIC=4, UNMAKE_UNDERLINE=5, FORMAT_UNKNOWN=6};

public:
    ~participant() {}
    //virtual void deliver(const message& msg) = 0;
    int getId() const;
    std::vector<int> localInsert(int index, wchar_t value, symbolStyle style);
    void localInsert(std::vector<symbolInfo> symbols);
    void localErase(int startIndex, int endIndex);
    void localFormat(int startIndex, int endIndex, int format);
    void localFontSizeChange(int startIndex, int endIndex, int fontSize);
    void localFontFamilyChange(int startIndex, int endIndex, const std::string& fontFamily);
    void localAlignmentChange(int startIndex, int endIndex, int alignment);
    //void process(const msgInfo& m);
    std::string to_string();
    std::vector<symbol> getSymbols(); //maybe we can use pointer (symbol*)
    void setSymbols(std::vector<symbol> symbols);
    void setCurrentFile(std::string uri);
    void setUsername(std::string userName);
    void setColor(std::string color);
    void setSiteId(int edId);
    std::string getCurrentFile();
    std::string getUsername();
    std::string getColor();
};


#endif //SERVERMODULE_PARTICIPANT_H