
#include "bzrannotatetask.h"

#include <functional>

#include <QtCore/QTimer>
#include <QtCore/QDateTime>
#include <QtCore/QDir>

#include <vcs/dvcs/dvcsjob.h>
#include <vcs/vcsannotation.h>
#include <vcs/vcsrevision.h>


BzrAnnotateTask::BzrAnnotateTask(KDevelop::DVcsJob* job, QObject* parent)
    : QObject(parent), _job(job)
{
    connect(this, SIGNAL(resultIsReady()), this, SLOT(setResultOfJobAndFinish()));
}

void BzrAnnotateTask::start()
{
    _outputLines = _job->output().split('\n');
    _currentLine = 0;

    QTimer::singleShot(0, this, SLOT(parseNextLine()));
}

void BzrAnnotateTask::parseNextLine()
{
    if (_currentLine == _outputLines.size()) {
        emit resultIsReady();
        return;
    }
    QString currentLine = _outputLines[_currentLine];
    if (currentLine.isEmpty()) {
        ++_currentLine;
        QTimer::singleShot(0, this, SLOT(parseNextLine()));
        return;
    }
    auto revision = currentLine.left(currentLine.indexOf(' ')).toULong();
    auto i = _commits.find(revision);
    if (i != _commits.end()) {
        KDevelop::VcsAnnotationLine line;
        line.setAuthor(i->second.author());
        line.setCommitMessage(i->second.message());
        line.setDate(i->second.date());
        line.setLineNumber(_currentLine);
        line.setRevision(i->second.revision());
        _results.append(QVariant::fromValue(line));
        ++_currentLine;
        QTimer::singleShot(0, this, SLOT(parseNextLine()));
    } else {
        prepareCommitInfo(revision);
    }
}

void BzrAnnotateTask::setResultOfJobAndFinish()
{
    _job->setResults(_results);         // FIXME CRASH
    deleteLater();
}

void BzrAnnotateTask::prepareCommitInfo(std::size_t revision)
{
    KDevelop::DVcsJob* job = new KDevelop::DVcsJob(_job->directory(), _job->vcsPlugin(), KDevelop::OutputJob::Silent);
    job->setType(KDevelop::VcsJob::Log);
    *job << "bzr" << "log" << "--long" << "-r" << QString::number(revision);
    connect(job, SIGNAL(readyForParsing(KDevelop::DVcsJob*)), this, SLOT(parseBzrLog(KDevelop::DVcsJob*)));
    job->start();
}

void BzrAnnotateTask::parseBzrLog(KDevelop::DVcsJob* job)
{
    QStringList outputLines = job->output().split('\n');
    KDevelop::VcsEvent commitInfo;
    std::size_t revision;
    for (QString line : outputLines) {
        if (line.startsWith("revno")) {
            QString revno = line.right(line.length() - 1 - line.lastIndexOf(' '));
            revision=revno.toULong();
            KDevelop::VcsRevision revision;
            revision.setRevisionValue(revno.toLongLong(),KDevelop::VcsRevision::GlobalNumber);
            commitInfo.setRevision(revision);
        }
        else if (line.startsWith("commiter")) {
            // TODO implementation
        }
    }
    _commits[revision]=commitInfo;
    QTimer::singleShot(0, this, SLOT(parseNextLine()));
}


#include "bzrannotatetask.moc"
