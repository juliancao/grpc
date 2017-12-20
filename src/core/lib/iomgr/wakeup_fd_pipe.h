/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef GRPC_CORE_LIB_IOMGR_WAKEUP_FD_PIPE_H
#define GRPC_CORE_LIB_IOMGR_WAKEUP_FD_PIPE_H

#include "src/core/lib/iomgr/wakeup_fd_posix.h"

//extern grpc_wakeup_fd_vtable grpc_pipe_wakeup_fd_vtable;
extern grpc_error* pipe_init(grpc_wakeup_fd* fd_info);
extern grpc_error* pipe_consume(grpc_wakeup_fd* fd_info);
extern grpc_error* pipe_wakeup(grpc_wakeup_fd* fd_info);
extern void pipe_destroy(grpc_wakeup_fd* fd_info);
extern int pipe_check_availability(void);

#endif /* GRPC_CORE_LIB_IOMGR_WAKEUP_FD_PIPE_H */
