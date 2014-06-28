#include "include/ByteCodeStream.hpp"
#include <Athena/Compression.hpp>
#include <memory.h>
#include <sha1.h>
#include <Athena/Utility.hpp>
#include <Athena/InvalidDataException.hpp>

ByteCodeWriter::ByteCodeWriter(const std::string& path)
    : base(path),
      m_memoryStream(new atUint8[1], 1)
{
}

ByteCodeWriter::~ByteCodeWriter()
{
    atInt8* magic=(atInt8*)"ASC\0";
    base::writeUint32(*(atUint32*)magic);
    base::writeUint32(m_memoryStream.length());
    atUint8* data = m_memoryStream.data();

    atUint8 compData[m_memoryStream.length()*2];
    atInt32 compLen = m_memoryStream.length()*2;
    compLen = Athena::io::Compression::compressZlib(data, m_memoryStream.length(), compData, compLen);
    if (compLen > 0 && compLen < (atInt32)m_memoryStream.length())
    {
        base::writeUint32(compLen);
        const atUint32 realLen = (compLen + 31) & ~31;
        atUint8 realData[realLen];
        Athena::utility::fillRandom(realData, realLen);
        memcpy(realData, compData, compLen);
        atUint8* sha1 = getSha1(realData, realLen);
        base::writeUBytes(sha1, 20);
        base::writeUBytes(realData, realLen);
    }
    else
    {
        base::writeUint32(0);
        const atUint32 realLen = (m_memoryStream.length() + 31) & ~31;
        atUint8 realData[realLen];
        Athena::utility::fillRandom(realData, realLen);
        memcpy(realData, data, m_memoryStream.length());
        atUint8* sha1 = getSha1(realData, realLen);
        base::writeUBytes(sha1, 20);
        base::writeUBytes(realData, realLen);
    }
    delete[] data;
}

void ByteCodeWriter::Write(const void* ptr, asUINT size)
{
    if (!size)
        return;

    m_memoryStream.writeUBytes((atUint8*)ptr, size);
}

void ByteCodeWriter::Read(void* /*ptr*/, asUINT /*size*/)
{
    return;
}


ByteCodeReader::ByteCodeReader(const std::string& filepath)
    : base(filepath),
      m_memoryStream(filepath)
{
    atUint32 magic = base::readUint32();
    if (magic != 0x00435341)
        THROW_INVALID_DATA_EXCEPTION("Not a valid AngelScript bytecode file,"
                                     " expected magic 0x43534100 got %8X", magic);

    atUint32 uncompLen = base::readUint32();
    atUint32 compLen   = base::readUint32();
    atUint8* sha1Hash  = base::readUBytes(20);
    atUint32 realLen = 0;

    if (compLen != 0)
        realLen = (compLen + 31) & ~31;
    else
        realLen = (uncompLen + 31) & ~31;

    atUint8* data = base::readUBytes(realLen);

    atUint8* calculatedHash = getSha1(data, realLen);

    if (memcmp(sha1Hash, calculatedHash, 20))
    {
        delete[] sha1Hash;
        delete[] calculatedHash;
        delete[] data;
        THROW_INVALID_DATA_EXCEPTION("Hash validation failure, file is either corrupt or has been tampered with");
    }
    delete[] sha1Hash;
    delete[] calculatedHash;
    atUint8* bytecode = new atUint8[uncompLen];
    atInt32 outSize = Athena::io::Compression::decompressZlib(data, compLen, bytecode, uncompLen);
    delete[] data;
    if (outSize != uncompLen)
    {
        delete[] bytecode;
        THROW_INVALID_DATA_EXCEPTION("Decompression of bytecode failed");
    }

    m_memoryStream.setData(bytecode, outSize);
}

void ByteCodeReader::Write(const void* /*ptr*/, asUINT /*size*/)
{
    THROW_NOT_IMPLEMENTED_EXCEPTION();
}

void ByteCodeReader::Read(void* ptr, asUINT size)
{
    if (!size)
        return;

    atUint8* data = m_memoryStream.readUBytes(size);
    memcpy(ptr, data, size);
    delete[] data;
}
