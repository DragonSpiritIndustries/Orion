#ifndef BYTECODESTREAM_HPP
#define BYTECODESTREAM_HPP

#include "angelscript/angelscript.h"
#include <string>
#include <physfs.h>
#include <Athena/FileWriter.hpp>
#include <Athena/FileReader.hpp>
#include <Athena/BinaryWriter.hpp>
#include <Athena/BinaryReader.hpp>
class ByteCodeWriter : public Athena::io::FileWriter, public asIBinaryStream
{
    FILEWRITER_BASE;
public:
    ByteCodeWriter(const std::string& filepath);
    ~ByteCodeWriter();

    void Write(const void *ptr, asUINT size);
    void Read(void *ptr, asUINT size);
private:
    Athena::io::BinaryWriter m_memoryStream;
};

class ByteCodeReader : public Athena::io::FileReader, public asIBinaryStream
{
    FILEREADER_BASE;
public:
    ByteCodeReader(const std::string& filepath);

    void Write(const void *ptr, asUINT size);
    void Read(void *ptr, asUINT size);
private:
    Athena::io::BinaryReader m_memoryStream;
};
#endif // BYTECODESTREAM_HPP
