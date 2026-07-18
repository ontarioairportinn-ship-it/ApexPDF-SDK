#include "apexpdf/apexpdf.h"

#include <assert.h>
#include <stdio.h>

#ifndef APX_FUZZ_CORPUS_DIRECTORY
#error APX_FUZZ_CORPUS_DIRECTORY must identify the checked-in seed corpus
#endif

typedef struct fuzz_stream {
    const unsigned char* bytes;
    size_t size;
} fuzz_stream;

static apx_status APX_CALL fuzz_size(void* user_data, uint64_t* size) {
    const fuzz_stream* stream = (const fuzz_stream*)user_data;
    *size = (uint64_t)stream->size;
    return APX_STATUS_OK;
}

static apx_status APX_CALL fuzz_read(
    void* user_data, uint64_t offset, void* destination, size_t requested, size_t* read) {
    const fuzz_stream* stream = (const fuzz_stream*)user_data;
    size_t available;
    size_t count;
    size_t index;
    if (offset > (uint64_t)stream->size) {
        return APX_STATUS_OUT_OF_RANGE;
    }
    available = stream->size - (size_t)offset;
    count = requested < available ? requested : available;
    for (index = 0; index < count; index++) {
        ((unsigned char*)destination)[index] = stream->bytes[(size_t)offset + index];
    }
    *read = count;
    return APX_STATUS_OK;
}

static void exercise(const unsigned char* data, size_t size) {
    fuzz_stream state = {data, size};
    apx_random_access_stream stream = {sizeof(stream), &state, fuzz_size, fuzz_read};
    unsigned char output[32];
    uint64_t reported_size = 0U;
    size_t read = 0U;
    uint64_t offset = size == 0U ? 0U : (uint64_t)(data[0] % (size + 1U));
    size_t requested = size < sizeof(output) ? size : sizeof(output);
    assert(apx_stream_get_size(&stream, &reported_size) == APX_STATUS_OK);
    assert(reported_size == size);
    assert(apx_stream_read_at(&stream, offset, output, requested, &read) == APX_STATUS_OK);
    assert(read <= requested);
    assert(read <= size - (size_t)offset);
}

static void replay(const char* path) {
    FILE* file = fopen(path, "rb");
    unsigned char bytes[4096];
    size_t size;
    assert(file != NULL);
    size = fread(bytes, 1U, sizeof(bytes), file);
    assert(ferror(file) == 0);
    assert(feof(file) != 0);
    assert(fclose(file) == 0);
    exercise(bytes, size);
}

int main(void) {
    replay(APX_FUZZ_CORPUS_DIRECTORY "/empty.seed");
    replay(APX_FUZZ_CORPUS_DIRECTORY "/minimal-pdf.seed");
    replay(APX_FUZZ_CORPUS_DIRECTORY "/binary.seed");
    return 0;
}
