#include <QEventLoop>
#include <QSignalSpy>
#include <QTimer>
//#include <QDebug>

#include "vdomkit.h"
#include "vdomkitloop.h"

const QByteArray NullString = "";

void VDomKit::setViewportSize(const QSize& size)
{
    m_page->setViewportSize(size);
}

void VDomKit::setJavascriptEnabled(bool en)
{
    m_page->settings()->setAttribute(QWebSettings::JavascriptEnabled, en);
}
void VDomKit::setAutoLoadImage(bool en)
{
    m_page->settings()->setAttribute(QWebSettings::AutoLoadImages, en);
}

void VDomKit::setJavaEnabled(bool en)
{
    m_page->settings()->setAttribute(QWebSettings::JavaEnabled, en);
}

void VDomKit::setPluginsEnabled(bool en)
{
    m_page->settings()->setAttribute(QWebSettings::PluginsEnabled, en);
}

void VDomKit::setTimeout(int timeout)
{
    m_timeout = timeout;
}

void VDomKit::setUserAgent(const QString& userAgent)
{
    m_page->setUserAgent(userAgent);
}

QWebPage* VDomKit::page()
{
    return m_page;
}
/*
void VDomKit::start()
{
    connect(m_page, SIGNAL(loadFinished(bool)), this, SLOT(load()));
}
*/
const QByteArray& VDomKit::dump(const QByteArray &url) {
    QUrl qurl;
    qurl.setEncodedUrl(url, QUrl::StrictMode);
    //qDebug() << qurl << endl;
    if (qurl.isValid()) {

#ifdef ENABLE_TIMER
        QTime t;
        t.start();
#endif
        m_page->mainFrame()->load(qurl);

        VDomKitLoop loop;
        QObject::connect(m_page, SIGNAL(loadFinished(bool)), &loop,
                SLOT(quitWithRes(bool)));

        QTimer timer;
        QSignalSpy timeoutSpy(&timer, SIGNAL(timeout()));
        if (m_timeout > 0) {
            QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
            timer.setSingleShot(true);
            timer.start(m_timeout);
        }
        loop.exec();

        if (!timeoutSpy.isEmpty()) {
            fprintf(stderr, "Page loading timeout (exceeded %d ms): %s\n",
                m_timeout, url.data());
            m_page->blockSignals(true);
            m_page->triggerAction(QWebPage::Stop);
            m_page->blockSignals(false);
            //return NullString;
        } else if (!loop.getRes()) {
            fprintf(stderr, "Failed to load page: %s\n", url.data());
            return NullString;
        }

#ifdef ENABLE_TIMER
        qDebug("loading elapsed: %d ms", t.elapsed());
        t.restart();
#endif

        const QByteArray& retval = m_webvdom->dump();

#ifdef ENABLE_TIMER
        qDebug("dumping elapsed: %d ms", t.elapsed());
#endif
        return retval;
    } else {
        return NullString;
    }
}

