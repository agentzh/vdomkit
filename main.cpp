#include "vdomkit.h"
#include "vdomkit_version.h"
#include "vdomwebpage.h"
#include "fcgi_util.h"
#include <qwebpage.h>
#include <cstdio>

#include <QNetworkProxy>
#include <QTextStream>
#include <QFile>

static void help(int status_code) {
    fprintf(status_code == 0 ? stdout : stderr,
        "Usage: vdomkit [Options] [URL]\n"
        "Options:\n"
        "  --help                       Print this help page and exit\n"
        "  --enable-js                  Enable JavaScript (default: disabled)\n"
        "  --auto-load-images           Automatic image loading (default: off)\n"
        "  --fastcgi                    Enabled fastcgi(default: off)\n"
        "  --timeout=<int>              Page loading timeout in seconds\n"
        "                               (default: 120)\n"
        "  --proxy=<hostname:port>      Request by http proxy (default: no proxy)\n"
        "  --user-agent=<agent>         User agent (default: none)\n"
        "  --viewport-size=<1024x768>   View port size (default: 1024x768)\n"
        "  --urlfile=<file path>        Url File Path (default: none)\n"
        "  --outfile=<file path>        Dump vdom File Path (default: none)\n"
        "  --outdir=<dir path>          Dump vdom Directory Path (default: none)\n"
        "  --version                    Display version number\n"
        "\n"
        "Copyright (c), Yahoo! China EEEE Works, Alibaba Inc.\n"
    );
    exit(status_code);
}

void streamDump(VDomKit* kit, QTextStream &urlStream,
        const QString &argOutFile, const QString& argOutDir);

void showVersion();

int main(int argc, char **argv) {
    if (argc == 1) {
        fprintf(stderr, "No command line arguments specified.\n\n");
        help(1);
    }

    QApplication app(argc, argv);
    const QStringList args = app.arguments();

    //bool argHelp = false;
    bool argEnabledJavascript = false;
    bool argAutoLoadImage = false;
    bool argEnabledFastCgi = false;

    QString argProxy;
    int argTimeout = 120;
    QString argUserAgent;
    QString argDumpDir;
    int argViewportWidth = 1024;
    int argViewportHeight = 768;
    QString argUrl;
    QString argOutDir;
    QString argUrlFile;
    QString argOutFile;

    for (int i = 1; i < args.count(); i++) {
        QString arg = args.at(i);
        if (arg.indexOf("--help") == 0) {
            //argHelp = true;
            help(0);
        } else if (arg.indexOf("--enable-js") == 0) {
            argEnabledJavascript = true;
        } else if (arg.indexOf("--version") == 0) {
            showVersion();
            return 0;
        } else if (arg.indexOf("--auto-load-image") == 0) {
            argAutoLoadImage = true;
        } else if (arg.indexOf("--fastcgi") == 0) {
            argEnabledFastCgi = true;
        } else if (arg.indexOf("--proxy=") == 0) {
            argProxy = arg.split("=").at(1);
        } else if (arg.indexOf("--urlfile=") == 0) {
            argUrlFile = arg.split("=").at(1);
        }  else if (arg.indexOf("--outdir=") == 0) {
            argOutDir = arg.split("=").at(1);
        } else if (arg.indexOf("--outfile=") == 0) {
            argOutFile = arg.split("=").at(1);
        } else if (arg.indexOf("--user-agent=") == 0) {
            argUserAgent = arg.split("=").at(1);
        } else if (arg.indexOf("--timeout=") == 0) {
            argTimeout = arg.split("=").at(1).toInt();
        } else if (arg.indexOf("--viewport-size=") == 0) {
            QString size = arg.split("=").at(1);
            const QStringList res = size.split("x");
            if (res.count() != 2) {
                fprintf(stderr, "Invalid viewport size: %s\n",
                        size.toUtf8().data());
                exit(1);
            }
            argViewportWidth = res.at(0).toInt();
            argViewportHeight = res.at(1).toInt();
        } else if (arg.indexOf("-") == 0) {
            fprintf(stderr, "Invalid command-line option: %s\n\n", arg.toUtf8().data());
            exit(1);
        } else {
            argUrl = arg;
        }
    }

    if (!argEnabledFastCgi && argUrl.isNull() && argUrlFile.isNull()) {
        fprintf(stderr, "No URL specified.\n\n");
        help(1);
    }

    if (!argOutDir.isNull() && !argOutFile.isNull()) {
        fprintf(stderr, "Can't specified outdir and outfile option all.\n\n");
        help(1);
    }


    //fprintf(stderr, "argUrl: [%s]\n", argUrl.toUtf8().data());

    if (!argProxy.isNull()) {
        const QStringList res = argProxy.split(":");
        if (res.count() != 2) {
            fprintf(stderr, "Invalid proxy syntax: %s\n\n", argProxy.toUtf8().data());
            exit(1);
        }
        QString proxyHost = res.at(0);
        QString proxyPort = res.at(1);
        //fprintf(stderr, "ProxyHost: [%s]\n", proxyHost.toUtf8().data());
        //fprintf(stderr, "ProxyPort: [%s]\n", proxyPort.toUtf8().data());

        QNetworkProxy proxy(QNetworkProxy::HttpProxy, proxyHost, proxyPort.toUShort());
        // XXX should we check return value here?
        QNetworkProxy::setApplicationProxy(proxy);
    }

    if (!argUrl.indexOf("http") == 0 && !argUrl.indexOf("file") == 0) {
        argUrl = "http://" + argUrl;
    }

    VDomWebPage *page = new VDomWebPage();
    VDomKit* kit = new VDomKit(page);

    kit->setJavaEnabled(false);
    kit->setPluginsEnabled(false);
    kit->setJavascriptEnabled(argEnabledJavascript);
    kit->setAutoLoadImage(argAutoLoadImage);
    kit->setTimeout(argTimeout * 1000);
    kit->setUserAgent(argUserAgent);
    kit->setViewportSize(QSize(argViewportWidth, argViewportHeight));

    if (argEnabledFastCgi) {
        enter_fastcgi_loop(kit);
    } else {
        if (argUrlFile.isNull()) {
            const QByteArray& vdom = kit->dump(argUrl.toUtf8());
            fprintf(stdout, "%s", vdom.data());
        } else {
            if (argUrlFile != "-") {
                QFile urlFile(argUrlFile);
                if (urlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream urlStream(&urlFile);
                    streamDump(kit, urlStream, argOutFile, argOutDir);
                } else {
                    fprintf(stderr, "Cannot open url file: %s.\n\n", argUrlFile.toUtf8().data());
                    help(1);
                }
            } else {
                QTextStream urlStream(stdin);
                streamDump(kit, urlStream, argOutFile, argOutDir);
            }
        }
    }
    //return app.exec();
}

