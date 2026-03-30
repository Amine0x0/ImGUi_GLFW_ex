#ifndef APP_GUI_PANELS_H
#define APP_GUI_PANELS_H

namespace APP {
    namespace GUI {
        enum class RAMPlotType {
            LINE_CHART,
            PIE_CHART,
            POLAR_CHART,
            BAR_CHART
        };

        void StatPanel(void);
        void RAMPlotPanel(void);
    }
}

#endif
