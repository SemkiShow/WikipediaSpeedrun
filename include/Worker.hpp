// SPDX-FileCopyrightText: 2025 SemkiShow
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QObject>
#include <functional>

class Worker : public QObject
{
    Q_OBJECT
  public:
    using Task = std::function<void(Worker*)>;
    explicit Worker(Task task) : _task(std::move(task)), counter(0) {}
    void SendProgress(const QString& message, int dropCount = 0)
    {
        if (++counter <= dropCount) return;
        emit progress(message);
        counter = 0;
    }

  signals:
    void progress(const QString& message);
    void progressMax(int value);
    void finished();
    void error(const QString& message);

  public slots:
    void run()
    {
        try
        {
            _task(this);
            emit finished();
        }
        catch (const std::exception& e)
        {
            emit error(e.what());
        }
    }

  private:
    Task _task;
    int counter = 0;
};

void RunWorker(QObject* context, std::function<void(Worker*)> task,
               std::function<void(const QString&)> onProgress,
               std::function<void(int)> onProgressMax, std::function<void()> onFinished);
