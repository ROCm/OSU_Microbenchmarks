/*
 * Copyright (C) 2002-2016 the Network-Based Computing Laboratory
 * (NBCL), The Ohio State University.
 *  
 * Contact: Dr. D. K. Panda (panda@cse.ohio-state.edu)
 *
 * For detailed copyright and licensing information, please refer to the
 * copyright file COPYRIGHT in the top level OMB directory.         
 */
#ifndef OSU_PT2PT_H
#define OSU_PT2PT_H 1

#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

#ifdef _ENABLE_CUDA_
#include "cuda.h"
#include "cuda_runtime.h"
#endif

#ifdef _ENABLE_OPENACC_
#include <openacc.h>
#endif

#ifdef _ENABLE_ROCM_
#define  __HIP_PLATFORM_HCC__
#include "hip/hip_runtime.h"
#endif



#ifdef PACKAGE_VERSION
#    define HEADER "# " BENCHMARK " v" PACKAGE_VERSION "\n"
#else
#    define HEADER "# " BENCHMARK "\n"
#endif

#ifndef FIELD_WIDTH
#   define FIELD_WIDTH 20
#endif

#ifndef FLOAT_PRECISION
#   define FLOAT_PRECISION 2
#endif

#define MAX_REQ_NUM 1000

#define MAX_MSG_SIZE (1<<28)
#define MYBUFSIZE (MAX_MSG_SIZE)

#define WINDOW_SIZE_LARGE  64
#define LARGE_MESSAGE_SIZE  (1<<25)

#ifdef _ENABLE_OPENACC_
#   define OPENACC_ENABLED 1
#else
#   define OPENACC_ENABLED 0
#endif

#ifdef _ENABLE_CUDA_
#   define CUDA_ENABLED 1
#else
#   define CUDA_ENABLED 0
#endif

#ifdef _ENABLE_ROCM_
#   define ROCM_ENABLED 1
#else
#   define ROCM_ENABLED 0
#endif



extern MPI_Request request[MAX_REQ_NUM];
extern MPI_Status  reqstat[MAX_REQ_NUM];
extern MPI_Request send_request[MAX_REQ_NUM];
extern MPI_Request recv_request[MAX_REQ_NUM];

#ifdef _ENABLE_CUDA_
extern CUcontext cuContext;
#endif

#define BW 0
#define LAT 1
#define LAT_MT 2

#define BW_LOOP_SMALL   100
#define BW_SKIP_SMALL   10
#define BW_LOOP_LARGE   20
#define BW_SKIP_LARGE   2

#define LAT_LOOP_SMALL  10000
#define LAT_SKIP_SMALL  100
#define LAT_LOOP_LARGE  1000
#define LAT_SKIP_LARGE  10

#define DEF_NUM_THREADS 2
#define MIN_NUM_THREADS 1
#define MAX_NUM_THREADS 128

enum po_ret_type {
    po_cuda_not_avail,
    po_openacc_not_avail,
    po_rocm_not_avail,
    po_bad_usage,
    po_help_message,
    po_okay,
};

enum accel_type {
    none,
    cuda,
    openacc,
    rocm
};

struct options_t {
    char src;
    char dst;
    int srcgid;
    int dstgid;
    enum accel_type accel;
    int loop;
    int loop_large;
    int skip;
    int skip_large;
    int num_threads;
    char managedSend;
    char managedRecv;
};

extern struct options_t options;

void usage (char const *);
int process_options (int argc, char *argv[], int type);
int allocate_memory (char **sbuf, char **rbuf, int rank);
void print_header (int rank, int type);
void touch_data (void *sbuf, void *rbuf, int rank, size_t size);
void free_memory (void *sbuf, void *rbuf, int rank);
int init_accel (void);
int cleanup_accel (void);

void set_header (const char * header);

#endif
