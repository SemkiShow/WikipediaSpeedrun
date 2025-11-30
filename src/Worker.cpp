// SPDX-FileCopyrightText: 2025 SemkiShow
//
// SPDX-License-Identifier: GPL-3.0-only

#include "Worker.hpp"
#include <QThread>

void RunWorker(QObject* context, std::function<void(Worker*)> task,
               std::function<void(const QString&)> onProgress,
               std::function<void(int)> onProgressMax, std::function<void()> onFinished)
{
    QThread* thread = new QThread();
    Worker* worker = new Worker(task);
    worker->moveToThread(thread);

    QObject::connect(thread, &QThread::started, worker, &Worker::run);
    QObject::connect(worker, &Worker::finished, thread, &QThread::quit);
    QObject::connect(worker, &Worker::finished, worker, &Worker::deleteLater);
    QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    QObject::connect(worker, &Worker::progress, context,
                     [onProgress](const QString& msg) { onProgress(msg); });

    QObject::connect(worker, &Worker::progressMax, context,
                     [onProgressMax](int value) { onProgressMax(value); });

    QObject::connect(worker, &Worker::finished, context, [onFinished]() { onFinished(); });

    thread->start();
}
