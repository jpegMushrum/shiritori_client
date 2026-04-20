#ifndef SERVICETHREAD_H
#define SERVICETHREAD_H

#include <QThread>
#include <QObject>

class TcpClient;
class ApiService;

/**
 * ServiceThread manages TcpClient and ApiService in a separate thread.
 * This keeps network I/O operations off the UI thread.
 */
class ServiceThread : public QObject
{
    Q_OBJECT

public:
    explicit ServiceThread(QObject *parent = nullptr);
    ~ServiceThread();

    // Get pointers to services (safe to use from any thread via signals/slots)
    TcpClient *getTcpClient() const { return m_tcpClient; }
    ApiService *getApiService() const { return m_apiService; }

    // Start the service thread
    void start();

    // Stop the service thread
    void stop();

    bool isRunning() const;

private:
    TcpClient *m_tcpClient = nullptr;
    ApiService *m_apiService = nullptr;
    QThread *m_thread = nullptr;
};

#endif // SERVICETHREAD_H
