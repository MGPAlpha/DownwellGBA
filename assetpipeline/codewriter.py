class CPPFileWriter:
    text = ""
    indent_amount = 4
    indent_level = 0

    def writeIndented(self, text: str):
        if self.text[-1] == "\n": self.text += " " * self.indent_amount * self.indent_level
        self.text += text

    def writeLineIndented(self, text: str):
        self.writeIndented(text + "\n")

    def writeIncludeFile(self, filename: str):
        self.text += f"#include \"{filename}\"\n"

    def writeIncludeLibrary(self, filename: str):
        self.text += f"#include <{filename}>\n"
    
    def writeIfndef(self, symbol: str):
        self.text += f"#ifndef {symbol}\n"

    def writeDefine(self, symbol: str):
        self.text += f"#define {symbol}\n"
    
    def writeEndif(self):
        self.text += "#endif\n"

    def writeNewline(self):
        self.text += "\n"

    def indent(self):
        self.indent_level += 1

    def undent(self):
        self.indent_level -= 1
        if self.indent_level < 0: self.indent_level = 0

    def openNamespace(self, name: str):
        self.writeLineIndented("namespace " + name + " {")
        self.indent()

    def closeNamespace(self):
        self.undent()
        self.writeLineIndented("}")

    def writeVarDeclaration(self, var_type: str, var_name: str):
        self.writeIndented(f"{var_type} {var_name}")

    def writeVarDeclarationN(self, var_type: str, var_name: str):
        self.writeVarDeclaration(var_type, var_name)
        self.writeEndStatement()

    def writeVarDefinitionOpen(self, var_type: str, var_name: str):
        self.writeVarDeclaration(var_type, var_name)
        self.writeIndented(" = ")

    def writeVarDefinitionFull(self, var_type: str, var_name: str, value: str):
        self.writeVarDefinitionOpen(var_type, var_name)
        self.writeIndented(value)
        self.writeEndStatement()

    def writeEndStatement(self):
        self.writeLineIndented(";")

    def writeArrayInline(self, array: list):
        self.writeIndented("{" + ", ".join(array) + "}")

    def writeBeginCompoundLiteral(self, lit_type: str):
        self.writeIndented("(" + lit_type + ") {")

    def writeEndCompoundLiteral(self):
        self.writeIndented("}")

    def dumpFile(self, dest: str):
        with open(dest, "w") as out_file:
            out_file.write(self.text)

class HeaderAndImplementationWriter:
    
    cpp = CPPFileWriter()
    hpp = CPPFileWriter()
    
    def __init__(self, name: str) -> None:
        self.name = name

    def begin(self):
        symbol = self.name.capitalize().replace(".", "_").replace(" ", "_")
        self.hpp.writeIfndef(symbol)
        self.hpp.writeDefine(symbol)
        self.hpp.writeNewline()

        self.cpp.writeIncludeFile(self.name + ".hpp")
        self.cpp.writeNewline()

    def end(self):
        self.hpp.writeEndif()

    def openNamespace(self, name: str):
        self.hpp.openNamespace(name)
        self.cpp.openNamespace(name)

    def closeNamespace(self):
        self.hpp.closeNamespace()
        self.cpp.closeNamespace()

    def writeVarDeclAndDefOpen(self, var_type: str, var_name: str):
        self.hpp.writeVarDeclarationN("extern " + var_type, var_name)
        self.cpp.writeVarDefinitionOpen(var_type, var_name)

    def writeVarDeclAndDef(self, var_type: str, var_name: str, value: str):
        self.writeVarDeclAndDefOpen(var_type, var_name)
        self.cpp.writeIndented(value)
        self.cpp.writeEndStatement()