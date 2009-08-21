
#include "vdomwebpage.h"

QString VDomWebPage::chooseFile(QWebFrame* /*frame*/, const QString& /*suggestedFile*/) {
  return QString::null;
}

bool VDomWebPage::javaScriptConfirm(QWebFrame* /*frame*/, const QString& /*msg*/) {
  return true;
}

bool VDomWebPage::javaScriptPrompt(QWebFrame* /*frame*/,
                           const QString& /*msg*/,
                           const QString& /*defaultValue*/,
                           QString* /*result*/) {
  return true;
}

void VDomWebPage::javaScriptConsoleMessage(const QString& /*message*/,
                                   int /*lineNumber*/,
                                   const QString& /*sourceID*/) {
  // noop
}


void VDomWebPage::javaScriptAlert(QWebFrame* /*frame*/, const QString& /*msg*/)
{
    // noop
}

QString VDomWebPage::userAgentForUrl(const QUrl& url) const {

  if (!m_userAgent.isNull())
    return m_userAgent;

  return QWebPage::userAgentForUrl(url);
}

void VDomWebPage::setUserAgent(const QString& userAgent) {
    m_userAgent = userAgent;
}

