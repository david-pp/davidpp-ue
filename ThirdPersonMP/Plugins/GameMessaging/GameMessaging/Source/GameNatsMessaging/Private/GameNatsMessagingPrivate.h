// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/* Private constants
 *****************************************************************************/

/** Declares a log category for this module. */
// DECLARE_LOG_CATEGORY_EXTERN(LogNatsMessaging, Log, All);

/** Defines the maximum number of annotations a message can have. */
#define TCP_MESSAGING_MAX_ANNOTATIONS 128

/** Defines the maximum number of recipients a message can have. */
#define TCP_MESSAGING_MAX_RECIPIENTS 1024

/** Defines the desired size of socket send buffers (in bytes). */
#define TCP_MESSAGING_SEND_BUFFER_SIZE 2 * 1024 * 1024

/** Defines the desired size of socket receive buffers (in bytes). */
#define TCP_MESSAGING_RECEIVE_BUFFER_SIZE 2 * 1024 * 1024

/** Defines a magic number for the the TCP message transport. */
#define TCP_MESSAGING_TRANSPORT_PROTOCOL_MAGIC 0x45504943

