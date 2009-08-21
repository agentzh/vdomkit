#include "fcgi_util.h"
#include <fcgi_stdio.h>
#include <cstdio>
#include <QTextStream>

void enter_fastcgi_loop (VDomKit* kit) {
    while (FCGI_Accept() >= 0) {
        char *query_string = getenv("QUERY_STRING");
        QUrl fakeUrl;
        //QByteArray urlStr(query_string);
        fakeUrl.setEncodedQuery(QByteArray(query_string));
        //urlStr = fakeUrl.toString();
        QString urlStr = fakeUrl.queryItemValue("url");
        if (urlStr.isNull()) {
            printf("Status: 404\r\n\r\nNo 'url' parameter found.\n");
            return;
        }
        //fprintf(stderr, "the target URL: %s\n", urlStr.toUtf8().data());
        const QByteArray& vdom = kit->dump(urlStr.toUtf8());
        printf("Content-type: text/plain; charset=utf-8\r\n\r\n%s", vdom.data());
    }
}

