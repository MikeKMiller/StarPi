/**
 * The stellarium telescope library helps building
 * telescope server programs, that can communicate with stellarium
 * by means of the stellarium TCP telescope protocol.
 * It also contains smaple server classes (dummy, Meade LX200).
 *
 * Author and Copyright of this file and of the stellarium telescope library:
 * Johannes Gajdosik, 2006
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Socket.hpp"
#include <stdint.h>

/** Connection Class
 * TCP/IP connection to a client.
 */
class Connection : public Socket
{
    public:
    /** Constructor
     */
        Connection( Server &server, SOCKET Fd );
    /** A get function for the difference in server times
     * @return int64_t the difference in times
     */
        int64_t GetServerMinusClientTime( void );
     
    protected:
    /** Receives data from a TCP/IP connection and stores it in the read buffer.
     */
        void PerformReading( void );
    /** Sends the contents of the write buffer over a TCP/IP connection.
     */
        void PerformWriting( void );
    /** Performs TCP/IP communication and handles new connections.
     * If a new connection is established, creates a new Connection object
     * and passes it to the parent Server with Server::addConnection().
     * @param ReadFds reference
     * @param WriteFds reference
     * @param FdMax reference
     */
        void PrepareSelectFds( fd_set &ReadFds, fd_set &WriteFds, int16_t &FdMax );
    
    private:
    /** Returns true, as by default Connection implements a TCP/IP connection.
     */
        virtual bool IsTcpConnection( void ) { return true; }
    /** Returns false, as by default Connection implements a TCP/IP connection.
     */
        virtual bool IsAsciiConnection( void ){ return false; }
    /** Handle the selected rw files
     * @param ReadFds reference to the read file
     * @param WriteFds reference to the write file
     */
        void HandleSelectFds( const fd_set &ReadFds, const fd_set &WriteFds );
    /** Parses the read buffer and handles any messages contained within it.
     * If the data contains a Stellarium telescope control command,
     * dataReceived() calls the appropriate method of Server.
     * For example, "MessageGoto" (type 0) causes a call to Server::gotoReceived().
     * @param BufferPtr reference to the buffer pointer
     * @param ReadBuffEnd pointer to the end of the buffer
     */
        virtual void DataReceived( const uint8_t* &BufferPtr, const uint8_t *ReadBuffEnd );
    /** Composes a "MessageCurrentPosition" in the write buffer.
     * This is a Stellarium telescope control protocol message containing
     * the current right ascension, declination and status of the telescope mount.
     * @param RAInt uint32_T version of the Right Ascension
     * @param DecInt int32_t version of the Declination
     * @param Status int32_t version of the 
     */
        void SendPosition( uint32_t RAInt, int32_t DecInt, int32_t Status );
    
    protected:
        uint8_t ReadBuff[120];             /**< Read buffer */
        uint8_t *ReadBuffEnd;              /**< End of read buffer */
        uint8_t WriteBuff[120];            /**< Write buffer */
        uint8_t *WriteBuffEnd;             /**< end of write buffer */
    
    private:
        int64_t ServerMinusClientTime;  /** difference in the client and server times */
};

#endif /* CONNECTION_HPP */
