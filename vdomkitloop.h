#ifndef VDOMKITLOOP_H
#define VDOMKITLOOP_H

#include <QEventLoop>

class VDomKitLoop: public QEventLoop {
    Q_OBJECT

public Q_SLOTS:
    void quitWithRes(bool res) {
        m_res = res;
        exit(0);
    }
    bool getRes() {
        return m_res;
    }
private:
    bool m_res;
};

#endif

