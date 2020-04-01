//
// Created by giova on 10/10/2019.
//

#ifndef SERVERMODULE_MSGINFO_H
#define SERVERMODULE_MSGINFO_H

#include <string>
#include "symbol.h"

class msgInfo {

private:
    int type; //0 = 'insertion'; 1 = 'removal'; 2 = 'format'; 3 = 'font_size'; 4 = 'font_family'; 5 = 'alignment'; 6 = 'insertion_range'
    int editorId;
    symbol s;
    int newIndex; //this param will assume the meaning of 'format' if we're using type 2 or 'font_size' if we're using type 3
    std::string fontFamily;
    int range;
    std::vector<symbol> symbols;

public:
    msgInfo(int type, int editorId, symbol s);
    msgInfo(int type, int editorId, symbol s, int newIndex);
    msgInfo(int type, int editorId, symbol s, int range, int format);
    msgInfo(int type, int editorId, symbol s, int range, std::string family);
    msgInfo(int type, int editorId, std::vector<symbol> symbols, int startIndex);
    msgInfo(int type, int editorId, symbol s, std::string family);
    msgInfo(int type, int editorId, int range, int alignment);
    int getType() const;
    int getRange() const;
    int getEditorId();
    symbol getSymbol() const;
    int getNewIndex() const;
    std::string getFontFamily() const;
    std::vector<symbol> getSymbolVector() const;
    std::string toString();
};


#endif //SERVERMODULE_MESSAGE_H
