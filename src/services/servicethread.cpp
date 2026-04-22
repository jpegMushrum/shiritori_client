#include "servicethread.h"
#include "tcpclient.h"
#include "apiservice.h"
#include <QDebug>

ServiceThread::ServiceThread(QObject *parent)
    : QObject(parent), m_thread(nullptr)
{
}

ServiceThread::~ServiceThread()
{
    stop();
}

void ServiceThread::start()
{
    if (m_thread)
    {
        qWarning() << "ServiceThread already running";
        return;
    }

    m_thread = new QThread;

    // Create services - they will be moved to the thread
    m_tcpClient = new TcpClient;
    m_apiService = new ApiService(m_tcpClient);

    // Move services to the service thread
    m_tcpClient->moveToThread(m_thread);
    m_apiService->moveToThread(m_thread);

    connect(m_thread, &QThread::started, this, [this]()
            { qDebug() << "Service thread started"; });

    connect(m_thread, &QThread::finished, this, [this]()
            {
        qDebug() << "Service thread finished";
        m_tcpClient = nullptr;
        m_apiService = nullptr; });

    connect(m_thread, &QThread::finished, m_tcpClient, &QObject::deleteLater);
    connect(m_thread, &QThread::finished, m_apiService, &QObject::deleteLater);

    // Start the thread
    m_thread->start();
}

void ServiceThread::stop()
{
    if (!m_thread)
    {
        return;
    }

    // Quit and wait for thread to finish
    m_thread->quit();
    m_thread->wait();

    delete m_thread;
    m_thread = nullptr;
}

bool ServiceThread::isRunning() const
{
    return m_thread && m_thread->isRunning();
}
