#ifndef _TEXTCTRLSTREAMBUF_H_
#define _TEXTCTRLSTREAMBUF_H_

#include <wx/wx.h>
#include <iostream>
#include <streambuf>

class TextCtrlStreambuf : public std::streambuf {
public:
    TextCtrlStreambuf(wxTextCtrl* textCtrl) : textCtrl(textCtrl) {}

protected:
    virtual int_type overflow(int_type c = traits_type::eof()) override {
        if (c != traits_type::eof()) {
            textCtrl->AppendText(wxString(static_cast<wxChar>(c)));
        }
        return c;
    }

    virtual std::streamsize xsputn(const char* s, std::streamsize n) override {
        textCtrl->AppendText(wxString(s, n));
        return n;
    }

private:
    wxTextCtrl* textCtrl;
};

#endif