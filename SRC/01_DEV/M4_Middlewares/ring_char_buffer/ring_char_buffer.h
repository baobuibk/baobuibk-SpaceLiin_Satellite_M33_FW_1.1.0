#ifndef _RING_CHAR_BUFFER_H_
#define _RING_CHAR_BUFFER_H_

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <stdint.h>
#include <stdbool.h>

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Cấu trúc Ring Char Buffer */
typedef struct _ring_char_buffer_t_
{
    uint8_t *buffer;    /* Con trỏ đến mảng tĩnh do người dùng cung cấp */

    volatile uint32_t head;      /* Vị trí đầu (để thêm phần tử) */
    volatile uint32_t tail;      /* Vị trí đuôi (để lấy phần tử) */
    uint32_t max_items; /* Số lượng phần tử tối đa */

} ring_char_buffer_t;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Khởi tạo bộ đệm, yêu cầu người dùng cung cấp con trỏ đến mảng tĩnh */
uint32_t ring_char_buffer_init(ring_char_buffer_t * const me, uint8_t *static_buffer,  uint32_t max_items);

/* Đưa một phần tử vào bộ đệm */
uint32_t ring_char_buffer_push(ring_char_buffer_t * const me, uint8_t const item);

/* Lấy một phần tử ra khỏi bộ đệm */
uint32_t ring_char_buffer_pop(ring_char_buffer_t * const me, uint8_t * const item);

/* Ghi đề một phần tử vào bộ đệm mà không tăng index */
uint32_t ring_char_buffer_overwrite(ring_char_buffer_t * const me, uint8_t const item);

/* Đọc giá trị của phần tử hiện tại mà không tăng index */
uint32_t ring_char_buffer_peak(ring_char_buffer_t * const me, uint8_t * const item);

/* Kiểm tra bộ đệm rỗng */
bool ring_char_buffer_is_empty(ring_char_buffer_t * const me);

/* Kiểm tra bộ đệm đầy */
bool ring_char_buffer_is_full(ring_char_buffer_t * const me);

/* Lấy số lượng phần tử hiện tại trong bộ đệm */
uint32_t ring_char_buffer_get_buffer_count(ring_char_buffer_t const * const me);

/* Lấy số lượng phần tử còn trống trong bộ đệm */
uint32_t ring_char_buffer_get_free_space(ring_char_buffer_t const * const me);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* _RING_CHAR_BUFFER_H_ */