# Forge

The forge is the middle / backend for compilers for Nabla. Right now its being developed with Del, but once completed it will be moved to its own library so other languages
can be built to target the NablaVM. The idea of the Forge is to abstract away the vm instructions to a series of higher-level abstract instructions that can be utilized by 
to easily-ish translate high-level language implementations to to VM instructions. 

This is going to be a work in progress for quite some time.

Notes:

This does not do semantic analysis of the outer language. It does whatever its told. If the user causes an error, an error will be handed back and it is upto the caller to report the error to the user.

Things the forge will offer:

A symbol table - A user can use the symbol table to help perform its own semantic anlysis. 

..
..
..

And more too