/* ========================================================================= *
 *                                                                           *
 *                 The Apache Software License,  Version 1.1                 *
 *                                                                           *
 *          Copyright (c) 1999-2001 The Apache Software Foundation.          *
 *                           All rights reserved.                            *
 *                                                                           *
 * ========================================================================= *
 *                                                                           *
 * Redistribution and use in source and binary forms,  with or without modi- *
 * fication, are permitted provided that the following conditions are met:   *
 *                                                                           *
 * 1. Redistributions of source code  must retain the above copyright notice *
 *    notice, this list of conditions and the following disclaimer.          *
 *                                                                           *
 * 2. Redistributions  in binary  form  must  reproduce the  above copyright *
 *    notice,  this list of conditions  and the following  disclaimer in the *
 *    documentation and/or other materials provided with the distribution.   *
 *                                                                           *
 * 3. The end-user documentation  included with the redistribution,  if any, *
 *    must include the following acknowlegement:                             *
 *                                                                           *
 *       "This product includes  software developed  by the Apache  Software *
 *        Foundation <http://www.apache.org/>."                              *
 *                                                                           *
 *    Alternately, this acknowlegement may appear in the software itself, if *
 *    and wherever such third-party acknowlegements normally appear.         *
 *                                                                           *
 * 4. The names  "The  Jakarta  Project",  "Jk",  and  "Apache  Software     *
 *    Foundation"  must not be used  to endorse or promote  products derived *
 *    from this  software without  prior  written  permission.  For  written *
 *    permission, please contact <apache@apache.org>.                        *
 *                                                                           *
 * 5. Products derived from this software may not be called "Apache" nor may *
 *    "Apache" appear in their names without prior written permission of the *
 *    Apache Software Foundation.                                            *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED WARRANTIES *
 * INCLUDING, BUT NOT LIMITED TO,  THE IMPLIED WARRANTIES OF MERCHANTABILITY *
 * AND FITNESS FOR  A PARTICULAR PURPOSE  ARE DISCLAIMED.  IN NO EVENT SHALL *
 * THE APACHE  SOFTWARE  FOUNDATION OR  ITS CONTRIBUTORS  BE LIABLE  FOR ANY *
 * DIRECT,  INDIRECT,   INCIDENTAL,  SPECIAL,  EXEMPLARY,  OR  CONSEQUENTIAL *
 * DAMAGES (INCLUDING,  BUT NOT LIMITED TO,  PROCUREMENT OF SUBSTITUTE GOODS *
 * OR SERVICES;  LOSS OF USE,  DATA,  OR PROFITS;  OR BUSINESS INTERRUPTION) *
 * HOWEVER CAUSED AND  ON ANY  THEORY  OF  LIABILITY,  WHETHER IN  CONTRACT, *
 * STRICT LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN *
 * ANY  WAY  OUT OF  THE  USE OF  THIS  SOFTWARE,  EVEN  IF  ADVISED  OF THE *
 * POSSIBILITY OF SUCH DAMAGE.                                               *
 *                                                                           *
 * ========================================================================= *
 *                                                                           *
 * This software  consists of voluntary  contributions made  by many indivi- *
 * duals on behalf of the  Apache Software Foundation.  For more information *
 * on the Apache Software Foundation, please see <http://www.apache.org/>.   *
 *                                                                           *
 * ========================================================================= */

#ifndef JK_REQ_H
#define JK_REQ_H

#include "jk_global.h"
#include "jk_logger.h"
#include "jk_pool.h"
#include "jk_msg_buff.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Abstract interface to jk marshalling. Different encodings and
 * communication mechanisms can be supported.
 *
 * This object is recyclable, but is not thread safe - it can
 * handle a single message at a time.
 *
 * It is created by a channel( XXX endpoint ? )
 * and can be sent/received only on that channel.
 *
 * XXX Lifecycle: on send the buffer will be reused after send
 *     On receive - it will be recycled after reset() or equiv.
 *     Same as on the java side.
 *
 * XXX JNI: this was specially designed so it can be used for a JNI
 * channel. It'll collect the params and convert them to java types.
 *
 * @author Costin Manolache
 */
struct jk_msg {
    /** Human-readable method name */
    char *name;

    /** Method id - to be sent in the packet
     */
    int id;
 
    /** List of properties. The handler can be configured.
     *  ( password for login, etc ).
     */
    /*
      char **supportedProperties;
      jk_map_t *properties;
    */

    /*
     * Prepare the buffer for a new invocation 
     */
    void (*reset)(struct jk_msg *_this);

    /*
     * Finalize the buffer before sending - set length fields, etc
     */
    void (*end)(struct jk_msg *_this);

    /*
     * Dump the buffer header
     *   @param err Message text
     */
    void (*dump)(struct jk_msg *_this, char *err);

    void (*appendByte)(struct jk_msg *_this, unsigned char val);
    
    void (*appendBytes)(struct jk_msg *_this, 
                       const unsigned char * param,
                       const int len);

    void (*appendInt)(struct jk_msg *_this, 
                      const unsigned short val);

    void (*appendLong)(struct jk_msg *_this, 
                       const unsigned long val);

    void (*appendString)(struct jk_msg *_this, 
                         const char *param);

    unsigned char (*getByte)(struct jk_msg *_this);

    unsigned short (*getInt)(struct jk_msg *_this);

    unsigned long (*getLong)(struct jk_msg *_this);

    char * (*getString)(struct jk_msg *_this);
                             
    
    void (*getBytes)(struct jk_msg *_this, char *buf, int len);
                             
    
    void *_privatePtr;
};
    
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif 
