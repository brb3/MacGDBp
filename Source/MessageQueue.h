/*
 * MacGDBp
 * Copyright (c) 2013, Blue Static <http://www.bluestatic.org>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */

#import <Foundation/Foundation.h>

#import "BSProtocolThreadInvoker.h"

@protocol MessageQueueDelegate;

// MessageQueue operates a listening socket, that is connected to another
// program with which it exchanges UTF8 string messages. A message contains two
// parts, both terminated by '\0'. The first is an ASCII integer number that is
// the length of the second part. The second part is the actual string message.
@interface MessageQueue : NSObject

// Creates a new MessasgeQueue that will listen on |port| and report information
// to its |delegate|.
- (id)initWithPort:(NSUInteger)port delegate:(id<MessageQueueDelegate>)delegate;

// Whether or not the message queue has attached itself to a child.
- (BOOL)isConnected;

// Opens a socket that will listen for connections.
- (void)connect;

// Closes either the listening or child socket and completely disconnects.
- (void)disconnect;

// Enqueues a |message| to be sent to the client. This may be called from any
// thread.
- (void)sendMessage:(NSString*)message;

@end

// Delegate ////////////////////////////////////////////////////////////////////

// The delegate for the message queue. These methods are called from the thread
// on which the MessageQueue was initialized.
@protocol MessageQueueDelegate <NSObject>
// Callback for any errors that the MessageQueue encounters.
- (void)messageQueue:(MessageQueue*)queue error:(NSError*)error;

// Called when the listening socket has accepted a child socket.
- (void)messageQueueDidConnect:(MessageQueue*)queue;

// Called when the child socket has been disconnected.
- (void)messageQueueDidDisconnect:(MessageQueue*)queue;

// Callback for when a message has been sent.
- (void)messageQueue:(MessageQueue*)queue didSendMessage:(NSString*)message;

// Callback with the message content when one has been receieved.
- (void)messageQueue:(MessageQueue*)queue didReceiveMessage:(NSString*)message;
@end