void streamDump(VDomKit* kit, QTextStream &urlStream, const QString &argOutFile, const QString& argOutDir)
{
    while (true) {
        QString url = urlStream.readLine();
        if (url.isNull()) {
            break;
        }
        const QByteArray& vdom = kit->dump(url.toUtf8());
        if (!argOutDir.isNull()) {
            QString dumpFileName = url;
            dumpFileName.replace(QRegExp("^\\w+://"), "");
            dumpFileName.replace(QRegExp("[^-A-Za-z0-9_]"), "_");
            QString dumpPath =  argOutDir + "/" + dumpFileName + ".dom";
            FILE * pFile;
            pFile = fopen(dumpPath.toUtf8().data(), "w");
            if (pFile) {
                fprintf(pFile, "%s", vdom.data());
                fclose(pFile);
            } else {
                fprintf(stderr, "Cannot open dump file: %s.\n\n", dumpPath.toUtf8().data());
                help(1);
            }
        } else if (!argOutFile.isNull()) {
            FILE * pFile;
            pFile = fopen (argOutFile.toUtf8().data(), "w");
            if (pFile) {
                fprintf(pFile, "%s", vdom.data());
                fclose(pFile);
            } else {
                fprintf(stderr, "Cannot open dump file: %s.\n\n", argOutFile.toUtf8().data());
                help(1);
            }
        } else {
            fprintf(stdout, "%s", vdom.data());
        }
    }
}

void showVersion () {
    printf("vdomkit %d.%d.%d\n", VDOMKIT_MAJORVERSION_NUMBER,
        VDOMKIT_MINORVERSION_NUMBER, VDOMKIT_PATCHLEVEL_NUMBER);
}

