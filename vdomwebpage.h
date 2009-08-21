/*
 *
 *
 */

#ifndef VDOMWEBPAGE_H
#define VDOMWEBPAGE_H

#include <qwebframe.h>
#include <qwebpage.h>
#include <qwebsettings.h>

#include <QtGui>
#include <QDebug>

#include <QString>

class VDomWebPage : public QWebPage
{
public:
    VDomWebPage(QWidget *parent = NULL) : QWebPage(parent) {}

    void setUserAgent(const QString& userAgent);

protected:
    QString chooseFile(QWebFrame *frame, const QString& suggestedFile);
    void javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID);
    bool javaScriptPrompt(QWebFrame* frame, const QString& msg, const QString& defaultValue, QString* result);
    void javaScriptAlert(QWebFrame* frame, const QString& msg);
    bool javaScriptConfirm(QWebFrame* frame, const QString& msg);
    QString userAgentForUrl(const QUrl& url) const;

private:
    QString m_userAgent;
};

#endif
