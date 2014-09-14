// This file is part of Orion.
//
// Orion is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Orion is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Orion.  If not, see <http://www.gnu.org/licenses/>

#ifndef BYTECODESTREAM_HPP
#define BYTECODESTREAM_HPP

#include "angelscript/angelscript.h"
#include <string>
#include <physfs.h>
#include <Athena/FileWriter.hpp>
#include <Athena/FileReader.hpp>
#include <Athena/BinaryWriter.hpp>
#include <Athena/BinaryReader.hpp>
#include <Athena/PHYSFSFileReader.hpp>
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

class ByteCodeReader : public Athena::io::PHYSFSFileReader, public asIBinaryStream
{
    PHYSFSFILEREADER_BASE();
public:
    ByteCodeReader(const std::string& filepath);

    void Write(const void *ptr, asUINT size);
    void Read(void *ptr, asUINT size);
private:
    Athena::io::BinaryReader m_memoryStream;
};
#endif // BYTECODESTREAM_HPP
