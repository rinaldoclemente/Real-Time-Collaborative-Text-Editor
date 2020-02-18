//
// Created by giova on 31/10/2019.
//

#ifndef SERVERMODULE_SYMBOL_STYLE_H
#define SERVERMODULE_SYMBOL_STYLE_H

#include <vector>
#include <string>
#include <array>

#define DEFAULT_FONT_FAMILY "Times New Roman"
#define DEFAULT_FONT_SIZE 14

class symbolStyle {

private:
    bool _isBold;
    bool _isItalic;
    bool _isUnderlined;
    std::string _fontFamily;
    int _fontSize;

public:
    /* Constructors */
    symbolStyle(); //default style parameters
    symbolStyle(bool isBold, bool isItalic, bool isUnderlined, std::string fontFamily, int fontSize);

    /* Getters */
    std::string getFontFamily() const;
    int getFontSize() const;
    bool isBold() const;
    bool isItalic() const;
    bool isUnderlined() const;

    /* Setters */
    void setBold(bool val);
    void setItalic(bool val);
    void setUnderlined(bool val);
    void setFontFamily(std::string fontFamily);
    void setFontSize(int value);
};


#endif //SERVERMODULE_SYMBOL_STYLE_H
