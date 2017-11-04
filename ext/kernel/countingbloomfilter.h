
/*
  +------------------------------------------------------------------------+
  | Phalcon Framework                                                      |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2014 Phalcon Team (http://www.phalconphp.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@phalconphp.com so we can send you a copy immediately.       |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
  |          Eduar Carvajal <eduar@phalconphp.com>                         |
  |          Vladimir Kolesnikov <vladimir@extrememember.com>              |
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  +------------------------------------------------------------------------+
*/

#ifndef PHALCON_KERNEL_DABLOOMS_H
#define PHALCON_KERNEL_DABLOOMS_H

#ifdef ZEND_ENABLE_ZVAL_LONG64

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    size_t bytes;
    int    fd;
    char  *array;
} bitmap_t;


bitmap_t *bitmap_resize(bitmap_t *bitmap, size_t old_size, size_t new_size);
bitmap_t *new_bitmap(int fd, size_t bytes);

int bitmap_increment(bitmap_t *bitmap, unsigned int index, long offset);
int bitmap_decrement(bitmap_t *bitmap, unsigned int index, long offset);
int bitmap_check(bitmap_t *bitmap, unsigned int index, long offset);
int bitmap_flush(bitmap_t *bitmap);

void free_bitmap(bitmap_t *bitmap);

typedef struct {
    uint64_t id;
    uint32_t count;
    uint32_t _pad;
} counting_bloom_header_t;


typedef struct {
    counting_bloom_header_t *header;
    unsigned int capacity;
    long offset;
    unsigned int counts_per_func;
    uint32_t *hashes;
    size_t nfuncs;
    size_t size;
    size_t num_bytes;
    double error_rate;
    bitmap_t *bitmap;
} counting_bloom_t;

int free_counting_bloom(counting_bloom_t *bloom);
counting_bloom_t *new_counting_bloom(unsigned int capacity, double error_rate, const char *filename);
counting_bloom_t *new_counting_bloom_from_file(unsigned int capacity, double error_rate, const char *filename);
int counting_bloom_add(counting_bloom_t *bloom, const char *s, size_t len);
int counting_bloom_remove(counting_bloom_t *bloom, const char *s, size_t len);
int counting_bloom_check(counting_bloom_t *bloom, const char *s, size_t len);

typedef struct {
    uint64_t max_id;
    uint64_t mem_seqnum;
    uint64_t disk_seqnum;
} scaling_bloom_header_t;

typedef struct {
    scaling_bloom_header_t *header;
    unsigned int capacity;
    unsigned int num_blooms;
    size_t num_bytes;
    double error_rate;
    int fd;
    counting_bloom_t **blooms;
    bitmap_t *bitmap;
} scaling_bloom_t;

scaling_bloom_t *new_scaling_bloom(unsigned int capacity, double error_rate, const char *filename);
scaling_bloom_t *new_scaling_bloom_from_file(unsigned int capacity, double error_rate, const char *filename);
int free_scaling_bloom(scaling_bloom_t *bloom);
int scaling_bloom_add(scaling_bloom_t *bloom, const char *s, size_t len, uint64_t id);
int scaling_bloom_remove(scaling_bloom_t *bloom, const char *s, size_t len, uint64_t id);
int scaling_bloom_check(scaling_bloom_t *bloom, const char *s, size_t len);
int scaling_bloom_flush(scaling_bloom_t *bloom);
uint64_t scaling_bloom_mem_seqnum(scaling_bloom_t *bloom);
uint64_t scaling_bloom_disk_seqnum(scaling_bloom_t *bloom);

#endif /* ZEND_ENABLE_ZVAL_LONG64 */

#endif /* PHALCON_KERNEL_DABLOOMS_H */
