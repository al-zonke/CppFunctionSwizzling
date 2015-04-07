//
//  main.cpp
//  CppFunctionSwizzling
//
//  Created by Al Zonke on 4/7/15.
//  Copyright (c) 2015 Al Zonke. All rights reserved.
//
// ************************************************************************** //
//
//  This example shows how to swizzle functions in C++ with virtual table
//  substitution.
//
//  You should inherit hook class from your target class and overload
//  methods you need.
//
// ************************************************************************** //

#include <iostream>

// ************************************************************************** //

class BasicService
{
public:
    
    virtual void printName()
    {
        std::cout << "BasicService" << std::endl;
    }
    
    virtual void printInfo()
    {
        std::cout << "Basic Service for Test Purposes" << std::endl;
    }
    
};

// ************************************************************************** //

class ExtendedService : public BasicService
{
public:
    virtual void printName()
    {
        std::cout << "ExtendedService" << std::endl;
    }
};

// ************************************************************************** //

class HookService : public ExtendedService
{
#define super ExtendedService
public:
    virtual void printName()
    {
        super::printName();
        std::cout << "! HookService" << std::endl;
    }
    
    virtual void printInfo()
    {
        super::printInfo();
        std::cout << "! Hook Service Was Here" << std::endl;
    }
#undef super
};

// ************************************************************************** //

#define VTableRef(objectRef) *(void **)objectRef

// ************************************************************************** //

static void *HookVtable = NULL;
static void *ExtendedVtable = NULL;

static void FillVirtualTtables()
{
    HookService *hook = new HookService();
    HookVtable = VTableRef(hook);
    delete hook;

    ExtendedService *extended = new ExtendedService();
    ExtendedVtable = VTableRef(extended);
    delete extended;
}

// ************************************************************************** //

int main(int argc, const char * argv[])
{
    // Fill vtable pointers
    FillVirtualTtables();

    ExtendedService *extended = new ExtendedService();

    // Print original
    extended->printName();
    extended->printInfo();
    std::cout << std::endl;
    
    // Set hook
    VTableRef(extended) = HookVtable;
    extended->printName();
    extended->printInfo();
    std::cout << std::endl;
    
    // Remove hook
    VTableRef(extended) = ExtendedVtable;
    extended->printName();
    extended->printInfo();

    return 0;
}

// ************************************************************************** //
