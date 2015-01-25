<!--
#
# Copyright (c) 2013, Xin YUAN, courses of Zhejiang University
# All rights reserved.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the 2-Clause BSD License.
#
# Author contact information:
#   yxxinyuan@zju.edu.cn
#
-->

# WLang Programming Language

WLang language is designed according to C++, C#, Java, and so on.
WLang focuses on the high-level conceptions of programming,
and is suitable for cooperation. WLang keeps the high efficiency of executive
comapred with C++.

## Grammar

### Comment

The comment of WLang is the same as C++ with two types, /**/ and //.

### The Basic Data Types

The basic data types of WLang are shown as follows:

| Type | Description |
|-----|-----|
|char| signed character, 1 byte |
|byte| unsigned character, 1 byte |
|short| signed short integer, 2 bytes |
|ushort| unsigned short integer, 2 bytes |
|int| signed integer, 4 bytes |
|uint| unsigned integer, 4 bytes |
|int64| signed 64-bit integer, 8 bytes |
|uint64| unsigned 64-bit integer, 8 bytes |
|float| single precision float, 4 bytes |
|double| double precision float, 8 bytes |
|intptr| signed pointer integer, 4 bytes on 32-bit machine, 8 bytes on 64-bit machine |
|uintptr| unsigned pointer integer, 4 bytes on 32-bit machine, 8 bytes on 64-bit machine |
|bool| boolean, 1 byte |
|guid| global unique identifier, 16 bytes |

where, bool type can be assigned as true and false. A bool type is a logical expression
which represents the false logic as its value is false, and vice versa.

### Composite Types

WLang has two composite types: array and class. Array is also a type of "class"
with the replicated pattern. Class is a free-extended pattern. The basic data types
are also regarded as "class". Class can be considered as uniform type.

Because the different operations may be applied to the same data elements in an array,
e.g., comparing two strings with case-sensitive or case-insensitive, the array type
needs to be defined as special template form.

The array type can be defined as follows:

```
//fixed-length array
A[10] a;
A[10]<SpecialCompareTrait<A>> a;
//dynamic array
share A[] a;
weak A[] a2;
```

Enumerator is a special type which is compatible with integer. Only specified values
can be assigned to the enumerator variable. An enumerator can be defined as follows:

```
enum StreamOpenFlags
{
	Read = 0x00000001U;
...
}
```

### Constant

Constant can be defined for each data type except share and share array.
The constants of the basic data types are shown as follows:

|Type|Description|
|---|---|
|char,short,int,int64,intptr|Number(s)|
|byte,ushort,uint,uint64,uintptr|Number(s)+U|
|float|Number(s)+.+Number(s)+\[e+-Number(s)\]+f|
|double|Number(s)+.+Number(s)+\[e+-Number(s)\]|
|guid|{8-4-4-4-12}(hex numbers)|

When the constants are represented by hexadecimal numbers, they use the prefix of 0x.

The constants can be defined as follows:

```
const int c_Num(5);
```

The composite constants can be defined by a list embraced with curly braces.
Commas are used to seperate the items. list can be defined recursively.

The constant array can be defined as follows:

```
const int[] c_arr = { 0, 1, 2 };
```

The constants must be defined in class or method body similar to static variables.

### Namespace

The namespace organizes the classes to a hierarchical structure according to conceptions.
The namespace can be defined recursively. A namespace can be considered as a degraded class.
The definitions of WLang classes must locate in a namespace.

A namespace can be defined as follows:

```
namespace UI {
...
}
```

### Class and Object

*Class* is a basic unit for providing service and an uniform programming model.
*Object* is an instance of *Class*.

Class has a qualifier (private or public) for describing the visibility
in namespace. Class can be defined recursively.

Class consists of three elements: property, method and event. Property and method
have three visibility qualifiers: private, protected and public.
Event consists of one or more interfaces with `event` keyword.
Use `fire` keyword to trigger the events.

After the declaration of method, `nothrow` qualifier can be added to
ensure the method should not throw any exceptions.

`static` qualifier can be used to declare a static method which is not relative
to instance of class.

Class can be defined in a method.

WLang enables multi-inheritance with several base classes the same having alias.
Using alias and field symbol `::` to call these methods of base classes.
The method can overwrite the implementation of base class by adding `override`
qualifier and optional field symbol.

WLang only has public inheritance.

Class and methods can be designed as template form. The parameter of template
can be type, integer constant and enumerator constant.

The object can be defined as reference form (ref, share, weak).

The fetch and assignment of ref object must conform the life cycle.
The reference objects could not be defined directly, they must be accessed
by the property of wrapped class. Property has two types: reference and
object itself. Ref object cannot be fetched from share object and weak object.

The copy constructor, assignment operator and move operator of object are
implemented by default. The dereference object of reference object is
the default behavior of object operation.
Use ref() operator to get the reference semantics of reference object.
Use clone() operator to copy a reference object.
Use is_null() operator to determine a null reference object.

Use as operator to take the type cast. A type can be casted to other basic types,
base class, derived class (by template parameter).
The reference object can be casted directly.
A component object object can be casted to an interface object with
the query implementation.

### Interface

Interface defining by `interface` keyword consists of public property and method
without visibility qualifier.

A derived class can be inherited from interface and implement the interface method
with override qualifier.

### Component

Components have the types of in-process, out-of-process and network.

Components support as operator.

## The Basic Class Library


### Data Structure

Data structure is only designed in System namespace and enable data casted from
intptr, uintptr to reference object. Data structure can use MemoryPool class and
conform the life cycle.

Data structure and array use iterators to access the elements in collection.
Use auto_type keyword to define the iterator object.

### Thread and Synchronization

They are done in application layer.
