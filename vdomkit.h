#ifndef VDOMKIT_H
#define VDOMKIT_H

#include <qwebpage.h>
#include <qwebframe.h>
#include <qwebvdom.h>
#include <qwebsettings.h>

#include <QEventLoop>
#include <QDebug>
#include <QTime>

#include "vdomwebpage.h"

class VDomKit : public QObject
{
    Q_OBJECT

public:
    VDomKit(VDomWebPage* page, int timeout = 0)
            : m_page(page)
            , m_webvdom(new QWebVDom(page->mainFrame()))
            , m_timeout(timeout)
    {
        QWebSettings* settings = m_page->settings();
        settings->setObjectCacheCapacities(0, 0, 0);
        //m_page->settings()->setMaximumPagesInCache(4);
        //m_page->settings()->setObjectCacheCapacities((16*1024*1024) / 8, (16*1024*1024) / 8, 16*1024*1024);
    }

    void setViewportSize(const QSize&);
    void setJavascriptEnabled(bool);
    void setAutoLoadImage(bool);
    void setPluginsEnabled(bool);
    void setJavaEnabled(bool);
    void setTimeout(int timeout);
    void setUserAgent(const QString& userAgent);
    QString userAgentForUrl(const QUrl& url) const;

    QWebPage* page();

    const QByteArray& dump(const QByteArray& url);
protected:
    VDomWebPage* m_page;
    QWebVDom* m_webvdom;
    int m_timeout;
};

#endif
