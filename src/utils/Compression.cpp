#include <Logger.h>
#include <zlib/zlib.h>

#include <ResourceLoader.hpp>
#include <utils/Compression.hpp>

using namespace utils;

bool Compression::compress(const char *filename, char *data, size_t dataLen) {
    Bytef buff[1024];

    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    defstream.avail_in = (uInt)(dataLen);
    defstream.next_in = (Bytef *)data;
    defstream.avail_out = 0;
    defstream.next_out = 0;
    deflateInit(&defstream, Z_BEST_COMPRESSION);
    FILE *f = fopen(filename, "wb");
    if (!f) {
        LOGE("%s: fopen failed", filename);
        exit(0);
        return false;
    }
    uint64_t size = dataLen;
    if (fwrite(&size, 1, sizeof(size), f) != sizeof(size)) {
        LOGE("%s: fwrite failed", filename);
        exit(0);
        return false;
    }
    while (1) {
        defstream.next_out = buff;
        defstream.avail_out = (uInt)sizeof(buff);
        int err = deflate(&defstream, Z_FINISH);
        LOGD("%s: deflate: %d", filename, err);
        uInt toWrite = (uInt)sizeof(buff) - defstream.avail_out;
        if (fwrite(buff, 1, toWrite, f) != toWrite) {
            LOGE("%s: fwrite failed", filename);
            exit(0);
            return false;
        }
        if (err == Z_STREAM_END) {
            break;
        } else if (err == Z_OK) {
            continue;
        } else {
            LOGE("Failed deflating %s: %d", filename, err);
            return false;
        }
    }
    deflateEnd(&defstream);
    if (fclose(f)) {
        LOGE("%s: fclose failed", filename);
        exit(0);
        return false;
    }
    LOGD("Saved to %s", filename);
    return true;
}

char *decompress(const char *name, const char *filename, size_t *outDataLen) {
    size_t compressedSize;
    char *data = ResourceLoader::readFile(filename, &compressedSize);
    uint64_t size = *((uint64_t *)data);
    char *decompressed = new char[size];

    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    defstream.avail_in = (uInt)(compressedSize - sizeof(uint64_t));
    defstream.next_in = (Bytef *)(data + sizeof(uint64_t));
    defstream.avail_out = (uInt)size;
    defstream.next_out = (Bytef *)decompressed;
    inflateInit(&defstream);
    while (1) {
        int err = inflate(&defstream, Z_NO_FLUSH);
        LOGD("%s: inflate: %d", filename, err);
        if (err == Z_STREAM_END) {
            break;
        } else if (err == Z_OK) {
            continue;
        } else {
            LOGE("Failed inflating %s: %d", filename, err);
            free(data);
            return 0;
        }
    }
    inflateEnd(&defstream);
    free(data);
    return decompressed;
}