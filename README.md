
<center> <h1> <b> Δ Del  </b> </h1> </center>

[![Build Status](https://travis-ci.com/NablaVM/del.svg?branch=master)](https://travis-ci.com/NablaVM/del)
![](https://img.shields.io/badge/License-MIT-blueviolet)
![](https://img.shields.io/badge/Built%20with-C%2B%2B-red)

## Development Updates: 

***09/ July/ 2020***

After much consideration and branches of rewrites I've decided that this implementation of DEL will remain as-is. Its not great, but its a decent start. There is no printing, no working with the VM Net/File IO, and it has no dynamic memory. This means its not _really_ usable. Realistically, writing programs in ASM for Nabla is a better idea. The DEL language was meant as a POC and I think it does that nicely.  

:star2: ***22/ June/ 2020*** :star2:

The Del Compiler is on its way, and recently pulled out of the Nabal VM application. Initially I thought it would a good idea to couple the compiler with the virtual machine, but I recently decided against doing that. This is an application specifically to compile the Del language to Nabla Bytecode that can then be executed by the [NablaVM](https://github.com/NablaVM/nabla).
