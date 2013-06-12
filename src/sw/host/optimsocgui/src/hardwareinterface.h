/**
 * This file is part of OpTiMSoC-GUI.
 *
 * OpTiMSoC-GUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * OpTiMSoC-GUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with OpTiMSoC. If not, see <http://www.gnu.org/licenses/>.
 *
 * =================================================================
 *
 * Driver for the simple message passing hardware.
 *
 * (c) 2012-2013 by the author(s)
 *
 * Author(s):
 *    Philipp Wagner, philipp.wagner@tum.de
 *    Stefan Wallentowitz, stefan.wallentowitz@tum.de
 */

#ifndef HARDWAREINTERFACE_H
#define HARDWAREINTERFACE_H

#include <QThread>
#include <QMap>

#include "liboptimsochost.h"

/**
 * Hardware Interface: connect with the OpTiMSoC system
 *
 * This class is implemented as singleton, use instance() to get an
 * instance of it.
 */
class HardwareInterface : public QObject
{
    Q_OBJECT

public:
    enum ConnectionStatus {
        Connected,
        Connecting,
        Disconnected,
        Disconnecting
    };

    static HardwareInterface* instance();
    static void instrTraceCallback(struct optimsoc_ctx *incoming_ctx,
                                   int core_id, uint32_t timestamp,
                                   uint32_t pc, int count);
    static void softwareTraceCallback(uint32_t core_id, uint32_t timestamp,
                                      uint16_t id, uint32_t value);

    virtual ~HardwareInterface();

    void emitInstructionTraceReceived(int core_id, uint32_t timestamp,
                                      uint32_t pc, int count);
    void emitSoftwareTraceReceived(uint32_t core_id, uint32_t timestamp,
                                   uint16_t id, uint32_t value);

    void configure(optimsoc_backend_id type, QMap<QString,QString> options);
    bool configured();

public slots:
    void threadStarted() {}
    void threadFinished() {}

    void connect();
    void disconnect();
    void initMemory(int memoryTileId, QByteArray data);
    void startCpus();
    void reset();

private:
    static HardwareInterface *s_instance;
    struct optimsoc_ctx *m_octx;
    ConnectionStatus m_connectionStatus;

    HardwareInterface(QObject *parent = 0);

signals:
    void connectionStatusChanged(HardwareInterface::ConnectionStatus oldStatus,
                                 HardwareInterface::ConnectionStatus newStatus);
    void systemDiscovered(int systemId);
    void memoryWriteFinished(bool success);
    void instructionTraceReceived(int core_id, unsigned int timestamp,
                                  unsigned int pc, int count);
    void softwareTraceReceived(unsigned int core_id, unsigned int timestamp,
                               unsigned int id, unsigned int value);

};

#endif // HARDWAREINTERFACE_H