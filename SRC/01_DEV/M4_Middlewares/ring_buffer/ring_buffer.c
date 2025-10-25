/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <string.h>

#include "ring_buffer.h"

/* Dependency includes. */
#include "error_codes.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static void ring_buffer_advance_index(volatile uint32_t* p_index, uint32_t max_items);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Khởi tạo bộ đệm */
uint32_t ring_buffer_init(ring_buffer_t* const me, uint8_t *static_buffer, uint32_t buffer_size, uint32_t max_items, uint32_t item_size)
{
    if (me == NULL || static_buffer == NULL || max_items == 0 || item_size == 0 || buffer_size == 0)
    {
        return ERROR_INVALID_PARAM;
    }

    /* Kiểm tra xem bộ đệm tĩnh có đủ kích thước không */
    if (max_items * item_size > buffer_size)
    {
        return ERROR_OUT_OF_MEMORY;
    }

    me->buffer      = static_buffer;
    me->buffer_size = buffer_size;
    me->item_size   = item_size;
    me->max_items   = max_items;
    me->head        = 0;
    me->tail        = 0;

    return ERROR_OK;
}

/* Đưa một phần tử vào bộ đệm */
uint32_t ring_buffer_push(ring_buffer_t* const me, void const * const item)
{
    uint32_t result;

	if (me == NULL || item == NULL )
    {
        result = ERROR_INVALID_PARAM;
    }
    else if (ring_buffer_is_full(me))
    {
        result = ERROR_BUFFER_FULL;
    }
	else
	{
		/* Tính vị trí trong bộ đệm */
		uint8_t *dest = me->buffer + (me->head * me->item_size);
		memcpy(dest, item, me->item_size);

		/* Cập nhật head và count */
		ring_buffer_advance_index(&me->head, me->max_items);
		result = ERROR_OK;

	}
    return result;
}

/* Lấy một phần tử ra khỏi bộ đệm */
uint32_t ring_buffer_pop(ring_buffer_t* const me, void * item)
{
    uint32_t result;

	if (me == NULL || item == NULL )
    {
		result = ERROR_INVALID_PARAM;
    }
    else if (ring_buffer_is_empty(me))
    {
        result = ERROR_BUFFER_EMPTY;
    }
	else
	{

		/* Tính vị trí trong bộ đệm */
		uint8_t *src = me->buffer + (me->tail * me->item_size);
		uint8_t *des = (uint8_t *)item;
		memcpy(des, src, me->item_size);


		/* Cập nhật tail và count */
		ring_buffer_advance_index(&me->tail, me->max_items);

		result = ERROR_OK;
	}
    return result;
}

/* Ghi đề một phần tử vào bộ đệm mà không tăng index */
uint32_t ring_buffer_overwrite(ring_buffer_t* const me, void const * const item)
{
    uint32_t result ;

	if (me == NULL )
    {
        result = ERROR_INVALID_PARAM;
    }
	else
	{
		/* Tính vị trí trong bộ đệm */
		uint8_t *dest = me->buffer + (me->head * me->item_size);
		memcpy(dest, item, me->item_size);

		result = ERROR_OK;
	}

    return result;
}

/* Đọc giá trị của phần tử hiện tại mà không tăng index */
uint32_t ring_buffer_peak(ring_buffer_t* const me, void * item)
{
    uint32_t result ;

	if (me == NULL )
    {
        result = ERROR_INVALID_PARAM;
    }
	else
	{
		/* Tính vị trí trong bộ đệm */
		uint8_t *src = me->buffer + (me->tail * me->item_size);
		uint8_t *des = (uint8_t *)item;
		memcpy(des, src, me->item_size);

		result = ERROR_OK;
	}

    return result;
}

/* Kiểm tra bộ đệm rỗng */
bool ring_buffer_is_empty(ring_buffer_t* const me)
{
    if (me == NULL)
    {
        return true;
    }

    uint32_t head = me->head;
    uint32_t tail = me->tail;

    return ((head == tail) ? 1 : 0);
}

/* Kiểm tra bộ đệm đầy */
bool ring_buffer_is_full(ring_buffer_t* const me)
{
    if (me == NULL)
    {
        return true;
    }

    uint32_t head       = me->head;
    uint32_t tail       = me->tail;
    uint32_t max_item   = me->max_items;

    return((((head + 1) % max_item) == tail) ? 1 : 0);
}

uint32_t ring_buffer_get_buffer_count(ring_buffer_t* const me)
{
    if (me == NULL)
    {
        return true;
    }

    uint32_t head       = me->head;
    uint32_t tail       = me->tail;
    uint32_t max_item   = me->max_items;

    return((head >= tail) ? (head - tail) : (max_item - (tail - head)));
}

uint32_t ring_buffer_get_free_space(ring_buffer_t* const me)
{ 
    if (me == NULL)
    {
        return 0; // Return 0 if the buffer is not initialized
    }

    uint32_t count      = ring_buffer_get_buffer_count(me);
    uint32_t max_item   = me->max_items;

    // Calculate the number of free bytes in the ring buffer
    return max_item - count; 
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static void ring_buffer_advance_index(volatile uint32_t* p_index, uint32_t max_items)
{
    *p_index = (*p_index + 1) % max_items;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */