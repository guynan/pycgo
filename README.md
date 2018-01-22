# Pycgo

Here I endeavour to create a wrapper whereby one write Go code and call that
without too much hastle from Python. Where this differs from other implementations
is that I have decided to keep both the Python and Go interfaces clean from the
wrapping code. Meaning that there are only self aware. This means that you can for example
call a Go function by passing in a Python list, accept this as a slice in your
Go code, and then return a value which is converted from a native Go type to a 
native Python type. 

The wrapper is written in the native interface for both languages, that being C.
This means that transactions between the two languages are fast and do not complicate
the logic for either language.

This is a hobby project that I am doing as a proof of concept to potentially 
integrate into Odoo so that we can enjoy a speed boost there.
