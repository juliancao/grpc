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

#include "src/core/lib/iomgr/port.h"

#ifdef GRPC_POSIX_WAKEUP_FD

#include <stddef.h>

#include "src/core/lib/iomgr/wakeup_fd_cv.h"
#include "src/core/lib/iomgr/wakeup_fd_pipe.h"
#include "src/core/lib/iomgr/wakeup_fd_posix.h"

static const grpc_wakeup_fd_vtable *wakeup_fd_vtable = NULL;

extern grpc_error* eventfd_create(grpc_wakeup_fd* fd_info);
extern grpc_error* eventfd_consume(grpc_wakeup_fd* fd_info);
extern grpc_error* eventfd_wakeup(grpc_wakeup_fd* fd_info);
extern void eventfd_destroy(grpc_wakeup_fd* fd_info);
extern int eventfd_check_availability(void);
extern int check_availability_invalid(void);

int grpc_allow_specialized_wakeup_fd = 1;
int grpc_allow_pipe_wakeup_fd = 1;

int has_real_wakeup_fd = 1;
int cv_wakeup_fds_enabled = 0;

int wakeuptable = 0;

void grpc_wakeup_fd_global_init(void) {
  if (grpc_allow_specialized_wakeup_fd &&
#ifdef GRPC_POSIX_NO_SPECIAL_WAKEUP_FD
      check_availability_invalid()) {
#else
	  eventfd_check_availability()) {
#endif
    //wakeup_fd_vtable = &grpc_specialized_wakeup_fd_vtable;
	  wakeuptable = 1;
  } else if (grpc_allow_pipe_wakeup_fd &&
             pipe_check_availability()) {
    //wakeup_fd_vtable = &grpc_pipe_wakeup_fd_vtable;
	  wakeuptable = 2;
  } else {
    has_real_wakeup_fd = 0;
  }
}

void grpc_wakeup_fd_global_destroy(void) { wakeup_fd_vtable = NULL; }

int grpc_has_wakeup_fd(void) { return has_real_wakeup_fd; }

int grpc_cv_wakeup_fds_enabled(void) { return cv_wakeup_fds_enabled; }

void grpc_enable_cv_wakeup_fds(int enable) { cv_wakeup_fds_enabled = enable; }

grpc_error *grpc_wakeup_fd_init(grpc_wakeup_fd *fd_info) {
  if (cv_wakeup_fds_enabled) {
    return cv_fd_init(fd_info);
  }
  if (wakeuptable == 1) {
#ifdef GRPC_LINUX_EVENTFD
    return eventfd_create(fd_info);
#endif
  } else {
    return pipe_init(fd_info);
  }
}

grpc_error *grpc_wakeup_fd_consume_wakeup(grpc_wakeup_fd *fd_info) {
  if (cv_wakeup_fds_enabled) {
    return cv_fd_consume(fd_info);
  }
  if (wakeuptable == 1) {
#ifdef GRPC_LINUX_EVENTFD
    return eventfd_consume(fd_info);
#endif
  } else {
    return pipe_consume(fd_info);
  }
}

grpc_error *grpc_wakeup_fd_wakeup(grpc_wakeup_fd *fd_info) {
  if (cv_wakeup_fds_enabled) {
    return cv_fd_wakeup(fd_info);
  }
  if (wakeuptable == 1) {
#ifdef GRPC_LINUX_EVENTFD
    return eventfd_wakeup(fd_info);
#endif
  } else {
    return pipe_wakeup(fd_info);
  }
  
}

void grpc_wakeup_fd_destroy(grpc_wakeup_fd *fd_info) {
  if (cv_wakeup_fds_enabled) {
    cv_fd_destroy(fd_info);
  } else {  
  if (wakeuptable == 1) {
#ifdef GRPC_LINUX_EVENTFD
    eventfd_destroy(fd_info);
#endif
  } else if (wakeuptable == 2) {
    pipe_destroy(fd_info);
  }    
  }
}

#endif /* GRPC_POSIX_WAKEUP_FD */
