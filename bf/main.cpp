#include <iostream>
#include <fstream>
#include <stack>
#include <cstdio>
#include <asmjit/asmjit.h>

#define DEBUG(x) ; // x
#define ARRAYSIZE 1024

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "Usage: ./bf input_program.txt" << std::endl;
        exit(-1);
    }

    std::ifstream fin;
    try
    {
        fin.open(argv[1], std::ifstream::in);
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "Wrong Filename?" << std::endl;
        std::cerr << e.what() << std::endl;
        exit(-2);
    }

    asmjit::StringLogger logger;
    asmjit::JitRuntime runtime;
    asmjit::X86Assembler a(&runtime);
    asmjit::X86Compiler c(&a);
    a.setLogger(&logger);

    c.addFunc(asmjit::FuncBuilder1<int, char*>(asmjit::kCallConvHost));

    asmjit::X86GpVar p = c.newIntPtr("P");
    asmjit::X86GpVar x = c.newInt8("x");
    asmjit::X86GpVar address = c.newIntPtr("Address");
    asmjit::X86CallNode* ext_putchar;
    asmjit::X86CallNode* ext_getchar;

    c.setArg(0, p);

    std::stack<asmjit::Label> labels;
    asmjit::Label L1, L2;

    char inst;
    while (!fin.eof()) {
        DEBUG(std::cout << inst << ' ');
        switch (inst = fin.get()) {
        case '>':
            c.add(p, x.getSize());
            break;
        case '<':
            c.sub(p, x.getSize());
            break;
        case '+':
            c.mov(x, asmjit::x86::ptr(p));
            c.inc(x);
            c.mov(asmjit::x86::ptr(p), x);
            break;
        case '-':
            c.mov(x, asmjit::x86::ptr(p));
            c.dec(x);
            c.mov(asmjit::x86::ptr(p), x);
            break;
        case '[':
            L1 = c.newLabel();
            L2 = c.newLabel();
            c.bind(L1);
            c.mov(x, asmjit::x86::ptr(p));
            c.test(x, x);
            c.je(L2);
            labels.push(L2);
            labels.push(L1);
            break;
        case ']':
            L1 = labels.top();
            labels.pop();
            L2 = labels.top();
            labels.pop();
            c.jmp(L1);
            c.bind(L2);
            break;
        case '.':
            c.mov(address, (size_t) & std::putchar);
            c.mov(x, asmjit::x86::ptr(p));
            ext_putchar = c.call(address, asmjit::FuncBuilder1<void, char>(asmjit::kCallConvHost));
            ext_putchar->setArg(0, x);
            ext_putchar->setComment("call putchar");
            break;
        case ',':
            c.mov(address, (size_t) & std::getchar);
            ext_getchar = c.call(address, asmjit::FuncBuilder0<char>(asmjit::kCallConvHost));
            ext_getchar->setRet(0, x);
            ext_getchar->setComment("call getchar");
            c.mov(asmjit::x86::ptr(p), x);
            break;
        }
    }
    DEBUG(std::cout << std::endl);

    c.ret(x);

    c.endFunc();
    c.finalize();

    void* funcPtr = a.make();
    typedef void (*FuncType)(char*);

    FuncType func = asmjit_cast<FuncType>(funcPtr);

    char large_array[ARRAYSIZE] = { 0 };
    func(large_array);

    std::cout << std::endl;
    DEBUG(std::cout << "Assembly: " << std::endl << logger.getString() << std::endl);

    runtime.release((void*)func);

    return 0;
}
