#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE 8U

typedef struct
{
    uint8_t buffer[BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} RingBuf;

void ringbuf_init(RingBuf *rb)
{
    rb->head = 0U;
    rb->tail = 0U;
    rb->count = 0U;
}

int ringbuf_write(RingBuf *rb, uint8_t data)
{
    if (rb->count == BUFFER_SIZE)
    {
        return -1; /* Buffer full */
    }

    rb->buffer[rb->head] = data;

    /* BUFFER_SIZE is a power of 2 (8).
     * Using '& (BUFFER_SIZE - 1)' is faster than '% BUFFER_SIZE'
     * because it avoids division instructions on many MCUs.
     * This optimization only works when BUFFER_SIZE is a power of 2.
     */
    rb->head = (rb->head + 1U) & (BUFFER_SIZE - 1U);

    rb->count++;

    return 0;
}

int ringbuf_read(RingBuf *rb, uint8_t *data)
{
    if (rb->count == 0U)
    {
        return -1; /* Buffer empty */
    }

    *data = rb->buffer[rb->tail];

    rb->tail = (rb->tail + 1U) & (BUFFER_SIZE - 1U);

    rb->count--;

    return 0;
}

uint8_t ringbuf_count(const RingBuf *rb)
{
    return rb->count;
}

int ringbuf_is_full(const RingBuf *rb)
{
    return (rb->count == BUFFER_SIZE);
}

int ringbuf_is_empty(const RingBuf *rb)
{
    return (rb->count == 0U);
}

int main(void)
{
    RingBuf rb;
    uint8_t data;
    ringbuf_init(&rb);

    uint8_t writes[] = {0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48};

    for (uint8_t i = 0U; i < BUFFER_SIZE; i++) {
        int result = ringbuf_write(&rb, writes[i]);
        printf("[WRITE] 0x%02X -> %s (count=%u)%s\n",
               writes[i],
               (result == 0) ? "OK" : "FAIL",
               ringbuf_count(&rb),
               ringbuf_is_full(&rb) ? " FULL" : "");
    }

    printf("[WRITE] 0x99 -> FAIL (buffer full)\n");

    for (uint8_t i = 0U; i < 3U; i++) {
        ringbuf_read(&rb, &data);
        printf("[READ]        -> 0x%02X  (count=%u)\n", data, ringbuf_count(&rb));
    }

    uint8_t new_writes[] = {0x49, 0x4A, 0x4B};
    for (uint8_t i = 0U; i < 3U; i++) {
        ringbuf_write(&rb, new_writes[i]);
        printf("[WRITE] 0x%02X -> OK  (count=%u)\n", new_writes[i], ringbuf_count(&rb));
    }

    while (!ringbuf_is_empty(&rb)) {
        ringbuf_read(&rb, &data);
        printf("[READ]        -> 0x%02X  (count=%u)\n", data, ringbuf_count(&rb));
    }

    printf("[READ] (empty) -> FAIL (buffer empty)\n");

    return 0;
}