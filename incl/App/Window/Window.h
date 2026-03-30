#ifndef APP_WINDOW_WINDOW_H
#define APP_WINDOW_WINDOW_H

#include "App/Types.h"

namespace APP {
    namespace Window {
        Main_Window* init_backend();
        void WindowLoop(Main_Window* window_main);
        Main_Window* Window_running();
        void cleanup(Main_Window* window_main);
    }
}

#endif
