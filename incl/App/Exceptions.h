#ifndef APP_EXCEPTIONS_H
#define APP_EXCEPTIONS_H

#include <exception>

namespace APP {
    class init_except : public std::exception {
        virtual const char *what() const throw() {
            return "INIT_ERROR";
        }
    };

    class aborted_window : public std::exception {
        virtual const char *what() const throw() {
            return "window aborted abnormaly, maybe ESC";
        }
    };
}

#endif
