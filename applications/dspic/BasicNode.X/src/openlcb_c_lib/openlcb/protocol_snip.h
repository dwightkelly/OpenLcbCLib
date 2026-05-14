/** \copyright
 * Copyright (c) 2024, Jim Kueneman
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file protocol_snip.h
 * @brief Simple Node Information Protocol (SNIP) handler.
 *
 * @details Returns manufacturer info (from node params) and user
 * name/description (from config memory) in a single reply message.  Provides
 * individual field loaders for assembling the SNIP payload and a validation
 * function for incoming SNIP replies.
 *
 * @author Jim Kueneman
 * @date 28 Apr 2026
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef __OPENLCB_PROTOCOL_SNIP__
#define __OPENLCB_PROTOCOL_SNIP__

#include <stdbool.h>
#include <stdint.h>

#include "openlcb_types.h"

    /** @brief Callback interface for SNIP.  config_memory_read is REQUIRED. */
typedef struct {

        /** @brief Read from config memory (ACDI User space) for user name/description.  REQUIRED. */
   uint16_t(*config_memory_read)(openlcb_node_t *openlcb_node, uint32_t address, uint16_t count, configuration_memory_buffer_t *buffer);

        /** @brief A remote node returned a SNIP reply we asked for.  Optional. */
   void (*on_snip_reply)(source_info_t *source, openlcb_msg_t *incoming_msg);

} interface_openlcb_protocol_snip_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

        /**
         * @brief Stores the callback interface.  Call once at startup.
         *
         * @param interface_openlcb_protocol_snip  Pointer to @ref interface_openlcb_protocol_snip_t (must remain valid for application lifetime).
         */
    extern void ProtocolSnip_initialize(const interface_openlcb_protocol_snip_t *interface_openlcb_protocol_snip);

        /**
         * @brief Builds a SNIP reply (MTI 0x0A08) from node params + config memory.
         *
         * @details Assembles eight fields: mfg version, name, model, HW ver, SW ver,
         *          user version, user name, user description.  Max 253 bytes.
         *
         * @param statemachine_info  Pointer to @ref openlcb_statemachine_info_t context.
         */
    extern void ProtocolSnip_handle_simple_node_info_request(openlcb_statemachine_info_t *statemachine_info);

        /**
         * @brief Handles an incoming SNIP reply (MTI 0x0A08).  No automatic response.
         *
         * @param statemachine_info  Pointer to @ref openlcb_statemachine_info_t context.
         */
    extern void ProtocolSnip_handle_simple_node_info_reply(openlcb_statemachine_info_t *statemachine_info);

    // ---- Individual SNIP field loaders (used internally to build the reply) ----
    //
    // All share the same signature:
    //   (node, outgoing_msg, payload_offset, max_bytes) → bytes_written

        /**
         * @brief Copies manufacturer version ID byte (1 byte).
         *
         * @param openlcb_node     Pointer to @ref openlcb_node_t source node.
         * @param outgoing_msg     Pointer to @ref openlcb_msg_t reply being built.
         * @param offset           Payload byte offset to write at.
         * @param requested_bytes  Maximum bytes to copy.
         * @return Bytes actually written.
         */
    extern uint16_t ProtocolSnip_load_manufacturer_version_id(openlcb_node_t *openlcb_node, openlcb_msg_t *outgoing_msg, uint16_t offset, uint16_t requested_bytes);

        /**
         * @brief Copies manufacturer name string (null-terminated).
         *
         * @param openlcb_node     Pointer to @ref openlcb_node_t source node.
         * @param outgoing_msg     Pointer to @ref openlcb_msg_t reply being built.
         * @param offset           Payload byte offset to write at.
         * @param requested_bytes  Maximum bytes to copy.
         * @return Bytes actually written.
         */
    extern uint16_t ProtocolSnip_load_name(openlcb_node_t *openlcb_node, openlcb_msg_t *outgoing_msg, uint16_t offset, uint16_t requested_bytes);

        /**
         * @brief Copies model string (null-terminated).
         *
         * @param openlcb_node     Pointer to @ref openlcb_node_t source node.
         * @param outgoing_msg     Pointer to @ref openlcb_msg_t reply being built.
         * @param offset           Payload byte offset to write at.
         * @param requested_bytes  Maximum bytes to copy.
         * @return Bytes actually written.
         */
    extern uint16_t ProtocolSnip_load_model(openlcb_node_t *openlcb_node, openlcb_msg_t *outgoing_msg, uint16_t offset, uint16_t requested_bytes);

        /**
         * @brief Copies hardware version string (null-terminated).
         *
         * @param openlcb_node     Pointer to @ref openlcb_node_t source node.
         * @param outgoing_msg     Pointer to @ref openlcb_msg_t reply being built.
         * @param offset           Payload byte offset to write at.
         * @param requested_bytes  Maximum bytes to copy.
         * @return Bytes actually written.
         */
    extern uint16_t ProtocolSnip_load_hardware_version(openlcb_node_t *openlcb_node, openlcb_msg_t *outgoing_msg, uint16_t offset, uint16_t requested_bytes);

        /**
         * @brief Copies software version string (null-terminated).
         *
         * @param openlcb_node     Pointer to @ref openlcb_node_t source node.
         * @param outgoing_msg     Pointer to @ref openlcb_msg_t reply being built.
         * @param offset           Payload byte offset to write at.
         * @param requested_bytes  Maximum bytes to copy.
         * @return Bytes actually written.
         */
    extern uint16_t ProtocolSnip_load_software_version(openlcb_node_t *openlcb_node, openlcb_msg_t *outgoing_msg, uint16_t offset, uint16_t requested_bytes);

        /**
         * @brief Copies user version ID byte (1 byte).
         *
         * @param openlcb_node     Pointer to @ref openlcb_node_t source node.
         * @param outgoing_msg     Pointer to @ref openlcb_msg_t reply being built.
         * @param offset           Payload byte offset to write at.
         * @param requested_bytes  Maximum bytes to copy.
         * @return Bytes actually written.
         */
    extern uint16_t ProtocolSnip_load_user_version_id(openlcb_node_t *openlcb_node, openlcb_msg_t *outgoing_msg, uint16_t offset, uint16_t requested_bytes);

        /**
         * @brief Reads user name from config memory (null-terminated, max 63 bytes).
         *
         * @param openlcb_node     Pointer to @ref openlcb_node_t source node.
         * @param outgoing_msg     Pointer to @ref openlcb_msg_t reply being built.
         * @param offset           Payload byte offset to write at.
         * @param requested_bytes  Maximum bytes to copy.
         * @return Bytes actually written.
         */
    extern uint16_t ProtocolSnip_load_user_name(openlcb_node_t *openlcb_node, openlcb_msg_t *outgoing_msg, uint16_t offset, uint16_t requested_bytes);

        /**
         * @brief Reads user description from config memory (null-terminated, max 64 bytes).
         *
         * @param openlcb_node     Pointer to @ref openlcb_node_t source node.
         * @param outgoing_msg     Pointer to @ref openlcb_msg_t reply being built.
         * @param offset           Payload byte offset to write at.
         * @param requested_bytes  Maximum bytes to copy.
         * @return Bytes actually written.
         */
    extern uint16_t ProtocolSnip_load_user_description(openlcb_node_t *openlcb_node, openlcb_msg_t *outgoing_msg, uint16_t offset, uint16_t requested_bytes);

        /**
         * @brief Validates a SNIP reply: correct MTI, valid length, exactly 6 null terminators.
         *
         * @param snip_reply_msg  Pointer to @ref openlcb_msg_t message to validate.
         * @return true if the message is a well-formed SNIP reply.
         */
    extern bool ProtocolSnip_validate_snip_reply(openlcb_msg_t *snip_reply_msg);

    // ---- Individual SNIP field extractors (used to read fields from a received reply) ----
    //
    // Each reads one field from a validated SNIP reply payload and copies it into a
    // caller-provided buffer.  Use after ProtocolSnip_validate_snip_reply() returns true.

        /**
         * @brief Reads the manufacturer version ID byte from a SNIP reply.
         *
         * @param incoming_msg  Pointer to a validated @ref openlcb_msg_t SNIP reply.
         * @param out_version   Destination for the version byte.
         * @return 1 on success, 0 if the field could not be located.
         */
    extern uint16_t ProtocolSnip_extract_manufacturer_version_id(openlcb_msg_t *incoming_msg, uint8_t *out_version);

        /**
         * @brief Reads the manufacturer name string from a SNIP reply.
         *
         * @param incoming_msg  Pointer to a validated @ref openlcb_msg_t SNIP reply.
         * @param out_buffer    Destination buffer; result is null-terminated.
         * @param max_bytes     Size of out_buffer in bytes (must be > 0).
         * @return Number of bytes written, including the terminating null.
         */
    extern uint16_t ProtocolSnip_extract_name(openlcb_msg_t *incoming_msg, char *out_buffer, uint16_t max_bytes);

        /**
         * @brief Reads the model name string from a SNIP reply.
         *
         * @param incoming_msg  Pointer to a validated @ref openlcb_msg_t SNIP reply.
         * @param out_buffer    Destination buffer; result is null-terminated.
         * @param max_bytes     Size of out_buffer in bytes (must be > 0).
         * @return Number of bytes written, including the terminating null.
         */
    extern uint16_t ProtocolSnip_extract_model(openlcb_msg_t *incoming_msg, char *out_buffer, uint16_t max_bytes);

        /**
         * @brief Reads the hardware version string from a SNIP reply.
         *
         * @param incoming_msg  Pointer to a validated @ref openlcb_msg_t SNIP reply.
         * @param out_buffer    Destination buffer; result is null-terminated.
         * @param max_bytes     Size of out_buffer in bytes (must be > 0).
         * @return Number of bytes written, including the terminating null.
         */
    extern uint16_t ProtocolSnip_extract_hardware_version(openlcb_msg_t *incoming_msg, char *out_buffer, uint16_t max_bytes);

        /**
         * @brief Reads the software version string from a SNIP reply.
         *
         * @param incoming_msg  Pointer to a validated @ref openlcb_msg_t SNIP reply.
         * @param out_buffer    Destination buffer; result is null-terminated.
         * @param max_bytes     Size of out_buffer in bytes (must be > 0).
         * @return Number of bytes written, including the terminating null.
         */
    extern uint16_t ProtocolSnip_extract_software_version(openlcb_msg_t *incoming_msg, char *out_buffer, uint16_t max_bytes);

        /**
         * @brief Reads the user version ID byte from a SNIP reply.
         *
         * @param incoming_msg  Pointer to a validated @ref openlcb_msg_t SNIP reply.
         * @param out_version   Destination for the version byte.
         * @return 1 on success, 0 if the field could not be located.
         */
    extern uint16_t ProtocolSnip_extract_user_version_id(openlcb_msg_t *incoming_msg, uint8_t *out_version);

        /**
         * @brief Reads the user-provided node name string from a SNIP reply.
         *
         * @param incoming_msg  Pointer to a validated @ref openlcb_msg_t SNIP reply.
         * @param out_buffer    Destination buffer; result is null-terminated.
         * @param max_bytes     Size of out_buffer in bytes (must be > 0).
         * @return Number of bytes written, including the terminating null.
         */
    extern uint16_t ProtocolSnip_extract_user_name(openlcb_msg_t *incoming_msg, char *out_buffer, uint16_t max_bytes);

        /**
         * @brief Reads the user-provided node description string from a SNIP reply.
         *
         * @param incoming_msg  Pointer to a validated @ref openlcb_msg_t SNIP reply.
         * @param out_buffer    Destination buffer; result is null-terminated.
         * @param max_bytes     Size of out_buffer in bytes (must be > 0).
         * @return Number of bytes written, including the terminating null.
         */
    extern uint16_t ProtocolSnip_extract_user_description(openlcb_msg_t *incoming_msg, char *out_buffer, uint16_t max_bytes);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OPENLCB_PROTOCOL_SNIP__ */
